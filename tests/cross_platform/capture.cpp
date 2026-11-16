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

#include "vectormath_trig.h"
#include "vectormath_exp.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

struct Case {
    std::string id, type, op, policy;
    int width, rounding, parameter;
    uint64_t a[16]{}, b[16]{}, c[16]{};
};

template<class V> void execute(Case const& item) {
    using T = decltype(V()[0]);
    constexpr int lanes = V::size();
    T aa[lanes], bb[lanes], cc[lanes], out[lanes];
    for (int i=0;i<lanes;++i) {
        if constexpr(sizeof(T)==4) {
            uint32_t a=uint32_t(item.a[i]), b=uint32_t(item.b[i]), c=uint32_t(item.c[i]);
            std::memcpy(aa+i,&a,4); std::memcpy(bb+i,&b,4); std::memcpy(cc+i,&c,4);
        } else {
            std::memcpy(aa+i,item.a+i,8); std::memcpy(bb+i,item.b+i,8); std::memcpy(cc+i,item.c+i,8);
        }
    }
    V a,b,c,result;
    a.load(aa); b.load(bb); c.load(cc);
    uint64_t raw[16]{};
    bool raw_result=false;
    auto const& op=item.op;
    if(op=="load") result=a;
    else if(op=="add") result=a+b;
    else if(op=="sub") result=a-b;
    else if(op=="mul") result=a*b;
    else if(op=="lt") {
        auto mask=a<b;
        for(int i=0;i<lanes;++i) raw[i]=mask[i]?1:0;
        raw_result=true;
    } else if(op=="select") result=select(a<b,a,b);
    else if constexpr(std::is_integral<T>::value) {
        if(op=="xor") result=a^b;
        else if(op=="shift_left") result=a<<3;
        else if(op=="shift_right") result=a>>3;
        else throw std::runtime_error("Unknown integer operation");
    } else {
        if(op=="div") result=a/b;
        else if(op=="min") result=min(a,b);
        else if(op=="max") result=max(a,b);
        else if(op=="neg") result=-a;
        else if(op=="mul_add") result=mul_add(a,b,c);
        else if(op=="classify") {
            auto nan=is_nan(a), inf=is_inf(a), sub=is_subnormal(a);
            for(int i=0;i<lanes;++i) raw[i]=nan[i]?1:inf[i]?2:aa[i]==T(0)?4:sub[i]?8:16;
            raw_result=true;
        } else if(op=="roundi") {
            auto converted=roundi(a);
            for(int i=0;i<lanes;++i) {
                if constexpr(sizeof(T)==4) raw[i]=uint32_t(converted[i]);
                else raw[i]=uint64_t(converted[i]);
            }
            raw_result=true;
        } else if(op=="load_partial") result.load_partial(item.parameter,aa);
        else if(op=="store_partial") {
            T buffer[lanes+2];
            for(int i=0;i<lanes+2;++i) {
                if constexpr(sizeof(T)==4) { uint32_t v=0x1234;std::memcpy(buffer+i,&v,4); }
                else { uint64_t v=0x1234;std::memcpy(buffer+i,&v,8); }
            }
            a.store_partial(item.parameter,buffer+1);
            uint64_t first=0,last=0;
            std::memcpy(&first,buffer,sizeof(T));std::memcpy(&last,buffer+lanes+1,sizeof(T));
            if(first!=0x1234 || last!=0x1234) throw std::runtime_error("Partial-store canary changed");
            result.load(buffer+1);
        } else if(op=="approx_recipr") {
            if constexpr(std::is_same<T,float>::value) result=approx_recipr(a);
            else throw std::runtime_error("Reciprocal profile is FP32 only");
        } else if(op=="exp") result=exp(a);
        else if(op=="expm1") result=expm1(a);
        else if(op=="exp2") result=exp2(a);
        else if(op=="exp10") result=exp10(a);
        else if(op=="log") result=log(a);
        else if(op=="log1p") result=log1p(a);
        else if(op=="log2") result=log2(a);
        else if(op=="log10") result=log10(a);
        else if(op=="sin") result=sin(a);
        else if(op=="cos") result=cos(a);
        else if(op=="tan") result=tan(a);
        else throw std::runtime_error("Unknown floating-point operation");
    }
    if(!raw_result) {
        result.store(out);
        for(int i=0;i<lanes;++i) std::memcpy(raw+i,out+i,sizeof(T));
    }
    std::printf("{\"id\":\"%s\",\"type\":\"%s\",\"width\":%d,\"op\":\"%s\",\"rounding\":%d,\"policy\":\"%s\",\"bits\":[",
                item.id.c_str(),item.type.c_str(),item.width,item.op.c_str(),item.rounding,item.policy.c_str());
    for(int i=0;i<lanes;++i) std::printf(i?",\"%0*llx\"":"\"%0*llx\"",int(sizeof(T)*2),(unsigned long long)raw[i]);
    std::puts("]}");
}

