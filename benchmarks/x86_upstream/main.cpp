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
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include <immintrin.h>
#include "common.h"
#include <algorithm>
#include <atomic>
#include <cfenv>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

// MSVC has no supported /Qvec- switch. Control its reference loops explicitly.
#if defined(_MSC_VER) && !defined(__clang__)
#define SCALAR_LOOP __pragma(loop(no_vector))
#else
#define SCALAR_LOOP
#endif

struct Guarded {
    unsigned char* base;
    unsigned char* data;
    size_t size, page;
    explicit Guarded(size_t bytes) {
        SYSTEM_INFO s{}; GetSystemInfo(&s); page=s.dwPageSize;
        size=(bytes+page-1)/page*page;
        base=static_cast<unsigned char*>(VirtualAlloc(nullptr,size+2*page,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS));
        DWORD old=0;
        if (!base || !VirtualProtect(base+page,size,PAGE_READWRITE,&old)) std::abort();
        data=base+page+size-bytes;
    }
    ~Guarded() { VirtualFree(base,0,MEM_RELEASE); }
};
void initialize(Inputs& p) {
    uint32_t state=0x5eed1234;
    SCALAR_LOOP
    for (int j=0;j<capacity;++j) {
        state=state*1664525u+1013904223u;
        // Exactly representable binary fractions, shared across all builds.
        double x=0.25+double(state&4095u)/4096.;
        p.f[j]=float(x); p.d[j]=x;
        p.fg[j]=float(0.125+double(j&31)/256.); p.dg[j]=p.fg[j];
        p.fh[j]=float(0.875+double(j&15)/128.); p.dh[j]=p.fh[j];
    }
    SCALAR_LOOP
    for(int j=0;j<256;++j) p.i[j]=(j*73+19)&127;
}
template<class T> uint64_t bits(T x) {
    uint64_t n=0; std::memcpy(&n,&x,sizeof(x)); return n;
}
template<class T> T scalar_step(char const* op,T x) {
    if (!std::strcmp(op,"add") || !std::strcmp(op,"classify")) return T(x+T(.0001));
    if (!std::strcmp(op,"mul")) return T(x*T(1.000001));
    if (!std::strcmp(op,"div")) return T(x/T(1.000001));
    if (!std::strcmp(op,"sqrt")) return std::sqrt(T(x+T(.75)));
    if (!std::strcmp(op,"round")) return T(std::nearbyint(x)+T(.75));
    if (!std::strcmp(op,"truncate")) return T(T(int64_t(x))+T(.75));
    std::abort();
}
template<class T> bool check_fp(Case const& c, Inputs const& p, Outputs const& out, uint64_t& max_ulp) {
    T const* src; T const* actual;
    if constexpr (sizeof(T)==4) { src=p.f; actual=out.f; }
    else { src=p.d; actual=out.d; }
    bool approximate=!std::strcmp(c.operation,"sin") || !std::strcmp(c.operation,"exp");
    SCALAR_LOOP
    for (int j=0;j<c.count;++j) {
        T expected=src[j];
        if(c.chains) {
            SCALAR_LOOP
            for (int k=0;k<recurrence_steps;++k) expected=scalar_step(c.operation,expected);
        }
        else if(!std::strcmp(c.operation,"stream")) {
            T b, d;
            if constexpr (sizeof(T)==4) { b=p.fg[j]; d=p.fh[j]; }
            else { b=p.dg[j]; d=p.dh[j]; }
            expected=T(T(expected+b)*d);
        } else if(!std::strcmp(c.operation,"sin")) expected=std::sin(expected);
        else expected=std::exp(expected);
        uint64_t a=bits(actual[j]), b=bits(expected), ulp=a>b?a-b:b-a;
        max_ulp=std::max(max_ulp,ulp);
        if(!std::isfinite(actual[j]) || ulp>(approximate?4u:0u)) {
            std::fprintf(stderr,"CHECK FAILED %s lane=%d actual=%.17g expected=%.17g ulp=%llu\n",
                c.name,j,double(actual[j]),double(expected),static_cast<unsigned long long>(ulp));
            return false;
        }
    }
    return true;
}
bool check(Case const& c,Inputs const& p,Outputs const& out,uint64_t& max_ulp) {
    if(!std::strcmp(c.type,"f32")) return check_fp<float>(c,p,out,max_ulp);
    if(!std::strcmp(c.type,"f64")) return check_fp<double>(c,p,out,max_ulp);
    SCALAR_LOOP
    for(int j=0;j<c.count;++j) {
        int32_t x=p.i[j];
        SCALAR_LOOP
        for(int k=0;k<recurrence_steps;++k) {
            if(!std::strcmp(c.operation,"divide")) x=x/7+19;
            else x=p.i[x&255]+1;
        }
        if(x!=out.i[j]) { std::fprintf(stderr,"CHECK FAILED %s lane=%d\n",c.name,j); return false; }
    }
    return true;
}
uint64_t checksum(Case const& c,Outputs const& out) {
    uint64_t h=14695981039346656037ull;
    auto bytes= reinterpret_cast<unsigned char const*>(!std::strcmp(c.type,"f32")?static_cast<void const*>(out.f):
        !std::strcmp(c.type,"f64")?static_cast<void const*>(out.d):static_cast<void const*>(out.i));
    size_t n=c.count*(!std::strcmp(c.type,"f64")?8:4);
    SCALAR_LOOP
    for(size_t j=0;j<n;++j) h=(h^bytes[j])*1099511628211ull;
    return h;
}
int probe() {
    int r[4]; __cpuid(r,0); int highest=r[0]; char vendor[13]{};
    std::memcpy(vendor,r+1,4); std::memcpy(vendor+4,r+3,4); std::memcpy(vendor+8,r+2,4);
    __cpuidex(r,1,0); unsigned c=unsigned(r[2]),d=unsigned(r[3]);
    uint64_t xcr0=(c&(1u<<27))?_xgetbv(0):0;
    bool avx=(c&(1u<<28)) && (xcr0&6)==6;
    __cpuidex(r,7,0); unsigned b=highest>=7?unsigned(r[1]):0;
    bool avx2=avx && (b&(1u<<5)) && (c&(1u<<12)) && (c&(1u<<29));
    unsigned mask=(1u<<16)|(1u<<17)|(1u<<30)|(1u<<31);
    bool avx512=avx2 && (b&mask)==mask && (xcr0&0xe6)==0xe6;
    std::printf("{\"vendor\":\"%s\",\"sse2\":%s,\"sse42\":%s,\"avx2\":%s,\"avx512\":%s,\"xcr0\":\"0x%llx\",\"cpuid1_ecx\":%u,\"cpuid7_ebx\":%u}\n",
        vendor,(d&(1u<<26))?"true":"false",(c&(1u<<20))?"true":"false",avx2?"true":"false",avx512?"true":"false",
        static_cast<unsigned long long>(xcr0),c,b);
    return 0;
}
int main(int argc,char** argv) {
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    if(argc>1 && !std::strcmp(argv[1],"--probe")) return probe();
    if(argc>2 && !std::strcmp(argv[1],"--fault")) {
        Guarded g(4096); volatile unsigned char* p=g.data;
        int n=std::atoi(argv[2]); ptrdiff_t offset=n<2?-1:4096;
        if(n%2) p[offset]=7; else return p[offset];
        return 0;
    }
    bool measure=false; int trials=7, cpu=0; double target_ms=4.;
    for(int j=1;j<argc;++j) {
        if(!std::strcmp(argv[j],"--measure")) measure=true;
        else if(!std::strcmp(argv[j],"--trials") && j+1<argc) trials=std::atoi(argv[++j]);
        else if(!std::strcmp(argv[j],"--sample-ms") && j+1<argc) target_ms=std::atof(argv[++j]);
        else if(!std::strcmp(argv[j],"--cpu") && j+1<argc) cpu=std::atoi(argv[++j]);
        else if(std::strcmp(argv[j],"--check")) return 2;
    }
    DWORD_PTR process_mask=0,system_mask=0;
    if(cpu<0 || cpu>=64 || trials<1 || target_ms<=0 ||
        !GetProcessAffinityMask(GetCurrentProcess(),&process_mask,&system_mask) ||
        !(process_mask&(DWORD_PTR(1)<<cpu)) || !SetThreadAffinityMask(GetCurrentThread(),DWORD_PTR(1)<<cpu)) return 3;
    std::fesetround(FE_TONEAREST); _mm_setcsr(0x1f80); // nearest-even, masked exceptions, no FTZ/DAZ
    Guarded gi(sizeof(Inputs)),go(sizeof(Outputs));
    auto* p=new (gi.data) Inputs; auto* out=new (go.data) Outputs;
    initialize(*p);
    uint64_t input_hash=14695981039346656037ull;
    SCALAR_LOOP
    for(size_t j=0;j<sizeof(Inputs);++j) input_hash=(input_hash^gi.data[j])*1099511628211ull;
    size_t count=0; auto list=cases(count);
    std::printf("{\"kind\":\"environment\",\"cpu\":%d,\"mxcsr\":%u,\"page_size\":%zu,\"input_fnv1a64\":\"%016llx\",\"cases\":%zu}\n",
        cpu,_mm_getcsr(),gi.page,static_cast<unsigned long long>(input_hash),count);
    SCALAR_LOOP
    for(size_t j=0;j<count;++j) {
        auto const& c=list[j];
        std::memset(out,0,sizeof(*out)); c.kernel(p,out);
        uint64_t max_ulp=0;
        if(!check(c,*p,*out,max_ulp)) return 4;
        uint64_t expected_hash=checksum(c,*out);
        std::printf("{\"kind\":\"check\",\"case\":\"%s\",\"pass\":true,\"max_ulp\":%llu,\"output_fnv1a64\":\"%016llx\"}\n",
            c.name,static_cast<unsigned long long>(max_ulp),static_cast<unsigned long long>(expected_hash));
        if(!measure) continue;
        auto elapsed=[&](uint64_t repeats) {
            std::atomic_signal_fence(std::memory_order_seq_cst);
            auto begin=std::chrono::steady_clock::now();
            SCALAR_LOOP
            for(uint64_t k=0;k<repeats;++k) c.kernel(p,out);
            auto end=std::chrono::steady_clock::now();
            std::atomic_signal_fence(std::memory_order_seq_cst);
            return std::chrono::duration<double,std::nano>(end-begin).count();
        };
        // Warm each kernel for at least the target sample length, then calibrate.
        uint64_t repeats=1;
        while(elapsed(repeats)<target_ms*1e6 && repeats<(1ull<<26)) repeats*=2;
        double units=c.chains?double(recurrence_steps)*c.chains:double(c.count)/(c.bits/(!std::strcmp(c.type,"f64")?64:32));
        for(int k=0;k<trials;++k) {
            double ns=elapsed(repeats);
            if(checksum(c,*out)!=expected_hash) return 5;
            std::printf("{\"kind\":\"sample\",\"case\":\"%s\",\"trial\":%d,\"calls\":%llu,\"elapsed_ns\":%.0f,\"units_per_call\":%.0f,\"ns_per_unit\":%.9g}\n",
                c.name,k,static_cast<unsigned long long>(repeats),ns,units,ns/(double(repeats)*units));
        }
        std::fflush(stdout);
    }
    return 0;
}
