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

"""Validate portable SIMD bundles and compare an explicitly selected width scope."""
import argparse
from fractions import Fraction
import hashlib
import json
from pathlib import Path
import re
import sys

from simd_corpus import DIRECTORY, SCHEMA, digest, encoded, input_bytes, load, rational, rounded


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError('Duplicate JSON key: ' + key)
        result[key] = value
    return result


def read_json(path):
    return json.loads(Path(path).read_text(encoding='utf-8'), object_pairs_hook=unique_object)


def distance(left, right, bits):
    sign = 1 << (bits - 1)
    mask = (1 << bits) - 1
    ordered = lambda s: (~int(s,16) & mask) if int(s,16) & sign else int(s,16) | sign
    return abs(ordered(left) - ordered(right))


def bounds(case, metadata, policies):
    if case['policy'] == 'ulp':
        return policies['exp_log_ulp'][case['op']]
    if case['policy'] == 'trig':
        if case['type']=='f32' and case['op']=='tan' and metadata['backend']!=3 and metadata['instrset']<8:
            return policies['legacy_f32_tan_normalized']
        return policies['trig_normalized'][case['op']]
    return 0


def rounding_contract(case, metadata):
    if case['type']=='f64':
        return 'current' if metadata['backend']==1 and metadata['instrset'] in (2,10,11,12) else 'nearest'
    return 'nearest' if metadata['backend']==1 and metadata['instrset']>=10 and case['width']==512 else 'current'


def reference_values(case, metadata):
    if case['policy']=='fma':
        return case['expected'][metadata['fma']]
    if case['policy']=='roundi':
        return case['expected'][rounding_contract(case,metadata)]
    return case['expected']


def reference_spacing(expected, bits):
    magnitude = abs(rational(expected,bits))
    if magnitude < Fraction(1,100):
        magnitude = rational(rounded(Fraction(1,100) if bits==32 else Fraction(1),bits),bits)
    raw = int(rounded(magnitude,bits),16)
    spacing = rational(format(raw+1,'x'),bits) - rational(format(raw,'x'),bits)
    return spacing


def normalized_error(actual, expected, bits):
    return abs(rational(actual,bits)-rational(expected,bits)) / reference_spacing(expected,bits)


def validate_rows(rows, cases, metadata, policies):
    if metadata.get('schema')!=SCHEMA or metadata.get('backend') not in (1,2,3) or metadata.get('max_width') not in (128,256,512):
        raise ValueError('Invalid capture metadata')
    if metadata.get('fma') not in ('fused','unfused') or not isinstance(metadata.get('instrset'),int):
        raise ValueError('Missing FMA/ISA metadata')
    if metadata['instrset'] not in {1:(2,8,10,11,12),2:(6,),3:(0,)}[metadata['backend']]:
        raise ValueError('Incompatible backend/ISA metadata')
    expected_fma = 'fused' if metadata['backend']==3 or metadata['instrset']>=8 else 'unfused'
    if metadata['fma']!=expected_fma:
        raise ValueError('FMA metadata does not match the declared backend/ISA contract')
    if metadata.get('fp_control',0xffffffff) & 0xe040:
        raise ValueError('Expected nearest rounding with FTZ/DAZ disabled')
    required = {c['id']:c for c in cases if c['width']<=metadata['max_width']}
    actual = {}
    for row in rows:
        if row.get('id') in actual:
            raise ValueError('Duplicate output ID: ' + row['id'])
        actual[row.get('id')] = row
    if set(actual)!=set(required):
        raise ValueError('Missing/extra cases: ' + str(dict(missing=sorted(set(required)-set(actual)),extra=sorted(set(actual)-set(required),key=str))))
    failures = []
    lanes = 0
    allowed_fields = {'id','type','width','op','rounding','policy','bits'}
    for key, case in required.items():
        row = actual[key]
        if set(row)!=allowed_fields or any(row[name]!=case[name] for name in allowed_fields-{'bits'}):
            raise ValueError('Output metadata mismatch: ' + key)
        if case['policy'] not in policies['policies']:
            raise ValueError('Unknown policy: ' + case['policy'])
        bits = int(case['type'][1:])
        if len(row['bits'])!=case['width']//bits or any(not isinstance(v,str) or not re.fullmatch('[0-9a-f]{'+str(bits//4)+'}',v) for v in row['bits']):
            raise ValueError('Malformed lane bits: ' + key)
        expected = reference_values(case,metadata)
        for index, (got, ref) in enumerate(zip(row['bits'],expected)):
            lanes += 1
            ok = got==ref
            if not ok and case['policy'] in ('ulp','trig','reciprocal'):
                try:
                    value, reference = rational(got,bits), rational(ref,bits)
                    # Zero signs are exact where the selected reference is zero.
                    if reference != 0:
                        if case['policy']=='ulp':
                            ok = distance(got,ref,bits)<=bounds(case,metadata,policies)
                        elif case['policy']=='trig':
                            ok = normalized_error(got,ref,bits)<=bounds(case,metadata,policies)
                        else:
                            reference = 1/rational(case['a'][index],bits)
                            ok = abs((value-reference)/reference)<Fraction(1,2048)
                except ValueError:
                    ok = False
            if not ok:
                failures.append(dict(id=key,lane=index,actual=got,expected=ref,policy=case['policy']))
    failed_cases=len({f['id'] for f in failures})
    return dict(registered=len(required),executed=len(actual),passed=len(required)-failed_cases,
                failed=failed_cases,skipped=0,timed_out=0,lanes=lanes,
                failed_lanes=len(failures),failures=failures,success=not failures)