bool dispatch(Case const& item) {
#define CAPTURE(t,w,v) if(item.type==t && item.width==w) {execute<v>(item);return true;}
    CAPTURE("i32",128,Vec4i) CAPTURE("u32",128,Vec4ui)
    CAPTURE("i64",128,Vec2q) CAPTURE("u64",128,Vec2uq)
    CAPTURE("f32",128,Vec4f) CAPTURE("f64",128,Vec2d)
#if MAX_VECTOR_SIZE >= 256
    CAPTURE("i32",256,Vec8i) CAPTURE("u32",256,Vec8ui)
    CAPTURE("i64",256,Vec4q) CAPTURE("u64",256,Vec4uq)
    CAPTURE("f32",256,Vec8f) CAPTURE("f64",256,Vec4d)
#endif
#if MAX_VECTOR_SIZE >= 512
    CAPTURE("i32",512,Vec16i) CAPTURE("u32",512,Vec16ui)
    CAPTURE("i64",512,Vec8q) CAPTURE("u64",512,Vec8uq)
    CAPTURE("f32",512,Vec16f) CAPTURE("f64",512,Vec8d)
#endif
#undef CAPTURE
    return false;
}

int main(int argc,char** argv) {
    try {
        if(argc!=2) throw std::runtime_error("Expected a generated input file");
        std::ifstream input(argv[1]);
        if(!input) throw std::runtime_error("Cannot open input file");
        reset_control_word();
#if VCL_BACKEND == VCL_BACKEND_NEON
        const int instruction_set=0;
#else
        const int instruction_set=INSTRSET;
#endif
#if VCL_BACKEND == VCL_BACKEND_NEON || defined(__FMA__) || defined(__FMA4__)
        const char* fma="fused";
#else
        const char* fma="unfused";
#endif
        std::printf("{\"metadata\":true,\"schema\":1,\"backend\":%d,\"instrset\":%d,\"max_width\":%d,\"fma\":\"%s\",\"fp_control\":%u}\n",
                    VCL_BACKEND,instruction_set,MAX_VECTOR_SIZE,fma,unsigned(get_control_word()));
        Case item;
        while(input>>item.id>>item.type>>item.width>>item.op>>item.rounding>>item.parameter>>item.policy) {
            if(item.width!=128 && item.width!=256 && item.width!=512) throw std::runtime_error("Invalid width");
            if(item.rounding<0 || item.rounding>3 || item.parameter<0) throw std::runtime_error("Invalid parameters");
            int bits=(item.type=="f32" || item.type=="i32" || item.type=="u32")?32:64;
            int lanes=item.width/bits;
            if(item.parameter>lanes) throw std::runtime_error("Invalid partial count");
            for(auto buffer:{item.a,item.b,item.c}) for(int i=0;i<lanes;++i) {
                std::string value;
                if(!(input>>value)) throw std::runtime_error("Incomplete inputs");
                buffer[i]=std::stoull(value,nullptr,16);
            }
            set_control_word(0x1f80u | (unsigned(item.rounding)<<13));
            if((get_control_word()&0xe040u)!=(unsigned(item.rounding)<<13))
                throw std::runtime_error("FP rounding/flush environment mismatch");
            if(!dispatch(item)) throw std::runtime_error("Unsupported type/width");
        }
        if(!input.eof()) throw std::runtime_error("Malformed input");
        reset_control_word();
        return 0;
    } catch(std::exception const& error) {
        std::fprintf(stderr,"Capture failed: %s\n",error.what());
        return 1;
    }
}
