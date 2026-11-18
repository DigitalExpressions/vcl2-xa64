/****************************  instrset.h   **********************************
* Author:        Agner Fog
* Date created:  2012-05-30
* Last modified: 2026-09-05
* Version:       2.02.04
* Project:       vector class library
* Description:
* Header file for various compiler-specific tasks as well as common
* macros and templates. This file contains:
*
* > Selection of the supported instruction set
* > Defines compiler version macros
* > Undefines certain macros that prevent function overloading
* > Helper functions that depend on instruction set, compiler, or platform
* > Common templates for permute, blend, etc.
*
* For instructions, see vcl_manual.pdf
*
* (c) Copyright 2012-2026 Agner Fog.
* Apache License version 2.0 or later.
******************************************************************************/

#ifndef INSTRSET_COMPAT_NEON_H
#define INSTRSET_COMPAT_NEON_H 20200
#include "vcl_config.h"
#if VCL_BACKEND != VCL_BACKEND_SSE2NEON
#error Compatibility headers require VCL_BACKEND_SSE2NEON
#endif
#include "sse2neon.h"
#ifndef INSTRSET
#define INSTRSET 6
#elif INSTRSET != 6
#error sse2neon requires INSTRSET=6
#endif

// check if compiled for C++17
#if defined(_MSVC_LANG)  // MS compiler has its own version of __cplusplus with different value
#if _MSVC_LANG < 201703
#error Please compile for C++17 or higher
#endif
#else
#if __cplusplus < 201703
#error Please compile for C++17 or higher
#endif
#endif

// Allow the use of floating point permute instructions on integer vectors.
// Some CPU's have an extra latency of 1 or 2 clock cycles for this, but
// it may still be faster than alternative implementations:
#define ALLOW_FP_PERMUTE  true


// Compiler intrinsics for ARM-compatible bit scans under MSVC.
#ifdef _MSC_VER                        // Microsoft compiler or compatible Intel compiler
#include <intrin.h>
#pragma warning(disable: 6323 4514 4710 4711) // Diasble annoying warnings
#endif

#include <stdint.h>                    // Define integer types with known size
#include <limits.h>                    // Define INT_MAX
#include <stdlib.h>                    // define abs(int)


// ARM runtime query API; the x86 feature queries return false.
// Implemented in instrset_detect_neon.cpp:
#ifdef VCL_NAMESPACE
namespace VCL_NAMESPACE {
#endif
    constexpr bool has_neon() { return VCL_ARCH_ARM64 != 0; }
    int  instrset_detect(void);        // tells which instruction sets are supported
    bool hasFMA3(void);                // true if FMA3 instructions supported
    bool hasFMA4(void);                // true if FMA4 instructions supported
    bool hasXOP(void);                 // true if XOP  instructions supported
    bool hasAVX512ER(void);            // true if AVX512ER instructions supported
    bool hasAVX512VBMI(void);          // true if AVX512VBMI instructions supported
    bool hasAVX512VBMI2(void);         // true if AVX512VBMI2 instructions supported
    bool hasF16C(void);                // true if F16C instructions supported
    bool hasAVX512FP16(void);          // true if AVX512_FP16 instructions supported

    // function in physical_processors.cpp:
    int physicalProcessors(int * logical_processors = 0);

#ifdef VCL_NAMESPACE
}
#endif


// GCC version
#if defined(__GNUC__) && !defined (GCC_VERSION) && !defined (__clang__)
#define GCC_VERSION  ((__GNUC__) * 10000 + (__GNUC_MINOR__) * 100 + (__GNUC_PATCHLEVEL__))
#endif

// Clang version
#if defined (__clang__)
#define CLANG_VERSION  ((__clang_major__) * 10000 + (__clang_minor__) * 100 + (__clang_patchlevel__))
// Problem: The version number is not consistent across platforms
// http://llvm.org/bugs/show_bug.cgi?id=12643
// Apple bug 18746972
#endif

