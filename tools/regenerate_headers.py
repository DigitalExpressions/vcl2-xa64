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
UPSTREAM_MANIFEST='audit/vectorclass-2.02.04.json'
SSE2NEON_AUDIT='audit/sse2neon-2272251.json'


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


def pristine(root,manifest,directory,*,exact_inventory=True):
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
    if exact_inventory:
        actual={p.relative_to(vendor).as_posix() for p in vendor.rglob('*') if p.is_file()}
        if actual!=set(files): raise ValueError('Missing/extra pristine upstream paths')
    if git_tree(entries)!=metadata['tree']: raise ValueError('Pinned upstream Git tree mismatch')
    return files,metadata


def original_manifest(root):
    # Historical checkouts use their frozen checker, not current-layout fallbacks.
    if not path_in(root,UPSTREAM_MANIFEST).is_file():
        raise ValueError('Missing required upstream audit: '+UPSTREAM_MANIFEST)
    return UPSTREAM_MANIFEST


def verify_sse2neon(root):
    audit_bytes=path_in(root,SSE2NEON_AUDIT).read_bytes()
    audit=json.loads(audit_bytes)
    retained=audit['retained'];upstream=audit['upstream'];proof=audit['representation_proof']
    if audit['schema']!=1 or audit['source_path']!='sse2neon.h' or retained['paths']!=['sse2neon.h']:
        raise ValueError('sse2neon audit must pin only the root sse2neon.h')
    if (retained['encoding']!='Windows-1252' or retained['line_endings']!='CRLF' or retained['final_newline'] is not False
            or upstream['encoding']!='UTF-8' or upstream['line_endings']!='LF' or upstream['final_newline'] is not True
            or proof['matches_retained_bytes_exactly'] is not True or proof['semantic_source_edits'] is not False):
        raise ValueError('sse2neon audit representation contract differs')
    data=path_in(root,'sse2neon.h').read_bytes()
    if len(data)!=retained['bytes'] or digest(data)!=retained['sha256'] or git_object('blob',data)!=retained['git_blob']:
        raise ValueError('sse2neon retained representation differs from upstream audit')
    bare=data.replace(b'\r\n',b'')
    if b'\n' in bare or b'\r' in bare or data.endswith((b'\n',b'\r')):
        raise ValueError('sse2neon retained line endings/final newline differ')
    # Analysis only: never write or normalize the protected root header.
    restored=(data.decode('cp1252').replace('\r\n','\n')+'\n').encode('utf-8')
    if len(restored)!=upstream['bytes'] or digest(restored)!=upstream['sha256'] or git_object('blob',restored)!=upstream['git_blob']:
        raise ValueError('sse2neon upstream representation proof differs')
    return dict(path='sse2neon.h',sha256=digest(data),bytes=len(data),
        audit_sha256=digest(audit_bytes),upstream_commit=audit['commit'],upstream_tree=audit['tree'])



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
    if (root/'vendor').exists() or (root/'vendor').is_symlink():
        raise ValueError('Retired vendor catalog must not be reintroduced')
    manifest=original_manifest(root)
    original, upstream = pristine(root, manifest, '.', exact_inventory=False)
    rows = []
    for name, expected in sorted(original.items()):
        actual = path_in(root, name).read_bytes()
        if actual != expected:
            raise ValueError('Changed protected root original (exact bytes required): ' + name)
        rows.append(dict(path=name, sha256=digest(actual), exact_bytes=True))
    sse2neon=verify_sse2neon(root)
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
        sse2neon=sse2neon, generation_retired=True, arm_x86_code_rejected=True)


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
    except (OSError,ValueError,KeyError,TypeError) as error:
        print('Source integrity failed: '+str(error),file=sys.stderr)
        return 1


if __name__=='__main__': sys.exit(main())
