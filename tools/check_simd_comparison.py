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

"""Positive/negative controls for corpus oracles, integrity and comparison gates."""
import copy
from contextlib import contextmanager
from fractions import Fraction
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from compare_simd_results import compare, load_bundle, reference_values, validate_rows
from simd_corpus import DIRECTORY, ROOT, digest, encoded, input_bytes, load, rational, rounded


@contextmanager
def retained_controls():
    # Keep negative-control inputs/results under ignored build/ for inspection.
    yield tempfile.mkdtemp(prefix='simd-controls-',dir=ROOT/'build')


class ComparisonControls(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cases,cls.policies=load()
        cls.meta=dict(metadata=True,schema=1,backend=1,instrset=2,max_width=512,fma='unfused',fp_control=0x1f80)

    def golden_rows(self,metadata=None):
        metadata=metadata or self.meta
        return [dict(id=c['id'],type=c['type'],width=c['width'],op=c['op'],rounding=c['rounding'],policy=c['policy'],
                     bits=copy.deepcopy(reference_values(c,metadata)))
                for c in self.cases if c['width']<=metadata['max_width']]

    def bundle(self,root,name,metadata=None,rows=None):
        metadata=metadata or self.meta
        rows=rows if rows is not None else self.golden_rows(metadata)
        directory=Path(root)/name
        directory.mkdir()
        (directory/'outputs-test.jsonl').write_bytes(b''.join(encoded(row) for row in rows))
        counts=validate_rows(rows,self.cases,metadata,self.policies)
        controls=dict([('guard-positive',0),('runtime-control',0)]+[('guard-negative-'+str(i),0xc0000005) for i in range(4)])
        manifest=dict(schema=1,harness_version='simd-capture-1',run_id='comparator-controls',profile=name,preparation=True,
            source=dict(head='a'*40,tree='b'*40,clean=False,working_fingerprint='c'*64),capture=metadata,
            corpus_sha256=digest(DIRECTORY/'corpus.jsonl'),policies_sha256=digest(DIRECTORY/'policies.json'),
            input_file_sha256=hashlib.sha256(input_bytes(self.cases,metadata['max_width'])).hexdigest(),outputs='outputs-test.jsonl',
            host=dict(label='windows-amd'),controls=controls,guard_page_size=4096,sanitizer='not requested',
            reference_validation={k:v for k,v in counts.items() if k!='failures'})
        (directory/'manifest.json').write_bytes(encoded(manifest))
        self.rehash(directory)
        return directory

    @staticmethod
    def rehash(directory):
        (directory/'checksums.sha256').write_bytes(''.join(digest(p)+'  '+p.name+'\n' for p in sorted(directory.iterdir()) if p.name!='checksums.sha256').encode('ascii'))

    def mutate_manifest(self,directory,change):
        path=directory/'manifest.json'
        data=json.loads(path.read_text(encoding='utf-8'))
        change(data)
        path.write_bytes(encoded(data))
        self.rehash(directory)

    def test_exact_rounding_oracle(self):
        for bits,one,tiny,max_sub in [(32,'3f800000','00000001','007fffff'),(64,'3ff0000000000000','0000000000000001','000fffffffffffff')]:
            self.assertEqual(rounded(Fraction(1),bits),one)
            self.assertEqual(rounded(rational(tiny,bits),bits),tiny)
            self.assertEqual(rounded(rational(max_sub,bits),bits),max_sub)
            self.assertEqual(rounded(rational(tiny,bits)/2,bits),'0'*(bits//4))
            self.assertEqual(rounded(rational(one,bits)+Fraction(1,2**(24 if bits==32 else 53)),bits),one)
            self.assertEqual(rounded(Fraction(1)+Fraction(3,2**(24 if bits==32 else 53)),bits),format(int(one,16)+2,'0'+str(bits//4)+'x'))

    def test_golden_positive(self):
        result=validate_rows(self.golden_rows(),self.cases,self.meta,self.policies)
        self.assertTrue(result['success'])
        self.assertEqual(result['registered'],len(self.cases))

    def test_missing_duplicate_unknown_and_metadata(self):
        original=self.golden_rows()
        mutations=[original[:-1],original+[copy.deepcopy(original[0])]]
        for field,value in [('id','unknown'),('policy','unknown'),('op','unknown'),('rounding',9),('bits',['xyz'])]:
            rows=copy.deepcopy(original)
            rows[0][field]=value
            mutations.append(rows)
        for rows in mutations:
            with self.subTest(rows=len(rows)),self.assertRaises(ValueError):
                validate_rows(rows,self.cases,self.meta,self.policies)

    def test_corrupted_exact_and_signed_zero(self):
        rows=self.golden_rows()
        rows[0]['bits'][0]='00000001'
        self.assertFalse(validate_rows(rows,self.cases,self.meta,self.policies)['success'])
        rows=self.golden_rows()
        target=next(r for r in rows if r['type']=='f32' and r['op']=='load' and '80000000' in r['bits'])
        target['bits'][target['bits'].index('80000000')]='00000000'
        self.assertFalse(validate_rows(rows,self.cases,self.meta,self.policies)['success'])

    def test_numerical_gate_not_unlimited(self):
        rows=self.golden_rows()
        row=next(r for r in rows if r['policy']=='ulp' and r['type']=='f32' and r['op']=='exp')
        row['bits'][0]=format(int(row['bits'][0],16)+1000,'08x')
        self.assertFalse(validate_rows(rows,self.cases,self.meta,self.policies)['success'])

    def test_fp_environment_and_fma_metadata(self):
        for change in [dict(fp_control=0x9fc0),dict(fma='fused'),dict(backend=2,instrset=2)]:
            metadata=dict(self.meta,**change)
            with self.subTest(change=change),self.assertRaises(ValueError):
                validate_rows(self.golden_rows(),self.cases,metadata,self.policies)
        metadata=dict(self.meta,instrset=8,fma='fused')
        rows=self.golden_rows(metadata)
        row=next(r for r in rows if r['type']=='f64' and r['op']=='roundi' and r['rounding']==1)
        row['bits'][0]='fffffffffffffffd'  # Current-direction result is wrong for this fixed-nearest path.
        self.assertFalse(validate_rows(rows,self.cases,metadata,self.policies)['success'])

    def test_bundle_integrity_and_guards(self):
        with retained_controls() as temporary:
            directory=self.bundle(temporary,'control')
            load_bundle(directory,self.cases,self.policies,True)
            for field,value in [('corpus_sha256','0'*64),('policies_sha256','0'*64),('input_file_sha256','0'*64),('schema',99)]:
                original=(directory/'manifest.json').read_bytes()
                self.mutate_manifest(directory,lambda m:m.update({field:value}))
                with self.subTest(field=field),self.assertRaises(ValueError):
                    load_bundle(directory,self.cases,self.policies,True)
                (directory/'manifest.json').write_bytes(original)
                self.rehash(directory)
            self.mutate_manifest(directory,lambda m:m['controls'].update({'guard-negative-3':0}))
            with self.assertRaises(ValueError):
                load_bundle(directory,self.cases,self.policies,True)

    def test_checksums_and_final_preparation_rejection(self):
        with retained_controls() as temporary:
            directory=self.bundle(temporary,'control')
            with self.assertRaises(ValueError):
                load_bundle(directory,self.cases,self.policies)
            (directory/'outputs-test.jsonl').write_bytes(b'corrupt\n')
            with self.assertRaises(ValueError):
                load_bundle(directory,self.cases,self.policies,True)

    def test_strict_ubsan_host_exit_codes(self):
        with retained_controls() as temporary:
            for host,metadata,accepted in [
                    ('windows-amd',self.meta,(1,)),
                    ('mac-m2',dict(self.meta,backend=3,instrset=0,fma='fused'),(1,-6))]:
                directory=self.bundle(temporary,host,metadata)
                controls=dict([('guard-positive',0),('ubsan-positive',0),('ubsan-negative',1),
                               ('runtime-control' if host=='windows-amd' else 'runtime-run',0)]+
                              [('guard-negative-'+str(i),0xc0000005 if host=='windows-amd' else -10)
                               for i in range(4)])
                self.mutate_manifest(directory,lambda m:m.update(
                    host=dict(label=host),sanitizer='strict UBSan',controls=controls))
                for code in accepted:
                    self.mutate_manifest(directory,lambda m:m['controls'].update({'ubsan-negative':code}))
                    with self.subTest(host=host,accepted=code):
                        load_bundle(directory,self.cases,self.policies,True)
                for code in (0,2,124,-6,-9,-10,-11,0xc0000005):
                    if code in accepted:
                        continue
                    self.mutate_manifest(directory,lambda m:m['controls'].update({'ubsan-negative':code}))
                    with self.subTest(host=host,rejected=code),self.assertRaisesRegex(ValueError,'strict UBSan controls'):
                        load_bundle(directory,self.cases,self.policies,True)
                for missing in ('ubsan-positive','ubsan-negative'):
                    self.mutate_manifest(directory,lambda m:m['controls'].update(
                        {'ubsan-positive':0,'ubsan-negative':accepted[-1]}))
                    self.mutate_manifest(directory,lambda m:m['controls'].pop(missing))
                    with self.subTest(host=host,missing=missing),self.assertRaisesRegex(ValueError,'strict UBSan controls'):
                        load_bundle(directory,self.cases,self.policies,True)
                self.mutate_manifest(directory,lambda m:m['controls'].update(
                    {'ubsan-positive':1,'ubsan-negative':accepted[-1]}))
                with self.subTest(host=host,failed_positive=True),self.assertRaisesRegex(ValueError,'strict UBSan controls'):
                    load_bundle(directory,self.cases,self.policies,True)

    def test_pairwise_categories_and_source_scope(self):
        with retained_controls() as temporary:
            left=self.bundle(temporary,'left')
            right=self.bundle(temporary,'right')
            result=compare(left,right,[128,256,512],True)
            self.assertTrue(result['success'])
            self.assertEqual(result['exact'],result['lanes'])
            rows=self.golden_rows()
            row=next(r for r in rows if r['policy']=='ulp' and r['type']=='f32' and r['op']=='exp')
            row['bits'][0]=format(int(row['bits'][0],16)+1,'08x')
            numeric=self.bundle(temporary,'numeric',rows=rows)
            result=compare(left,numeric,[128,256,512],True)
            self.assertEqual(result['numerical'],1)
            fused=self.bundle(temporary,'fused',dict(self.meta,instrset=8,fma='fused'))
            result=compare(left,fused,[128,256,512],True)
            self.assertGreater(result['documented_fma'],0)
            self.assertGreater(result['documented_rounding'],0)
            limited=self.bundle(temporary,'limited',dict(self.meta,max_width=128))
            with self.assertRaises(ValueError):
                compare(left,limited,[128,256,512],True)
            self.mutate_manifest(right,lambda m:m['source'].update(head='d'*40))
            with self.assertRaises(ValueError):
                compare(left,right,[128],True)


if __name__=='__main__':
    unittest.main(verbosity=2)
