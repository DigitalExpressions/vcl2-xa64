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

"""Native Windows x64 baseline validation with controls and retained evidence."""
import argparse
import ctypes
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import struct
import sys
import xml.etree.ElementTree as ET

from validation_evidence import EvidenceRun, FIXTURES, package_evidence, sha


class WindowsRun(EvidenceRun):
    def binary(self, label, path):
        path = Path(path)
        data = path.read_bytes()
        pe = struct.unpack_from('<I', data, 0x3c)[0]
        if data[:2] != b'MZ' or data[pe:pe+4] != b'PE\0\0' or struct.unpack_from('<H', data, pe+4)[0] != 0x8664:
            raise RuntimeError('Expected a native x64 PE executable: ' + str(path))
        return dict(path=str(path), sha256=sha(path), pe_machine='0x8664')


def source_manifest(run, label):
    paths = run.git(label + '-files', 'ls-files', '--cached', '--others', '--exclude-standard', '-z').split('\0')
    rows = [dict(path=p, sha256=sha(run.checkout / p)) if (run.checkout / p).is_file()
            else dict(path=p, sha256=None, deleted=True) for p in sorted(set(paths)) if p]
    fingerprint = hashlib.sha256(''.join(row['path']+'\0'+(row['sha256'] or 'DELETED')+'\n' for row in rows).encode()).hexdigest()
    (run.output / (label + '.json')).write_text(json.dumps(dict(sha256=fingerprint, files=rows), indent=2))
    return fingerprint


def native_host():
    if platform.system() != 'Windows' or struct.calcsize('P') != 8:
        raise RuntimeError('Use native Windows x64 Python in an x64 Visual Studio developer environment')
    kernel = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel.GetCurrentProcess.restype = ctypes.c_void_p
    kernel.IsWow64Process2.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_ushort), ctypes.POINTER(ctypes.c_ushort)]
    process, native = ctypes.c_ushort(), ctypes.c_ushort()
    if not kernel.IsWow64Process2(kernel.GetCurrentProcess(), ctypes.byref(process), ctypes.byref(native)):
        raise RuntimeError('IsWow64Process2 could not establish native execution')
    if native.value != 0x8664 or process.value != 0:
        raise RuntimeError('Windows ARM/x86 emulation cannot establish native x64 coverage')
    return dict(native_machine=hex(native.value), process_machine=hex(process.value), python=sys.version)


