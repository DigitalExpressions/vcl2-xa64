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

"""Compile-time contracts for backend isolation, header order, and width guards."""
import pathlib
import re
import subprocess
import sys
import tempfile

compiler, root = sys.argv[1:3]
driver = sys.argv[4].split("=", 1)[1] if len(sys.argv) > 4 and sys.argv[4].startswith("--driver=") else "gnu"
target_flags = sys.argv[5:] if len(sys.argv) > 4 and sys.argv[4].startswith("--driver=") else sys.argv[4:]
if driver in ("msvc", "clang-cl"):
    base = [compiler, "/nologo", "/std:c++17", "/Zs", "/TP", "/WX", "/I" + root]
else:
    base = [compiler, "-std=c++17", "-fsyntax-only", "-I" + root, "-x", "c++"]
base += target_flags
cases = [
    (True, [], '#include "vcl2_xa64.h"\nstatic_assert(MAX_VECTOR_SIZE==512);\n'),
    (True, ["-DMAX_VECTOR_SIZE=128"], '#include "vcl2_xa64.h"\nstatic_assert(MAX_VECTOR_SIZE==128);\n'),
    (False, ["-DMAX_VECTOR_SIZE=192"], '#include "vcl2_xa64.h"\n'),
    (False, ["-DVCL_BACKEND=99"], '#include "vcl2_xa64.h"\n'),
]
cases += [
    (False, ["-D_M_ARM64EC=1"], '#include "vcl2_xa64.h"\n'),
]
if sys.argv[3] == "arm64":
    native = ["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE=128"]
    cases += [
        (False, [], '#include "vectormath_lib.h"\n'),
        (False, ["-DVCL_BACKEND=3", "-DMAX_VECTOR_SIZE=128"], '#include "vcl2_xa64.h"\n'),
        (True, ["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON"], '#include "vcl2_xa64.h"\n'),
        (False, ["-DINSTRSET=8"], '#include "vcl2_xa64.h"\n'),
        (False, [], '#include "vectori128_neon.h"\n'),
        (True, native, '#include "instrset_neon.h"\n#include "vectori128_neon.h"\n#include "vectorf128_neon.h"\n#include "vcl2_xa64.h"\n#include "vcl2_xa64.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked into native backend\n#endif\nstatic_assert(sizeof(Vec4f)==16);\n'),
        (True, native, '#include "vectori128_div_neon.h"\n#include "vcl2_xa64.h"\nVec4i f(Vec4i x) { return divide_by_i<13>(x); }\n'),
        (True, native+["-DVCL_NAMESPACE=isolated"], '#include "vectorf128_neon.h"\n#include "instrset_neon.h"\n#include "vcl2_xa64.h"\nstatic_assert(isolated::Vec2d::size()==2);\n'),
    ]
    # Upstream movement cases define indexes before including VCL headers.
    # Keep that real integration macro compatible with all native widths/scopes.
    for width in [128,256,512]:
        for ns in [False,True]:
            flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON",
                   "-DMAX_VECTOR_SIZE="+str(width), "-Dindexes=0,1,-1,V_DC"]
            if ns: flags.append("-DVCL_NAMESPACE=isolated")
            source='#include "vcl2_xa64.h"\n'
            if ns: source+='using namespace isolated;\n'
            source+='auto macro_permute(Vec4i a) { return permute4<indexes>(a)+permute4<1,1,1,1>(a); }\n'
            source+='auto macro_blend(Vec2d a,Vec2d b) { return blend2<0,3>(a,b)+permute2<1,-1>(a); }\n'
            cases.append((True,flags,source))
    cases.append((True, native, '#include "vectori128_perm_neon.h"\n#include "vectorf128_neon.h"\nauto movement(Vec4i x) { return blend4<0,7,-1,V_DC>(x,x); }\n'))
    cases += [
        (True, native, '#include "instrset_helpers_neon.h"\n#include "vcl2_xa64.h"\nVec128b& f(Vec4ib& m) { return m.load(nullptr); }\nVec4i g(Vec4ib a) { return a+a; }\nVec4ui h() { return constant4ui<1,2,3,4>(); }\n'),
        (True, native+["-DVCL_NAMESPACE=isolated"], '#include "instrset_helpers_neon.h"\n#include "vectori128_perm_neon.h"\n#include "vcl2_xa64.h"\nconstexpr int i[]={1,0,-1,isolated::V_DC};\nstatic_assert(isolated::zero_mask(i)==3);\nisolated::Vec4i& f(isolated::Vec4ui& u) { return u; }\n'),
        (False, native, '#include "vcl2_xa64.h"\nVec4ui u=Vec4i(1);\n'),
        (False, native, '#include "vcl2_xa64.h"\nVec4ib m(1);\n'),
        (True, native, '#include "vectorf128_helpers_neon.h"\n#include "vector_convert_neon.h"\n#include "vectormath_common_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked into native backend\n#endif\nVec2q f(Vec4f x) { return reinterpret_i(x); }\nVec16c g(Vec2d x) { return reinterpret_i(x); }\nauto h(Vec4f x) { return polynomial_3(x,1.f,2.f,3.f,4.f); }\n'),
        (True, native+["-DVCL_NAMESPACE=isolated"], '#include "vector_convert_neon.h"\n#include "vectormath_common_neon.h"\n#include "vectorf128_helpers_neon.h"\nisolated::Vec4f f(isolated::Vec4f x) { return mul_sub_x(x,2.f,1.f); }\n'),
        (False, native, '#include "vectorclass_neon.h"\nauto x=pow(Vec4f(1),0.5f);\n'),
        (False, native, '#include "vcl2_xa64.h"\nauto x=approx_recipr(Vec2d(1));\n'),
        (False, native, '#include "vcl2_xa64.h"\nauto x=approx_rsqrt(Vec2d(1));\n'),
        (True, ["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE=256"], '#include "vcl2_xa64.h"\nstatic_assert(sizeof(Vec8f)==32);\n'),
    ]
    for expression in ["permute4<0,1,2,4>(Vec4i())", "permute4<0,-2,2,3>(Vec4i())",
                       "blend2<0,4>(Vec2d(),Vec2d())", "permute8<0,1>(Vec8s())",
                       "gather4i<0,-1,2,3>(nullptr)", "gather2d<0,-1>(nullptr)",
                       "blend16<0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,32>(Vec16c(),Vec16c())"]:
        cases.append((False, native, '#include "vcl2_xa64.h"\nauto rejected = '+expression+';\n'))
    for expression in ["Vec4i(1)/const_int(0)", "Vec8us(1)/const_uint(65536)",
                       "Vec4ui(1)/const_int(-3)", "Vec4i(1)/const_uint(0x80000000u)",
                       "Vec8s(1)/const_uint(32768)", "Vec16uc(1)/const_int(128)",
                       "Vec2q(1)/const_int(3)"]:
        cases.append((False, native, '#include "vcl2_xa64.h"\nauto rejected = '+expression+';\n'))
    wide = ["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE=256"]
    cases += [
        (True, wide+["-DVCL_NAMESPACE=isolated"], '#include "vectori256e_neon.h"\n#include "vectorf256e_neon.h"\n#include "vector_convert_neon.h"\n#include "vectormath_common_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\nisolated::Vec8f f(isolated::Vec8f x) { return permute8<7,6,5,4,3,2,1,0>(x); }\n'),
        (False, wide, '#include "vcl2_xa64.h"\nauto x=permute8<0,1,2,3,4,5,6,8>(Vec8i());\n'),
        (False, wide, '#include "vcl2_xa64.h"\nauto x=blend4<0,1,2,8>(Vec4d(),Vec4d());\n'),
        (False, wide, '#include "vcl2_xa64.h"\nauto x=gather8i<0,1,2,3,4,5,6,-1>(nullptr);\n'),
        (False, wide, '#include "vcl2_xa64.h"\nVec8ib x(1);\n'),
    ]
    widest = ["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE=512"]
    for header in ["vectori512e_neon.h", "vectorf512e_neon.h", "vectori512se_neon.h", "vector_convert_neon.h"]:
        cases.append((True, widest+["-DVCL_NAMESPACE=isolated"], '#include "'+header+'"\n#include "vcl2_xa64.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\nstatic_assert(sizeof(isolated::Vec64c)==64);\nstatic_assert(sizeof(isolated::Vec16f)==64);\n'))
    for count,typ in [(64,"Vec64c"),(32,"Vec32s"),(16,"Vec16i"),(8,"Vec8q"),(16,"Vec16f"),(8,"Vec8d")]:
        for op,invalid in [("permute",count),("blend",2*count),("blend",-2)]:
            indexes=','.join(map(str,list(range(count-1))+[invalid]))
            args=typ+'()'+(','+typ+'()' if op=='blend' else '')
            cases.append((False, widest, '#include "vcl2_xa64.h"\nauto x='+op+str(count)+'<'+indexes+'>('+args+');\n'))
    cases += [
        (True, widest, '#include "vcl2_xa64.h"\nauto f(Vec8uq a, Vec8uq b) { return a & ~b; }\n'),
        (False, widest, '#include "vcl2_xa64.h"\nVec64cb m(1);\n'),
        (False, widest, '#include "vcl2_xa64.h"\nauto x=gather8d<0,1,2,3,4,5,6,-1>(nullptr);\n'),
        (False, native, '#include "vcl2_xa64.h"\nVec16f x;\n'),
        (False, wide, '#include "vcl2_xa64.h"\nVec16f x;\n'),
    ]
    for width,typ in [(128,"Vec4f"),(256,"Vec8f"),(512,"Vec16f")]:
        flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE="+str(width)]
        for ns in [False,True]:
            scope='using namespace isolated;\n' if ns else ''
            cases.append((True,flags+(["-DVCL_NAMESPACE=isolated"] if ns else []), '#include "vectormath_exp_neon.h"\n#include "vcl2_xa64.h"\n#include "vectormath_exp_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\n'+scope+typ+' f('+typ+' x) { return exp(x)+expm1(x)+exp2(x)+exp10(x)+log(x)+log1p(x)+log2(x)+log10(x); }\n'))
    for width,types in [(128,["Vec4f","Vec2d"]),(256,["Vec8f","Vec4d"]),(512,["Vec16f","Vec8d"])]:
        flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE="+str(width)]
        for ns in [False,True]:
            source='#include "vectormath_exp_neon.h"\n#include "vectormath_exp_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\n'
            if ns: source+='using namespace isolated;\n'
            for typ in types:
                source+=typ+' f('+typ+' x,'+typ+' y) { return pow(x,y)+pow(x,.5)+pow(x,.5f)+cbrt(x)+reciprocal_cbrt(x)+square_cbrt(x)+pow_ratio(x,5,6); }\n'
            cases.append((True,flags+(["-DVCL_NAMESPACE=isolated", "-DSIGNED_ZERO=1"] if ns else []),source))
    for width,types in [(128,["Vec4f","Vec2d"]),(256,["Vec8f","Vec4d"]),(512,["Vec16f","Vec8d"])]:
        flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE="+str(width)]
        for ns in [False,True]:
            source='#include "vectormath_trig_neon.h"\n#include "vectormath_exp_neon.h"\n#include "vectormath_trig_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\n'
            if ns: source+='using namespace isolated;\n'
            for typ in types:
                source+=typ+' f('+typ+' x) { '+typ+' c; return sin(x)+cos(x)+tan(x)+sincos(&c,x)+c; }\n'
            cases.append((True,flags+(["-DVCL_NAMESPACE=isolated", "-DSIGNED_ZERO=1"] if ns else []),source))
    for width,types in [(128,["Vec4f","Vec2d"]),(256,["Vec8f","Vec4d"]),(512,["Vec16f","Vec8d"])]:
        flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE="+str(width)]
        for ns in [False,True]:
            source='#include "vectormath_trig_neon.h"\n#include "vectormath_exp_neon.h"\n#include "vectormath_trig_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\n'
            if ns: source+='using namespace isolated;\n'
            for typ in types:
                source+=typ+' f('+typ+' x,'+typ+' y) { '+typ+' c; return asin(x)+acos(x)+atan(x)+atan2(x,y)+sinpi(x)+cospi(x)+tanpi(x)+sincospi(&c,x)+c; }\n'
            cases.append((True,flags+(["-DVCL_NAMESPACE=isolated", "-DSIGNED_ZERO=1"] if ns else []),source))
    for width,types in [(128,["Vec4f","Vec2d"]),(256,["Vec8f","Vec4d"]),(512,["Vec16f","Vec8d"])]:
        flags=["-DVCL_BACKEND=3", "-DVCL_ENABLE_EXPERIMENTAL_NEON", "-DMAX_VECTOR_SIZE="+str(width)]
        for ns in [False,True]:
            source='#include "vectormath_hyp_neon.h"\n#include "vectormath_trig_neon.h"\n#include "vectormath_exp_neon.h"\n#include "vectormath_hyp_neon.h"\n#ifdef SSE2NEON_H\n#error Translation layer leaked\n#endif\n'
            if ns: source+='using namespace isolated;\n'
            for typ in types:
                source+=typ+' f('+typ+' x) { return sinh(x)+cosh(x)+tanh(x)+asinh(x)+acosh(x)+atanh(x); }\n'
            cases.append((True,flags+(["-DVCL_NAMESPACE=isolated", "-DSIGNED_ZERO=1"] if ns else []),source))
