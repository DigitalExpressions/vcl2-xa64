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

"""Exercise comparison snapshot creation/reuse and evidence-preserving rejection."""
import datetime
import hashlib
import json
from pathlib import Path
import shutil
from regenerate_headers import ROOT, UPSTREAM_MANIFEST, pristine
from restore_upstream_vcl import restore


def inventory(root):
    return {p.relative_to(root).as_posix(): (hashlib.sha256(p.read_bytes()).hexdigest(),p.stat().st_mtime_ns)
            for p in root.rglob('*') if p.is_file()}


def main():
    output=ROOT/'build'/('upstream-vcl-restore-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    original,_=pristine(ROOT,UPSTREAM_MANIFEST,'.',exact_inventory=False)
    before={name:(hashlib.sha256((ROOT/name).read_bytes()).hexdigest(),(ROOT/name).stat().st_mtime_ns) for name in original}
    positive=output/'positive'
    created=restore(ROOT,positive)
    assert created['status']=='created verified snapshot' and created['files']==28
    saved=inventory(positive)
    reused=restore(ROOT,positive)
    assert reused['status']=='verified existing snapshot' and inventory(positive)==saved
    results=[dict(control='create exact 28-file snapshot',passed=True),dict(control='reuse without rewriting bytes or mtimes',passed=True)]
    def rejected(name,change):
        target=output/name
        shutil.copytree(positive,target)
        change(target)
        saved=inventory(target)
        try:restore(ROOT,target)
        except (ValueError,OSError) as error:
            assert inventory(target)==saved
            results.append(dict(control=name,passed=True,diagnostic=str(error),failed_input_not_overwritten=True))
            return
        raise AssertionError('Accepted invalid snapshot: '+name)
    rejected('changed-file',lambda r:(r/'vectorclass.h').write_bytes(b'changed\n'))
    rejected('missing-file',lambda r:(r/'vectorclass.h').rename(r/'retained-original.h'))
    rejected('extra-file',lambda r:(r/'extra.h').write_bytes(b'extra\n'))
    for label,target in [('outside-build',ROOT/'restore-must-not-be-created'),('build-root',ROOT/'build'),('parent-traversal',ROOT/'build/../restore-must-not-be-created')]:
        try:restore(ROOT,target)
        except ValueError as error:results.append(dict(control=label,passed=True,diagnostic=str(error)))
        else:raise AssertionError('Accepted unsafe target: '+label)
    assert not (ROOT/'restore-must-not-be-created').exists()
    bad=output/'changed-source';bad.mkdir();(bad/'audit').mkdir()
    shutil.copy2(ROOT/UPSTREAM_MANIFEST,bad/UPSTREAM_MANIFEST)
    for name in original:shutil.copy2(ROOT/name,bad/name)
    (bad/'vectorclass.h').write_bytes(b'changed source\n')
    try:restore(bad,bad/'build/snapshot')
    except ValueError as error:
        assert not (bad/'build/snapshot').exists()
        results.append(dict(control='changed source rejected before destination creation',passed=True,diagnostic=str(error)))
    else:raise AssertionError('Copied changed protected source')
    assert before=={name:(hashlib.sha256((ROOT/name).read_bytes()).hexdigest(),(ROOT/name).stat().st_mtime_ns) for name in original}
    record=dict(passed=len(results),failed=0,controls=results,primary_root_bytes_and_mtimes_unchanged=True)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(record,indent=2));print('Retained controls:',output)


if __name__=='__main__':main()
