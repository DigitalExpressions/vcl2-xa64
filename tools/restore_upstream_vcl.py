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

"""Reconstruct a byte-exact VCL comparison snapshot under ignored build/.

No network, Git configuration or source checkout change is needed. The shipped
root originals and retained upstream README must match every recorded SHA256, Git blob and
the complete upstream Git tree before copying. Existing snapshots are verified
and reused without overwriting changed, missing or extra files.
"""
import argparse
import json
from pathlib import Path
import sys
from regenerate_headers import ROOT, original_manifest, pristine


def restore(root, output):
    root=root.resolve()
    output=output.absolute()
    build=(root/'build').resolve()
    resolved=output.resolve()
    if resolved==build or not resolved.is_relative_to(build):
        raise ValueError('Comparison snapshot must be a child of this checkout\'s ignored build/')
    # Reject symlink/junction destinations and parents, including links whose
    # current targets happen to remain inside build/.
    for path in [output,*output.parents]:
        if path.resolve()==root: break
        if path.is_symlink() or (path.exists() and getattr(path.stat(),'st_file_attributes',0)&1024):
            raise ValueError('Linked comparison snapshot path is not allowed')
    output=resolved
    manifest=original_manifest(root)
    files,metadata=pristine(root,manifest,'.',exact_inventory=False)
    if output.exists():
        pristine(root,manifest,str(output.relative_to(root)).replace('\\','/'))
        return dict(status='verified existing snapshot',files=len(files),commit=metadata['commit'],tree=metadata['tree'],output=str(output))
    output.mkdir(parents=True,exist_ok=False)
    for name,data in files.items():
        dest=output/name
        dest.parent.mkdir(parents=True,exist_ok=True)
        dest.write_bytes(data)
    pristine(root,manifest,str(output.relative_to(root)).replace('\\','/'))
    return dict(status='created verified snapshot',files=len(files),commit=metadata['commit'],tree=metadata['tree'],output=str(output))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,default=ROOT/'build/upstream/vectorclass-6675774')
    args=parser.parse_args()
    try:
        print(json.dumps(restore(ROOT,args.output),indent=2))
        return 0
    except (OSError,ValueError,KeyError) as error:
        print('Upstream snapshot reconstruction failed: '+str(error),file=sys.stderr)
        return 1


if __name__=='__main__':sys.exit(main())