if sys.argv[3] == "arm64":
    # Core-only negative API contracts must stay core-only now that the public
    # facade intentionally includes the supported in-tree math surface.
    cases = [(ok, flags, source.replace('"vcl2_xa64.h"','"vectorclass_neon.h"'))
             for ok, flags, source in cases]
    cases += [(True, [], '#include "vcl2_xa64.h"\nauto f(Vec4f x){return exp(x)+sin(x)+cosh(x);}\n')]
with tempfile.TemporaryDirectory(prefix="vcl-configuration-") as directory:
    path = pathlib.Path(directory) / "probe.cpp"
    checked_flags = set()
    for i, (success, flags, source) in enumerate(cases):
        if driver in ("msvc", "clang-cl"):
            flags = ["/D" + flag[2:] if flag.startswith("-D") else flag for flag in flags]
        command = base + flags + [str(path)]
        # A failure must come from the contract, never an unusable toolchain or
        # unsupported target option. Check every distinct flag set positively.
        key = tuple(flags)
        if key not in checked_flags:
            path.write_text("int vcl_configuration_positive_control;\n")
            control = subprocess.run(command, text=True, capture_output=True, cwd=directory)
            rejected_option = re.search(r"D9002|unknown (?:argument|option)|unsupported (?:argument|option)|argument unused", control.stdout + control.stderr, re.I)
            if control.returncode or rejected_option:
                sys.stderr.write(f"Configuration flags rejected: {command!r}\n{control.stdout}{control.stderr}")
                sys.exit(1)
            checked_flags.add(key)
        path.write_text(source)
        result = subprocess.run(command, text=True, capture_output=True, cwd=directory)
        print(f"case {i}: expected_success={success} exit={result.returncode} command={command!r}")
        if (result.returncode == 0) != success:
            sys.stderr.write(f"Configuration case {i} failed expectations:\n{result.stdout}{result.stderr}")
            sys.exit(1)
print(f"{len(cases)} configuration contracts passed")
