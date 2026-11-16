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

"""Verify pristine upstream and reproduce shipped headers without a build step.

Default/--check is read-only. --write replaces only registered root products.
No downloads, Git mutations, patch fuzz, external patch program or compiler.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys

ROOT=Path(__file__).resolve().parents[1]


def digest(data): return hashlib.sha256(data).hexdigest()
def lf(data): return data.replace(b'\r\n',b'\n')


def path_in(root,name):
    relative=PurePosixPath(name)
    if relative.is_absolute() or '..' in relative.parts or '\\' in name or ':' in name:
        raise ValueError('Unsafe source path: '+repr(name))
    path=root/name
    if not path.resolve().is_relative_to(root.resolve()) or path.is_symlink():
        raise ValueError('Source escapes its directory: '+name)
    return path


def git_object(kind,data):
    return hashlib.sha1(kind.encode()+b' '+str(len(data)).encode()+b'\0'+data).hexdigest()


def git_tree(entries):
    tree={}
    for name,mode,blob in entries:
        parts=PurePosixPath(name).parts
        node=tree
        for part in parts[:-1]: node=node.setdefault(part,{})
        if parts[-1] in node: raise ValueError('Duplicate vendor path: '+name)
        node[parts[-1]]=(mode,blob)
    def encode(node):
        rows=[]
        for name,item in sorted(node.items(),key=lambda pair:(pair[0]+('/' if isinstance(pair[1],dict) else '')).encode()):
            mode,blob=('40000',encode(item)) if isinstance(item,dict) else item
            rows.append(mode.encode()+b' '+name.encode()+b'\0'+bytes.fromhex(blob))
        return git_object('tree',b''.join(rows))
    return encode(tree)


def pristine(root,manifest,directory):
    metadata=json.loads(path_in(root,manifest).read_text(encoding='utf-8'))
    vendor=path_in(root,directory)
    files={};entries=[]
    for row in metadata['files']:
        name=row['path'];data=path_in(vendor,name).read_bytes()
        if name in files or digest(data)!=row['sha256']:
            raise ValueError('Changed or duplicated pristine upstream file: '+name)
        blob=git_object('blob',data)
        if blob!=row['git_blob'] or row['mode'] not in {'100644','100755'}:
            raise ValueError('Upstream Git blob/mode mismatch: '+name)
        files[name]=data;entries.append((name,row['mode'],blob))
    actual={p.relative_to(vendor).as_posix() for p in vendor.rglob('*') if p.is_file()}
    if actual!=set(files): raise ValueError('Missing/extra pristine upstream paths')
    if git_tree(entries)!=metadata['tree']: raise ValueError('Pinned upstream Git tree mismatch')
    return files,metadata


def apply_patch(files,data):
    """Apply exact unified text hunks to LF-normalized upstream files."""
    lines=data.decode('utf-8').splitlines(keepends=True)
    i=0;changed=[]
    while i<len(lines):
        if not lines[i].startswith('--- a/') or i+1>=len(lines) or not lines[i+1].startswith('+++ b/'):
            raise ValueError('Expected ordinary unified file header')
        name=lines[i][6:].rstrip('\n');new_name=lines[i+1][6:].rstrip('\n');i+=2
        if name!=new_name or name not in files or name in changed:
            raise ValueError('Unexpected or repeated patch path: '+name)
        original=files[name].decode('utf-8').splitlines(keepends=True)
        output=[];position=0;hunks=0
        while i<len(lines) and lines[i].startswith('@@ '):
            header=re.fullmatch(r'@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@[^\n]*\n',lines[i])
            if not header: raise ValueError('Malformed patch hunk: '+name)
            start=int(header[1]);old_count=int(header[2] or '1');new_count=int(header[4] or '1')
            start=start-1 if old_count else start
            if start<position or start>len(original): raise ValueError('Overlapping/out-of-range hunk: '+name)
            output.extend(original[position:start]);position=start;i+=1;old_used=new_used=0
            while old_used<old_count or new_used<new_count:
                if i>=len(lines) or not lines[i] or lines[i][0] not in ' +-':
                    raise ValueError('Incomplete patch hunk: '+name)
                kind=lines[i][0];text=lines[i][1:];i+=1
                if kind in ' -':
                    if position>=len(original) or original[position]!=text:
                        raise ValueError('Patch context mismatch, no fuzz allowed: '+name)
                    position+=1;old_used+=1
                if kind in ' +': output.append(text);new_used+=1
                if old_used>old_count or new_used>new_count: raise ValueError('Patch count mismatch: '+name)
            hunks+=1
        if not hunks: raise ValueError('Patch file has no hunks: '+name)
        output.extend(original[position:]);files[name]=''.join(output).encode();changed.append(name)
    return changed


def render(root):
    config=json.loads(path_in(root,'adaptation/manifest.json').read_text(encoding='utf-8'))
    if config['schema']!=1: raise ValueError('Unsupported adaptation schema')
    original,upstream=pristine(root,config['upstream_manifest'],config['upstream_directory'])
    files={name:lf(data) for name,data in original.items()}
    patches=[]
    for name in config['patches']:
        data=path_in(root,name).read_bytes()
        patches.append(dict(path=name,sha256=digest(data),changed_paths=apply_patch(files,data)))
    products={};sources=[]
    def product(name,data):
        if len(PurePosixPath(name).parts)!=1 or Path(name).suffix not in {'.h','.cpp'} or name in products:
            raise ValueError('Invalid or duplicate root product: '+name)
        path_in(root,name);products[name]=data
    for row in config['upstream_products']:
        data=files[row['upstream']]
        if row['eol']=='crlf': data=data.replace(b'\n',b'\r\n')
        elif row['eol']!='lf': raise ValueError('Unsupported product line ending')
        product(row['output'],data)
    for row in config['copy_products']:
        data=path_in(root,row['source']).read_bytes()
        sources.append(dict(path=row['source'],sha256=digest(data)))
        product(row['output'],data)
    return products,dict(upstream_commit=upstream['commit'],upstream_tree=upstream['tree'],
        pristine_upstream_files=len(original),patches=patches,backend_sources=sources)


def verify(root,products,strict=False):
    rows=[];failures=[]
    for name,expected in sorted(products.items()):
        path=path_in(root,name);actual=path.read_bytes() if path.is_file() else None
        match=actual==expected
        normalized=actual is not None and lf(actual)==lf(expected)
        if not normalized or (strict and not match): failures.append(name)
        rows.append(dict(path=name,expected_sha256=digest(expected),actual_sha256=digest(actual) if actual is not None else None,
            exact_bytes=match,lf_normalized_equal=normalized))
    return rows,failures


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    mode=parser.add_mutually_exclusive_group();mode.add_argument('--check',action='store_true');mode.add_argument('--write',action='store_true')
    parser.add_argument('--strict-bytes',action='store_true',help='Require exact generated bytes, including declared line endings')
    parser.add_argument('--json',type=Path,help='Save a new verification record; existing records are never overwritten')
    args=parser.parse_args()
    if args.json and args.json.exists(): parser.error('Verification record exists; use a new path')
    try:
        products,record=render(ROOT)
        if args.write:
            for name,data in products.items(): path_in(ROOT,name).write_bytes(data)
        rows,failures=verify(ROOT,products,args.strict_bytes)
        record.update(schema=1,mode='write' if args.write else 'check',products=rows,
            generated_products=len(products),strict_bytes=args.strict_bytes,failures=failures,success=not failures)
        if args.json:
            args.json.parent.mkdir(parents=True,exist_ok=True)
            args.json.write_bytes((json.dumps(record,indent=2,sort_keys=True)+'\n').encode())
        print(json.dumps(dict(success=not failures,pristine_upstream_files=record['pristine_upstream_files'],
            generated_products=len(products),exact_byte_matches=sum(r['exact_bytes'] for r in rows),failures=failures),indent=2))
        return int(bool(failures))
    except (OSError,ValueError,KeyError) as error:
        print('Header generation failed: '+str(error),file=sys.stderr);return 1


if __name__=='__main__': sys.exit(main())
