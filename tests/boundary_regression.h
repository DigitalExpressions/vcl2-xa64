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
#ifndef VCL_TEST_BOUNDARY_REGRESSION_H
#define VCL_TEST_BOUNDARY_REGRESSION_H
#include "vectorclass.h"
#include "protected_memory.h"
#include <cstdint>
#include <cstdio>
namespace vcl_test {
#ifdef VCL_NAMESPACE
using namespace VCL_NAMESPACE;
#endif
// Use scalar index arrays so each boundary entry is checked in every vector
// lane. No table padding is present before the inaccessible trailing page.
template<int N,class V,class I,class T,class Index> int check_lookup_boundary() {
    const size_t page=page_size();
    ProtectedMemory allocation(2*page);
    allocation.writable(0,page);
    T* table=reinterpret_cast<T*>(allocation.data()+page)-N;
    for(int i=0;i<N;++i)table[i]=T(i+3);
    int failures=0;
    for(int start=0;start<N;++start) {
        Index indices[I::size()];
        T output[V::size()];
        for(int lane=0;lane<I::size();++lane)indices[lane]=Index((start+lane)%N);
        I index;index.load(indices);
        lookup<N>(index,table).store(output);
        for(int lane=0;lane<V::size();++lane) {
            const T expected=T(int(indices[lane])+3);
            if(output[lane]!=expected) {
                if(failures<4)std::fprintf(stderr,"bounded lookup N=%d lanes=%d start=%d lane=%d\n",N,V::size(),start,lane);
                ++failures;
            }
        }
    }
    return failures;
}
inline int boundary_regression() {
    int failures=check_lookup_boundary<17,Vec8s,Vec8s,int16_t,int16_t>();
    const Vec4i quotient=Vec4i(INT32_MIN,0,1,INT32_MAX)/const_int(INT32_MIN);
    for(int lane=0;lane<4;++lane)failures+=quotient[lane]!=(lane==0?1:0);
    const Vec8s short_quotient=Vec8s(INT16_MIN,0,1,INT16_MAX,INT16_MIN,-1,2,-2)/const_int(INT16_MIN);
    for(int lane=0;lane<8;++lane)failures+=short_quotient[lane]!=((lane==0||lane==4)?1:0);
#if MAX_VECTOR_SIZE >= 256
    failures+=check_lookup_boundary<1,Vec32c,Vec32c,int8_t,int8_t>();
    failures+=check_lookup_boundary<35,Vec32c,Vec32c,int8_t,int8_t>();
    failures+=check_lookup_boundary<17,Vec16s,Vec16s,int16_t,int16_t>();
    failures+=check_lookup_boundary<1,Vec8i,Vec8i,int32_t,int32_t>();
    failures+=check_lookup_boundary<1,Vec8f,Vec8i,float,int32_t>();
    failures+=check_lookup_boundary<1,Vec4d,Vec4q,double,int64_t>();
    const Vec32c zero=shift_bytes_down<32>(Vec32c(1));
    for(int lane=0;lane<32;++lane)failures+=zero[lane]!=0;
    const int32_t words[]={INT32_MIN,-1001,-1,0,1,65537,1000000007,INT32_MAX};
    Vec8i values;values.load(words);
    const Vec8i composed=compress(extend_low(values),extend_high(values));
    for(int lane=0;lane<8;++lane)failures+=composed[lane]!=words[lane];
#endif
#if MAX_VECTOR_SIZE >= 512
    failures+=check_lookup_boundary<1,Vec16i,Vec16i,int32_t,int32_t>();
    failures+=check_lookup_boundary<1,Vec16f,Vec16i,float,int32_t>();
    failures+=check_lookup_boundary<1,Vec8d,Vec8q,double,int64_t>();
    const Vec64c zero64=shift_bytes_down<64>(Vec64c(1));
    for(int lane=0;lane<64;++lane)failures+=zero64[lane]!=0;
#endif
    return failures;
}
} // namespace vcl_test
#endif
