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

"""Verify protected upstream originals and the separated ARM include graph.

All modes are read-only. The old adapted-root writer is permanently retired.
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



def check_arm_source(path):
    # Preserve quoted literals while removing comments. Check inactive branches
    # too: ARM-owned sources must not accumulate old x86 repairs again.
    token = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
    code = token.sub(lambda m: ' ' if m[0].startswith(('/', '/*')) else m[0], path.read_text())
    forbidden = [r'\bVCL_BACKEND_X86\b', r'\bvcl_detail_x86\b',
        r'\b__(?:AVX|FMA|XOP)\w*', r'\b(?:__x86_64__|__i386__?|__amd64__?|_M_X64|_M_AMD64|_M_IX86)\b',
        r'\b_mm(?:256|512)_\w*', r'\b__m(?:256|512)\w*', r'\b__mmask\d*',
        r'\bINSTRSET\s*(?:>=|==)\s*(?:[7-9]|1[0-2])\b',
        r'\bINSTRSET\s*>\s*(?:[6-9]|1[0-2])\b',
        r'#\s*include\s*[<"](?:x86intrin|immintrin)[.h>"/]*']
    for pattern in forbidden:
        match = re.search(pattern, code)
        if match:
            raise ValueError('x86-only code in ARM source: '+path.name+' -> '+match[0])

def render(root):
    original, upstream = pristine(root, 'vendor/vectorclass.json', 'vendor/vectorclass')
    rows = []
    for name, expected in sorted(original.items()):
        actual = path_in(root, name).read_bytes()
        if actual != expected:
            raise ValueError('Changed protected root original (exact bytes required): ' + name)
        rows.append(dict(path=name, sha256=digest(actual), exact_bytes=True))
    if (root/'sse2neon.h').read_bytes() != (root/'vendor/sse2neon/sse2neon.h').read_bytes():
        raise ValueError('sse2neon dependency copy differs from retained vendor dependency')
    family = sorted(root.glob('*_neon.h'))
    if not family or not (root/'vcl2_xa64.h').is_file():
        raise ValueError('Missing separated ARM family or facade')
    protected_headers = {n for n in original if n.endswith('.h')}
    for path in family:
        check_arm_source(path)
        for header in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', path.read_text(), re.M):
            if header in protected_headers:
                raise ValueError('ARM include reaches protected x86 original: '+path.name+' -> '+header)
            path_in(root, header)
            if not (root/header).is_file():
                raise ValueError('Missing ARM dependency: '+header)
    config = json.loads((root/'adaptation/manifest.json').read_text())
    if config.get('schema') != 2 or config.get('patches') or config.get('copy_products') or config.get('upstream_products'):
        raise ValueError('Retired adaptation writer must have no active outputs or patches')
    return {}, dict(upstream_commit=upstream['commit'], upstream_tree=upstream['tree'],
        pristine_upstream_files=len(original), protected_root_files=len(rows),
        root_originals=rows, arm_sources=[dict(path=p.name,sha256=digest(p.read_bytes())) for p in family],
        generation_retired=True, arm_x86_code_rejected=True)


def verify(root, products, strict=False):
    if products:
        raise ValueError('Generation is retired; no root write products are allowed')
    return [], []


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    mode=parser.add_mutually_exclusive_group()
    mode.add_argument('--check',action='store_true')
    mode.add_argument('--write',action='store_true',help='Retired: always rejected without writing')
    parser.add_argument('--strict-bytes',action='store_true',help='All original checks always require exact bytes')
    parser.add_argument('--json',type=Path)
    args=parser.parse_args()
    if args.write:
        parser.error('Header generation is retired; protected root originals cannot be written')
    if args.json and args.json.exists():
        parser.error('Verification record exists; use a new path')
    try:
        _,record=render(ROOT)
        record.update(success=True,mode='check',generated_products=0,failures=[])
        if args.json:
            args.json.parent.mkdir(parents=True,exist_ok=True)
            args.json.write_text(json.dumps(record,indent=2,sort_keys=True)+'\n')
        print(json.dumps({k:record[k] for k in ['success','pristine_upstream_files','protected_root_files','generated_products','generation_retired']},indent=2))
        return 0
    except (OSError,ValueError,KeyError) as error:
        print('Source integrity failed: '+str(error),file=sys.stderr)
        return 1


if __name__=='__main__': sys.exit(main())