def controls(run, compiler, args, generator):
    source = run.output / 'control-source'
    source.mkdir()
    (source / 'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.20)\n'
        'project(windows_controls LANGUAGES CXX)\nset(VCL_BACKEND 1 CACHE STRING "" FORCE)\n'
        'add_subdirectory("${VCL_CHECKOUT}" library)\n'
        'add_executable(runtime runtime.cpp)\ntarget_link_libraries(runtime PRIVATE vcl::runtime)\n'
        'target_compile_definitions(runtime PRIVATE VCL_REQUIRED_INSTRSET=${VCL_REQUIRED_INSTRSET})\n'
        'add_executable(guard guard.cpp)\ntarget_include_directories(guard PRIVATE "${VCL_CHECKOUT}/tests")\n'
        'target_compile_features(guard PRIVATE cxx_std_17)\n'
        'if(VCL_SANITIZE)\nadd_executable(ubsan ubsan.cpp)\n'
        'target_compile_options(ubsan PRIVATE -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)\n'
        'target_link_options(ubsan PRIVATE -fsanitize=undefined)\nendif()\n')
    (source / 'runtime.cpp').write_text('#include "vectorclass.h"\n#include <cstdio>\n'
        'int main(){int level=instrset_detect();std::printf("instrset_detect=%d required=%d pointer_bits=%zu long_bits=%zu arm64=%d\\n",'
        'level,VCL_REQUIRED_INSTRSET,sizeof(void*)*8,sizeof(long)*8,int(VCL_ARCH_ARM64));'
        'return VCL_ARCH_ARM64 || sizeof(void*)!=8 || level<VCL_REQUIRED_INSTRSET;}\n')
    (source / 'guard.cpp').write_text('#include "protected_memory.h"\n'
        'int main(int argc,char**argv){SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);'
        'size_t page=vcl_test::page_size();vcl_test::ProtectedMemory m(3*page);m.writable(page,page);'
        'volatile unsigned char*p=m.data();p[page]=1;p[2*page-1]=2;'
        'if(argc==1){std::printf("page_size=%zu\\n",page);return p[page]+p[2*page-1]!=3;}'
        "int mode=argv[1][0]-'0';size_t offset=mode<2?page-1:2*page;"
        'if(mode%2)p[offset]=5;else return p[offset];return 0;}\n')
    (source / 'ubsan.cpp').write_text('#include <climits>\n#include <cstdio>\n'
        'int main(int argc,char**){if(argc==1){std::puts("positive control");return 0;}'
        'volatile int x=INT_MAX;volatile int y=x+1;std::printf("UB continued %d\\n",int(y));return 0;}\n')
    build = run.output / 'control-build'
    flags = ['-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded'] if args.sanitize else []
    run.command('control-configure', ['cmake', '-S', source, '-B', build, '-G', generator,
        '-DVCL_CHECKOUT='+str(run.checkout), '-DCMAKE_CXX_COMPILER='+compiler,
        '-DVCL_REQUIRED_INSTRSET='+('6' if args.testbench_dir else '2'),
        '-DCMAKE_BUILD_TYPE='+args.mode, '-DVCL_SANITIZE='+('ON' if args.sanitize else 'OFF'),
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON', *flags])
    run.command('control-build', ['cmake','--build',build,'--parallel','3','--verbose'])
    binaries = [run.binary('guard',build/'guard.exe'),run.binary('runtime',build/'runtime.exe')]
    run.command('runtime-control',[build/'runtime.exe'],timeout=60)
    run.command('guard-positive',[build/'guard.exe'],timeout=60)
    for mode in range(4):
        code, _ = run.command('guard-negative-'+str(mode),[build/'guard.exe',str(mode)],required=False,timeout=60)
        if code & 0xffffffff != 0xc0000005:
            raise RuntimeError('Protected page did not produce the expected access violation: '+str(mode))
    if args.sanitize:
        binaries.append(run.binary('ubsan',build/'ubsan.exe'))
        run.command('ubsan-positive',[build/'ubsan.exe'],timeout=60)
        code, output = run.command('ubsan-negative',[build/'ubsan.exe','trigger'],required=False,timeout=60)
        if code in (0,124) or 'runtime error: signed integer overflow' not in output or 'UB continued' in output:
            raise RuntimeError('Strict UBSan runtime unavailable or failed its intentional-UB control')
    (run.output/'control-binaries.json').write_text(json.dumps(binaries,indent=2))


def validate(run, compiler, args, generator):
    controls(run, compiler, args, generator)
    build = run.output/'suite'
    flags = ['-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded'] if args.sanitize else []
    run.command('suite-configure',['cmake','-S',run.checkout,'-B',build,'-G',generator,
        '-DCMAKE_CXX_COMPILER='+compiler,'-DCMAKE_BUILD_TYPE='+args.mode,'-DVCL_BACKEND=1',
        '-DVCL_BUILD_TESTS=ON','-DVCL_BUILD_EXAMPLES=ON','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
        '-DVCL_SANITIZE='+('ON' if args.sanitize else 'OFF'),
        '-DPython3_EXECUTABLE='+sys.executable,
        '-DVCL_TESTBENCH_DIR='+(str(args.testbench_dir.resolve()) if args.testbench_dir else ''),*flags])
    run.command('suite-build',['cmake','--build',build,'--parallel','3','--verbose'])
    _, inventory = run.command('suite-inventory',['ctest','--test-dir',build,'--show-only=json-v1'])
    tests = json.loads(inventory)['tests']
    expected = 1348 if args.testbench_dir else 40
    if len(tests) != expected or not FIXTURES.issubset({t['name'] for t in tests}):
        raise RuntimeError('Unexpected registration; reconcile inventories without removing cases')
    for test in tests:
        if {p['name']:p['value'] for p in test['properties']}.get('TIMEOUT') != 60:
            raise RuntimeError('Changed test deadline: '+test['name'])
    code, _ = run.command('suite-ctest',['ctest','--test-dir',build,'--output-on-failure','--no-tests=error',
        '--timeout','60','--parallel','3','--output-junit','results.xml'],required=False)
    cases = list(ET.parse(build/'results.xml').getroot().iter('testcase'))
    failures = [c.attrib['name'] for c in cases if c.find('failure') is not None or c.find('error') is not None]
    skipped = [c.attrib['name'] for c in cases if c.find('skipped') is not None]
    timeouts = [c.attrib['name'] for c in cases if (c.find('failure') is not None or c.find('error') is not None)
                and 'timeout' in ET.tostring(c,encoding='unicode').lower()]
    binaries = [dict(run.binary(test['name'],test['command'][0]),test=test['name']) for test in tests]
    (run.output/'suite-binaries.json').write_text(json.dumps(binaries,indent=2))
    commands = json.loads((build/'compile_commands.json').read_text())
    for entry in commands:
        flags = entry['command']
        match = re.search(r'CMakeFiles[/\\]([^/\\]+)\.dir[/\\]',flags)
        if not match:
            raise RuntimeError('Unrecognized compilation record')
        target = match[1]
        if any(flag in flags for flag in ['-ffast-math','-Ofast','-march=native','-mcpu=native','/arch:AVX','-mavx']):
            raise RuntimeError('Unexpected math or higher-ISA option')
        if target in {'vcl_detect','vcl_example'}:
            continue
        if args.driver=='msvc':
            if ('/fp:fast' in flags) != (target=='fp_classification') or ('/fp:strict' in flags) == (target=='fp_classification'):
                raise RuntimeError('Unexpected MSVC floating-point flags')
        elif '-ffp-contract=off' not in flags or ('-ffinite-math-only' in flags) != (target=='fp_classification'):
            raise RuntimeError('Unexpected Clang floating-point flags')
        if args.sanitize and not all(f in flags for f in ['-fsanitize=undefined','-fno-sanitize-recover=undefined','-fno-omit-frame-pointer']):
            raise RuntimeError('Incomplete strict UBSan instrumentation')
    _, example = run.command('example',[build/'examples/vcl_example.exe'],timeout=60)
    if example.split()!=['2','5','10','17']:
        raise RuntimeError('Unexpected example output')
    logs = (build/'Testing/Temporary/LastTest.log').read_text(errors='replace')
    result = dict(registered=len(tests),executed=len(cases),passed=len(cases)-len(failures)-len(skipped),
        failed=failures,skipped=skipped,timed_out=timeouts,ctest_exit=code)
    (run.output/'results.json').write_text(json.dumps(result,indent=2))
    if code or failures or skipped or len(cases)!=len(tests) or {c.attrib['name'] for c in cases}!={t['name'] for t in tests}:
        raise RuntimeError('Regression failures, skips, timeouts or incomplete execution; inspect results.json')
    if 'runtime error:' in logs or 'UndefinedBehaviorSanitizer:' in logs:
        raise RuntimeError('Sanitizer diagnostic in suite log')
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver',choices=['msvc','clang'],required=True)
    parser.add_argument('--mode',choices=['Release','Debug'],required=True)
    parser.add_argument('--compiler',help='Explicit native x64 compiler executable')
    parser.add_argument('--sanitize',action='store_true',help='Strict UBSan; requires GNU-driver Clang Debug')
    parser.add_argument('--testbench-dir',type=Path,help='Existing pristine pinned upstream testbench checkout')
    parser.add_argument('--plan',action='store_true')
    args=parser.parse_args()
    if args.sanitize and (args.driver!='clang' or args.mode!='Debug'):
        parser.error('Strict UBSan requires GNU-driver Clang Debug')
    if args.plan:
        print(json.dumps(dict(driver=args.driver,mode=args.mode,sanitize=args.sanitize,
            expected_cases=1348 if args.testbench_dir else 40,timeout_seconds=60,concurrency=3),indent=2))
        return 0
    run=None
    try:
        host=native_host()
        root=Path(__file__).resolve().parents[1]
        output=root/'build'/('windows-native-'+args.driver+'-'+args.mode.lower()+'-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
        output.mkdir(parents=True,exist_ok=False)
        run=WindowsRun(root,output)
        print('Evidence directory: '+str(output),flush=True)
        compiler=shutil.which(args.compiler or ('cl' if args.driver=='msvc' else 'clang++'))
        if not compiler:
            raise RuntimeError('Compiler unavailable; initialize an x64 Visual Studio developer environment')
        for tool in ['cmake','ctest','git','ninja']:
            if not shutil.which(tool):
                raise RuntimeError('Missing required tool: '+tool)
        head=run.git('source-head','rev-parse','HEAD')
        tree=run.git('source-tree','rev-parse','HEAD^{tree}')
        status=run.git('source-status','status','--porcelain')
        run.git('ignored-output','check-ignore',str(output))
        run.command('working-patch',['git','diff','--binary','HEAD'])
        added=run.git('added-files','ls-files','--others','--exclude-standard','-z').split('\0')
        for index,path in enumerate(p for p in added if p):
            code,_=run.command('added-file-patch-'+str(index),['git','diff','--no-index','--binary','--','/dev/null',path],required=False)
            if code!=1:
                raise RuntimeError('Could not capture added source file: '+path)
        fingerprint=source_manifest(run,'source-manifest')
        inventory=dict(host,head=head,tree=tree,working_tree_status=status,source_fingerprint=fingerprint,
            compiler=compiler,driver=args.driver,mode=args.mode,sanitize=args.sanitize,
            UBSAN_OPTIONS=run.env['UBSAN_OPTIONS'],testbench=str(args.testbench_dir) if args.testbench_dir else None)
        (output/'inventory.json').write_text(json.dumps(inventory,indent=2))
        for label, command in [('host',['powershell','-NoProfile','-NonInteractive','-Command',
                '@{cpu=Get-CimInstance Win32_Processor | Select-Object Name,Manufacturer,Architecture;os=Get-CimInstance Win32_OperatingSystem | Select-Object Caption,Version,BuildNumber,OSArchitecture} | ConvertTo-Json -Depth 4']),
                ('compiler',[compiler,'/Bv'] if args.driver=='msvc' else [compiler,'--version']),
                ('cmake',['cmake','--version']),('ctest',['ctest','--version']),('ninja',['ninja','--version'])]:
            run.command(label,command,required=label!='compiler' or args.driver!='msvc')
        if args.testbench_dir:
            directory=args.testbench_dir.resolve()
            if run.git('testbench-head','rev-parse','HEAD',cwd=directory)!='a7293702385e5169a5f58b83177f2a43cd59139b' or run.git('testbench-status','status','--porcelain',cwd=directory):
                raise RuntimeError('Use the pristine pinned upstream testbench checkout')
        run.command('repository-check',[sys.executable,'-B','tools/check_repository.py'])
        result=validate(run,compiler,args,'Ninja')
        final_head=run.git('source-final-head','rev-parse','HEAD')
        final_fingerprint=source_manifest(run,'source-final-manifest')
        if final_head!=head or final_fingerprint!=fingerprint:
            raise RuntimeError('Source changed during validation')
        (output/'summary.json').write_text(json.dumps(dict(success=True,head=head,tree=tree,
            source_fingerprint=fingerprint,native_windows_x64=True,driver=args.driver,mode=args.mode,
            sanitizer='strict UBSan' if args.sanitize else 'not requested',results=result),indent=2))
        package_evidence(run)
        return 0
    except (RuntimeError,OSError,ValueError,ET.ParseError) as error:
        print('Validation failed or unavailable: '+str(error),file=sys.stderr)
        if run:
            (run.output/'summary.json').write_text(json.dumps(dict(success=False,error=str(error)),indent=2))
            package_evidence(run)
        return 1


if __name__=='__main__':
    sys.exit(main())
