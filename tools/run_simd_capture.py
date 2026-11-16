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

"""Capture one native SIMD corpus profile; full native suites run separately."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys

from compare_simd_results import load_bundle, validate_rows
from simd_corpus import DIRECTORY, ROOT, SCHEMA, SEED, digest, encoded, input_bytes, load
from validation_evidence import package_evidence


def git(*args):
    return subprocess.check_output(['git',*args],cwd=ROOT).decode('utf-8').strip()


def snapshot():
    head,tree,status=git('rev-parse','HEAD'),git('rev-parse','HEAD^{tree}'),git('status','--porcelain')
    patch=subprocess.check_output(['git','diff','--binary','HEAD'],cwd=ROOT)
    files=git('ls-files','--cached','--others','--exclude-standard','-z').split('\0')
    rows=[dict(path=p,sha256=digest(ROOT/p) if (ROOT/p).is_file() else None) for p in sorted(set(files)) if p]
    return dict(head=head,tree=tree,clean=not status,patch_sha256=hashlib.sha256(patch).hexdigest(),
                working_fingerprint=tree if not status else hashlib.sha256(encoded(rows)).hexdigest()),rows


def native(run,args):
    if platform.system()=='Windows':
        from run_windows_validation import native_host
        architecture=native_host()
        _,host=run.command('host',['powershell','-NoProfile','-NonInteractive','-Command',
            '@{cpu=Get-CimInstance Win32_Processor | Select-Object Name,Manufacturer;os=Get-CimInstance Win32_OperatingSystem | Select-Object Caption,Version,BuildNumber} | ConvertTo-Json -Depth 4'])
        host=json.loads(host)
        if host['cpu']['Manufacturer']!='AuthenticAMD':
            raise ValueError('This workflow requires the native AMD host; physical Intel is omitted')
        if args.backend!=1 or args.isa=='neon' or args.driver=='apple-clang':
            raise ValueError('Windows requires x86 backend and an x86 compiler/profile')
        return dict(label='windows-amd',architecture=architecture,host=host)
    if platform.system()!='Darwin' or platform.machine()!='arm64' or args.backend not in (2,3) or args.isa!='neon' or args.driver!='apple-clang':
        raise ValueError('Use native Mac ARM64/Apple Clang or native AMD Windows')
    translated=subprocess.run(['sysctl','-in','sysctl.proc_translated'],capture_output=True,text=True)
    if translated.returncode not in (0,1) or translated.stdout.strip() not in ('','0'):
        raise ValueError('Rosetta/unknown translation state is not native ARM coverage')
    _,cpu=run.command('host-cpu',['sysctl','-n','machdep.cpu.brand_string'])
    _,version=run.command('host-os',['sw_vers'])
    if 'Apple M2' not in cpu:
        raise ValueError('Expected the native Apple M2 host')
    return dict(label='mac-m2',cpu=cpu.strip(),os=version.strip(),architecture='arm64',translated=False)


def run_profile(args):
    cases,policies=load()
    initial,rows=snapshot()
    if not args.preparation and (not initial['clean'] or args.expected_commit!=initial['head']):
        raise ValueError('Final capture needs a clean checkout and exact --expected-commit SHA')
    if any(os.environ.get(key) for key in ('CXXFLAGS','CFLAGS','CPPFLAGS','LDFLAGS','CL','_CL_')):
        raise ValueError('Unset external compiler/link flag overrides for controlled capture')
    if args.backend!=3 and args.max_width!=512:
        raise ValueError('x86/compatibility profiles must cover the full logical width set')
    if args.sanitize and (args.driver not in ('clang','apple-clang') or args.mode!='Debug'):
        raise ValueError('Strict UBSan requires GNU-driver Clang/Apple Clang Debug')
    if args.driver=='apple-clang' and args.mode=='Debug' and not args.sanitize:
        raise ValueError('The Mac Debug profile requires strict UBSan')
    profile=f'{args.driver}-{args.isa}-b{args.backend}-w{args.max_width}-{args.mode.lower()}'
    if args.sanitize:
        profile+='-ubsan'
    stamp=datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    output=ROOT/'build'/('simd-capture-'+stamp+'-'+profile)
    output.mkdir(parents=True,exist_ok=False)
    if platform.system()=='Windows':
        from run_windows_validation import WindowsRun
        run=WindowsRun(ROOT,output)
    else:
        from run_native_arm_validation import Run
        run=Run(ROOT,output)
    print('Evidence directory: '+str(output),flush=True)
    packaged=False
    try:
        run.git('ignored-output','check-ignore',str(output))
        (output/'source-files.json').write_bytes(encoded(rows))
        (output/'source.json').write_bytes(encoded(initial))
        run.command('working-patch',['git','diff','--binary','HEAD'])
        for i,path in enumerate(p for p in git('ls-files','--others','--exclude-standard','-z').split('\0') if p):
            code,_=run.command('added-patch-'+str(i),['git','diff','--no-index','--binary','--','/dev/null',path],required=False)
            if code!=1:
                raise ValueError('Cannot capture added source: '+path)
        host=native(run,args)
        compiler=shutil.which(args.compiler or ('cl' if args.driver=='msvc' else 'clang++'))
        if not compiler:
            raise ValueError('Compiler unavailable; use the native developer shell')
        _,compiler_text=run.command('compiler',[compiler,'/Bv'] if args.driver=='msvc' else [compiler,'--version'],required=False)
        if args.driver=='msvc':
            version=re.search(r'Compiler Version ([0-9.]+)',compiler_text)
            if not version or 'for x64' not in compiler_text:
                raise ValueError('Expected native x64 MSVC')
            compiler_version='MSVC '+version[1]
        else:
            target='x86_64' if platform.system()=='Windows' else 'arm64'
            if not re.search(r'Target: '+target,compiler_text) or (args.driver=='apple-clang' and 'Apple clang' not in compiler_text):
                raise ValueError('Compiler target/driver mismatch')
            compiler_version=compiler_text.splitlines()[0]
        tools={}
        for name in ('cmake','ctest','ninja'):
            _,text=run.command(name,[name,'--version'])
            tools[name]=text.splitlines()[0]
        if platform.system()=='Windows':
            from run_windows_validation import controls
            args.testbench_dir=None
            controls(run,compiler,args,'Ninja')
            runtime=(output/'runtime-control.log').read_text()
            level=int(re.search(r'instrset_detect=(\d+)',runtime)[1])
            if level<{'sse2':2,'avx2':8,'avx512':10}[args.isa]:
                raise ValueError('CPU/OS does not support the selected ISA; no higher-ISA execution attempted')
            host['runtime_control']=runtime.strip()
        else:
            from run_native_arm_validation import probes
            probes(run,compiler,'Ninja')
        inputs=output/'inputs.txt'
        inputs.write_bytes(input_bytes(cases,args.max_width))
        build=output/'capture-build'
        configure=['cmake','-S',DIRECTORY,'-B',build,'-G','Ninja','-DCMAKE_CXX_COMPILER='+compiler,
            '-DCMAKE_BUILD_TYPE='+args.mode,'-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
            '-DVCL_BACKEND='+str(args.backend),'-DVCL_CAPTURE_ISA='+args.isa,
            '-DVCL_CAPTURE_MAX_WIDTH='+str(args.max_width),'-DVCL_NATIVE_MAX_VECTOR_SIZE='+str(args.max_width),
            '-DVCL_ENABLE_EXPERIMENTAL_NEON=ON','-DVCL_SANITIZE='+('ON' if args.sanitize else 'OFF')]
        if args.sanitize and platform.system()=='Windows':
            configure.append('-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded')
        if platform.system()=='Darwin':
            configure.append('-DCMAKE_OSX_ARCHITECTURES=arm64')
        run.command('capture-configure',configure)
        run.command('capture-build',['cmake','--build',build,'--parallel','3','--verbose'])
        commands=json.loads((build/'compile_commands.json').read_text())
        flags=next(c['command'] for c in commands if c['file'].replace('\\','/').endswith('/capture.cpp'))
        if any(flag in flags for flag in ('/fp:fast','-ffast-math','-Ofast','-ffinite-math-only','-march=native','-mcpu=native')):
            raise ValueError('Unexpected broad math/CPU flags')
        required=['/fp:strict'] if args.driver=='msvc' else ['-ffp-contract=off','-frounding-math']
        if args.sanitize:
            required+=['-fsanitize=undefined','-fno-sanitize-recover=undefined','-fno-omit-frame-pointer']
        if not all(flag in flags for flag in required):
            raise ValueError('Missing FP/sanitizer flags')
        binary=build/('simd_capture.exe' if platform.system()=='Windows' else 'simd_capture')
        artifact=run.binary('capture',binary)
        _,text=run.command('capture-execute',[binary,inputs],timeout=60)
        records=[json.loads(line) for line in text.splitlines()]
        metadata=records.pop(0)
        if metadata.get('metadata') is not True or metadata['backend']!=args.backend or metadata['max_width']!=args.max_width:
            raise ValueError('Executable metadata mismatch')
        if args.backend==1 and metadata['instrset'] not in {'sse2':(2,),'avx2':(8,),'avx512':(10,11,12)}[args.isa]:
            raise ValueError('Compiled ISA does not match profile')
        result=validate_rows(records,cases,metadata,policies)
        (output/'reference-validation.json').write_bytes(encoded(result))
        if snapshot()[0]!=initial:
            raise ValueError('Source changed during capture')
        (output/'capture-result.json').write_bytes(encoded(dict(source=initial,results=result,preparation=args.preparation)))
        # Package raw logs before creating the portable bundle, avoiding checksum cycles.
        package_evidence(run)
        packaged=True
        # Diagnostic bundles are kept even on numerical failure; comparator rejects them.
        bundle=output/'bundle'
        bundle.mkdir()
        filename='outputs-'+profile+'.jsonl'
        (bundle/filename).write_bytes(b''.join(encoded(row) for row in records))
        journal=[json.loads(line) for line in (output/'commands.jsonl').read_text(encoding='utf-8').splitlines()]
        control_names=('runtime-control','guard-positive','ubsan-positive','ubsan-negative')
        controls_result={r['label']:r['exit_code'] for r in journal if r['label'] in control_names or r['label'].startswith('guard-negative-')}
        if platform.system()=='Darwin':
            controls_result={r['label']:r['exit_code'] for r in journal if r['label'].startswith(('guard-','ubsan-','runtime-')) and not r['label'].endswith(('-compile','-configure','-build','-file'))}
        # Store only reviewed option tokens; machine-local paths stay in raw commands.
        flag_tokens=[token for token in re.findall(r'[^\s"]+',flags) if token.startswith(('-f','/fp:','/arch:','-m','-O','/O','-g','/Zi','-D','/D','-std','-fsanitize'))]
        if platform.system()=='Windows':
            crt_match=re.search(r'(?:^|\s)[/-](MDd?|MTd?)(?:\s|$)',flags)
            crt=crt_match[1] if crt_match else 'libcmt (static release)' if '--dependent-lib=libcmt' in flags else 'msvcrtd (dynamic debug)' if '--dependent-lib=msvcrtd' in flags else 'msvcrt (dynamic release)' if '--dependent-lib=msvcrt' in flags else None
            if crt is None:
                raise ValueError('Cannot identify Windows CRT flags')
        else:
            crt='macOS system runtime'
        page_size=int(re.search(r'page_size=(\d+)',(output/'guard-positive.log').read_text())[1])
        link_flags=['-fsanitize=undefined'] if args.sanitize else []
        if args.sanitize and '-fsanitize=undefined' not in (build/'build.ninja').read_text():
            raise ValueError('Missing sanitizer link option')
        manifest=dict(schema=SCHEMA,harness_version='simd-capture-1',run_id=args.run_id,attempt_id=args.attempt_id,
            preparation=args.preparation,scope='corpus-only; full native suite is required separately',
            source=initial,profile=profile,host=host,compiler=compiler_version,tools=tools,python=platform.python_version(),
            mode=args.mode,isa=args.isa,backend=args.backend,max_width=args.max_width,capture=metadata,
            sanitizer='strict UBSan' if args.sanitize else 'not requested',
            crt=crt,fp_flags=flag_tokens,link_flags=link_flags,rounding_modes=[0,1,2,3],FTZ=False,DAZ=False,seed=SEED,
            timeout_seconds=60,concurrency=3,controls=controls_result,guard_page_size=page_size,
            raw_evidence=dict(directory=output.name,archive='return-evidence.zip',sha256=digest(output/'return-evidence.zip')),
            corpus_sha256=digest(DIRECTORY/'corpus.jsonl'),policies_sha256=digest(DIRECTORY/'policies.json'),
            input_file_sha256=digest(inputs),binary_sha256=artifact['sha256'],outputs=filename,
            reference_validation={k:v for k,v in result.items() if k!='failures'})
        (bundle/'manifest.json').write_bytes(encoded(manifest))
        (bundle/'summary.md').write_bytes(('# SIMD corpus capture\n\n'+
            ('Preparation only; not final validation.\n\n' if args.preparation else 'Frozen source corpus capture; full native suite is separate.\n\n')+
            f"Profile: `{profile}`. Registered/executed: {result['registered']}/{result['executed']}; passed: {result['passed']}; failed: {result['failed']}.\n\n"+
            f"Source: `{initial['head']}`, tree `{initial['tree']}`.\n\n"+
            'No cross-platform equality claim is made until the other host is captured and compared.\n').encode('utf-8'))
        (bundle/'checksums.sha256').write_bytes(''.join(digest(path)+'  '+path.name+'\n' for path in sorted(bundle.iterdir()) if path.is_file()).encode('ascii'))
        if result['success']:
            load_bundle(bundle,cases,policies,allow_preparation=args.preparation)
        (output/'summary.json').write_bytes(encoded(dict(success=result['success'],preparation=args.preparation,
            bundle=str(bundle),source=initial,results={k:v for k,v in result.items() if k!='failures'})))
        if not result['success']:
            raise ValueError('Independent reference failures; retain this attempt and inspect reference-validation.json')
        print('Portable bundle: '+str(bundle),flush=True)
    except Exception as error:
        (output/'failure.json').write_bytes(encoded(dict(success=False,error=str(error),preparation=args.preparation,source=initial)))
        raise
    finally:
        if not packaged:
            package_evidence(run)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run-id',required=True)
    parser.add_argument('--attempt-id',required=True)
    parser.add_argument('--driver',choices=('msvc','clang','apple-clang'),required=True)
    parser.add_argument('--compiler')
    parser.add_argument('--mode',choices=('Release','Debug'),required=True)
    parser.add_argument('--isa',choices=('sse2','avx2','avx512','neon'),required=True)
    parser.add_argument('--backend',type=int,choices=(1,2,3),required=True)
    parser.add_argument('--max-width',type=int,choices=(128,256,512),default=512)
    parser.add_argument('--sanitize',action='store_true')
    parser.add_argument('--expected-commit')
    parser.add_argument('--preparation',action='store_true',help='Explicit dirty-source development check; never final validation')
    args=parser.parse_args()
    if any(not re.fullmatch('[A-Za-z0-9][A-Za-z0-9_.-]*',value) for value in (args.run_id,args.attempt_id)):
        parser.error('Use simple nonempty run/attempt IDs')
    try:
        run_profile(args)
        return 0
    except (ValueError,RuntimeError,OSError,KeyError) as error:
        print('SIMD capture failed: '+str(error),file=sys.stderr)
        return 1


if __name__=='__main__':
    sys.exit(main())
