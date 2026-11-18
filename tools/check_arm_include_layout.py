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

"""Native M2 facade/include isolation, installed-header and target rejection checks."""
import argparse
import datetime
import json
from pathlib import Path
import platform
import re
import shutil
import subprocess
from regenerate_headers import render
from validation_evidence import EvidenceRun, sha

ROOT=Path(__file__).resolve().parents[1]


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    if platform.system()!='Darwin' or platform.machine()!='arm64':
        parser.error('Use native macOS arm64; native Windows checks are a separate handoff')
    output=args.output or ROOT/'build'/('arm-include-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True,exist_ok=False)
    run=EvidenceRun(ROOT,output)
    _,before=render(ROOT)
    compiler=subprocess.check_output(['xcrun','--find','clang++'],text=True).strip()
    installed=output/'include-only';installed.mkdir()
    for p in [*ROOT.glob('*_neon.h'),ROOT/'vcl2_xa64.h',ROOT/'vcl_config.h',ROOT/'sse2neon.h']:
        shutil.copy2(p,installed/p.name)
    source=output/'consumer.cpp'
    source.write_text('#include "vcl2_xa64.h"\n'
        '#ifdef VECTORCLASS_H\n#error Original x86 aggregate leaked into ARM\n#endif\n'
        '#if VCL_BACKEND == VCL_BACKEND_NEON && defined(SSE2NEON_H)\n#error Compatibility leaked into Direct Neon\n#endif\n'
        '#ifdef VCL_NAMESPACE\nusing namespace VCL_NAMESPACE;\n#endif\n'
        'int main(){Vec4f x(0);Vec4i i(1);return horizontal_add(exp(x)+cos(x)+sinh(x))!=8 || horizontal_add(i)!=4;}\n')
    records=[]
    for backend,width in [(2,512),(3,128),(3,256),(3,512)]:
        name=('compat' if backend==2 else 'neon')+str(width)
        flags=['-std=c++17','-arch','arm64','-O2','-ffp-contract=off','-DVCL_BACKEND='+str(backend),'-DMAX_VECTOR_SIZE='+str(width)]
        if backend==3:flags+=['-DVCL_ENABLE_EXPERIMENTAL_NEON=1']
        _,trace=run.command(name+'-includes',[compiler,*flags,'-I'+str(ROOT),'-H','-fsyntax-only',source],timeout=60)
        paths=[Path(m[1]) for line in trace.splitlines() if (m:=re.match(r'^\.+ (.+)$',line))]
        local=[p.relative_to(ROOT).as_posix() for p in paths if p.is_relative_to(ROOT)]
        protected={r['path'] for r in before['root_originals']}
        if any(p in protected or p.startswith('vendor/vectorclass/') or p.startswith('backends/') for p in local):
            raise RuntimeError('ARM include trace reached protected/retired source: '+name)
        if backend==3 and any('compat_neon.h' in p or 'sse2neon' in p for p in local):
            raise RuntimeError('Direct Neon include trace reached compatibility')
        exe=output/(name+'-consumer')
        run.command(name+'-installed-build',[compiler,*flags,'-DVCL_NAMESPACE=isolated','-I'+str(installed),source,'-o',exe],timeout=60)
        _,description=run.command(name+'-file',['file','-b',exe],timeout=60)
        if not description.strip().startswith('Mach-O 64-bit executable arm64'):raise RuntimeError('Non-native consumer')
        run.command(name+'-run',[exe],timeout=60)
        records.append(dict(profile=name,includes=local,binary_sha256=sha(exe),native_arm64=True,namespace='isolated',result='passed',installed_headers_have_no_original_x86_files=True))
    negative=[('wrong-backend',['-DVCL_BACKEND=1'],'x86 backend requires'),
              ('invalid-backend',['-DVCL_BACKEND=99'],'Invalid VCL_BACKEND'),
              ('missing-opt-in',['-DVCL_BACKEND=3'],'VCL_ENABLE_EXPERIMENTAL_NEON'),
              ('invalid-width',['-DMAX_VECTOR_SIZE=192'],'MAX_VECTOR_SIZE must'),
              ('arm64ec',['-D_M_ARM64EC=1'],'ARM64EC is not supported'),
              ('unsupported-target',['-U__aarch64__','-U__arm64__'],'Unsupported compiler target')]
    empty=output/'flags-positive.cpp';empty.write_text('int flags_positive;\n')
    for name,flags,diagnostic in negative:
        base=[compiler,'-std=c++17','-arch','arm64',*flags,'-I'+str(ROOT),'-fsyntax-only']
        run.command(name+'-flags-positive',base+[empty],timeout=60)
        code,text=run.command(name+'-rejected',base+[source],required=False,timeout=60)
        if code in (0,124) or diagnostic not in text:raise RuntimeError('Target/override rejection failed: '+name)
    _,after=render(ROOT)
    if before!=after:raise RuntimeError('Source integrity changed during include checks')
    record=dict(success=True,profiles=records,negative_controls=len(negative),root_originals=28,vendor_files=28,integrity_unchanged=True)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2))


if __name__=='__main__':main()
