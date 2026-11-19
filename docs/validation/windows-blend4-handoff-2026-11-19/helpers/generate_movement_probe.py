"""Emit the frozen candidate's exact added regression, without unrelated legacy instantiations.
This preparation helper is outside the captured library and its active build graph.
"""
from pathlib import Path
import argparse,hashlib,json
EXPECTED='7ee48af6fa40cd0b52c5a5be979bdaed0632833ab8500128be69ce1162373f25'
def digest(data):return hashlib.sha256(data).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--candidate-source',required=True,type=Path);p.add_argument('--output',required=True,type=Path);p.add_argument('--route',choices=['facade','originals'],required=True);a=p.parse_args()
 data=(a.candidate_source/'tests/movement_slice.cpp').read_bytes()
 if digest(data)!=EXPECTED:raise ValueError('Expected exact frozen candidate movement source; do not adjust the accepted hash')
 alias=next(x for x in data.splitlines(keepends=True) if x.startswith(b'template<class V> using Scalar='))
 check=next(x for x in data.splitlines(keepends=True) if x.startswith(b'#define CHECK(x)'))
 start=data.index(b'template<class V,int... I> V do_blend(')
 added=data.index(b'template<class V,int... I>\n#if defined(_MSC_VER)',start)
 end=data.index(b'template<int Mode,class V,size_t... Lane> void permutation_pattern',added)
 blend=data[start:added];regression=data[added:end]
 assert regression.count(b'transpose_blend_pattern<V,')==6 and b'0,1,4,5' in regression and b'2,3,6,7' in regression
 # Include only public headers plus the captured source's exact bodies and oracle.
 headers=b'#include "vcl2_xa64.h"\n' if a.route=='facade' else b'#include "vcl_config.h"\n#include "vendor/vectorclass/vectorclass.h"\n'
 prelude=data[:data.index(b'#include "vcl2_xa64.h"')]
 extra=b'#include <cstddef>\n#include <cstdint>\n#include <cstdio>\n#include <cstring>\n#ifdef VCL_NAMESPACE\nusing namespace VCL_NAMESPACE;\n#endif\nnamespace {\nint failures=0;\n'
 suffix=b'}\nint main(){transpose_blends();std::printf("%d isolated transpose blend failures\\n",failures);return failures!=0;}\n'
 source=prelude+headers+extra+check+alias+blend+regression+suffix
 if a.output.exists() or a.output.with_suffix('.extraction.json').exists():raise ValueError('Destination exists; retain it and choose a new attempt')
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(source)
 record=dict(candidate_test_sha256=EXPECTED,route=a.route,generated_sha256=digest(source),exact_sections=dict(scalar_alias=digest(alias),check_macro=digest(check),do_blend=digest(blend),added_regression=digest(regression)),added_regression_lines=len(regression.splitlines()),behavior='Only the captured added regression is called; all oracle/pattern bodies copied byte-for-byte',platform_note='FPCR/FPSR block remains under its captured ARM/compiler guard; x86 executes integer-bit checks only',native_execution=False)
 a.output.with_suffix('.extraction.json').write_text(json.dumps(record,indent=2)+'\n');print(json.dumps(record,indent=2))
if __name__=='__main__':main()
