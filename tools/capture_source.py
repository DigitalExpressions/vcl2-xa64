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

"""Capture complete uncommitted Git sources without creating a commit or index edit."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def digest(data):
    return hashlib.sha256(data).hexdigest()


def fingerprint(files):
    return digest(''.join(r['path']+'\0'+r['sha256']+'\n' for r in files).encode('utf-8'))


def safe_path(root, name):
    path = PurePosixPath(name)
    if path.is_absolute() or '..' in path.parts or '\\' in name or ':' in name:
        raise ValueError('Unsafe captured source path: '+name)
    target = root/name
    if not target.resolve().is_relative_to(root.resolve()) or target.is_symlink():
        raise ValueError('Captured source escape/symlink: '+name)
    return target


def verify_capture(root, manifest_path):
    record = json.loads(manifest_path.read_text())
    if record.get('schema') != 1 or record.get('kind') != 'frozen-uncommitted-source':
        raise ValueError('Not an explicit captured-source manifest')
    files = record['files']
    names = [r['path'] for r in files]
    if names != sorted(set(names), key=lambda n:n.encode('utf-8')):
        raise ValueError('Captured source inventory must be unique and UTF-8-byte sorted')
    if fingerprint(files) != record['fingerprint']:
        raise ValueError('Captured source fingerprint mismatch')
    for row in files:
        path = safe_path(root, row['path'])
        if digest(path.read_bytes()) != row['sha256']:
            raise ValueError('Captured source changed: '+row['path'])
    actual = set()
    for path in root.rglob('*'):
        relative = path.relative_to(root)
        if relative.parts[0] == 'build':
            continue
        if path.is_symlink():
            raise ValueError('Captured source contains a symlink: '+relative.as_posix())
        if path.is_file():
            actual.add(relative.as_posix())
    if actual != set(names):
        raise ValueError('Captured source has missing/extra files: '+repr(sorted(actual.symmetric_difference(names))))
    return record


def capture(checkout, output):
    def git(*args):
        return subprocess.check_output(['git','--no-optional-locks',*args],cwd=checkout)
    # Include tracked files and every nonignored addition; never use a staged-only patch.
    names = {n for n in git('ls-files','--cached','--others','--exclude-standard','-z').decode().split('\0') if n}
    names = sorted((n for n in names if safe_path(checkout,n).is_file()),key=lambda n:n.encode('utf-8'))
    tracked = {n for n in git('ls-tree','-r','--name-only','-z','HEAD').decode().split('\0') if n}
    if output.exists():
        raise ValueError('Capture destination exists; choose a new attempt')
    output.mkdir(parents=True)
    dest = output/'source'; dest.mkdir()
    files = []
    for name in names:
        path = safe_path(checkout,name)
        target = safe_path(dest,name); target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(path,target)
        files.append(dict(path=name,sha256=digest(target.read_bytes()),bytes=target.stat().st_size))
    patch = git('diff','--binary','HEAD','--','.')
    (output/'implementation.patch').write_bytes(patch)
    record = dict(schema=1,kind='frozen-uncommitted-source',
        base_commit=git('rev-parse','HEAD').decode().strip(),base_tree=git('rev-parse','HEAD^{tree}').decode().strip(),
        branch=git('branch','--show-current').decode().strip(),
        patch_sha256=digest(patch),patch_scope='tracked changes; all additions retained in source archive and added_paths',added_paths=sorted(set(names)-tracked),deleted_paths=sorted(tracked-set(names)),
        ordering='relative paths sorted by UTF-8 bytes; path + NUL + SHA256 hex + newline',
        fingerprint=fingerprint(files),files=files)
    manifest = output/'capture.json';manifest.write_text(json.dumps(record,indent=2)+'\n')
    verify_capture(dest,manifest)
    # Recheck source after copying, so moving edits cannot silently enter a capture.
    for row in files:
        if digest((checkout/row['path']).read_bytes()) != row['sha256']:
            raise ValueError('Implementation changed while being captured: '+row['path'])
    if git('diff','--binary','HEAD','--','.') != patch:
        raise ValueError('Implementation patch changed during capture')
    archive=output/'captured-source.zip'
    with zipfile.ZipFile(archive,'x',zipfile.ZIP_DEFLATED) as z:
        for row in files:z.write(dest/row['path'],'source/'+row['path'])
        for p in [manifest,output/'implementation.patch']:z.write(p,p.name)
    with zipfile.ZipFile(archive) as z:
        if z.testzip() is not None:raise ValueError('Captured source archive CRC failed')
        for row in files:
            if digest(z.read('source/'+row['path'])) != row['sha256']:raise ValueError('Capture archive hash mismatch')
    receipt=dict(archive=archive.name,sha256=digest(archive.read_bytes()),fingerprint=record['fingerprint'],files=len(files))
    (output/'archive-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
    (output/'checksums.sha256').write_text(''.join(digest(p.read_bytes())+'  '+p.name+'\n' for p in [archive,manifest,output/'implementation.patch']))
    print(json.dumps(receipt,indent=2));print('Captured source:',dest)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--checkout',type=Path,default=ROOT)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--verify-manifest',type=Path,help='Verify an existing complete capture without Git')
    args=parser.parse_args()
    if args.verify_manifest:
        if args.output:parser.error('Select capture output or manifest verification, not both')
        record=verify_capture(args.checkout.resolve(),args.verify_manifest.resolve())
        print(json.dumps(dict(verified=True,fingerprint=record['fingerprint'],files=len(record['files'])),indent=2))
    elif args.output:
        capture(args.checkout.resolve(),args.output.resolve())
    else:parser.error('Choose --output or --verify-manifest')


if __name__=='__main__':main()
