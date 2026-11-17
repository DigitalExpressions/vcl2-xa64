/*
 * Copyright 2026 DigitalExpressions Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Classification must inspect bits even with -ffinite-math-only.
#include "vectorclass.h"
#include <cstdio>
#include <cstring>
#include <limits>
#ifdef VCL_NAMESPACE
using namespace VCL_NAMESPACE;
#endif
// Keep the reference in integer bits: finite-math flags can annotate scalar
// floating parameters as excluding NaNs/infinities before an oracle runs.
template<class V,class U> inline int check_masks(V a,U bits,U exponent,U fraction,U sign) {
    const bool finite=(bits&exponent)!=exponent,zero_exp=(bits&exponent)==0;
    const bool expected[]={finite,(bits&~sign)==exponent,!finite && (bits&fraction)!=0,
        zero_exp && (bits&fraction)!=0,zero_exp,bool(bits&sign)};
    const uint64_t actual[]={uint64_t(to_bits(is_finite(a))),uint64_t(to_bits(is_inf(a))),
        uint64_t(to_bits(is_nan(a))),uint64_t(to_bits(is_subnormal(a))),
        uint64_t(to_bits(is_zero_or_subnormal(a))),uint64_t(to_bits(sign_bit(a)))};
    const uint64_t all=(uint64_t(1)<<V::size())-1;
    int failures=0;
    for(unsigned op=0;op<6;++op) {
        const uint64_t wanted=expected[op]?all:0;
        if(actual[op]!=wanted) {
            std::fprintf(stderr,"classification lanes=%d scalar_bytes=%zu op=%u bits=%llx mask=%llx expected=%llx\n",
                V::size(),sizeof(U),op,(unsigned long long)bits,(unsigned long long)actual[op],(unsigned long long)wanted);
            ++failures;
        }
    }
    return failures;
}

template<class V,class T,class U> int check(U const* patterns,unsigned count,U exponent,U fraction,U sign) {
    int failures=0;
    for(unsigned i=0;i<count;++i) {
        volatile U input=patterns[i]; U bits=input; T scalar; std::memcpy(&scalar,&bits,sizeof(scalar)); V a(scalar);
        const bool finite=(bits&exponent)!=exponent,zero_exp=(bits&exponent)==0;
        failures+=horizontal_and(is_finite(a))!=finite;
        failures+=horizontal_and(is_inf(a))!=((bits&~sign)==exponent);
        failures+=horizontal_and(is_nan(a))!=(!finite && (bits&fraction)!=0);
        failures+=horizontal_and(is_subnormal(a))!=(zero_exp && (bits&fraction)!=0);
        failures+=horizontal_and(is_zero_or_subnormal(a))!=zero_exp;
        failures+=horizontal_and(sign_bit(a))!=bool(bits&sign);
        failures+=check_masks<V,U>(a,bits,exponent,fraction,sign);
    }
    return failures;
}

// Mixed lanes exercise broad/compact mask construction and the SSE2 double
// integer-comparison fallback. Include every special pattern in every lane,
// followed by deterministic random bit patterns. The oracle stays integer-only.
template<class V,class T,class U> int check_mixed(U const* patterns,unsigned count,U exponent,U fraction,U sign) {
    U state=U(0x9E3779B9u);
    int failures=0;
    for(unsigned batch=0;batch<256;++batch) {
        T values[V::size()];
        uint64_t expected[6]={};
        for(int lane=0;lane<V::size();++lane) {
            state=state*U(1664525u)+U(1013904223u);
            U bits=batch<count?patterns[(batch+unsigned(lane))%count]:state;
            std::memcpy(values+lane,&bits,sizeof(T));
            const bool finite=(bits&exponent)!=exponent,zero_exp=(bits&exponent)==0;
            const bool wanted[]={finite,(bits&~sign)==exponent,!finite && (bits&fraction)!=0,
                zero_exp && (bits&fraction)!=0,zero_exp,bool(bits&sign)};
            for(unsigned op=0;op<6;++op) if(wanted[op]) expected[op]|=uint64_t(1)<<lane;
        }
        V a; a.load(values);
        const uint64_t actual[]={uint64_t(to_bits(is_finite(a))),uint64_t(to_bits(is_inf(a))),
            uint64_t(to_bits(is_nan(a))),uint64_t(to_bits(is_subnormal(a))),
            uint64_t(to_bits(is_zero_or_subnormal(a))),uint64_t(to_bits(sign_bit(a)))};
        for(unsigned op=0;op<6;++op) if(actual[op]!=expected[op]) {
            std::fprintf(stderr,"mixed classification lanes=%d scalar_bytes=%zu batch=%u op=%u mask=%llx expected=%llx\n",
                V::size(),sizeof(U),batch,op,(unsigned long long)actual[op],(unsigned long long)expected[op]);
            ++failures;
        }
    }
    return failures;
}

#if MAX_VECTOR_SIZE >= 512 && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
// Keep this small, direct constant-broadcast reproducer separate from the
// generic scalar oracle: materializing a vector across a call hides the bug.
int check_wide_broadcasts() {
    Vec16f f(std::numeric_limits<float>::quiet_NaN());
    Vec8d d(std::numeric_limits<double>::quiet_NaN());
    Vec16f fi(std::numeric_limits<float>::infinity());
    Vec8d di(std::numeric_limits<double>::infinity());
    const unsigned fn=to_bits(is_nan(f)),dn=to_bits(is_nan(d));
    const unsigned ff=to_bits(is_inf(fi)),df=to_bits(is_inf(di));
    Vec8f f256(std::numeric_limits<float>::quiet_NaN());
    Vec4d d256(std::numeric_limits<double>::quiet_NaN());
    const unsigned fn256=to_bits(is_nan(f256)),dn256=to_bits(is_nan(d256));
    const int failures=int(fn!=0xffff)+int(dn!=0xff)+int(ff!=0xffff)+int(df!=0xff)
        +int(fn256!=0xff)+int(dn256!=0xf);
    if(fn256!=0xff || dn256!=0xf)
        std::fprintf(stderr,"256-bit broadcast NaN masks: %x/%x; expected ff/f\n",fn256,dn256);
    if(failures) std::fprintf(stderr,"512-bit broadcast masks: nan=%x/%x inf=%x/%x; expected ffff/ff\n",fn,dn,ff,df);
    return failures;
}
#endif
int main() {
    const uint32_t f[]={0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0x7f800000,0xff800000,0x7fc12345,0xff800001};
    const uint64_t d[]={0,0x8000000000000000ull,1,0x8000000000000001ull,0x000fffffffffffffull,0x0010000000000000ull,0x3ff0000000000000ull,0x7ff0000000000000ull,0xfff0000000000000ull,0x7ff8123456789abcull,0xfff0000000000001ull};
    int failures=check<Vec4f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check<Vec2d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
    failures+=check_mixed<Vec4f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check_mixed<Vec2d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
#if MAX_VECTOR_SIZE >= 256
    failures+=check<Vec8f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check<Vec4d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
    failures+=check_mixed<Vec8f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check_mixed<Vec4d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
#endif
#if MAX_VECTOR_SIZE >= 512
    failures+=check<Vec16f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check<Vec8d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
    failures+=check_mixed<Vec16f,float,uint32_t>(f,11,0x7f800000,0x007fffff,0x80000000)+check_mixed<Vec8d,double,uint64_t>(d,11,0x7ff0000000000000ull,0x000fffffffffffffull,0x8000000000000000ull);
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    failures+=check_wide_broadcasts();
#endif
#endif
    std::printf("%d finite-math classification failures\n",failures); return failures!=0;
}