// Fix problem with non-overloadable macros named min and max in WinDef.h
#ifdef _MSC_VER
#if defined (_WINDEF_) && defined(min) && defined(max)
#undef min
#undef max
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#endif // _MSC_VER

/* Clang problem:
The Clang compiler treats the intrinsic vector types __m128, __m128i, and __m128d as identical.
See the bug report at https://bugs.llvm.org/show_bug.cgi?id=17164
Additional problem: The version number is not consistent across platforms. The Apple build has
different version numbers. We have to rely on __apple_build_version__ on the Mac platform:
http://llvm.org/bugs/show_bug.cgi?id=12643
We have to make switches here when - hopefully - the error some day has been fixed.
We need different version checks with and whithout __apple_build_version__
*/
#if (defined (__clang__) || defined(__apple_build_version__)) && !defined(__INTEL_COMPILER)
#define FIX_CLANG_VECTOR_ALIAS_AMBIGUITY
#endif

#ifdef VCL_NAMESPACE
namespace VCL_NAMESPACE {
#endif

// Constant for indicating don't care in permute and blend functions.
// V_DC is -256 in Vector class library version 1.xx
// V_DC can be any value less than -1 in Vector class library version 2.00
constexpr int V_DC = -256;



/*****************************************************************************
*
*    Helper functions that depend on instruction set, compiler, or platform
*
*****************************************************************************/

// Define interface to cpuid instruction.
// input:  functionnumber = leaf (eax), ecxleaf = subleaf(ecx)
// output: output[0] = eax, output[1] = ebx, output[2] = ecx, output[3] = edx
static inline void cpuid(int output[4], int functionnumber, int ecxleaf = 0) {
    // ARM has no x86 CPUID leaves.
    (void)functionnumber; (void)ecxleaf;
    output[0] = output[1] = output[2] = output[3] = 0;
}


// Define popcount function. Gives sum of bits
// Use the sse2neon population-count adapters.
static inline uint32_t vml_popcnt(uint32_t a) {
    return (uint32_t)_mm_popcnt_u32(a);  // Intel intrinsic. Supported by gcc and clang
}
static inline int64_t vml_popcnt(uint64_t a) {
    return _mm_popcnt_u32(uint32_t(a >> 32)) + _mm_popcnt_u32(uint32_t(a));
}

// Define bit-scan-forward function. Gives index to lowest set bit
#if (defined (__GNUC__) || defined(__clang__)) && !defined (_MSC_VER)
// _BitScanForward intrinsics are defined only under Windows and only when _MSC_VER is defined

// Compiler builtins for ARM gcc and Clang
static uint32_t bit_scan_forward(uint32_t a) {
    return __builtin_ctz(a);
}
static inline uint32_t bit_scan_forward(uint64_t a) {
    uint32_t lo = uint32_t(a);
    if (lo) return bit_scan_forward(lo);
    uint32_t hi = uint32_t(a >> 32);
    return bit_scan_forward(hi) + 32;
}
#else
static inline uint32_t bit_scan_forward(uint32_t a) {
    unsigned long r;
    _BitScanForward(&r, a);            // defined in intrin.h for MS and Intel compilers
    return r;
}
static inline uint32_t bit_scan_forward(uint64_t a) {
    uint32_t lo = uint32_t(a);
    if (lo) return bit_scan_forward(lo);
    uint32_t hi = uint32_t(a >> 32);
    return bit_scan_forward(hi) + 32;
}
#endif


// Define bit-scan-reverse function. Gives index to highest set bit = floor(log2(a))
#if (defined (__GNUC__) || defined(__clang__)) && !defined (_MSC_VER)
// _BitScanReverse intrinsics are defined only under Windows and only when _MSC_VER is defined

// Compiler builtins for ARM gcc and Clang

static inline uint32_t bit_scan_reverse(uint32_t a) __attribute__((pure));
static inline uint32_t bit_scan_reverse(uint32_t a) {
    // As before, the input must be nonzero.
    return 31u - uint32_t(__builtin_clz(a));
}
static inline uint32_t bit_scan_reverse(uint64_t a) {
    uint64_t ahi = a >> 32;
    if (ahi == 0) return bit_scan_reverse(uint32_t(a));
    else return bit_scan_reverse(uint32_t(ahi)) + 32;
}
#else
static inline uint32_t bit_scan_reverse(uint32_t a) {
    unsigned long r;
    _BitScanReverse(&r, a);            // defined in intrin.h for MS compatible compilers
    return r;
}
static inline uint32_t bit_scan_reverse(uint64_t a) {
    uint64_t ahi = a >> 32;
    if (ahi == 0) return bit_scan_reverse(uint32_t(a));
    else return bit_scan_reverse(uint32_t(ahi)) + 32;
}
#endif 

// Same function, for compile-time constants
constexpr int bit_scan_reverse_const(uint64_t const n) {
    if (n == 0) return -1;
    uint64_t a = n, b = 0, j = 64, k = 0;
    do {
        j >>= 1;
        k = (uint64_t)1 << j;
        if (a >= k) {
            a >>= j;
            b += j;
        }
    } while (j > 0);
    return int(b);
}


/*****************************************************************************
*
*    Common templates
*
*****************************************************************************/

#ifdef VCL_NAMESPACE
#define NAMESPACEPREFIX VCL_NAMESPACE::
#else
#define NAMESPACEPREFIX
#endif

template <int32_t  n> class Const_int_t {};                // represent compile-time signed integer constant
template <uint32_t n> class Const_uint_t {};               // represent compile-time unsigned integer constant
#define const_int(n)  (NAMESPACEPREFIX Const_int_t <n>())  // n must be compile-time integer constant
#define const_uint(n) (NAMESPACEPREFIX Const_uint_t<n>())  // n must be compile-time unsigned integer constant


// template for producing quiet NAN
template <class VTYPE>
static inline VTYPE nan_vec(uint32_t payload = 0x100) {
    if constexpr (VTYPE::elementtype() == 17) {  // double
        union {
            uint64_t q;
            double f;
        } ud;
        // n is left justified to avoid loss of NAN payload when converting to float
        ud.q = 0x7FF8000000000000 | uint64_t(payload) << 29;
        return VTYPE(ud.f);
    }
    if constexpr (VTYPE::elementtype() == 16) {  // float
        union {
            uint32_t i;
            float f;
        } uf;
        uf.i = 0x7FC00000 | (payload & 0x003FFFFF);
        return VTYPE(uf.f);
    }
    /*  // defined in vectorfp16.h
    if constexpr (VTYPE::elementtype() == 15) {  // _Float16
        union {
            uint16_t i;
            _Float16 f;  // error if _Float16 not defined
        } uf;
        uf.i = 0x7C00 | (payload & 0x03FF);
        return VTYPE(uf.f);
    } */
}


// Test if a parameter is a compile-time constant
/* Unfortunately, this works only for macro parameters, not for inline function parameters.
   I hope that some solution will appear in the future, but for now it appears to be
   impossible to check if a function parameter is a compile-time constant.
   This would be useful in operator / and in function pow:
   #if defined(__GNUC__) || defined (__clang__)
   #define is_constant(a) __builtin_constant_p(a)
   #else
   #define is_constant(a) false
   #endif
*/


/*****************************************************************************
*
*    Helper functions for permute and blend functions
*
******************************************************************************
Rules for constexpr functions:

> All variable declarations must include initialization

> Do not put variable declarations inside a for-clause, e.g. avoid: for (int i=0; ..
  Instead, you have to declare the loop counter before the for-loop.

> Do not make constexpr functions that return vector types. This requires type
  punning with a union, which is not allowed in constexpr functions under C++17.

*****************************************************************************/


#ifdef VCL_NAMESPACE
}
#endif


#include "instrset_helpers_neon.h"

#endif // INSTRSET_COMPAT_NEON_H
