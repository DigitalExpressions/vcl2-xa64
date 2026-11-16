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

"""Untimed edge probes using a retained comparison's exact kernel compiler flags.

Run in the same x64 developer environment, after timing has finished. Original
evidence is read-only; each audit gets a new build directory. Failures are
reported as observations rather than hidden or relaxed.
"""
import argparse
import datetime as dt
import json
from pathlib import Path
import shutil
import sys
import zipfile

from run_x86_upstream_comparison import ROOT, Run, ISAS, sha, write_json, native_host


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run', required=True)
    p.add_argument('--portable-output', required=True)
    args=p.parse_args()
    native_host()
    original=Path(args.run).resolve()
    portable=Path(args.portable_output).resolve()
    if portable.exists(): p.error('Choose a new portable destination')
    meta=json.loads((original/'metadata.json').read_text(encoding='utf-8'))
    # Verify every exported source byte before compiling from that snapshot.
    for label in ('upstream','fork'):
        manifest=json.loads((original/(label+'-source-manifest.json')).read_text())
        if manifest['commit']!=meta['sources'][label]['commit']: raise RuntimeError('Source identity mismatch')
        for row in manifest['files']:
            if sha(original/(label+'-source')/row['path'])!=row['sha256']:
                raise RuntimeError('Changed exported source: '+row['path'])
    stamp=dt.datetime.now(dt.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    run=Run(ROOT/'build'/('x86-upstream-edges-'+stamp))
    print('Edge evidence:',run.output,flush=True)
    probe=run.output/'edge_probe.cpp'
    shutil.copyfile(ROOT/'benchmarks/x86_upstream/edge_probe.cpp',probe)
    shutil.copyfile(Path(__file__),run.output/Path(__file__).name)
    records={}
    for f in (original/'logs').glob('*.json'):
        record=json.loads(f.read_text())
        records[record['label']]=record
    observations=[]
    for driver in ('msvc','clang'):
        for isa in ISAS:
            if not meta['cpu_probe'][isa]: raise RuntimeError('Original ISA was not supported')
            for label in ('upstream','fork'):
                key='-'.join((driver,isa,label))
                dest=run.output/key; dest.mkdir()
                compile_args=list(records[key+'-kernels']['command'])
                old_obj=str(original/'binaries'/key/'kernels.obj')
                old_main=str(original/'binaries'/key/'main.obj')
                new_obj=str(dest/'edge.obj')
                compile_args=[str(probe) if Path(a).name=='kernels.cpp' else
                    a.replace(old_obj,new_obj) for a in compile_args]
                run.command(key+'-compile',compile_args,cwd=dest)
                link_args=[]
                for a in records[key+'-link']['command']:
                    if a==old_obj: continue
                    link_args.append(a.replace(old_main,new_obj).replace(
                        str(original/'binaries'/key/'compare.exe'),str(dest/'edge.exe')))
                run.command(key+'-link',link_args,cwd=dest)
                text=run.command(key+'-probe',[dest/'edge.exe'],expected=(0,1))
                rows=[json.loads(line) for line in text.splitlines()]
                observations.append(dict(driver=driver,isa=isa,source=label,
                    executable_sha256=sha(dest/'edge.exe'),records=rows))
    examples=[]
    for driver in ('msvc','clang'):
        compiler=records[driver+'-sse2-fork-kernels']['command'][0]
        build=run.output/(driver+'-example-build')
        run.command(driver+'-example-configure',['cmake','-S',original/'fork-source','-B',build,'-G','Ninja',
            '-DVCL_BUILD_EXAMPLES=ON','-DVCL_BACKEND=1','-DCMAKE_BUILD_TYPE=Release',
            '-DCMAKE_CXX_COMPILER='+compiler,'-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded'])
        run.command(driver+'-example-build',['cmake','--build',build,'--target','vcl_example','--parallel','1'])
        text=run.command(driver+'-example-run',[build/'examples/vcl_example.exe'])
        if text.strip()!='2 5 10 17': raise RuntimeError('Unexpected example output')
        examples.append(dict(driver=driver,source='fork',pass_=True,output=text.strip(),
            executable_sha256=sha(build/'examples/vcl_example.exe')))
    result=dict(original_run_id=original.name,sources=meta['sources'],examples=examples,
        probe_sha256=sha(probe),runner_sha256=sha(run.output/Path(__file__).name),
        shared_evidence_module_sha256=sha(ROOT/'tools/run_x86_upstream_comparison.py'),
        observations=observations,
        note='Untimed follow-up; exact existing kernel compile/link flags, independent IEEE and explicit rounding answers; exit 1 preserves failed lane checks.')
    write_json(run.output/'edge-results.json',result)
    (run.output/'checksums.sha256').write_bytes(''.join(f'{sha(f)}  {f.relative_to(run.output).as_posix()}\n'
        for f in sorted(run.output.rglob('*')) if f.is_file()).encode())
    archive=run.output/'edge-evidence.zip'
    with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
        for f in sorted(run.output.rglob('*')):
            if f.is_file() and f!=archive: z.write(f,f.relative_to(run.output).as_posix())
    receipt=dict(run_id=run.output.name,archive=archive.name,sha256=sha(archive))
    write_json(run.output/'archive-receipt.json',receipt)
    portable.mkdir(parents=True)
    shutil.copyfile(run.output/'edge-results.json',portable/'edge-results.json')
    write_json(portable/'archive-receipt.json',receipt)
    (portable/'checksums.sha256').write_bytes(''.join(f'{sha(f)}  {f.name}\n'
        for f in sorted(portable.iterdir()) if f.is_file()).encode())
    for row in observations:
        print(row['driver'],row['isa'],row['source'],row['records'][-1],flush=True)
    return 0


if __name__=='__main__': sys.exit(main())
