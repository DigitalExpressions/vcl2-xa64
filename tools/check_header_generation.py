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

from regenerate_headers import ROOT, render, verify


def main():
    output=ROOT/'build'/('header-generation-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    products,metadata=render(ROOT)
    _,failures=verify(ROOT,products)
    assert not failures
    results=[dict(control='clean generation and pristine upstream',passed=True)]
    def fresh(name):
        target=output/name;target.mkdir()
        for directory in ('vendor','adaptation','backends'): shutil.copytree(ROOT/directory,target/directory)
        for name,data in products.items(): (target/name).write_bytes(data)
        return target
    def rejected(name,change):
        target=fresh(name);change(target)
        try: render(target)
        except (ValueError,OSError) as error:
            results.append(dict(control=name,passed=True,diagnostic=str(error)));return
        raise AssertionError('Integrity control accepted invalid input: '+name)
    rejected('mutated-upstream',lambda r:(r/'vendor/vectorclass/vectorclass.h').write_bytes(b'changed upstream\n'))
    rejected('extra-upstream-path',lambda r:(r/'vendor/vectorclass/extra.h').write_bytes(b'extra\n'))
    def bad_patch(root):
        path=root/'adaptation/patches/0001-portability-and-correctness.patch'
        data=path.read_bytes();assert b'\n ' in data
        path.write_bytes(data.replace(b'\n ',b'\n BROKEN-CONTEXT ',1))
    rejected('patch-context-no-fuzz',bad_patch)
    def traversal(root):
        path=root/'adaptation/manifest.json';spec=json.loads(path.read_text())
        spec['copy_products'][0]['output']='../escape.h'
        path.write_text(json.dumps(spec),encoding='utf-8')
    rejected('output-path-escape',traversal)
    stale=fresh('stale-product');(stale/'vectorclass.h').write_bytes(b'hand edited output\n')
    _,failures=verify(stale,render(stale)[0]);assert failures==['vectorclass.h']
    results.append(dict(control='stale or hand-edited product',passed=True))
    canonical=fresh('changed-backend');path=canonical/'backends/x86/x86_classification.h'
    path.write_bytes(path.read_bytes()+b'\n// canonical source changed\n')
    _,failures=verify(canonical,render(canonical)[0]);assert failures==['x86_classification.h']
    results.append(dict(control='changed canonical backend requires regeneration',passed=True))
    ending=fresh('explicit-line-endings');path=ending/'vectorclass.h';data=path.read_bytes()
    assert b'\r\n' in data;path.write_bytes(data.replace(b'\r\n',b'\n'))
    derived=render(ending)[0]
    assert not verify(ending,derived)[1] and verify(ending,derived,strict=True)[1]==['vectorclass.h']
    results.append(dict(control='LF-normalized comparison distinct from exact bytes',passed=True))
    record=dict(passed=len(results),failed=0,upstream_commit=metadata['upstream_commit'],controls=results)
    (output/'results.json').write_bytes((json.dumps(record,indent=2,sort_keys=True)+'\n').encode())
    print(json.dumps(record,indent=2));print('Retained controls:',output)


if __name__=='__main__': main()
