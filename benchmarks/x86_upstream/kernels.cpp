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
#include "common.h"
#include "vectorclass.h"
#include "vectormath_exp.h"
#include "vectormath_trig.h"
#include <type_traits>

template<class V> auto input(Inputs const* p) {
    if constexpr (std::is_same_v<decltype(V()[0]), float>) return p->f;
    else return p->d;
}
template<class V> auto output(Outputs* p) {
    if constexpr (std::is_same_v<decltype(V()[0]), float>) return p->f;
    else return p->d;
}
enum Op { add_op, mul_op, div_op, sqrt_op, round_op, nan_op, trunc_op };
template<Op op, class V> inline V step(V x, V delta) {
    if constexpr (op == add_op) return x + delta;
    if constexpr (op == mul_op) return x * delta;
    if constexpr (op == div_op) return x / delta;
    if constexpr (op == sqrt_op) return sqrt(x + delta);
    if constexpr (op == round_op) return round(x) + delta;
    if constexpr (op == nan_op) return select(is_nan(x), V(0), x) + delta;
    if constexpr (op == trunc_op) return to_double(truncatei(x)) + delta;
}
// Each operation has a dependency-chain case and four independent chains.
// The feedback addition in sqrt/round/truncate/classify is part of the workload.
template<class V, Op op, int chains> __forceinline void recurrence(Inputs const* p, Outputs* out) {
    using T = decltype(V()[0]);
    constexpr int lanes = sizeof(V) / sizeof(T);
    V a, b, c, d, delta;
    auto src = input<V>(p);
    a.load(src); b.load(src + lanes); c.load(src + lanes*2); d.load(src + lanes*3);
    if constexpr (op == mul_op || op == div_op) delta = V(T(1.000001));
    else if constexpr (op == add_op || op == nan_op) delta = V(T(0.0001));
    else delta = V(T(0.75));
    for (int j = 0; j < recurrence_steps; ++j) {
        a = step<op>(a, delta);
        if constexpr (chains == 4) {
            b = step<op>(b, delta); c = step<op>(c, delta); d = step<op>(d, delta);
        }
    }
    auto dst = output<V>(out);
    a.store(dst);
    if constexpr (chains == 4) {
        b.store(dst + lanes); c.store(dst + lanes*2); d.store(dst + lanes*3);
    }
}
enum Stream { arithmetic, sine, exponential };
template<class V, Stream op, int n> __forceinline void stream(Inputs const* p, Outputs* out) {
    using T = decltype(V()[0]);
    constexpr int lanes = sizeof(V) / sizeof(T);
    auto src = input<V>(p); auto dst = output<V>(out);
    for (int j = 0; j < n; j += lanes) {
        V a; a.load(src+j);
        if constexpr (op == arithmetic) {
            V b, c;
            if constexpr (std::is_same_v<T, float>) { b.load(p->fg+j); c.load(p->fh+j); }
            else { b.load(p->dg+j); c.load(p->dh+j); }
            a = (a+b)*c;
        }
        if constexpr (op == sine) a = sin(a);
        if constexpr (op == exponential) a = exp(a);
        a.store(dst+j);
    }
}
template<class V, int op> __forceinline void integer(Inputs const* p, Outputs* out) {
    constexpr int lanes = sizeof(V)/sizeof(int32_t);
    V x; x.load(p->i);
    Divisor_i divisor(7);
    for (int j = 0; j < recurrence_steps; ++j) {
        if constexpr (op == 0) x = x / divisor + V(19);
        if constexpr (op == 1) x = lookup<256>(x & V(255), p->i) + V(1);
    }
    x.store(out->i);
}

#define FN(name, body) extern "C" __declspec(noinline) void name(Inputs const* p, Outputs* o) { body; }
#define REC(t, w, v, label, op) \
    FN(k_##t##_##w##_##label##_latency, (recurrence<v,op,1>(p,o))) \
    FN(k_##t##_##w##_##label##_throughput, (recurrence<v,op,4>(p,o)))
#define BASE(t,w,v) \
    REC(t,w,v,add,add_op) REC(t,w,v,mul,mul_op) REC(t,w,v,div,div_op) \
    REC(t,w,v,sqrt,sqrt_op) REC(t,w,v,round,round_op) \
    FN(k_##t##_##w##_classify_throughput, (recurrence<v,nan_op,4>(p,o))) \
    FN(k_##t##_##w##_stream_128, (stream<v,arithmetic,128>(p,o))) \
    FN(k_##t##_##w##_stream_16384, (stream<v,arithmetic,16384>(p,o))) \
    FN(k_##t##_##w##_sin_128, (stream<v,sine,128>(p,o))) \
    FN(k_##t##_##w##_exp_128, (stream<v,exponential,128>(p,o)))
BASE(f32,128,Vec4f) BASE(f32,256,Vec8f) BASE(f32,512,Vec16f)
BASE(f64,128,Vec2d) BASE(f64,256,Vec4d) BASE(f64,512,Vec8d)
REC(f64,128,Vec2d,truncate,trunc_op)
REC(f64,256,Vec4d,truncate,trunc_op)
REC(f64,512,Vec8d,truncate,trunc_op)
#define INT(w,v) \
    FN(k_i32_##w##_divide_latency, (integer<v,0>(p,o))) \
    FN(k_i32_##w##_lookup_latency, (integer<v,1>(p,o)))
INT(128,Vec4i) INT(256,Vec8i) INT(512,Vec16i)

#define ENTRY(t,w,op,ch,n,name) {#t "_" #w "_" #name, #op, #t, w, ch, n, k_##t##_##w##_##name},
#define R(t,w,op) \
    ENTRY(t,w,op,1,w/(sizeof(t)*8),op##_latency) \
    ENTRY(t,w,op,4,4*w/(sizeof(t)*8),op##_throughput)
// Type aliases make lane counts independent of vector ABI or padding.
using f32 = float; using f64 = double; using i32 = int32_t;
#define B(t,w) \
    R(t,w,add) R(t,w,mul) R(t,w,div) R(t,w,sqrt) R(t,w,round) \
    ENTRY(t,w,classify,4,4*w/(sizeof(t)*8),classify_throughput) \
    ENTRY(t,w,stream,0,128,stream_128) ENTRY(t,w,stream,0,16384,stream_16384) \
    ENTRY(t,w,sin,0,128,sin_128) ENTRY(t,w,exp,0,128,exp_128)
Case const* cases(size_t& count) {
    static Case const list[] = {
        B(f32,128) B(f32,256) B(f32,512) B(f64,128) B(f64,256) B(f64,512)
        R(f64,128,truncate) R(f64,256,truncate) R(f64,512,truncate)
        ENTRY(i32,128,divide,1,4,divide_latency) ENTRY(i32,256,divide,1,8,divide_latency)
        ENTRY(i32,512,divide,1,16,divide_latency)
        ENTRY(i32,128,lookup,1,4,lookup_latency) ENTRY(i32,256,lookup,1,8,lookup_latency)
        ENTRY(i32,512,lookup,1,16,lookup_latency)
    };
    count = sizeof(list)/sizeof(*list); return list;
}
