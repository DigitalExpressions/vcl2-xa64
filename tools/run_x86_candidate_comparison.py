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

"""Compare a reviewed, uncommitted source patch without creating a Git commit.

Use the standard comparison options plus --candidate-patch. --fork-commit is
the candidate's base; --upstream-commit can be that same unmodified base for a
before/after comparison, or original upstream for a separate comparison.
Only the ignored exported snapshot is patched. The index/checkout are untouched.
"""
import argparse
import hashlib
from pathlib import Path, PurePosixPath
import re
import sys
import tarfile

import run_x86_upstream_comparison as comparison
from regenerate_headers import render as render_headers, verify as verify_headers


def supported_path(path):
    name=PurePosixPath(path)
    if name.is_absolute() or '..' in name.parts or '\\' in path or ':' in path or not name.parts:
        return False
    if len(name.parts)==1: return name.suffix in {'.h','.cpp'}
    allowed={'tests':{'.h','.cpp','.py'},'tools':{'.py','.ps1'},
             'backends':{'.h'},'adaptation':{'.h','.patch','.json'}}
    return name.suffix in allowed.get(name.parts[0],set())


def check_generation(dest):
    if not (dest/'adaptation/manifest.json').exists(): return None
    products,provenance=render_headers(dest)
    rows,stale=verify_headers(dest,products)
    if stale: raise RuntimeError('Candidate has stale generated runtime products: '+', '.join(stale))
    return dict(pristine_upstream_files=provenance['pristine_upstream_files'],generated_products=len(products),
                exact_byte_matches=sum(row['exact_bytes'] for row in rows),success=True)


def candidate_provider(patch):
    data = patch.read_bytes()
    if re.search(rb'(?m)^(?:new file mode 120000|new mode 120000|rename (?:from|to) |copy (?:from|to) |GIT binary patch)',data):
        raise RuntimeError('Candidate patches must contain ordinary source files, without links, renames or binaries')

    def export(run, label, revision):
        dest, provenance = comparison.snapshot(run, label, revision)
        if label != 'fork':
            provenance['role'] = 'explicit unmodified reference'
            return dest, provenance
        saved = run.output/'candidate.patch'
        saved.write_bytes(data)
        listing = run.command('candidate-patch-paths', ['git','apply','--numstat','-z',saved])
        paths = []
        for entry in listing.split('\0'):
            if not entry: continue
            added, removed, path = entry.split('\t',2)
            if added == '-' or removed == '-' or not supported_path(path):
                raise RuntimeError('Unsupported candidate patch path: '+repr(path))
            paths.append(path)
        if not paths or len(paths)!=len(set(paths)):
            raise RuntimeError('Expected unique reviewed source paths in patch')
        prefix = dest.relative_to(comparison.ROOT).as_posix()
        run.command('candidate-patch-check', ['git','apply','--check','--directory='+prefix,saved])
        run.command('candidate-patch-apply', ['git','apply','--directory='+prefix,saved])
        generation=check_generation(dest)
        if generation is not None:
            comparison.write_json(run.output/'candidate-generation-check.json',generation)
        files = [dict(path=p.relative_to(dest).as_posix(),sha256=comparison.sha(p))
                 for p in sorted(dest.rglob('*'),key=lambda p:p.relative_to(dest).as_posix()) if p.is_file()]
        fingerprint = hashlib.sha256(''.join(r['path']+'\0'+r['sha256']+'\n' for r in files).encode()).hexdigest()
        archive = run.output/'fork-candidate-source.tar'
        with tarfile.open(archive,'w') as tar:
            for row in files:
                path = dest/row['path']
                info = tar.gettarinfo(str(path),arcname=row['path'])
                info.uid=info.gid=0; info.uname=info.gname=''; info.mtime=0
                with path.open('rb') as f: tar.addfile(info,f)
        record = dict(role='uncommitted reviewed candidate',commit=None,
            base_commit=provenance['commit'],base_tree=provenance['tree'],
            base_archive_sha256=provenance['archive_sha256'],
            patch_sha256=hashlib.sha256(data).hexdigest(),patch_paths=paths,
            fingerprint=fingerprint,fingerprint_order='relative POSIX paths sorted by Unicode code points',
            archive=archive.name,archive_sha256=comparison.sha(archive))
        comparison.write_json(run.output/'fork-source-manifest.json',dict(record,files=files))
        return dest,record

    return export


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--candidate-patch',type=Path,required=True)
    args,remaining=parser.parse_known_args()
    patch=args.candidate_patch.resolve(strict=True)
    sys.argv=[sys.argv[0],*remaining]
    return comparison.main(snapshot_provider=candidate_provider(patch),
        extra_harness=['tools/run_x86_candidate_comparison.py','tools/regenerate_headers.py'])


if __name__=='__main__': sys.exit(main())
