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

"""Prove frozen-source checks reject changed, omitted, extra and unsafe paths."""
import datetime
import hashlib
import json
from pathlib import Path
from capture_source import verify_capture

ROOT=Path(__file__).resolve().parents[1]


def main():
    output=ROOT/'build'/('source-capture-check-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True)
    results=[]
    def fixture(name):
        directory=output/name;source=directory/'source';source.mkdir(parents=True)
        (source/'core.h').write_bytes(b'// stable fixture\n')
        files=[dict(path='core.h',sha256=hashlib.sha256(b'// stable fixture\n').hexdigest())]
        digest=hashlib.sha256(('core.h\0'+files[0]['sha256']+'\n').encode()).hexdigest()
        manifest=directory/'capture.json';manifest.write_text(json.dumps(dict(schema=1,kind='frozen-uncommitted-source',files=files,fingerprint=digest)))
        return source,manifest
    source,manifest=fixture('positive');verify_capture(source,manifest)
    results.append(dict(control='complete fixture accepted',passed=True))
    def rejected(name,mutate):
        source,manifest=fixture(name);mutate(source,manifest)
        try:verify_capture(source,manifest)
        except (ValueError,OSError) as error:
            results.append(dict(control=name,passed=True,diagnostic=str(error)));return
        raise AssertionError('Capture verifier accepted invalid source: '+name)
    rejected('changed-bytes',lambda s,m:(s/'core.h').write_bytes(b'changed\n'))
    rejected('newline-only-change',lambda s,m:(s/'core.h').write_bytes(b'// stable fixture\r\n'))
    rejected('missing-file',lambda s,m:(s/'core.h').rename(s/'retained-original.h'))
    rejected('extra-added-file',lambda s,m:(s/'extra.h').write_bytes(b'// unlisted addition\n'))
    def metadata(source,manifest,change):
        data=json.loads(manifest.read_text());change(data);manifest.write_text(json.dumps(data))
    rejected('fingerprint-mismatch',lambda s,m:metadata(s,m,lambda d:d.update(fingerprint='0'*64)))
    rejected('duplicate-path',lambda s,m:metadata(s,m,lambda d:d['files'].append(d['files'][0])))
    def traversal(source,manifest):
        data=json.loads(manifest.read_text());data['files'][0]['path']='../core.h'
        data['fingerprint']=hashlib.sha256(('../core.h\0'+data['files'][0]['sha256']+'\n').encode()).hexdigest()
        manifest.write_text(json.dumps(data))
    rejected('path-traversal',traversal)
    record=dict(passed=len(results),failed=0,controls=results)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2));print('Retained controls:',output)


if __name__=='__main__':main()
