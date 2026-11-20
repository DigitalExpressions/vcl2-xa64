#!/usr/bin/env python3
# Copyright 2026 DigitalExpressions Sweden
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Opt-in same-host Windows x86 upstream/fork performance investigation.

No fetch, checkout mutation, installation, commit or push. Every invocation
uses new directories, clean Git source exports and a frozen copy of this harness.
"""
import argparse
from collections import Counter, defaultdict
import csv
import ctypes
import datetime as dt
import hashlib
import io
import json
import os
from pathlib import Path
import platform
import re
import shutil
import statistics
import struct
import subprocess
import sys
import tarfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
ISAS = {'sse2': 2, 'sse42': 6, 'avx2': 8, 'avx512': 10}
HARNESS = ['benchmarks/x86/common.h', 'benchmarks/x86/kernels.cpp',
           'benchmarks/x86/main.cpp', 'tools/run_x86_upstream_comparison.py',
           'tools/run_x86_upstream_comparison.ps1']


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def write_json(path, value):
    Path(path).write_bytes((json.dumps(value, indent=2, sort_keys=True) + '\n').encode('utf-8'))


class Run:
    def __init__(self, output):
        self.output = output
        output.mkdir(parents=True, exist_ok=False)
        (output / 'logs').mkdir()
        self.number = 0

    def command(self, label, args, cwd=None, expected=(0,), timeout=180, combined=False):
        self.number += 1
        prefix = self.output / 'logs' / f'{self.number:04d}-{label}'
        command = [str(a) for a in args]
        start = dt.datetime.now(dt.timezone.utc).isoformat()
        timed_out = False
        try:
            result = subprocess.run(command, cwd=cwd or ROOT, capture_output=True, timeout=timeout)
            code, stdout, stderr = result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired as exc:
            timed_out = True
            code, stdout, stderr = None, exc.stdout or b'', exc.stderr or b''
        prefix.with_suffix('.stdout').write_bytes(stdout)
        prefix.with_suffix('.stderr').write_bytes(stderr)
        record = dict(label=label, command=command, cwd=str(cwd or ROOT), start_utc=start,
                      end_utc=dt.datetime.now(dt.timezone.utc).isoformat(),
                      exit_code=code, timeout_seconds=timeout, timed_out=timed_out)
        write_json(prefix.with_suffix('.json'), record)
        if timed_out or code not in expected:
            raise RuntimeError(f'{label}: exit={code}, timeout={timed_out}; see {prefix}')
        return (stdout + (stderr if combined else b'')).decode('utf-8', errors='replace')


def git(*args):
    return subprocess.check_output(['git', '-C', str(ROOT), *args]).decode().strip()


def snapshot(run, label, revision):
    commit = git('rev-parse', '--verify', revision + '^{commit}')
    tree = git('rev-parse', commit + '^{tree}')
    archive = run.output / (label + '-source.tar')
    run.command(label + '-archive', ['git', 'archive', '--format=tar', '-o', archive, commit])
    dest = run.output / (label + '-source')
    dest.mkdir()
    with tarfile.open(archive) as tar:
        for member in tar.getmembers():
            target = (dest / member.name).resolve()
            if not target.is_relative_to(dest.resolve()) or member.issym() or member.islnk():
                raise RuntimeError('Unsafe Git archive member: ' + member.name)
            if member.isdir():
                target.mkdir(parents=True, exist_ok=True)
            elif member.isfile():
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(tar.extractfile(member).read())
    files = [dict(path=p.relative_to(dest).as_posix(), sha256=sha(p))
             for p in sorted(dest.rglob('*')) if p.is_file()]
    fingerprint = hashlib.sha256(''.join(r['path']+'\0'+r['sha256']+'\n' for r in files).encode()).hexdigest()
    write_json(run.output / (label + '-source-manifest.json'),
               dict(commit=commit, tree=tree, fingerprint=fingerprint, files=files))
    return dest, dict(commit=commit, tree=tree, fingerprint=fingerprint, archive_sha256=sha(archive))


def native_host():
    if platform.system() != 'Windows' or struct.calcsize('P') != 8:
        raise RuntimeError('Native Windows x64 Python is required')
    k = ctypes.WinDLL('kernel32', use_last_error=True)
    k.GetCurrentProcess.restype = ctypes.c_void_p
    k.IsWow64Process2.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_ushort), ctypes.POINTER(ctypes.c_ushort)]
    process, native = ctypes.c_ushort(), ctypes.c_ushort()
    if not k.IsWow64Process2(k.GetCurrentProcess(), ctypes.byref(process), ctypes.byref(native)):
        raise RuntimeError('Cannot establish native execution')
    if native.value != 0x8664 or process.value != 0:
        raise RuntimeError('Emulation is outside this experiment')
    k.GetProcessAffinityMask.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t)]
    process_mask, system_mask = ctypes.c_size_t(), ctypes.c_size_t()
    if not k.GetProcessAffinityMask(k.GetCurrentProcess(), ctypes.byref(process_mask), ctypes.byref(system_mask)):
        raise RuntimeError('Cannot read affinity')
    return dict(native_machine=hex(native.value), process_machine=hex(process.value),
                available_cpus=[n for n in range(64) if process_mask.value & (1 << n)],
                platform=platform.platform(), python=platform.python_version())


def parse_rows(text):
    return [json.loads(line) for line in text.splitlines() if line.strip()]


def disassembly(text):
    """Static instruction counts, not cycle estimates or byte-equivalence proof."""
    functions, current = {}, None
    for line in text.splitlines():
        header = re.match(r'^[0-9a-fA-F]+ <(k_[^>]+)>:', line)
        if header:
            current = header[1]
            functions[current] = []
        elif re.match(r'^[0-9a-fA-F]+ <', line):
            current = None
        elif current:
            instruction = re.match(r'^\s*[0-9a-fA-F]+:\s+([a-z][a-z0-9]*)\s*(.*)', line)
            if instruction:
                functions[current].append((instruction[1], instruction[2].strip()))
    return {name: dict(instructions=len(body), mnemonics=dict(Counter(x[0] for x in body)),
                       listing_sha256=hashlib.sha256(json.dumps(body).encode()).hexdigest())
            for name, body in functions.items()}


def compile_binary(run, driver, compiler, objdump, isa, source, harness, dest):
    dest.mkdir(parents=True)
    level = ISAS[isa]
    if driver == 'msvc':
        common = ['/nologo', '/std:c++17', '/O2', '/fp:strict', '/EHsc', '/MT', '/DNDEBUG']
        flags = [f'/DINSTRSET={level}', '/DMAX_VECTOR_SIZE=512', '/DVCL_BACKEND=1']
        flags += {'sse2': [], 'sse42': [], 'avx2': ['/arch:AVX2'], 'avx512': ['/arch:AVX512']}[isa]
        for unit in ('main', 'kernels'):
            options = common + (flags if unit == 'kernels' else ['/Qvec-report:2'])
            messages = run.command(dest.name+'-'+unit, [compiler, *options, '/c', harness / (unit+'.cpp'),
                        '/I'+str(source), '/Fo'+str(dest / (unit+'.obj'))], cwd=dest, combined=True)
            if 'warning D9002' in messages:
                raise RuntimeError('MSVC ignored a compiler option; inspect '+str(dest))
            if unit == 'main' and '1400' not in messages:
                raise RuntimeError('Missing MSVC no_vector diagnostic (reason 1400)')
        run.command(dest.name+'-link', [compiler, '/nologo', '/MT', dest/'main.obj', dest/'kernels.obj',
                    '/Fe'+str(dest/'compare.exe'), '/link', '/INCREMENTAL:NO', '/OPT:NOICF'], cwd=dest)
    else:
        common = ['-std=c++17', '-O2', '-ffp-model=strict', '-ffp-contract=off', '-DNDEBUG', '-fms-runtime-lib=static']
        flags = [f'-DINSTRSET={level}', '-DMAX_VECTOR_SIZE=512', '-DVCL_BACKEND=1']
        flags += {'sse2': ['-march=x86-64', '-mno-avx'],
                  'sse42': ['-march=x86-64', '-msse4.2', '-mno-avx'],
                  'avx2': ['-march=x86-64', '-mavx2', '-mfma', '-mf16c'],
                  'avx512': ['-march=x86-64', '-mavx2', '-mfma', '-mf16c', '-mavx512f', '-mavx512dq', '-mavx512bw', '-mavx512vl']}[isa]
        for unit in ('main', 'kernels'):
            options = common + (flags if unit == 'kernels' else ['-march=x86-64', '-mno-avx', '-fno-vectorize', '-fno-slp-vectorize'])
            messages = run.command(dest.name+'-'+unit, [compiler, *options, '-c', harness/(unit+'.cpp'), '-I'+str(source),
                        '-o', dest/(unit+'.obj')], cwd=dest, combined=True)
            if 'argument unused during compilation' in messages or 'unknown argument' in messages:
                raise RuntimeError('Clang ignored a compiler option; inspect '+str(dest))
        run.command(dest.name+'-link', [compiler, '-fms-runtime-lib=static', dest/'main.obj', dest/'kernels.obj',
                    '-o', dest/'compare.exe', '-Wl,/INCREMENTAL:NO,/OPT:NOICF'], cwd=dest)
    listing = run.command(dest.name+'-disassembly', [objdump, '-d', '-r', '--no-show-raw-insn', dest/'kernels.obj'])
    (dest/'kernels.disasm.txt').write_bytes(listing.encode())
    asm = disassembly(listing)
    write_json(dest/'assembly-summary.json', asm)
    if len(asm) != 102:
        raise RuntimeError(f'Expected 102 externally visible kernels, found {len(asm)} in {dest}')
    exe = dest/'compare.exe'
    data = exe.read_bytes()
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    if data[:2] != b'MZ' or data[pe:pe+4] != b'PE\0\0' or struct.unpack_from('<H', data, pe+4)[0] != 0x8664:
        raise RuntimeError('Expected x64 PE')
    return exe, asm


def analyze(measurements, assembly):
    grouped = defaultdict(lambda: defaultdict(list))
    by_pair = defaultdict(lambda: defaultdict(dict))
    for row in measurements:
        key = row['driver'], row['isa'], row['case']
        grouped[key][row['source']].append(row['ns_per_unit'])
        by_pair[key][row['pair']].setdefault(row['source'], []).append(row['ns_per_unit'])
    results = []
    for (driver, isa, case), sources in sorted(grouped.items()):
        values = {s: statistics.median(v) for s, v in sources.items()}
        ratios = [statistics.median(p['fork'])/statistics.median(p['upstream'])
                  for p in by_pair[(driver, isa, case)].values()]
        lo, hi = min(ratios), max(ratios)
        # Conservative descriptive flag; not a statistical significance claim.
        status = 'slower-consistent' if lo > 1.05 else 'faster-consistent' if hi < .95 else 'mixed-or-within-5pct'
        if lo < .95 and hi > 1.05:
            status = 'unstable-both-directions'
        asm_u = assembly[(driver, isa, 'upstream')]['k_'+case]
        asm_f = assembly[(driver, isa, 'fork')]['k_'+case]
        results.append(dict(driver=driver, isa=isa, case=case,
            upstream_median_ns=values['upstream'], fork_median_ns=values['fork'],
            fork_over_upstream=statistics.median(ratios), pair_ratio_min=lo, pair_ratio_max=hi,
            upstream_sample_min=min(sources['upstream']), upstream_sample_max=max(sources['upstream']),
            fork_sample_min=min(sources['fork']), fork_sample_max=max(sources['fork']),
            pairs=len(ratios), samples_per_source=len(sources['fork']), status=status,
            upstream_static_instructions=asm_u['instructions'], fork_static_instructions=asm_f['instructions'],
            mnemonic_histogram_equal=asm_u['mnemonics']==asm_f['mnemonics']))
    return results


def main(snapshot_provider=snapshot, extra_harness=()):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fork-commit', default='HEAD')
    parser.add_argument('--upstream-commit', default='667577495efaa36a10cbb5d6cb8c942c23913848')
    parser.add_argument('--clang', required=True)
    parser.add_argument('--objdump', required=True)
    parser.add_argument('--pairs', type=int, default=3)
    parser.add_argument('--trials', type=int, default=7)
    parser.add_argument('--sample-ms', type=float, default=4)
    parser.add_argument('--cpu', type=int, default=-1)
    parser.add_argument('--output')
    parser.add_argument('--portable-output')
    args = parser.parse_args()
    if args.pairs < 3 or args.trials < 3 or args.sample_ms <= 0:
        parser.error('Use at least three pairs and three samples per case')
    if args.portable_output and Path(args.portable_output).exists():
        parser.error('Portable output already exists; choose a new destination')
    stamp = dt.datetime.now(dt.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    output = Path(args.output).resolve() if args.output else ROOT/'build'/('x86-upstream-'+stamp)
    if not output.is_relative_to((ROOT/'build').resolve()):
        parser.error('Raw output must be under build/')
    run = Run(output)
    print('Evidence:', output, flush=True)
    host = native_host()
    cpu = args.cpu if args.cpu >= 0 else host['available_cpus'][0]
    if cpu not in host['available_cpus']:
        raise RuntimeError('Requested CPU is unavailable')
    run.command('git-state', ['git', 'status', '--short', '--branch'])
    run.command('git-diff', ['git', 'diff', '--binary', 'HEAD'])
    run.command('host', ['powershell', '-NoProfile', '-Command',
        'Get-CimInstance Win32_Processor | Select-Object Name,NumberOfCores,NumberOfLogicalProcessors | ConvertTo-Json; '
        'Get-CimInstance Win32_OperatingSystem | Select-Object Caption,Version,BuildNumber | ConvertTo-Json'])
    power = run.command('power', ['powercfg', '/getactivescheme'])
    versions = {}
    for driver, compiler in [('msvc', shutil.which('cl')), ('clang', args.clang)]:
        if not compiler or not Path(compiler).is_file():
            raise RuntimeError('Missing compiler: ' + driver)
        versions[driver] = run.command(driver+'-version', [compiler, '/Bv'] if driver=='msvc' else [compiler, '--version'], expected=(0, 2), combined=True)
    versions['objdump'] = run.command('objdump-version', [args.objdump, '--version'])
    harness = output/'harness'
    harness.mkdir()
    harness_files = HARNESS + list(extra_harness)
    for name in harness_files:
        shutil.copyfile(ROOT/name, harness/Path(name).name)
    harness_hashes = {name: sha(ROOT/name) for name in harness_files}
    write_json(output/'harness-manifest.json', harness_hashes)
    paths, provenance = {}, {}
    for label, revision in [('fork', args.fork_commit), ('upstream', args.upstream_commit)]:
        paths[label], provenance[label] = snapshot_provider(run, label, revision)
    meta = dict(schema=1, run_id=output.name, started_utc=stamp, host=host, cpu=cpu,
                power_scheme=power.strip(), versions=versions, sources=provenance,
                harness_sha256=harness_hashes, pairs=args.pairs, trials=args.trials, sample_ms=args.sample_ms,
                isa_levels=ISAS, fp='strict, no contraction, nearest-even, MXCSR=0x1f80 at startup',
                build='optimized O2, no LTO, no sanitizers, static release CRT',
                order='MSVC before Clang; ISA ascending; even pairs upstream/fork, odd pairs fork/upstream')
    write_json(output/'metadata.json', meta)
    executables, assembly, checks = {}, {}, []
    for driver, compiler in [('msvc', shutil.which('cl')), ('clang', args.clang)]:
        for isa in ISAS:
            if isa != 'sse2' and not meta['cpu_probe'][isa]:
                raise RuntimeError('Requested ISA unavailable: '+isa)
            for label in ('upstream', 'fork'):
                key = driver, isa, label
                dest = output/'binaries'/('-'.join(key))
                print('Build/check', '-'.join(key), flush=True)
                exe, asm = compile_binary(run, driver, compiler, args.objdump, isa, paths[label], harness, dest)
                executables[key], assembly[key] = exe, asm
                if 'cpu_probe' not in meta:
                    meta['cpu_probe'] = json.loads(run.command('cpu-probe', [exe, '--probe']))
                    write_json(output/'metadata.json', meta)
                    for n in range(4):
                        # Access violations are intentional negative guard controls.
                        run.command('guard-fault-'+str(n), [exe, '--fault', str(n)], expected=(0xc0000005, -1073741819))
                rows = parse_rows(run.command('-'.join(key)+'-check', [exe, '--check', '--cpu', cpu]))
                passed = [r for r in rows if r['kind']=='check']
                if len(passed) != 102 or not all(r['pass'] for r in passed):
                    raise RuntimeError('Incomplete correctness inventory')
                checks.extend(dict(driver=driver, isa=isa, source=label, **r) for r in rows)
    write_json(output/'checks.json', checks)
    measurements = []
    for driver in ('msvc', 'clang'):
        for isa in ISAS:
            for pair in range(args.pairs):
                order = ('upstream', 'fork') if pair%2==0 else ('fork', 'upstream')
                for label in order:
                    print('Measure', driver, isa, 'pair', pair+1, label, flush=True)
                    exe = executables[driver, isa, label]
                    rows = parse_rows(run.command(f'{driver}-{isa}-pair{pair+1}-{label}',
                        [exe, '--measure', '--cpu', cpu, '--trials', args.trials, '--sample-ms', args.sample_ms]))
                    samples = [r for r in rows if r['kind']=='sample']
                    if len(samples) != 102*args.trials:
                        raise RuntimeError('Incomplete timing inventory')
                    measurements.extend(dict(driver=driver, isa=isa, pair=pair+1, source=label, **r) for r in samples)
    write_json(output/'samples.json', measurements)
    results = analyze(measurements, assembly)
    write_json(output/'results.json', results)
    with (output/'results.csv').open('w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=list(results[0])); writer.writeheader(); writer.writerows(results)
    # Separate output-hash disagreement from correctness-reference success.
    outputs = defaultdict(dict)
    for row in checks:
        if row['kind']=='check': outputs[row['driver'], row['isa'], row['case']][row['source']]=row['output_fnv1a64']
    disagreements = [dict(driver=k[0], isa=k[1], case=k[2], hashes=v) for k,v in outputs.items()
                     if v['fork']!=v['upstream']]
    summary = dict(comparisons=len(results), correctness_cases=len(outputs)*2, failed=0, skipped=0, timed_out=0,
                   timing_samples=len(measurements), output_hash_disagreements=disagreements,
                   statuses=dict(Counter(r['status'] for r in results)),
                   binaries=[dict(driver=d, isa=i, source=s, sha256=sha(p), machine='0x8664')
                             for (d,i,s),p in executables.items()])
    write_json(output/'summary.json', summary)
    run.command('git-state-final', ['git', 'status', '--short', '--branch'])
    # Hash every raw evidence file, excluding the inventory/archive themselves.
    inventory = [f'{sha(p)}  {p.relative_to(output).as_posix()}\n' for p in sorted(output.rglob('*')) if p.is_file()]
    (output/'checksums.sha256').write_bytes(''.join(inventory).encode())
    archive = output/'evidence.zip'
    with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
        for p in sorted(output.rglob('*')):
            if p.is_file() and p!=archive: z.write(p, p.relative_to(output).as_posix())
    receipt = dict(archive=archive.name, sha256=sha(archive), run_id=output.name)
    write_json(output/'archive-receipt.json', receipt)
    if args.portable_output:
        portable = Path(args.portable_output).resolve()
        portable.mkdir(parents=True, exist_ok=False)
        # No paths, command logs, source archives or binaries in portable exports.
        for name in ('results.csv','results.json','summary.json','harness-manifest.json','archive-receipt.json'):
            shutil.copyfile(output/name, portable/name)
        safe_meta = {k:v for k,v in meta.items() if k!='versions'}
        safe_meta['compiler_versions'] = {k: '\n'.join(line for line in v.splitlines()
            if not re.search(r'(?i)(?:[a-z]:[\\/]|/(?:Users|home)/)', line)
            and ('version' in line.lower() or 'Target:' in line or 'Thread model:' in line))
            for k,v in versions.items()}
        write_json(portable/'metadata.json', safe_meta)
        (portable/'checksums.sha256').write_bytes(''.join(f'{sha(p)}  {p.name}\n'
            for p in sorted(portable.iterdir()) if p.is_file()).encode())
    print(json.dumps(summary, indent=2), flush=True)
    print('Archive:', receipt, flush=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
