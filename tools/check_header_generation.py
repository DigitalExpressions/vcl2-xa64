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
from regenerate_headers import ROOT, UPSTREAM_MANIFEST, SSE2NEON_AUDIT, digest, git_object, render


def main():
    output=ROOT/'build'/('header-generation-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    _,metadata=render(ROOT)
    results=[dict(control='clean audited originals, sse2neon and ARM graph',passed=True)]
    def fresh(name):
        target=output/name;target.mkdir()
        shutil.copytree(ROOT/'audit',target/'audit')
        (target/'adaptation').mkdir()
        shutil.copy2(ROOT/'adaptation/manifest.json',target/'adaptation/manifest.json')
        for path in [*(ROOT.glob('*.h')), ROOT/'README.md', ROOT/'LICENSE', ROOT/'changelog.txt', *ROOT.glob('*.cpp')]:
            if path.is_file(): shutil.copy2(path,target/path.name)
        return target
    def rejected(name,change,diagnostic=None):
        target=fresh(name);change(target)
        try: render(target)
        except (ValueError,OSError,KeyError,TypeError) as error:
            if diagnostic is not None and diagnostic not in str(error):
                raise AssertionError('Unexpected integrity diagnostic for '+name+': '+str(error)) from error
            results.append(dict(control=name,passed=True,diagnostic=str(error)));return
        raise AssertionError('Integrity control accepted invalid input: '+name)
    def manifest_change(root,change):
        path=root/UPSTREAM_MANIFEST;data=json.loads(path.read_text());change(data);path.write_text(json.dumps(data))
    rejected('changed-recorded-sha256',lambda r:manifest_change(r,lambda d:d['files'][0].update(sha256='0'*64)))
    rejected('missing-original-inventory-entry',lambda r:manifest_change(r,lambda d:d['files'].pop()))
    rejected('missing-upstream-audit',lambda r:(r/UPSTREAM_MANIFEST).unlink(),'Missing required upstream audit')
    rejected('reintroduced-empty-vendor-directory',lambda r:(r/'vendor').mkdir(),'Retired vendor catalog')
    rejected('reintroduced-vendor-file',lambda r:(r/'vendor').write_bytes(b'extra\n'),'Retired vendor catalog')
    def duplicate_vendor(root):
        path=root/'vendor/sse2neon';path.mkdir(parents=True)
        shutil.copy2(root/'sse2neon.h',path/'sse2neon.h')
    rejected('reintroduced-vendor-header',duplicate_vendor,'Retired vendor catalog')
    rejected('missing-root-sse2neon',lambda r:(r/'sse2neon.h').unlink(),'sse2neon.h')
    rejected('changed-root-sse2neon',lambda r:(r/'sse2neon.h').write_bytes(b'changed\n'),'retained representation differs')
    def sse_newlines(root):
        path=root/'sse2neon.h';data=path.read_bytes();assert b'\r\n' in data
        path.write_bytes(data.replace(b'\r\n',b'\n'))
    rejected('newline-only-sse2neon-change',sse_newlines,'retained representation differs')
    rejected('missing-sse2neon-audit',lambda r:(r/SSE2NEON_AUDIT).unlink(),SSE2NEON_AUDIT)
    def audit_change(root,change):
        path=root/SSE2NEON_AUDIT;data=json.loads(path.read_text());change(data);path.write_text(json.dumps(data))
    rejected('missing-sse2neon-audit-root-path',lambda r:audit_change(r,lambda d:d['retained'].update(paths=[])),'pin only the root')
    rejected('extra-sse2neon-audit-path',lambda r:audit_change(r,lambda d:d['retained']['paths'].append('extra.h')),'pin only the root')
    rejected('changed-sse2neon-audit-schema',lambda r:audit_change(r,lambda d:d.update(schema=99)),'pin only the root')
    for field,value in [('sha256','0'*64),('git_blob','0'*40),('bytes',0)]:
        rejected('changed-sse2neon-audit-'+field,lambda r,f=field,v=value:audit_change(r,lambda d:d['retained'].update({f:v})),
                 'retained representation differs')
    rejected('changed-sse2neon-audit-encoding',lambda r:audit_change(r,lambda d:d['retained'].update(encoding='UTF-8')),
             'representation contract differs')
    rejected('changed-sse2neon-audit-proof',lambda r:audit_change(r,lambda d:d['representation_proof'].update(semantic_source_edits=True)),
             'representation contract differs')
    def coherent_sse_change(root):
        path=root/'sse2neon.h';data=path.read_bytes()+b' changed';path.write_bytes(data)
        audit_change(root,lambda d:d['retained'].update(bytes=len(data),sha256=digest(data),git_blob=git_object('blob',data)))
    rejected('changed-sse2neon-with-matching-retained-pin',coherent_sse_change,'upstream representation proof differs')
    for field,value in [('sha256','0'*64),('git_blob','0'*40),('bytes',0)]:
        rejected('changed-sse2neon-upstream-'+field,lambda r,f=field,v=value:audit_change(r,lambda d:d['upstream'].update({f:v})),
                 'upstream representation proof differs')
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
