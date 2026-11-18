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

"""Exercise generation integrity controls in retained ignored test directories."""
import datetime
import json
from pathlib import Path
import shutil
import subprocess
import sys
from regenerate_headers import ROOT, render


def main():
    output=ROOT/'build'/('header-generation-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    _,metadata=render(ROOT)
    results=[dict(control='clean vendor, protected originals and ARM graph',passed=True)]
    def fresh(name):
        target=output/name;target.mkdir()
        shutil.copytree(ROOT/'vendor',target/'vendor')
        (target/'adaptation').mkdir()
        shutil.copy2(ROOT/'adaptation/manifest.json',target/'adaptation/manifest.json')
        for path in [*(ROOT.glob('*.h')), ROOT/'README.md', ROOT/'LICENSE', ROOT/'changelog.txt', *ROOT.glob('*.cpp')]:
            if path.is_file(): shutil.copy2(path,target/path.name)
        return target
    def rejected(name,change):
        target=fresh(name);change(target)
        try: render(target)
        except (ValueError,OSError) as error:
            results.append(dict(control=name,passed=True,diagnostic=str(error)));return
        raise AssertionError('Integrity control accepted invalid input: '+name)
    rejected('changed-vendor',lambda r:(r/'vendor/vectorclass/vectorclass.h').write_bytes(b'changed\n'))
    rejected('extra-vendor-path',lambda r:(r/'vendor/vectorclass/extra.h').write_bytes(b'extra\n'))
    rejected('changed-protected-root',lambda r:(r/'vectorclass.h').write_bytes(b'changed\n'))
    def newline(root):
        path=root/'vectorclass.h';data=path.read_bytes();assert b'\r\n' in data
        path.write_bytes(data.replace(b'\r\n',b'\n'))
    rejected('newline-only-root-change',newline)
    rejected('ARM-include-leaks-to-original',lambda r:(r/'vectorclass_neon.h').write_text('#include "vectorclass.h"\n'))
    def append_arm(root, text):
        with (root/'vectorclass_neon.h').open('a') as stream: stream.write(text)
    rejected('inactive-x86-backend-branch',lambda r:append_arm(r,'\n#if VCL_BACKEND == VCL_BACKEND_X86\n#endif\n'))
    rejected('inactive-x86-classification-helper',lambda r:append_arm(r,'\n#if 0\nvcl_detail_x86::nan_mask();\n#endif\n'))
    rejected('inactive-native-AVX-intrinsic',lambda r:append_arm(r,'\n#if 0\n_mm256_add_ps(a,b);\n#endif\n'))
    def old_writer(root):
        p=root/'adaptation/manifest.json';spec=json.loads(p.read_text());spec['copy_products']=[dict(output='vectorclass.h',source='vectorclass_neon.h')];p.write_text(json.dumps(spec))
    rejected('retired-output-list',old_writer)
    before={p.name:p.read_bytes() for p in ROOT.iterdir() if p.is_file()}
    attempt=subprocess.run([sys.executable,'-B',str(ROOT/'tools/regenerate_headers.py'),'--write'],capture_output=True,text=True)
    assert attempt.returncode and 'retired' in attempt.stderr
    assert all((ROOT/n).read_bytes()==data for n,data in before.items())
    results.append(dict(control='write-mode-rejected-with-no-root-writes',passed=True,exit_code=attempt.returncode))
    record=dict(passed=len(results),failed=0,upstream_commit=metadata['upstream_commit'],controls=results)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2));print('Retained controls:',output)


if __name__=='__main__':main()
