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

// Internal MSVC x86 classification, included after instrset.h. Keep native
// register operations here rather than routing x86 compiler workarounds through
// portable vector composition. ARM and other compilers retain their own paths.
#ifndef VCL_X86_CLASSIFICATION_H
#define VCL_X86_CLASSIFICATION_H
#if VCL_BACKEND == VCL_BACKEND_X86 && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#ifdef VCL_NAMESPACE
namespace VCL_NAMESPACE {
#endif
namespace vcl_detail_x86 {

static __forceinline __m128 nan_mask(__m128 a) {
    __m128i magnitude = _mm_and_si128(_mm_castps_si128(a), _mm_set1_epi32(0x7FFFFFFF));
    return _mm_castsi128_ps(_mm_cmpgt_epi32(magnitude, _mm_set1_epi32(0x7F800000)));
}

static __forceinline __m128d nan_mask(__m128d a) {
    __m128i magnitude = _mm_and_si128(_mm_castpd_si128(a), _mm_set_epi32(0x7FFFFFFF, -1, 0x7FFFFFFF, -1));
    __m128i infinity = _mm_set_epi32(0x7FF00000, 0, 0x7FF00000, 0);
#if INSTRSET >= 6
    return _mm_castsi128_pd(_mm_cmpgt_epi64(magnitude, infinity));
#else
    // Both operands are nonnegative signed 64-bit integers. Infinity-magnitude
    // cannot overflow; its sign is set exactly for NaN encodings. Replicate the
    // high dword sign to obtain a full mask without emulated 64-bit comparison.
    __m128i difference = _mm_sub_epi64(infinity, magnitude);
    return _mm_castsi128_pd(_mm_shuffle_epi32(_mm_srai_epi32(difference, 31), 0xF5));
#endif
}

#if INSTRSET >= 10
static __forceinline __m256i magnitude(__m256 a) {
    return _mm256_and_si256(_mm256_castps_si256(a), _mm256_set1_epi32(0x7FFFFFFF));
}
static __forceinline __m256i magnitude(__m256d a) {
    return _mm256_and_si256(_mm256_castpd_si256(a), _mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFll));
}

static __forceinline __mmask8 finite_mask(__m256 a) {
    return _mm256_cmp_epi32_mask(magnitude(a), _mm256_set1_epi32(0x7F800000), _MM_CMPINT_LT);
}
static __forceinline __mmask8 finite_mask(__m256d a) {
    return _mm256_cmp_epi64_mask(magnitude(a), _mm256_set1_epi64x(0x7FF0000000000000ll), _MM_CMPINT_LT);
}
static __forceinline __mmask8 inf_mask(__m256 a) {
    return _mm256_cmp_epi32_mask(magnitude(a), _mm256_set1_epi32(0x7F800000), _MM_CMPINT_EQ);
}
static __forceinline __mmask8 inf_mask(__m256d a) {
    return _mm256_cmp_epi64_mask(magnitude(a), _mm256_set1_epi64x(0x7FF0000000000000ll), _MM_CMPINT_EQ);
}
static __forceinline __mmask8 nan_mask(__m256 a) {
    return _mm256_cmp_epi32_mask(magnitude(a), _mm256_set1_epi32(0x7F800000), _MM_CMPINT_GT);
}
static __forceinline __mmask8 nan_mask(__m256d a) {
    return _mm256_cmp_epi64_mask(magnitude(a), _mm256_set1_epi64x(0x7FF0000000000000ll), _MM_CMPINT_GT);
}
static __forceinline __mmask8 subnormal_mask(__m256 a) {
    __m256i offset = _mm256_sub_epi32(magnitude(a), _mm256_set1_epi32(1));
    return _mm256_cmp_epu32_mask(offset, _mm256_set1_epi32(0x007FFFFF), _MM_CMPINT_LT);
}
static __forceinline __mmask8 subnormal_mask(__m256d a) {
    __m256i offset = _mm256_sub_epi64(magnitude(a), _mm256_set1_epi64x(1));
    return _mm256_cmp_epu64_mask(offset, _mm256_set1_epi64x(0x000FFFFFFFFFFFFFll), _MM_CMPINT_LT);
}
static __forceinline __mmask8 zero_subnormal_mask(__m256 a) {
    return _mm256_cmp_epi32_mask(magnitude(a), _mm256_set1_epi32(0x00800000), _MM_CMPINT_LT);
}
static __forceinline __mmask8 zero_subnormal_mask(__m256d a) {
    return _mm256_cmp_epi64_mask(magnitude(a), _mm256_set1_epi64x(0x0010000000000000ll), _MM_CMPINT_LT);
}
#endif

#if INSTRSET >= 9
static __forceinline __m512i magnitude(__m512 a) {
    return _mm512_and_si512(_mm512_castps_si512(a), _mm512_set1_epi32(0x7FFFFFFF));
}
static __forceinline __m512i magnitude(__m512d a) {
    return _mm512_and_si512(_mm512_castpd_si512(a), _mm512_set1_epi64(0x7FFFFFFFFFFFFFFFll));
}

static __forceinline __mmask16 finite_mask(__m512 a) {
    return _mm512_cmp_epi32_mask(magnitude(a), _mm512_set1_epi32(0x7F800000), _MM_CMPINT_LT);
}
static __forceinline __mmask8 finite_mask(__m512d a) {
    return _mm512_cmp_epi64_mask(magnitude(a), _mm512_set1_epi64(0x7FF0000000000000ll), _MM_CMPINT_LT);
}
static __forceinline __mmask16 inf_mask(__m512 a) {
    return _mm512_cmp_epi32_mask(magnitude(a), _mm512_set1_epi32(0x7F800000), _MM_CMPINT_EQ);
}
static __forceinline __mmask8 inf_mask(__m512d a) {
    return _mm512_cmp_epi64_mask(magnitude(a), _mm512_set1_epi64(0x7FF0000000000000ll), _MM_CMPINT_EQ);
}
static __forceinline __mmask16 nan_mask(__m512 a) {
    return _mm512_cmp_epi32_mask(magnitude(a), _mm512_set1_epi32(0x7F800000), _MM_CMPINT_GT);
}
static __forceinline __mmask8 nan_mask(__m512d a) {
    return _mm512_cmp_epi64_mask(magnitude(a), _mm512_set1_epi64(0x7FF0000000000000ll), _MM_CMPINT_GT);
}
static __forceinline __mmask16 subnormal_mask(__m512 a) {
    __m512i offset = _mm512_sub_epi32(magnitude(a), _mm512_set1_epi32(1));
    return _mm512_cmp_epu32_mask(offset, _mm512_set1_epi32(0x007FFFFF), _MM_CMPINT_LT);
}
static __forceinline __mmask8 subnormal_mask(__m512d a) {
    __m512i offset = _mm512_sub_epi64(magnitude(a), _mm512_set1_epi64(1));
    return _mm512_cmp_epu64_mask(offset, _mm512_set1_epi64(0x000FFFFFFFFFFFFFll), _MM_CMPINT_LT);
}
static __forceinline __mmask16 zero_subnormal_mask(__m512 a) {
    return _mm512_cmp_epi32_mask(magnitude(a), _mm512_set1_epi32(0x00800000), _MM_CMPINT_LT);
}
static __forceinline __mmask8 zero_subnormal_mask(__m512d a) {
    return _mm512_cmp_epi64_mask(magnitude(a), _mm512_set1_epi64(0x0010000000000000ll), _MM_CMPINT_LT);
}
#endif
} // namespace vcl_detail_x86
#ifdef VCL_NAMESPACE
} // namespace VCL_NAMESPACE
#endif
#endif
#endif
