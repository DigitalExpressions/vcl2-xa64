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

#if __has_include("vcl2_xa64.h")
#include "vcl2_xa64.h"
#else
#include "vectorclass.h"
#endif

#include <immintrin.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

// Untimed observations against exact IEEE encodings and explicit rounding
// answers. No call is made to an undefined out-of-range C++ integer conversion.
template<class V, class T, class U> int classification(char const* type) {
    constexpr int lanes=sizeof(V)/sizeof(T);
    constexpr U exponent=sizeof(T)==4?U(0x7f800000u):U(0x7ff0000000000000ull);
    constexpr U sign=U(1)<<(sizeof(T)*8-1);
    constexpr U fraction=sizeof(T)==4?U(0x007fffffu):U(0x000fffffffffffffull);
    volatile U patterns[8]={U(0),sign,exponent,U(exponent|sign),U(exponent|1),
        U(exponent|(U(1)<<(sizeof(T)==4?22:51))),U(1),U(exponent>>1)};
    int failures=0,checks=0;
    for(int rotation=0;rotation<8;++rotation) {
        T values[lanes]; U words[lanes];
        for(int j=0;j<lanes;++j) { words[j]=patterns[(j+rotation)&7]; std::memcpy(values+j,words+j,sizeof(T)); }
        V x; x.load(values);
        auto nan=is_nan(x), finite=is_finite(x), inf=is_inf(x), sub=is_subnormal(x), tiny=is_zero_or_subnormal(x);
        for(int j=0;j<lanes;++j) {
            U abs=words[j]&~sign;
            bool expected[5]={abs>exponent,abs<exponent,abs==exponent,
                (abs&exponent)==0 && (abs&fraction)!=0,(abs&exponent)==0};
            bool actual[5]={bool(nan[j]),bool(finite[j]),bool(inf[j]),bool(sub[j]),bool(tiny[j])};
            for(int k=0;k<5;++k) { ++checks; failures+=actual[k]!=expected[k]; }
        }
    }
    std::printf("{\"kind\":\"classification\",\"type\":\"%s\",\"bits\":%zu,\"lane_checks\":%d,\"failed\":%d}\n",
        type,sizeof(V)*8,checks,failures);
    return failures;
}
template<class V> int rounding() {
    constexpr int lanes=sizeof(V)/sizeof(double);
    volatile double patterns[8]={4503599627370497.,-4503599627370497.,9007199254740991.,
        1.5,2.5,-1.5,-2.5,0.};
    double expected[8]={4503599627370497.,-4503599627370497.,9007199254740991.,2.,2.,-2.,-2.,0.};
    int checks=0,failures=0;
    for(int rotation=0;rotation<8;++rotation) {
        double values[lanes], actual[lanes];
        for(int j=0;j<lanes;++j) values[j]=patterns[(j+rotation)&7];
        V x; x.load(values); round(x).store(actual);
        for(int j=0;j<lanes;++j) {
            ++checks;
            if(actual[j]!=expected[(j+rotation)&7]) {
                ++failures;
                std::printf("{\"kind\":\"rounding_difference\",\"bits\":%zu,\"input\":%.17g,\"actual\":%.17g,\"expected\":%.17g}\n",
                    sizeof(V)*8,values[j],actual[j],expected[(j+rotation)&7]);
            }
        }
    }
    std::printf("{\"kind\":\"rounding\",\"type\":\"f64\",\"bits\":%zu,\"lane_checks\":%d,\"failed\":%d}\n",
        sizeof(V)*8,checks,failures);
    return failures;
}
int main() {
    _mm_setcsr(0x1f80);
    int n=0;
    n+=classification<Vec4f,float,uint32_t>("f32");
    n+=classification<Vec8f,float,uint32_t>("f32");
    n+=classification<Vec16f,float,uint32_t>("f32");
    n+=classification<Vec2d,double,uint64_t>("f64");
    n+=classification<Vec4d,double,uint64_t>("f64");
    n+=classification<Vec8d,double,uint64_t>("f64");
    n+=rounding<Vec2d>(); n+=rounding<Vec4d>(); n+=rounding<Vec8d>();
    std::printf("{\"kind\":\"summary\",\"failed_lane_checks\":%d}\n",n);
    return n?1:0;
}