def load_bundle(directory, cases, policies, allow_preparation=False):
    directory = Path(directory)
    manifest = read_json(directory/'manifest.json')
    if manifest['schema']!=SCHEMA or manifest['harness_version']!='simd-capture-1':
        raise ValueError('Incompatible harness/schema')
    if manifest['corpus_sha256']!=digest(DIRECTORY/'corpus.jsonl') or manifest['policies_sha256']!=digest(DIRECTORY/'policies.json'):
        raise ValueError('Corpus or policy hash mismatch')
    if manifest['input_file_sha256']!=hashlib.sha256(input_bytes(cases,manifest['capture']['max_width'])).hexdigest():
        raise ValueError('Input bit-pattern hash mismatch')
    if not allow_preparation and (manifest['preparation'] or not manifest['source']['clean']):
        raise ValueError('Preparation/dirty results cannot be used as final validation')
    controls=manifest['controls']
    host=manifest['host']['label']
    if host not in ('windows-amd','mac-m2') or (host=='windows-amd')!=(manifest['capture']['backend']==1):
        raise ValueError('Native host/backend mismatch')
    if controls.get('guard-positive')!=0 or manifest['guard_page_size']<=0:
        raise ValueError('Missing/failed writable guard control')
    expected_faults=(0xc0000005,) if host=='windows-amd' else (-10,-11)
    if any(controls.get('guard-negative-'+str(i)) not in expected_faults for i in range(4)):
        raise ValueError('Missing/failed guard fault control')
    if controls.get('runtime-control' if host=='windows-amd' else 'runtime-run')!=0:
        raise ValueError('Missing/failed runtime consumer')
    if manifest['sanitizer']=='strict UBSan' and (controls.get('ubsan-positive')!=0 or controls.get('ubsan-negative')!=1):
        raise ValueError('Missing/failed strict UBSan controls')
    source = manifest['source']
    if not re.fullmatch('[0-9a-f]{40}',source['head']) or not re.fullmatch('[0-9a-f]{40}',source['tree']):
        raise ValueError('Invalid source commit/tree')
    lines = (directory/'checksums.sha256').read_text(encoding='utf-8').splitlines()
    checksums = {}
    for line in lines:
        match = re.fullmatch('([0-9a-f]{64})  ([A-Za-z0-9_.-]+)',line)
        if not match or match[2] in checksums:
            raise ValueError('Malformed or duplicate checksum entry')
        checksums[match[2]] = match[1]
    files = {p.name for p in directory.iterdir() if p.is_file() and p.name!='checksums.sha256'}
    if set(checksums)!=files or 'manifest.json' not in files:
        raise ValueError('Incomplete checksum inventory')
    for name, expected in checksums.items():
        if digest(directory/name)!=expected:
            raise ValueError('Result checksum mismatch: ' + name)
    if not re.fullmatch('outputs-[A-Za-z0-9_.-]+[.]jsonl',manifest['outputs']):
        raise ValueError('Invalid output filename')
    rows = [json.loads(line,object_pairs_hook=unique_object) for line in (directory/manifest['outputs']).read_text(encoding='utf-8').splitlines()]
    result = validate_rows(rows,cases,manifest['capture'],policies)
    if not result['success']:
        raise ValueError('Independent reference failures: ' + str(result['failures'][:8]))
    if manifest['reference_validation']!={k:v for k,v in result.items() if k!='failures'}:
        raise ValueError('Manifest result counts disagree with independently recomputed counts')
    return manifest, {row['id']:row for row in rows}


