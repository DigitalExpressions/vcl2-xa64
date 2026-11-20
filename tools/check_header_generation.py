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
from regenerate_headers import ROOT, UPSTREAM_MANIFEST, UPSTREAM_README, SSE2NEON_AUDIT, digest, git_object, render, verify
from restore_upstream_vcl import restore


def main():
    output=ROOT/'build'/('header-generation-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    _,metadata=render(ROOT)
    results=[dict(control='clean audited originals, sse2neon and ARM graph',passed=True)]
    def fresh(name):
        target=output/name;target.mkdir()
        shutil.copytree(ROOT/'audit',target/'audit')
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
    rejected('missing-retained-upstream-readme',lambda r:(r/UPSTREAM_README).unlink(),'vectorclass-README.md')
    rejected('changed-retained-upstream-readme',lambda r:(r/UPSTREAM_README).write_bytes(b'changed\n'),
             'pristine upstream file: README.md')
    target=fresh('editable-fork-readme')
    (target/'README.md').write_text('# Editable fork guide\n')
    _,proof=render(target)
    assert proof['protected_root_files']==27 and proof['pristine_upstream_files']==28
    results.append(dict(control='editable-fork-readme-with-all-upstream-bytes-verified',passed=True))
    snapshot=target/'build/upstream'
    receipt=restore(target,snapshot)
    assert receipt['files']==28 and (snapshot/'README.md').read_bytes()==(target/UPSTREAM_README).read_bytes()
    assert restore(target,snapshot)['status']=='verified existing snapshot'
    results.append(dict(control='restore-complete-upstream-tree-with-original-readme',passed=True))
    (snapshot/'README.md').write_bytes(b'changed\n')
    try: restore(target,snapshot)
    except ValueError as error:
        assert 'pristine upstream file: README.md' in str(error)
        assert (snapshot/'README.md').read_bytes()==b'changed\n'
        results.append(dict(control='changed-comparison-readme-rejected-without-overwrite',passed=True))
    else: raise AssertionError('Changed comparison README was accepted')
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
    rejected('reintroduced-empty-adaptation-directory',lambda r:(r/'adaptation').mkdir(),'Retired adaptation catalog')
    rejected('reintroduced-adaptation-file',lambda r:(r/'adaptation').write_bytes(b'extra\n'),'Retired adaptation catalog')
    def retired_file(root, name, data):
        path=root/'adaptation'/name;path.parent.mkdir(parents=True);path.write_text(data)
    rejected('reintroduced-adaptation-manifest',lambda r:retired_file(r,'manifest.json',
        json.dumps(dict(schema=2,patches=[],copy_products=[],upstream_products=[]))),'Retired adaptation catalog')
    rejected('reintroduced-adaptation-patch',lambda r:retired_file(r,'patches/old.patch','historical patch\n'),
             'Retired adaptation catalog')
    rejected('reintroduced-adaptation-include',lambda r:retired_file(r,'include/vectorclass.h','// duplicate\n'),
             'Retired adaptation catalog')
    try: verify(ROOT,{'vectorclass.h':b'forbidden generated output'})
    except ValueError as error:
        assert 'Generation is retired' in str(error)
        results.append(dict(control='retired-output-list',passed=True,diagnostic=str(error)))
    else: raise AssertionError('Integrity control accepted generated root products')
    before={p.name:p.read_bytes() for p in ROOT.iterdir() if p.is_file()}
    attempt=subprocess.run([sys.executable,'-B',str(ROOT/'tools/regenerate_headers.py'),'--write'],capture_output=True,text=True)
    assert attempt.returncode and 'retired' in attempt.stderr
    assert all((ROOT/n).read_bytes()==data for n,data in before.items())
    results.append(dict(control='write-mode-rejected-with-no-root-writes',passed=True,exit_code=attempt.returncode))
    record=dict(passed=len(results),failed=0,upstream_commit=metadata['upstream_commit'],controls=results)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2));print('Retained controls:',output)


if __name__=='__main__':main()