def compare(left_directory,right_directory,widths,allow_preparation=False):
    cases,policies = load()
    left,aa = load_bundle(left_directory,cases,policies,allow_preparation)
    right,bb = load_bundle(right_directory,cases,policies,allow_preparation)
    for field in ('run_id','corpus_sha256','policies_sha256','harness_version'):
        if left[field]!=right[field]:
            raise ValueError('Comparison metadata mismatch: ' + field)
    if left['source']!=right['source']:
        raise ValueError('Sources differ; compare exact matching source checkpoints')
    if not widths or any(w not in (128,256,512) for w in widths):
        raise ValueError('Explicit supported widths are required')
    if max(widths)>min(left['capture']['max_width'],right['capture']['max_width']):
        raise ValueError('Requested width is absent in one profile')
    result = dict(schema=SCHEMA,source=left['source'],run_id=left['run_id'],preparation=left['preparation'] or right['preparation'],
                  widths=sorted(set(widths)),excluded_widths=sorted({128,256,512}-set(widths)),
                  left_profile=left['profile'],right_profile=right['profile'],
                  cases=0,lanes=0,exact=0,numerical=0,documented_fma=0,documented_rounding=0,unexpected=0,differences=[])
    for case in cases:
        if case['width'] not in widths:
            continue
        result['cases'] += 1
        for index,(a,b) in enumerate(zip(aa[case['id']]['bits'],bb[case['id']]['bits'])):
            result['lanes'] += 1
            if a==b:
                result['exact'] += 1
                continue
            category = 'unexpected'
            if case['policy']=='fma' and left['capture']['fma']!=right['capture']['fma']:
                category='documented_fma'
            elif case['policy']=='roundi' and rounding_contract(case,left['capture'])!=rounding_contract(case,right['capture']):
                category='documented_rounding'
            elif case['policy'] in ('ulp','trig','reciprocal'):
                bits=int(case['type'][1:])
                total_bound = bounds(case,left['capture'],policies)+bounds(case,right['capture'],policies)
                if case['policy']=='ulp':
                    ok=distance(a,b,bits)<=total_bound
                elif case['policy']=='trig':
                    reference=case['expected'][index]
                    ok=abs(rational(a,bits)-rational(b,bits))/reference_spacing(reference,bits)<=total_bound
                else:
                    reference=1/rational(case['a'][index],bits)
                    ok=abs((rational(a,bits)-rational(b,bits))/reference)<Fraction(1,1024)
                if ok:
                    category='numerical'
            result[category] += 1
            result['differences'].append(dict(id=case['id'],lane=index,left=a,right=b,category=category))
    result['success']=result['unexpected']==0
    return result


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('left',type=Path)
    parser.add_argument('right',type=Path)
    parser.add_argument('--width',type=int,choices=(128,256,512),action='append',required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--allow-preparation',action='store_true')
    args=parser.parse_args()
    try:
        if args.output.exists():
            raise ValueError('Output exists; preserve earlier comparison attempts')
        result=compare(args.left,args.right,args.width,args.allow_preparation)
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_bytes(encoded(result))
        print(json.dumps({k:v for k,v in result.items() if k!='differences'},indent=2))
        return 0 if result['success'] else 1
    except (ValueError,OSError,KeyError,TypeError) as error:
        print('Comparison failed: '+str(error),file=sys.stderr)
        return 1


if __name__=='__main__':
    sys.exit(main())
