/****************************  vectori128.h   *******************************
* Author:        Agner Fog
* Date created:  2012-05-30
* Last modified: 2026-09-09
* Version:       2.02.04
* Project:       vector class library
* Description:
* Header file defining 128-bit integer vector classes
*
* Instructions: see vcl_manual.pdf
*
* The following vector classes are defined here:
* Vec128b   Vector of 128  bits. Used internally as base class
* Vec16c    Vector of  16  8-bit signed    integers
* Vec16uc   Vector of  16  8-bit unsigned  integers
* Vec16cb   Vector of  16  Booleans for use with Vec16c and Vec16uc
* Vec8s     Vector of   8  16-bit signed   integers
* Vec8us    Vector of   8  16-bit unsigned integers
* Vec8sb    Vector of   8  Booleans for use with Vec8s and Vec8us
* Vec4i     Vector of   4  32-bit signed   integers
* Vec4ui    Vector of   4  32-bit unsigned integers
* Vec4ib    Vector of   4  Booleans for use with Vec4i and Vec4ui
* Vec2q     Vector of   2  64-bit signed   integers
* Vec2uq    Vector of   2  64-bit unsigned integers
* Vec2qb    Vector of   2  Booleans for use with Vec2q and Vec2uq
*
* Each vector object is represented internally in the CPU as a 128-bit register.
* This header file defines operators and functions for these vectors.
*
* (c) Copyright 2012-2026 Agner Fog.
* Apache License version 2.0 or later.
*****************************************************************************/

#ifndef VECTORI128_COMPAT_NEON_H
#define VECTORI128_COMPAT_NEON_H

#ifndef VECTORCLASS_NEON_H
#include "vectorclass_neon.h"
#endif

#if VECTORCLASS_NEON_H < 20200
#error Incompatible versions of vector class library mixed
#endif

#ifdef VCL_NAMESPACE         // optional namespace
namespace VCL_NAMESPACE {
#endif


// Generate a constant vector of 4 integers stored in memory.
template <uint32_t i0, uint32_t i1, uint32_t i2, uint32_t i3>
static inline constexpr __m128i constant4ui() {
    /*
    const union {
        uint32_t i[4];
        __m128i  xmm;
    } u = { {i0,i1,i2,i3} };
    return u.xmm;
    */
    return _mm_setr_epi32(i0, i1, i2, i3);
}


/*****************************************************************************
*
*          Compact boolean vectors
*
*****************************************************************************/


/*****************************************************************************
*
*          Define operators and functions for Vec16b
*
*****************************************************************************/



/*****************************************************************************
*
*          Define operators and functions for Vec8b
*
*****************************************************************************/



/*****************************************************************************
*
*          Define operators for Vec4b
*
*****************************************************************************/


/*****************************************************************************
*
*     Vector of 128 bits. Used internally as base class
*
*****************************************************************************/
class Vec128b {
protected:
    __m128i xmm; // Integer vector
public:
    // Default constructor:
    Vec128b() = default;
    // Constructor to convert from type __m128i used in intrinsics:
    Vec128b(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec128b & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Type cast operator to convert to __m128i used in intrinsics
    operator __m128i() const {
        return xmm;
    }
    // Member function to load from array (unaligned)
    Vec128b & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array, aligned by 16
    // "load_a" is faster than "load" on older Intel processors (Pentium 4, Pentium M, Core 1,
    // Merom, Wolfdale, and Atom), but not on other processors from Intel, AMD or VIA.
    // You may use load_a instead of load if you are certain that p points to an address
    // divisible by 16.
    void load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
    }
    // Member function to store into array (unaligned)
    void store(void * p) const {
        _mm_storeu_si128((__m128i*)p, xmm);
    }
    // Member function storing into array, aligned by 16
    // "store_a" is faster than "store" on older Intel processors (Pentium 4, Pentium M, Core 1,
    // Merom, Wolfdale, and Atom), but not on other processors from Intel, AMD or VIA.
    // You may use store_a instead of store if you are certain that p points to an address
    // divisible by 16.
    void store_a(void * p) const {
        _mm_store_si128((__m128i*)p, xmm);
    }
    // Member function storing to aligned uncached memory (non-temporal store).
    // This may be more efficient than store_a when storing large blocks of memory if it 
    // is unlikely that the data will stay in the cache until it is read again.
    // Note: Will generate runtime error if p is not aligned by 16
    void store_nt(void * p) const {
        _mm_stream_si128((__m128i*)p, xmm);
    }
    static constexpr int size() {
        return 128;
    }
    static constexpr int elementtype() {
        return 1;
    }
    typedef __m128i registertype;
};

// Define operators for this class

// vector operator & : bitwise and
static inline Vec128b operator & (Vec128b const a, Vec128b const b) {
    return _mm_and_si128(a, b);
}
static inline Vec128b operator && (Vec128b const a, Vec128b const b) {
    return a & b;
}

// vector operator | : bitwise or
static inline Vec128b operator | (Vec128b const a, Vec128b const b) {
    return _mm_or_si128(a, b);
}
static inline Vec128b operator || (Vec128b const a, Vec128b const b) {
    return a | b;
}

// vector operator ^ : bitwise xor
static inline Vec128b operator ^ (Vec128b const a, Vec128b const b) {
    return _mm_xor_si128(a, b);
}

// vector operator ~ : bitwise not
static inline Vec128b operator ~ (Vec128b const a) {
    return _mm_xor_si128(a, _mm_set1_epi32(-1));
}

// vector operator &= : bitwise and
static inline Vec128b & operator &= (Vec128b & a, Vec128b const b) {
    a = a & b;
    return a;
}

// vector operator |= : bitwise or
static inline Vec128b & operator |= (Vec128b & a, Vec128b const b) {
    a = a | b;
    return a;
}

// vector operator ^= : bitwise xor
static inline Vec128b & operator ^= (Vec128b & a, Vec128b const b) {
    a = a ^ b;
    return a;
}

// Define functions for this class

// function andnot: a & ~ b
static inline Vec128b andnot(Vec128b const a, Vec128b const b) {
    return _mm_andnot_si128(b, a);
}


/*****************************************************************************
*
*          selectb function
*
*****************************************************************************/
// Select between two sources, byte by byte, using broad boolean vector s.
// Used in various functions and operators
// Corresponds to this pseudocode:
// for (int i = 0; i < 16; i++) result[i] = s[i] ? a[i] : b[i];
// Each byte in s must be either 0 (false) or 0xFF (true). No other values are allowed.
// The implementation depends on the instruction set:
// If SSE4.1 is supported then only bit 7 in each byte of s is checked,
// otherwise all bits in s are used.
static inline __m128i selectb(__m128i const s, __m128i const a, __m128i const b) {
    return _mm_blendv_epi8(b, a, s);
}


/*****************************************************************************
*
*          Horizontal Boolean functions
*
*****************************************************************************/

static inline bool horizontal_and(Vec128b const a) {
    return _mm_testc_si128(a, _mm_set1_epi32(-1)) != 0;
}

// horizontal_or. Returns true if at least one bit is 1
static inline bool horizontal_or(Vec128b const a) {
    return !_mm_testz_si128(a, a);
}


/*****************************************************************************
*
*          Vector of 16 8-bit signed integers
*
*****************************************************************************/

class Vec16c : public Vec128b {
public:
    // Default constructor:
    Vec16c() = default;
    // Constructor to broadcast the same value into all elements:
    Vec16c(int i) {
        xmm = _mm_set1_epi8((char)i);
    }
    // Constructor to build from all elements:
    Vec16c(int8_t i0, int8_t i1, int8_t i2, int8_t i3, int8_t i4, int8_t i5, int8_t i6, int8_t i7,
        int8_t i8, int8_t i9, int8_t i10, int8_t i11, int8_t i12, int8_t i13, int8_t i14, int8_t i15) {
        xmm = _mm_setr_epi8(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec16c(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec16c & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Type cast operator to convert to __m128i used in intrinsics
    operator __m128i() const {
        return xmm;
    }
    // Member function to load from array (unaligned)
    Vec16c & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec16c & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Partial load. Load n elements and set the rest to 0
    Vec16c & load_partial(int n, void const * p) {

        if (n >= 16) load(p);
        else if (n <= 0) *this = 0;
        else if (((int)(intptr_t)p & 0xFFF) < 0xFF0) {
            // p is at least 16 bytes from a page boundary. OK to read 16 bytes
            load(p);
        }
        else {
            // worst case. read 1 byte at a time and suffer store forwarding penalty
            // unless the compiler can optimize this
            char x[16] = {0};
            for (int i = 0; i < n; i++) x[i] = ((char const *)p)[i];
            load(x);
        }
        cutoff(n);
        return *this;
    }
    // Partial store. Store n elements
    void store_partial(int n, void * p) const {
        // therefore we have to rely on the compiler to optimize this
        int8_t s[16];
        store(s);
        if (uint32_t(n) > 16) n = 16;
        for (int i = 0; i < n; i++) {
            ((int8_t*)p)[i] = s[i];
        }
    }
    // cut off vector to n elements. The last 16-n elements are set to zero
    Vec16c & cutoff(int n) {
        if (uint32_t(n) >= 16) return *this;
        const int8_t mask[32] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        *this &= Vec16c().load(mask + 16 - n);
        return *this;
    }
    // Member function to change a single element in vector
    Vec16c const insert(int index, int8_t value) {
        const int8_t maskl[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            -1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        __m128i broad = _mm_set1_epi8(value);  // broadcast value into all elements
        __m128i mask  = _mm_loadu_si128((__m128i const*)(maskl + 16 - (index & 0x0F))); // mask with FF at index position
        xmm = selectb(mask, broad, xmm);
        return *this;
    }
    /* Note: The extract(), insert(), size(), [], etc. all use int index for consistency.
    An unsigned type for index might cause problems in case of underflow, for example:
    for (i = 0; i < a.size() - 4; i++) a[i] = ...
    This would go nuts if a.size() is 2.
    */
    // Member function extract a single element from vector
    int8_t extract(int index) const {
        int8_t x[16];
        store(x);
        return x[index & 0x0F];
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    int8_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int size() {
        return 16;
    }
    static constexpr int elementtype() {
        return 4;
    }
};


/*****************************************************************************
*
*          Vec16cb: Vector of 16 Booleans for use with Vec16c and Vec16uc
*
*****************************************************************************/
class Vec16cb : public Vec16c {
public:
    // Default constructor
    Vec16cb() = default;
    // Constructor to build from all elements:
    Vec16cb(bool x0, bool x1, bool x2, bool x3, bool x4, bool x5, bool x6, bool x7,
        bool x8, bool x9, bool x10, bool x11, bool x12, bool x13, bool x14, bool x15) {
        xmm = Vec16c(-int8_t(x0), -int8_t(x1), -int8_t(x2), -int8_t(x3), -int8_t(x4), -int8_t(x5), -int8_t(x6), -int8_t(x7),
            -int8_t(x8), -int8_t(x9), -int8_t(x10), -int8_t(x11), -int8_t(x12), -int8_t(x13), -int8_t(x14), -int8_t(x15));
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec16cb(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec16cb & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Constructor to broadcast scalar value:
    Vec16cb(bool b) : Vec16c(-int8_t(b)) {
    }
    // Assignment operator to broadcast scalar value:
    Vec16cb & operator = (bool b) {
        *this = Vec16cb(b);
        return *this;
    }
    // Member function to change a single element in vector
    Vec16cb & insert(int index, bool a) {
        Vec16c::insert(index, -(int)a);
        return *this;
    }
    // Member function to change a bitfield to a boolean vector
    Vec16cb & load_bits(uint16_t a) {
        uint16_t an = uint16_t(~a);              // invert because we have no compare-not-equal
        __m128i a1 = _mm_cvtsi32_si128(an);      // load into xmm register
        __m128i dist = constant4ui<0, 0, 0x01010101, 0x01010101>();
        __m128i a2 = _mm_shuffle_epi8(a1, dist); // one byte of a in each element
        __m128i mask = constant4ui<0x08040201, 0x80402010, 0x08040201, 0x80402010>();
        __m128i a3 = _mm_and_si128(a2, mask);    // isolate one bit in each byte
        xmm = _mm_cmpeq_epi8(a3, _mm_setzero_si128());  // compare with 0
        return *this;
    }
    // Member function extract a single element from vector
    bool extract(int index) const {
        return Vec16c::extract(index) != 0;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    bool operator [] (int index) const {
        return extract(index);
    }
    static constexpr int elementtype() {
        return 3;
    }
    // Prevent constructing from int, etc.
    Vec16cb(int b) = delete;
    Vec16cb & operator = (int x) = delete;
};



/*****************************************************************************
*
*          Define operators for Vec16cb
*
*****************************************************************************/


// vector operator & : bitwise and
static inline Vec16cb operator & (Vec16cb const a, Vec16cb const b) {
    return Vec16cb(Vec128b(a) & Vec128b(b));
}
static inline Vec16cb operator && (Vec16cb const a, Vec16cb const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec16cb & operator &= (Vec16cb & a, Vec16cb const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec16cb operator | (Vec16cb const a, Vec16cb const b) {
    return Vec16cb(Vec128b(a) | Vec128b(b));
}
static inline Vec16cb operator || (Vec16cb const a, Vec16cb const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec16cb & operator |= (Vec16cb & a, Vec16cb const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec16cb operator ^ (Vec16cb const a, Vec16cb const b) {
    return Vec16cb(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec16cb & operator ^= (Vec16cb & a, Vec16cb const b) {
    a = a ^ b;
    return a;
}

// vector operator == : xnor
static inline Vec16cb operator == (Vec16cb const a, Vec16cb const b) {
    return Vec16cb(a ^ (~b));
}

// vector operator != : xor
static inline Vec16cb operator != (Vec16cb const a, Vec16cb const b) {
    return Vec16cb(a ^ b);
}

// vector operator ~ : bitwise not
static inline Vec16cb operator ~ (Vec16cb const a) {
    return Vec16cb(~Vec128b(a));
}

// vector operator ! : element not
static inline Vec16cb operator ! (Vec16cb const a) {
    return ~a;
}

// vector function andnot
static inline Vec16cb andnot(Vec16cb const a, Vec16cb const b) {
    return Vec16cb(andnot(Vec128b(a), Vec128b(b)));
}

// horizontal_and. Returns true if all elements are true
static inline bool horizontal_and(Vec16cb const a) {
    return _mm_movemask_epi8(a) == 0xFFFF;
}

// horizontal_or. Returns true if at least one element is true
static inline bool horizontal_or(Vec16cb const a) {
    return !_mm_testz_si128(a, a);
}


/*****************************************************************************
*
*          Define operators for Vec16c
*
*****************************************************************************/

// vector operator + : add element by element
static inline Vec16c operator + (Vec16c const a, Vec16c const b) {
    return _mm_add_epi8(a, b);
}
// vector operator += : add
static inline Vec16c & operator += (Vec16c & a, Vec16c const b) {
    a = a + b;
    return a;
}

// postfix operator ++
static inline Vec16c operator ++ (Vec16c & a, int) {
    Vec16c a0 = a;
    a = a + 1;
    return a0;
}

// prefix operator ++
static inline Vec16c & operator ++ (Vec16c & a) {
    a = a + 1;
    return a;
}

// vector operator - : subtract element by element
static inline Vec16c operator - (Vec16c const a, Vec16c const b) {
    return _mm_sub_epi8(a, b);
}
// vector operator - : unary minus
static inline Vec16c operator - (Vec16c const a) {
    return _mm_sub_epi8(_mm_setzero_si128(), a);
}
// vector operator -= : add
static inline Vec16c & operator -= (Vec16c & a, Vec16c const b) {
    a = a - b;
    return a;
}

// postfix operator --
static inline Vec16c operator -- (Vec16c & a, int) {
    Vec16c a0 = a;
    a = a - 1;
    return a0;
}

// prefix operator --
static inline Vec16c & operator -- (Vec16c & a) {
    a = a - 1;
    return a;
}

// vector operator * : multiply element by element
static inline Vec16c operator * (Vec16c const a, Vec16c const b) {
    // There is no 8-bit multiply in SSE2. Split into two 16-bit multiplies
    __m128i aodd = _mm_srli_epi16(a, 8);         // odd numbered elements of a
    __m128i bodd = _mm_srli_epi16(b, 8);         // odd numbered elements of b
    __m128i muleven = _mm_mullo_epi16(a, b);     // product of even numbered elements
    __m128i mulodd = _mm_mullo_epi16(aodd, bodd);// product of odd  numbered elements
    mulodd = _mm_slli_epi16(mulodd, 8);          // put odd numbered elements back in place
    __m128i mask = _mm_set1_epi32(0x00FF00FF);   // mask for even positions
    return selectb(mask, muleven, mulodd);       // interleave even and odd
}

// vector operator *= : multiply
static inline Vec16c & operator *= (Vec16c & a, Vec16c const b) {
    a = a * b;
    return a;
}

// vector operator << : shift left all elements
static inline Vec16c operator << (Vec16c const a, int b) {
    if (uint32_t(b) >= 8) return _mm_setzero_si128();
    uint32_t mask = (uint32_t)0xFF >> (uint32_t)b;         // mask to remove bits that are shifted out
    __m128i am = _mm_and_si128(a, _mm_set1_epi8((char)mask));// remove bits that will overflow
    __m128i res = _mm_sll_epi16(am, _mm_cvtsi32_si128(b));// 16-bit shifts
    return res;
}
// vector operator <<= : shift left
static inline Vec16c & operator <<= (Vec16c & a, int b) {
    a = a << b;
    return a;
}

// vector operator >> : shift right arithmetic all elements
static inline Vec16c operator >> (Vec16c const a, int b) {
    if (uint32_t(b) >= 8) b = 8;
    __m128i aeven = _mm_slli_epi16(a, 8);                  // even numbered elements of a. get sign bit in position
    aeven = _mm_sra_epi16(aeven, _mm_cvtsi32_si128(b + 8));// shift arithmetic, back to position
    __m128i aodd = _mm_sra_epi16(a, _mm_cvtsi32_si128(b)); // shift odd numbered elements arithmetic
    __m128i mask = _mm_set1_epi32(0x00FF00FF);             // mask for even positions
    __m128i res = selectb(mask, aeven, aodd);              // interleave even and odd
    return res;
}
// vector operator >>= : shift right arithmetic
static inline Vec16c & operator >>= (Vec16c & a, int b) {
    a = a >> b;
    return a;
}

// vector operator == : returns true for elements for which a == b
static inline Vec16cb operator == (Vec16c const a, Vec16c const b) {
    return _mm_cmpeq_epi8(a, b);
}

// vector operator != : returns true for elements for which a != b
static inline Vec16cb operator != (Vec16c const a, Vec16c const b) {
    return Vec16cb(Vec16c(~(a == b)));
}

// vector operator > : returns true for elements for which a > b (signed)
static inline Vec16cb operator > (Vec16c const a, Vec16c const b) {
    return _mm_cmpgt_epi8(a, b);
}

// vector operator < : returns true for elements for which a < b (signed)
static inline Vec16cb operator < (Vec16c const a, Vec16c const b) {
    return b > a;
}

// vector operator >= : returns true for elements for which a >= b (signed)
static inline Vec16cb operator >= (Vec16c const a, Vec16c const b) {
    return Vec16cb(Vec16c(~(b > a)));
}

// vector operator <= : returns true for elements for which a <= b (signed)
static inline Vec16cb operator <= (Vec16c const a, Vec16c const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec16c operator & (Vec16c const a, Vec16c const b) {
    return Vec16c(Vec128b(a) & Vec128b(b));
}
static inline Vec16c operator && (Vec16c const a, Vec16c const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec16c & operator &= (Vec16c & a, Vec16c const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec16c operator | (Vec16c const a, Vec16c const b) {
    return Vec16c(Vec128b(a) | Vec128b(b));
}
static inline Vec16c operator || (Vec16c const a, Vec16c const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec16c & operator |= (Vec16c & a, Vec16c const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec16c operator ^ (Vec16c const a, Vec16c const b) {
    return Vec16c(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec16c & operator ^= (Vec16c & a, Vec16c const b) {
    a = a ^ b;
    return a;
}

// vector operator ~ : bitwise not
static inline Vec16c operator ~ (Vec16c const a) {
    return Vec16c(~Vec128b(a));
}

// vector operator ! : logical not, returns true for elements == 0
static inline Vec16cb operator ! (Vec16c const a) {
    return _mm_cmpeq_epi8(a, _mm_setzero_si128());
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 16; i++) result[i] = s[i] ? a[i] : b[i];
// Each byte in s must be either 0 (false) or -1 (true). No other values are allowed.
static inline Vec16c select(Vec16cb const s, Vec16c const a, Vec16c const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec16c if_add(Vec16cb const f, Vec16c const a, Vec16c const b) {
    return a + (Vec16c(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec16c if_sub(Vec16cb const f, Vec16c const a, Vec16c const b) {
    return a - (Vec16c(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec16c if_mul(Vec16cb const f, Vec16c const a, Vec16c const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline int32_t horizontal_add(Vec16c const a) {
    __m128i sum1 = _mm_sad_epu8(a, _mm_setzero_si128());
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);
    __m128i sum3 = _mm_add_epi16(sum1, sum2);
    int8_t  sum4 = (int8_t)_mm_cvtsi128_si32(sum3);        // truncate to 8 bits
    return  sum4;                                          // sign extend to 32 bits
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Each element is sign-extended before addition to avoid overflow
static inline int32_t horizontal_add_x(Vec16c const a) {
    __m128i aeven = _mm_slli_epi16(a, 8);                  // even numbered elements of a. get sign bit in position
    aeven = _mm_srai_epi16(aeven, 8);                      // sign extend even numbered elements
    __m128i aodd = _mm_srai_epi16(a, 8);                   // sign extend odd  numbered elements
    __m128i sum1 = _mm_add_epi16(aeven, aodd);             // add even and odd elements
    // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum2 = _mm_add_epi16(sum1, _mm_unpackhi_epi64(sum1, sum1));
    __m128i sum3 = _mm_add_epi16(sum2, _mm_shuffle_epi32(sum2, 1));
    __m128i sum4 = _mm_add_epi16(sum3, _mm_shufflelo_epi16(sum3, 1));
    int16_t sum5 = (int16_t)_mm_cvtsi128_si32(sum4);       // 16 bit sum
    return  sum5;                                          // sign extend to 32 bits
}


// function add_saturated: add element by element, signed with saturation
static inline Vec16c add_saturated(Vec16c const a, Vec16c const b) {
    return _mm_adds_epi8(a, b);
}

// function sub_saturated: subtract element by element, signed with saturation
static inline Vec16c sub_saturated(Vec16c const a, Vec16c const b) {
    return _mm_subs_epi8(a, b);
}

// function max: a > b ? a : b
static inline Vec16c max(Vec16c const a, Vec16c const b) {
    return _mm_max_epi8(a, b);
}

// function min: a < b ? a : b
static inline Vec16c min(Vec16c const a, Vec16c const b) {
    return _mm_min_epi8(a, b);
}

// function abs: a >= 0 ? a : -a
static inline Vec16c abs(Vec16c const a) {
    return _mm_abs_epi8(a);
}

// function abs_saturated: same as abs, saturate if overflow
static inline Vec16c abs_saturated(Vec16c const a) {
    __m128i absa = abs(a);                                 // abs(a)
    __m128i overfl = _mm_cmpgt_epi8(_mm_setzero_si128(), absa);// 0 > a
    return           _mm_add_epi8(absa, overfl);           // subtract 1 if 0x80
}

// function rotate_left: rotate each element left by b bits
// Use negative count to rotate right
static inline Vec16c rotate_left(Vec16c const a, int b) {
    int8_t  mask = int8_t(0xFFu << (uint32_t(b) & 7));                     // mask off overflow bits
    __m128i m = _mm_set1_epi8(mask);
    __m128i bb = _mm_cvtsi32_si128(b & 7);                 // b modulo 8
    __m128i mbb = _mm_cvtsi32_si128((uint32_t(0) - uint32_t(b)) & 7);             // 8-b modulo 8
    __m128i left = _mm_sll_epi16(a, bb);                   // a << b
    __m128i right = _mm_srl_epi16(a, mbb);                 // a >> 8-b
    left  = _mm_and_si128(m, left);                        // mask off overflow bits
    right = _mm_andnot_si128(m, right);
    return  _mm_or_si128(left, right);                     // combine left and right shifted bits
}


/*****************************************************************************
*
*          Vector of 16 8-bit unsigned integers
*
*****************************************************************************/

class Vec16uc : public Vec16c {
public:
    // Default constructor:
    Vec16uc() = default;
    // Constructor to broadcast the same value into all elements:
    Vec16uc(uint32_t i) {
        xmm = _mm_set1_epi8((char)i);
    }
    // Constructor to build from all elements:
    Vec16uc(uint8_t i0, uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4, uint8_t i5, uint8_t i6, uint8_t i7,
        uint8_t i8, uint8_t i9, uint8_t i10, uint8_t i11, uint8_t i12, uint8_t i13, uint8_t i14, uint8_t i15) {
        xmm = _mm_setr_epi8((int8_t)i0, (int8_t)i1, (int8_t)i2, (int8_t)i3, (int8_t)i4, (int8_t)i5, (int8_t)i6,
            (int8_t)i7, (int8_t)i8, (int8_t)i9, (int8_t)i10, (int8_t)i11, (int8_t)i12, (int8_t)i13, (int8_t)i14, (int8_t)i15);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec16uc(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec16uc & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Member function to load from array (unaligned)
    Vec16uc & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec16uc & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Member function to change a single element in vector
    Vec16uc const insert(int index, uint8_t value) {
        Vec16c::insert(index, (int8_t)value);
        return *this;
    }
    // Member function extract a single element from vector
    uint8_t extract(int index) const {
        return uint8_t(Vec16c::extract(index));
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    uint8_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int elementtype() {
        return 5;
    }
};

// Define operators for this class

// vector operator << : shift left all elements
static inline Vec16uc operator << (Vec16uc const a, uint32_t b) {
    if (b >= 8) return _mm_setzero_si128();
    uint32_t mask = (uint32_t)0xFF >> (uint32_t)b;               // mask to remove bits that are shifted out
    __m128i am = _mm_and_si128(a, _mm_set1_epi8((char)mask));  // remove bits that will overflow
    __m128i res = _mm_sll_epi16(am, _mm_cvtsi32_si128((int)b)); // 16-bit shifts
    return res;
}

// vector operator << : shift left all elements
static inline Vec16uc operator << (Vec16uc const a, int32_t b) {
    return a << (uint32_t)b;
}

// vector operator >> : shift right logical all elements
static inline Vec16uc operator >> (Vec16uc const a, uint32_t b) {
    if (b >= 8) return _mm_setzero_si128();
    uint32_t mask = (uint32_t)0xFF << (uint32_t)b;               // mask to remove bits that are shifted out
    __m128i am = _mm_and_si128(a, _mm_set1_epi8((char)mask));  // remove bits that will overflow
    __m128i res = _mm_srl_epi16(am, _mm_cvtsi32_si128((int)b)); // 16-bit shifts
    return res;
}

// vector operator >> : shift right logical all elements
static inline Vec16uc operator >> (Vec16uc const a, int32_t b) {
    return a >> (uint32_t)b;
}

// vector operator >>= : shift right logical
static inline Vec16uc & operator >>= (Vec16uc & a, int b) {
    a = a >> b;
    return a;
}

// vector operator >= : returns true for elements for which a >= b (unsigned)
static inline Vec16cb operator >= (Vec16uc const a, Vec16uc const b) {
    return (Vec16cb)_mm_cmpeq_epi8(_mm_max_epu8(a, b), a); // a == max(a,b)
}

// vector operator <= : returns true for elements for which a <= b (unsigned)
static inline Vec16cb operator <= (Vec16uc const a, Vec16uc const b) {
    return b >= a;
}

// vector operator > : returns true for elements for which a > b (unsigned)
static inline Vec16cb operator > (Vec16uc const a, Vec16uc const b) {
    return Vec16cb(Vec16c(~(b >= a)));
}

// vector operator < : returns true for elements for which a < b (unsigned)
static inline Vec16cb operator < (Vec16uc const a, Vec16uc const b) {
    return b > a;
}

// vector operator + : add
static inline Vec16uc operator + (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec16c(a) + Vec16c(b));
}

// vector operator - : subtract
static inline Vec16uc operator - (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec16c(a) - Vec16c(b));
}

// vector operator * : multiply
static inline Vec16uc operator * (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec16c(a) * Vec16c(b));
}

// vector operator & : bitwise and
static inline Vec16uc operator & (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec128b(a) & Vec128b(b));
}
static inline Vec16uc operator && (Vec16uc const a, Vec16uc const b) {
    return a & b;
}

// vector operator | : bitwise or
static inline Vec16uc operator | (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec128b(a) | Vec128b(b));
}
static inline Vec16uc operator || (Vec16uc const a, Vec16uc const b) {
    return a | b;
}

// vector operator ^ : bitwise xor
static inline Vec16uc operator ^ (Vec16uc const a, Vec16uc const b) {
    return Vec16uc(Vec128b(a) ^ Vec128b(b));
}

// vector operator ~ : bitwise not
static inline Vec16uc operator ~ (Vec16uc const a) {
    return Vec16uc(~Vec128b(a));
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 16; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec16uc select(Vec16cb const s, Vec16uc const a, Vec16uc const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec16uc if_add(Vec16cb const f, Vec16uc const a, Vec16uc const b) {
    return a + (Vec16uc(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec16uc if_sub(Vec16cb const f, Vec16uc const a, Vec16uc const b) {
    return a - (Vec16uc(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec16uc if_mul(Vec16cb const f, Vec16uc const a, Vec16uc const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
// (Note: horizontal_add_x(Vec16uc) is slightly faster)
static inline uint32_t horizontal_add(Vec16uc const a) {
    __m128i sum1 = _mm_sad_epu8(a, _mm_setzero_si128());
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);
    __m128i sum3 = _mm_add_epi16(sum1, sum2);
    uint16_t sum4 = (uint16_t)_mm_cvtsi128_si32(sum3);     // truncate to 16 bits
    return  sum4;
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Each element is zero-extended before addition to avoid overflow
static inline uint32_t horizontal_add_x(Vec16uc const a) {
    __m128i sum1 = _mm_sad_epu8(a, _mm_setzero_si128());
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);
    __m128i sum3 = _mm_add_epi16(sum1, sum2);
    return (uint32_t)_mm_cvtsi128_si32(sum3);
}

// function add_saturated: add element by element, unsigned with saturation
static inline Vec16uc add_saturated(Vec16uc const a, Vec16uc const b) {
    return _mm_adds_epu8(a, b);
}

// function sub_saturated: subtract element by element, unsigned with saturation
static inline Vec16uc sub_saturated(Vec16uc const a, Vec16uc const b) {
    return _mm_subs_epu8(a, b);
}

// function max: a > b ? a : b
static inline Vec16uc max(Vec16uc const a, Vec16uc const b) {
    return _mm_max_epu8(a, b);
}

// function min: a < b ? a : b
static inline Vec16uc min(Vec16uc const a, Vec16uc const b) {
    return _mm_min_epu8(a, b);
}



/*****************************************************************************
*
*          Vector of 8 16-bit signed integers
*
*****************************************************************************/

class Vec8s : public Vec128b {
public:
    // Default constructor:
    Vec8s() = default;
    // Constructor to broadcast the same value into all elements:
    Vec8s(int i) {
        xmm = _mm_set1_epi16((int16_t)i);
    }
    // Constructor to build from all elements:
    Vec8s(int16_t i0, int16_t i1, int16_t i2, int16_t i3, int16_t i4, int16_t i5, int16_t i6, int16_t i7) {
        xmm = _mm_setr_epi16(i0, i1, i2, i3, i4, i5, i6, i7);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec8s(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec8s & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Type cast operator to convert to __m128i used in intrinsics
    operator __m128i() const {
        return xmm;
    }
    // Member function to load from array (unaligned)
    Vec8s & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec8s & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Partial load. Load n elements and set the rest to 0
    Vec8s & load_partial(int n, void const * p) {

        if (n >= 8) load(p);
        else if (n <= 0) * this = 0;
        else if (((int)(intptr_t)p & 0xFFF) < 0xFF0) {
            // p is at least 16 bytes from a page boundary. OK to read 16 bytes
            load(p);
        }
        else {
            // worst case. read 1 word at a time and suffer store forwarding penalty
            int16_t x[8];
            for (int i = 0; i < n; i++) x[i] = ((int16_t const *)p)[i];
            load(x);
        }
        cutoff(n);
        return *this;
    }
    // Partial store. Store n elements
    void store_partial(int n, void * p) const {
        int16_t s[8];
        store(s);
        if (uint32_t(n) > 8) n = 8;
        for (int i = 0; i < n; i++) {
            ((int16_t*)p)[i] = s[i];
        }
    }
    // cut off vector to n elements. The last 8-n elements are set to zero
    Vec8s & cutoff(int n) {
        *this = Vec16c(xmm).cutoff(n * 2);
        return *this;
    }
    // Member function to change a single element in vector
    Vec8s const insert(int index, int16_t value) {
        switch (index) {
        case 0:
            xmm = _mm_insert_epi16(xmm, value, 0);  break;
        case 1:
            xmm = _mm_insert_epi16(xmm, value, 1);  break;
        case 2:
            xmm = _mm_insert_epi16(xmm, value, 2);  break;
        case 3:
            xmm = _mm_insert_epi16(xmm, value, 3);  break;
        case 4:
            xmm = _mm_insert_epi16(xmm, value, 4);  break;
        case 5:
            xmm = _mm_insert_epi16(xmm, value, 5);  break;
        case 6:
            xmm = _mm_insert_epi16(xmm, value, 6);  break;
        case 7:
            xmm = _mm_insert_epi16(xmm, value, 7);  break;
        }
        return *this;
    }
    // Member function extract a single element from vector
    int16_t extract(int index) const {
        switch (index) {
        case 0:
            return (int16_t)_mm_extract_epi16(xmm, 0);
        case 1:
            return (int16_t)_mm_extract_epi16(xmm, 1);
        case 2:
            return (int16_t)_mm_extract_epi16(xmm, 2);
        case 3:
            return (int16_t)_mm_extract_epi16(xmm, 3);
        case 4:
            return (int16_t)_mm_extract_epi16(xmm, 4);
        case 5:
            return (int16_t)_mm_extract_epi16(xmm, 5);
        case 6:
            return (int16_t)_mm_extract_epi16(xmm, 6);
        case 7:
            return (int16_t)_mm_extract_epi16(xmm, 7);
        }
        return 0;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    int16_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int size() {
        return 8;
    }
    static constexpr int elementtype() {
        return 6;
    }
};

/*****************************************************************************
*
*          Vec8sb: Vector of 8 Booleans for use with Vec8s and Vec8us
*
*****************************************************************************/

class Vec8sb : public Vec8s {
public:
    // Constructor to build from all elements:
    Vec8sb(bool x0, bool x1, bool x2, bool x3, bool x4, bool x5, bool x6, bool x7) {
        xmm = Vec8s(-int16_t(x0), -int16_t(x1), -int16_t(x2), -int16_t(x3), -int16_t(x4), -int16_t(x5), -int16_t(x6), -int16_t(x7));
    }
    // Default constructor:
    Vec8sb() = default;
    // Constructor to convert from type __m128i used in intrinsics:
    Vec8sb(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec8sb & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Constructor to broadcast scalar value:
    Vec8sb(bool b) : Vec8s(-int16_t(b)) {
    }
    // Assignment operator to broadcast scalar value:
    Vec8sb & operator = (bool b) {
        *this = Vec8sb(b);
        return *this;
    }
    Vec8sb & insert(int index, bool a) {
        Vec8s::insert(index, -(int16_t)a);
        return *this;
    }
    // Member function extract a single element from vector
    // Note: This function is inefficient. Use store function if extracting more than one element
    bool extract(int index) const {
        return Vec8s::extract(index) != 0;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    bool operator [] (int index) const {
        return extract(index);
    }
    // Member function to change a bitfield to a boolean vector
    Vec8sb & load_bits(uint8_t a) {
        __m128i b1 = _mm_set1_epi8((int8_t)a);  // broadcast byte. Invert because we have no compare-not-equal
        __m128i m1 = constant4ui<0x00020001, 0x00080004, 0x00200010, 0x00800040>();
        __m128i c1 = _mm_and_si128(b1, m1); // isolate one bit in each byte
        xmm = _mm_cmpgt_epi16(c1, _mm_setzero_si128());  // compare with 0
        return *this;
    }
    static constexpr int elementtype() {
        return 3;
    }
    // Prevent constructing from int, etc.
    Vec8sb(int b) = delete;
    Vec8sb & operator = (int x) = delete;
};


/*****************************************************************************
*
*          Define operators for Vec8sb
*
*****************************************************************************/

// vector operator & : bitwise and
static inline Vec8sb operator & (Vec8sb const a, Vec8sb const b) {
    return Vec8sb(Vec128b(a) & Vec128b(b));
}
static inline Vec8sb operator && (Vec8sb const a, Vec8sb const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec8sb & operator &= (Vec8sb & a, Vec8sb const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec8sb operator | (Vec8sb const a, Vec8sb const b) {
    return Vec8sb(Vec128b(a) | Vec128b(b));
}
static inline Vec8sb operator || (Vec8sb const a, Vec8sb const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec8sb & operator |= (Vec8sb & a, Vec8sb const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec8sb operator ^ (Vec8sb const a, Vec8sb const b) {
    return Vec8sb(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec8sb & operator ^= (Vec8sb & a, Vec8sb const b) {
    a = a ^ b;
    return a;
}

// vector operator == : xnor
static inline Vec8sb operator == (Vec8sb const a, Vec8sb const b) {
    return Vec8sb(a ^ (~b));
}

// vector operator != : xor
static inline Vec8sb operator != (Vec8sb const a, Vec8sb const b) {
    return Vec8sb(a ^ b);
}

// vector operator ~ : bitwise not
static inline Vec8sb operator ~ (Vec8sb const a) {
    return Vec8sb(~Vec128b(a));
}

// vector operator ! : element not
static inline Vec8sb operator ! (Vec8sb const a) {
    return ~a;
}

// vector function andnot
static inline Vec8sb andnot(Vec8sb const a, Vec8sb const b) {
    return Vec8sb(andnot(Vec128b(a), Vec128b(b)));
}

// horizontal_and. Returns true if all elements are true
static inline bool horizontal_and(Vec8sb const a) {
    return _mm_movemask_epi8(a) == 0xFFFF;
}

// horizontal_or. Returns true if at least one element is true
static inline bool horizontal_or(Vec8sb const a) {
    return !_mm_testz_si128(a, a);
}


/*****************************************************************************
*
*         operators for Vec8s
*
*****************************************************************************/

// vector operator + : add element by element
static inline Vec8s operator + (Vec8s const a, Vec8s const b) {
    return _mm_add_epi16(a, b);
}
// vector operator += : add
static inline Vec8s & operator += (Vec8s & a, Vec8s const b) {
    a = a + b;
    return a;
}

// postfix operator ++
static inline Vec8s operator ++ (Vec8s & a, int) {
    Vec8s a0 = a;
    a = a + 1;
    return a0;
}
// prefix operator ++
static inline Vec8s & operator ++ (Vec8s & a) {
    a = a + 1;
    return a;
}

// vector operator - : subtract element by element
static inline Vec8s operator - (Vec8s const a, Vec8s const b) {
    return _mm_sub_epi16(a, b);
}
// vector operator - : unary minus
static inline Vec8s operator - (Vec8s const a) {
    return _mm_sub_epi16(_mm_setzero_si128(), a);
}
// vector operator -= : subtract
static inline Vec8s & operator -= (Vec8s & a, Vec8s const b) {
    a = a - b;
    return a;
}

// postfix operator --
static inline Vec8s operator -- (Vec8s & a, int) {
    Vec8s a0 = a;
    a = a - 1;
    return a0;
}
// prefix operator --
static inline Vec8s & operator -- (Vec8s & a) {
    a = a - 1;
    return a;
}

// vector operator * : multiply element by element
static inline Vec8s operator * (Vec8s const a, Vec8s const b) {
    return _mm_mullo_epi16(a, b);
}

// vector operator *= : multiply
static inline Vec8s & operator *= (Vec8s & a, Vec8s const b) {
    a = a * b;
    return a;
}

// vector operator / : divide all elements by same integer. See bottom of file

// vector operator << : shift left
static inline Vec8s operator << (Vec8s const a, int b) {
    return _mm_sll_epi16(a, _mm_cvtsi32_si128(b));
}

// vector operator <<= : shift left
static inline Vec8s & operator <<= (Vec8s & a, int b) {
    a = a << b;
    return a;
}

// vector operator >> : shift right arithmetic
static inline Vec8s operator >> (Vec8s const a, int b) {
    return _mm_sra_epi16(a, _mm_cvtsi32_si128(b));
}

// vector operator >>= : shift right arithmetic
static inline Vec8s & operator >>= (Vec8s & a, int b) {
    a = a >> b;
    return a;
}

// vector operator == : returns true for elements for which a == b
static inline Vec8sb operator == (Vec8s const a, Vec8s const b) {
    return _mm_cmpeq_epi16(a, b);
}

// vector operator != : returns true for elements for which a != b
static inline Vec8sb operator != (Vec8s const a, Vec8s const b) {
    return Vec8sb(~(a == b));
}

// vector operator > : returns true for elements for which a > b
static inline Vec8sb operator > (Vec8s const a, Vec8s const b) {
    return _mm_cmpgt_epi16(a, b);
}

// vector operator < : returns true for elements for which a < b
static inline Vec8sb operator < (Vec8s const a, Vec8s const b) {
    return b > a;
}

// vector operator >= : returns true for elements for which a >= b (signed)
static inline Vec8sb operator >= (Vec8s const a, Vec8s const b) {
    return Vec8sb(~(b > a));
}

// vector operator <= : returns true for elements for which a <= b (signed)
static inline Vec8sb operator <= (Vec8s const a, Vec8s const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec8s operator & (Vec8s const a, Vec8s const b) {
    return Vec8s(Vec128b(a) & Vec128b(b));
}
static inline Vec8s operator && (Vec8s const a, Vec8s const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec8s & operator &= (Vec8s & a, Vec8s const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec8s operator | (Vec8s const a, Vec8s const b) {
    return Vec8s(Vec128b(a) | Vec128b(b));
}
static inline Vec8s operator || (Vec8s const a, Vec8s const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec8s & operator |= (Vec8s & a, Vec8s const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec8s operator ^ (Vec8s const a, Vec8s const b) {
    return Vec8s(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec8s & operator ^= (Vec8s & a, Vec8s const b) {
    a = a ^ b;
    return a;
}

// vector operator ~ : bitwise not
static inline Vec8s operator ~ (Vec8s const a) {
    return Vec8s(~Vec128b(a));
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 8; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec8s select(Vec8sb const s, Vec8s const a, Vec8s const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec8s if_add(Vec8sb const f, Vec8s const a, Vec8s const b) {
    return a + (Vec8s(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec8s if_sub(Vec8sb const f, Vec8s const a, Vec8s const b) {
    return a - (Vec8s(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec8s if_mul(Vec8sb const f, Vec8s const a, Vec8s const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline int16_t horizontal_add(Vec8s const a) {
    __m128i sum1 = _mm_unpackhi_epi64(a, a);               // 4 high elements
    __m128i sum2 = _mm_add_epi16(a, sum1);                 // 4 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 2 high elements
    __m128i sum4 = _mm_add_epi16(sum2, sum3);              // 2 sums
    __m128i sum5 = _mm_shufflelo_epi16(sum4, 0x01);        // 1 high element
    __m128i sum6 = _mm_add_epi16(sum4, sum5);              // 1 sum
    int16_t sum7 = (int16_t)_mm_cvtsi128_si32(sum6);       // 16 bit sum
    return  sum7;                                          // sign extend to 32 bits
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Elements are sign extended before adding to avoid overflow
static inline int32_t horizontal_add_x(Vec8s const a) {
    __m128i aeven = _mm_slli_epi32(a, 16);                 // even numbered elements of a. get sign bit in position
    aeven = _mm_srai_epi32(aeven, 16);                     // sign extend even numbered elements
    __m128i aodd = _mm_srai_epi32(a, 16);                  // sign extend odd  numbered elements
    __m128i sum1 = _mm_add_epi32(aeven, aodd);             // add even and odd elements
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);         // 2 high elements
    __m128i sum3 = _mm_add_epi32(sum1, sum2);
    __m128i sum4 = _mm_shuffle_epi32(sum3, 1);             // 1 high elements
    __m128i sum5 = _mm_add_epi32(sum3, sum4);
    return  _mm_cvtsi128_si32(sum5);                       // 32 bit sum
}

// function add_saturated: add element by element, signed with saturation
static inline Vec8s add_saturated(Vec8s const a, Vec8s const b) {
    return _mm_adds_epi16(a, b);
}

// function sub_saturated: subtract element by element, signed with saturation
static inline Vec8s sub_saturated(Vec8s const a, Vec8s const b) {
    return _mm_subs_epi16(a, b);
}

// function max: a > b ? a : b
static inline Vec8s max(Vec8s const a, Vec8s const b) {
    return _mm_max_epi16(a, b);
}

// function min: a < b ? a : b
static inline Vec8s min(Vec8s const a, Vec8s const b) {
    return _mm_min_epi16(a, b);
}

// function abs: a >= 0 ? a : -a
static inline Vec8s abs(Vec8s const a) {
    return _mm_abs_epi16(a);
}

// function abs_saturated: same as abs, saturate if overflow
static inline Vec8s abs_saturated(Vec8s const a) {
    __m128i absa = abs(a);                                 // abs(a)
    __m128i overfl = _mm_srai_epi16(absa, 15);             // sign
    return           _mm_add_epi16(absa, overfl);          // subtract 1 if 0x8000
}

// function rotate_left all elements
// Use negative count to rotate right
static inline Vec8s rotate_left(Vec8s const a, int b) {
    __m128i left  = _mm_sll_epi16(a, _mm_cvtsi32_si128(b & 0x0F));     // a << b
    __m128i right = _mm_srl_epi16(a, _mm_cvtsi32_si128((uint32_t(0) - uint32_t(b)) & 0x0F));  // a >> (16 - b)
    __m128i rot   = _mm_or_si128(left, right);                         // or
    return  rot;
}


/*****************************************************************************
*
*          Vector of 8 16-bit unsigned integers
*
*****************************************************************************/

class Vec8us : public Vec8s {
public:
    // Default constructor:
    Vec8us() = default;
    // Constructor to broadcast the same value into all elements:
    Vec8us(uint32_t i) {
        xmm = _mm_set1_epi16((int16_t)i);
    }
    // Constructor to build from all elements:
    Vec8us(uint16_t i0, uint16_t i1, uint16_t i2, uint16_t i3, uint16_t i4, uint16_t i5, uint16_t i6, uint16_t i7) {
        xmm = _mm_setr_epi16((int16_t)i0, (int16_t)i1, (int16_t)i2, (int16_t)i3, (int16_t)i4, (int16_t)i5, (int16_t)i6, (int16_t)i7);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec8us(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec8us & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Member function to load from array (unaligned)
    Vec8us & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec8us & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Member function to change a single element in vector
    // Note: This function is inefficient. Use load function if changing more than one element
    Vec8us const insert(int index, uint16_t value) {
        Vec8s::insert(index, (int16_t)value);
        return *this;
    }
    // Member function extract a single element from vector
    uint16_t extract(int index) const {
        return (uint16_t)Vec8s::extract(index);
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    uint16_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int elementtype() {
        return 7;
    }
};

// Define operators for this class

// vector operator + : add
static inline Vec8us operator + (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec8s(a) + Vec8s(b));
}

// vector operator - : subtract
static inline Vec8us operator - (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec8s(a) - Vec8s(b));
}

// vector operator * : multiply
static inline Vec8us operator * (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec8s(a) * Vec8s(b));
}

// vector operator / : divide
// See bottom of file

// vector operator >> : shift right logical all elements
static inline Vec8us operator >> (Vec8us const a, uint32_t b) {
    return _mm_srl_epi16(a, _mm_cvtsi32_si128((int)b));
}

// vector operator >> : shift right logical all elements
static inline Vec8us operator >> (Vec8us const a, int32_t b) {
    return a >> (uint32_t)b;
}

// vector operator >>= : shift right logical
static inline Vec8us & operator >>= (Vec8us & a, int b) {
    a = a >> b;
    return a;
}

// vector operator << : shift left all elements
static inline Vec8us operator << (Vec8us const a, uint32_t b) {
    return _mm_sll_epi16(a, _mm_cvtsi32_si128((int)b));
}

// vector operator << : shift left all elements
static inline Vec8us operator << (Vec8us const a, int32_t b) {
    return a << (uint32_t)b;
}

// vector operator >= : returns true for elements for which a >= b (unsigned)
static inline Vec8sb operator >= (Vec8us const a, Vec8us const b) {
    __m128i max_ab = _mm_max_epu16(a, b);                   // max(a,b), unsigned
    return _mm_cmpeq_epi16(a, max_ab);                      // a == max(a,b)
}

// vector operator <= : returns true for elements for which a <= b (unsigned)
static inline Vec8sb operator <= (Vec8us const a, Vec8us const b) {
    return b >= a;
}

// vector operator > : returns true for elements for which a > b (unsigned)
static inline Vec8sb operator > (Vec8us const a, Vec8us const b) {
    return Vec8sb(~(b >= a));
}

// vector operator < : returns true for elements for which a < b (unsigned)
static inline Vec8sb operator < (Vec8us const a, Vec8us const b) {
    return b > a;
}

// vector operator & : bitwise and
static inline Vec8us operator & (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec128b(a) & Vec128b(b));
}
static inline Vec8us operator && (Vec8us const a, Vec8us const b) {
    return a & b;
}

// vector operator | : bitwise or
static inline Vec8us operator | (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec128b(a) | Vec128b(b));
}
static inline Vec8us operator || (Vec8us const a, Vec8us const b) {
    return a | b;
}

// vector operator ^ : bitwise xor
static inline Vec8us operator ^ (Vec8us const a, Vec8us const b) {
    return Vec8us(Vec128b(a) ^ Vec128b(b));
}

// vector operator ~ : bitwise not
static inline Vec8us operator ~ (Vec8us const a) {
    return Vec8us(~Vec128b(a));
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 8; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec8us select(Vec8sb const s, Vec8us const a, Vec8us const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec8us if_add(Vec8sb const f, Vec8us const a, Vec8us const b) {
    return a + (Vec8us(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec8us if_sub(Vec8sb const f, Vec8us const a, Vec8us const b) {
    return a - (Vec8us(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec8us if_mul(Vec8sb const f, Vec8us const a, Vec8us const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements.
// Overflow will wrap around
static inline uint32_t horizontal_add(Vec8us const a) {
    __m128i sum1 = _mm_unpackhi_epi64(a, a);               // 4 high elements
    __m128i sum2 = _mm_add_epi16(a, sum1);                 // 4 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 2 high elements
    __m128i sum4 = _mm_add_epi16(sum2, sum3);              // 2 sums
    __m128i sum5 = _mm_shufflelo_epi16(sum4, 0x01);        // 1 high element
    __m128i sum6 = _mm_add_epi16(sum4, sum5);              // 1 sum
    uint16_t sum7 = (uint16_t)_mm_cvtsi128_si32(sum6);     // 16 bit sum
    return  sum7;                                          // zero extend to 32 bits
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Each element is zero-extended before addition to avoid overflow
static inline uint32_t horizontal_add_x(Vec8us const a) {
    __m128i mask  = _mm_set1_epi32(0x0000FFFF);            // mask for even positions
    __m128i aeven = _mm_and_si128(a, mask);                // even numbered elements of a
    __m128i aodd = _mm_srli_epi32(a, 16);                  // zero extend odd numbered elements
    __m128i sum1 = _mm_add_epi32(aeven, aodd);             // add even and odd elements
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);         // 2 high elements
    __m128i sum3 = _mm_add_epi32(sum1, sum2);
    __m128i sum4 = _mm_shuffle_epi32(sum3, 0x01);          // 1 high elements
    __m128i sum5 = _mm_add_epi32(sum3, sum4);
    return  (uint32_t)_mm_cvtsi128_si32(sum5);             // 16 bit sum
}

// function add_saturated: add element by element, unsigned with saturation
static inline Vec8us add_saturated(Vec8us const a, Vec8us const b) {
    return _mm_adds_epu16(a, b);
}

// function sub_saturated: subtract element by element, unsigned with saturation
static inline Vec8us sub_saturated(Vec8us const a, Vec8us const b) {
    return _mm_subs_epu16(a, b);
}

// function max: a > b ? a : b
static inline Vec8us max(Vec8us const a, Vec8us const b) {
    return _mm_max_epu16(a, b);
}

// function min: a < b ? a : b
static inline Vec8us min(Vec8us const a, Vec8us const b) {
    return _mm_min_epu16(a, b);
}


/*****************************************************************************
*
*          Vector of 4 32-bit signed integers
*
*****************************************************************************/

class Vec4i : public Vec128b {
public:
    // Default constructor:
    Vec4i() = default;
    // Constructor to broadcast the same value into all elements:
    Vec4i(int i) {
        xmm = _mm_set1_epi32(i);
    }
    // Constructor to build from all elements:
    Vec4i(int32_t i0, int32_t i1, int32_t i2, int32_t i3) {
        xmm = _mm_setr_epi32(i0, i1, i2, i3);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec4i(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec4i & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Type cast operator to convert to __m128i used in intrinsics
    operator __m128i() const {
        return xmm;
    }
    // Member function to load from array (unaligned)
    Vec4i & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec4i & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Partial load. Load n elements and set the rest to 0
    Vec4i & load_partial(int n, void const * p) {

        switch (n) {
        case 0:
            *this = 0;  break;
        case 1:
            xmm = _mm_cvtsi32_si128(*(int32_t const*)p);  break;
        case 2:
            xmm = _mm_setr_epi32(((int32_t const*)p)[0], ((int32_t const*)p)[1], 0, 0);  break;
        case 3:
            xmm = _mm_setr_epi32(((int32_t const*)p)[0], ((int32_t const*)p)[1], ((int32_t const*)p)[2], 0);  break;
        case 4:
            load(p);  break;
        default:
            break;
        }
        return *this;
    }
    // Partial store. Store n elements
    void store_partial(int n, void * p) const {
        int32_t s[4];
        store(s);
        if (uint32_t(n) > 4) n = 4;
        for (int i = 0; i < n; i++) {
            ((int32_t*)p)[i] = s[i];
        }
    }
    // cut off vector to n elements. The last 4-n elements are set to zero
    Vec4i & cutoff(int n) {
        * this = Vec16c(xmm).cutoff(n * 4);
        return *this;
    }
    // Member function to change a single element in vector
    Vec4i const insert(int index, int32_t value) {
        __m128i broad = _mm_set1_epi32(value);  // broadcast value into all elements
        const int32_t maskl[8] = { 0,0,0,0,-1,0,0,0 };
        __m128i mask = _mm_loadu_si128((__m128i const*)(maskl + 4 - (index & 3))); // mask with FFFFFFFF at index position
        xmm = selectb(mask, broad, xmm);
        return *this;
    }
    // Member function extract a single element from vector
    int32_t extract(int index) const {
        int32_t x[4];
        store(x);
        return x[index & 3];
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    int32_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int size() {
        return 4;
    }
    static constexpr int elementtype() {
        return 8;
    }
};


/*****************************************************************************
*
*          Vec4ib: Vector of 4 Booleans for use with Vec4i and Vec4ui
*
*****************************************************************************/

class Vec4ib : public Vec4i {
public:
    // Default constructor:
    Vec4ib() = default;
    // Constructor to build from all elements:
    Vec4ib(bool x0, bool x1, bool x2, bool x3) {
        xmm = Vec4i(-int32_t(x0), -int32_t(x1), -int32_t(x2), -int32_t(x3));
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec4ib(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec4ib & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Constructor to broadcast scalar value:
    Vec4ib(bool b) : Vec4i(-int32_t(b)) {
    }
    // Assignment operator to broadcast scalar value:
    Vec4ib & operator = (bool b) {
        *this = Vec4ib(b);
        return *this;
    }
    Vec4ib & insert(int index, bool a) {
        Vec4i::insert(index, -(int)a);
        return *this;
    }
    // Member function extract a single element from vector
    bool extract(int index) const {
        return Vec4i::extract(index) != 0;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    bool operator [] (int index) const {
        return extract(index);
    }
    // Member function to change a bitfield to a boolean vector
    Vec4ib & load_bits(uint8_t a) {
        __m128i b1 = _mm_set1_epi8((int8_t)a);  // broadcast byte
        __m128i m1 = constant4ui<1, 2, 4, 8>();
        __m128i c1 = _mm_and_si128(b1, m1); // isolate one bit in each byte
        xmm = _mm_cmpgt_epi32(c1, _mm_setzero_si128());  // compare signed because no numbers are negative
        return *this;
    }
    static constexpr int elementtype() {
        return 3;
    }
    // Prevent constructing from int, etc.
    Vec4ib(int b) = delete;
    Vec4ib & operator = (int x) = delete;
};



/*****************************************************************************
*
*          Define operators for Vec4ib
*
*****************************************************************************/


// vector operator & : bitwise and
static inline Vec4ib operator & (Vec4ib const a, Vec4ib const b) {
    return Vec4ib(Vec128b(a) & Vec128b(b));
}
static inline Vec4ib operator && (Vec4ib const a, Vec4ib const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec4ib & operator &= (Vec4ib & a, Vec4ib const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec4ib operator | (Vec4ib const a, Vec4ib const b) {
    return Vec4ib(Vec128b(a) | Vec128b(b));
}
static inline Vec4ib operator || (Vec4ib const a, Vec4ib const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec4ib & operator |= (Vec4ib & a, Vec4ib const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec4ib operator ^ (Vec4ib const a, Vec4ib const b) {
    return Vec4ib(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec4ib & operator ^= (Vec4ib & a, Vec4ib const b) {
    a = a ^ b;
    return a;
}

// vector operator == : xnor
static inline Vec4ib operator == (Vec4ib const a, Vec4ib const b) {
    return Vec4ib(a ^ (~b));
}

// vector operator != : xor
static inline Vec4ib operator != (Vec4ib const a, Vec4ib const b) {
    return Vec4ib(a ^ b);
}

// vector operator ~ : bitwise not
static inline Vec4ib operator ~ (Vec4ib const a) {
    return Vec4ib(~Vec128b(a));
}

// vector operator ! : element not
static inline Vec4ib operator ! (Vec4ib const a) {
    return ~a;
}

// vector function andnot
static inline Vec4ib andnot(Vec4ib const a, Vec4ib const b) {
    return Vec4ib(andnot(Vec128b(a), Vec128b(b)));
}

// horizontal_and. Returns true if all elements are true
static inline bool horizontal_and(Vec4ib const a) {
    return _mm_movemask_epi8(a) == 0xFFFF;
}

// horizontal_or. Returns true if at least one element is true
static inline bool horizontal_or(Vec4ib const a) {
    return !_mm_testz_si128(a, a);
}


/*****************************************************************************
*
*          Operators for Vec4i
*
*****************************************************************************/

// vector operator + : add element by element
static inline Vec4i operator + (Vec4i const a, Vec4i const b) {
    return _mm_add_epi32(a, b);
}
// vector operator += : add
static inline Vec4i & operator += (Vec4i & a, Vec4i const b) {
    a = a + b;
    return a;
}

// postfix operator ++
static inline Vec4i operator ++ (Vec4i & a, int) {
    Vec4i a0 = a;
    a = a + 1;
    return a0;
}
// prefix operator ++
static inline Vec4i & operator ++ (Vec4i & a) {
    a = a + 1;
    return a;
}

// vector operator - : subtract element by element
static inline Vec4i operator - (Vec4i const a, Vec4i const b) {
    return _mm_sub_epi32(a, b);
}
// vector operator - : unary minus
static inline Vec4i operator - (Vec4i const a) {
    return _mm_sub_epi32(_mm_setzero_si128(), a);
}
// vector operator -= : subtract
static inline Vec4i & operator -= (Vec4i & a, Vec4i const b) {
    a = a - b;
    return a;
}

// postfix operator --
static inline Vec4i operator -- (Vec4i & a, int) {
    Vec4i a0 = a;
    a = a - 1;
    return a0;
}
// prefix operator --
static inline Vec4i & operator -- (Vec4i & a) {
    a = a - 1;
    return a;
}

// vector operator * : multiply element by element
static inline Vec4i operator * (Vec4i const a, Vec4i const b) {
    return _mm_mullo_epi32(a, b);
}

// vector operator *= : multiply
static inline Vec4i & operator *= (Vec4i & a, Vec4i const b) {
    a = a * b;
    return a;
}

// vector operator / : divide all elements by same integer. See bottom of file

// vector operator << : shift left
static inline Vec4i operator << (Vec4i const a, int32_t b) {
    return _mm_sll_epi32(a, _mm_cvtsi32_si128(b));
}
// vector operator <<= : shift left
static inline Vec4i & operator <<= (Vec4i & a, int32_t b) {
    a = a << b;
    return a;
}

// vector operator >> : shift right arithmetic
static inline Vec4i operator >> (Vec4i const a, int32_t b) {
    return _mm_sra_epi32(a, _mm_cvtsi32_si128(b));
}
// vector operator >>= : shift right arithmetic
static inline Vec4i & operator >>= (Vec4i & a, int32_t b) {
    a = a >> b;
    return a;
}

// vector operator == : returns true for elements for which a == b
static inline Vec4ib operator == (Vec4i const a, Vec4i const b) {
    return _mm_cmpeq_epi32(a, b);
}

// vector operator != : returns true for elements for which a != b
static inline Vec4ib operator != (Vec4i const a, Vec4i const b) {
    return Vec4ib(Vec4i(~(a == b)));
}

// vector operator > : returns true for elements for which a > b
static inline Vec4ib operator > (Vec4i const a, Vec4i const b) {
    return _mm_cmpgt_epi32(a, b);
}

// vector operator < : returns true for elements for which a < b
static inline Vec4ib operator < (Vec4i const a, Vec4i const b) {
    return b > a;
}

// vector operator >= : returns true for elements for which a >= b (signed)
static inline Vec4ib operator >= (Vec4i const a, Vec4i const b) {
    return Vec4ib(Vec4i(~(b > a)));
}

// vector operator <= : returns true for elements for which a <= b (signed)
static inline Vec4ib operator <= (Vec4i const a, Vec4i const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec4i operator & (Vec4i const a, Vec4i const b) {
    return Vec4i(Vec128b(a) & Vec128b(b));
}
static inline Vec4i operator && (Vec4i const a, Vec4i const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec4i & operator &= (Vec4i & a, Vec4i const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec4i operator | (Vec4i const a, Vec4i const b) {
    return Vec4i(Vec128b(a) | Vec128b(b));
}
static inline Vec4i operator || (Vec4i const a, Vec4i const b) {
    return a | b;
}
// vector operator |= : bitwise and
static inline Vec4i & operator |= (Vec4i & a, Vec4i const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec4i operator ^ (Vec4i const a, Vec4i const b) {
    return Vec4i(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise and
static inline Vec4i & operator ^= (Vec4i & a, Vec4i const b) {
    a = a ^ b;
    return a;
}

// vector operator ~ : bitwise not
static inline Vec4i operator ~ (Vec4i const a) {
    return Vec4i(~Vec128b(a));
}

// vector operator ! : returns true for elements == 0
static inline Vec4ib operator ! (Vec4i const a) {
    return _mm_cmpeq_epi32(a, _mm_setzero_si128());
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 4; i++) result[i] = s[i] ? a[i] : b[i];
// Each byte in s must be either 0 (false) or -1 (true). No other values are allowed.
// (s is signed)
static inline Vec4i select(Vec4ib const s, Vec4i const a, Vec4i const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec4i if_add(Vec4ib const f, Vec4i const a, Vec4i const b) {
    return a + (Vec4i(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec4i if_sub(Vec4ib const f, Vec4i const a, Vec4i const b) {
    return a - (Vec4i(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec4i if_mul(Vec4ib const f, Vec4i const a, Vec4i const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline int32_t horizontal_add(Vec4i const a) {
    __m128i sum1 = _mm_unpackhi_epi64(a, a);               // 2 high elements
    __m128i sum2 = _mm_add_epi32(a, sum1);                 // 2 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 1 high element
    __m128i sum4 = _mm_add_epi32(sum2, sum3);              // 2 sums
    return          _mm_cvtsi128_si32(sum4);               // 32 bit sum
}

// function used below
static inline int64_t _emulate_movq(__m128i const x) {
    // Preserve the compatibility storage extraction.
    union {
        __m128i m;
        int64_t y;
    } u;
    _mm_storel_epi64(&u.m, x);
    return u.y;
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Elements are sign extended before adding to avoid overflow
static inline int64_t horizontal_add_x(Vec4i const a) {
    __m128i signs = _mm_srai_epi32(a, 31);                 // sign of all elements
    __m128i a01 = _mm_unpacklo_epi32(a, signs);            // sign-extended a0, a1
    __m128i a23 = _mm_unpackhi_epi32(a, signs);            // sign-extended a2, a3
    __m128i sum1 = _mm_add_epi64(a01, a23);                // add
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);         // high qword
    __m128i sum3 = _mm_add_epi64(sum1, sum2);              // add
    return _emulate_movq(sum3);
}

// function add_saturated: add element by element, signed with saturation
static inline Vec4i add_saturated(Vec4i const a, Vec4i const b) {
    // is there a faster method?
    __m128i sum = _mm_add_epi32(a, b);                     // a + b
    __m128i axb = _mm_xor_si128(a, b);                     // check if a and b have different sign
    __m128i axs = _mm_xor_si128(a, sum);                   // check if a and sum have different sign
    __m128i overf1 = _mm_andnot_si128(axb, axs);           // check if sum has wrong sign
    __m128i overf2 = _mm_srai_epi32(overf1, 31);           // -1 if overflow
    __m128i asign = _mm_srli_epi32(a, 31);                 // 1  if a < 0
    __m128i sat1 = _mm_srli_epi32(overf2, 1);              // 7FFFFFFF if overflow
    __m128i sat2 = _mm_add_epi32(sat1, asign);             // 7FFFFFFF if positive overflow 80000000 if negative overflow
    return  selectb(overf2, sat2, sum);                    // sum if not overflow, else sat2
}

// function sub_saturated: subtract element by element, signed with saturation
static inline Vec4i sub_saturated(Vec4i const a, Vec4i const b) {
    __m128i diff = _mm_sub_epi32(a, b);                    // a + b
    __m128i axb = _mm_xor_si128(a, b);                     // check if a and b have different sign
    __m128i axs = _mm_xor_si128(a, diff);                  // check if a and sum have different sign
    __m128i overf1 = _mm_and_si128(axb, axs);              // check if sum has wrong sign
    __m128i overf2 = _mm_srai_epi32(overf1, 31);           // -1 if overflow
    __m128i asign = _mm_srli_epi32(a, 31);                 // 1  if a < 0
    __m128i sat1 = _mm_srli_epi32(overf2, 1);              // 7FFFFFFF if overflow
    __m128i sat2 = _mm_add_epi32(sat1, asign);             // 7FFFFFFF if positive overflow 80000000 if negative overflow
    return  selectb(overf2, sat2, diff);                   // diff if not overflow, else sat2
}

// function max: a > b ? a : b
static inline Vec4i max(Vec4i const a, Vec4i const b) {
    return _mm_max_epi32(a, b);
}

// function min: a < b ? a : b
static inline Vec4i min(Vec4i const a, Vec4i const b) {
    return _mm_min_epi32(a, b);
}

// function abs: a >= 0 ? a : -a
static inline Vec4i abs(Vec4i const a) {
    return _mm_abs_epi32(a);
}

// function abs_saturated: same as abs, saturate if overflow
static inline Vec4i abs_saturated(Vec4i const a) {
    __m128i absa = abs(a);                                 // abs(a)
    __m128i overfl = _mm_srai_epi32(absa, 31);             // sign
    return           _mm_add_epi32(absa, overfl);          // subtract 1 if 0x80000000
}

// function rotate_left all elements
// Use negative count to rotate right
static inline Vec4i rotate_left(Vec4i const a, int b) {
    __m128i left = _mm_sll_epi32(a, _mm_cvtsi32_si128(b & 0x1F));    // a << b
    __m128i right = _mm_srl_epi32(a, _mm_cvtsi32_si128((uint32_t(0) - uint32_t(b)) & 0x1F));// a >> (32 - b)
    __m128i rot = _mm_or_si128(left, right);                         // or
    return  rot;
}


/*****************************************************************************
*
*          Vector of 4 32-bit unsigned integers
*
*****************************************************************************/

class Vec4ui : public Vec4i {
public:
    // Default constructor:
    Vec4ui() = default;
    // Constructor to broadcast the same value into all elements:
    Vec4ui(uint32_t i) {
        xmm = _mm_set1_epi32((int32_t)i);
    }
    // Constructor to build from all elements:
    Vec4ui(uint32_t i0, uint32_t i1, uint32_t i2, uint32_t i3) {
        xmm = _mm_setr_epi32((int32_t)i0, (int32_t)i1, (int32_t)i2, (int32_t)i3);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec4ui(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec4ui & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Member function to load from array (unaligned)
    Vec4ui & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec4ui & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Member function to change a single element in vector
    Vec4ui const insert(int index, uint32_t value) {
        Vec4i::insert(index, (int32_t)value);
        return *this;
    }
    // Member function extract a single element from vector
    uint32_t extract(int index) const {
        return (uint32_t)Vec4i::extract(index);
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    uint32_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int elementtype() {
        return 9;
    }
};

// Define operators for this class

// vector operator + : add
static inline Vec4ui operator + (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec4i(a) + Vec4i(b));
}

// vector operator - : subtract
static inline Vec4ui operator - (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec4i(a) - Vec4i(b));
}

// vector operator * : multiply
static inline Vec4ui operator * (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec4i(a) * Vec4i(b));
}

// vector operator / : divide. See bottom of file

// vector operator >> : shift right logical all elements
static inline Vec4ui operator >> (Vec4ui const a, uint32_t b) {
    return _mm_srl_epi32(a, _mm_cvtsi32_si128((int)b));
}
// vector operator >> : shift right logical all elements
static inline Vec4ui operator >> (Vec4ui const a, int32_t b) {
    return a >> (uint32_t)b;
}
// vector operator >>= : shift right logical
static inline Vec4ui & operator >>= (Vec4ui & a, int b) {
    a = a >> b;
    return a;
}

// vector operator << : shift left all elements
static inline Vec4ui operator << (Vec4ui const a, uint32_t b) {
    return Vec4ui((Vec4i)a << (int32_t)b);
}
// vector operator << : shift left all elements
static inline Vec4ui operator << (Vec4ui const a, int32_t b) {
    return Vec4ui((Vec4i)a << (int32_t)b);
}

// vector operator > : returns true for elements for which a > b (unsigned)
static inline Vec4ib operator > (Vec4ui const a, Vec4ui const b) {
    __m128i signbit = _mm_set1_epi32(0x80000000);
    __m128i a1 = _mm_xor_si128(a, signbit);
    __m128i b1 = _mm_xor_si128(b, signbit);
    return (Vec4ib)_mm_cmpgt_epi32(a1, b1);                // signed compare
}

// vector operator < : returns true for elements for which a < b (unsigned)
static inline Vec4ib operator < (Vec4ui const a, Vec4ui const b) {
    return b > a;
}

// vector operator >= : returns true for elements for which a >= b (unsigned)
static inline Vec4ib operator >= (Vec4ui const a, Vec4ui const b) {
    __m128i max_ab = _mm_max_epu32(a, b);                  // max(a,b), unsigned
    return (Vec4ib)_mm_cmpeq_epi32(a, max_ab);             // a == max(a,b)
}

// vector operator <= : returns true for elements for which a <= b (unsigned)
static inline Vec4ib operator <= (Vec4ui const a, Vec4ui const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec4ui operator & (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec128b(a) & Vec128b(b));
}
static inline Vec4ui operator && (Vec4ui const a, Vec4ui const b) {
    return a & b;
}

// vector operator | : bitwise or
static inline Vec4ui operator | (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec128b(a) | Vec128b(b));
}
static inline Vec4ui operator || (Vec4ui const a, Vec4ui const b) {
    return a | b;
}

// vector operator ^ : bitwise xor
static inline Vec4ui operator ^ (Vec4ui const a, Vec4ui const b) {
    return Vec4ui(Vec128b(a) ^ Vec128b(b));
}

// vector operator ~ : bitwise not
static inline Vec4ui operator ~ (Vec4ui const a) {
    return Vec4ui(~Vec128b(a));
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 8; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec4ui select(Vec4ib const s, Vec4ui const a, Vec4ui const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec4ui if_add(Vec4ib const f, Vec4ui const a, Vec4ui const b) {
    return a + (Vec4ui(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec4ui if_sub(Vec4ib const f, Vec4ui const a, Vec4ui const b) {
    return a - (Vec4ui(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec4ui if_mul(Vec4ib const f, Vec4ui const a, Vec4ui const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline uint32_t horizontal_add(Vec4ui const a) {
    return (uint32_t)horizontal_add((Vec4i)a);
}

// Horizontal add extended: Calculates the sum of all vector elements.
// Elements are zero extended before adding to avoid overflow
static inline uint64_t horizontal_add_x(Vec4ui const a) {
    __m128i zero = _mm_setzero_si128();                    // 0
    __m128i a01  = _mm_unpacklo_epi32(a, zero);            // zero-extended a0, a1
    __m128i a23  = _mm_unpackhi_epi32(a, zero);            // zero-extended a2, a3
    __m128i sum1 = _mm_add_epi64(a01, a23);                // add
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);         // high qword
    __m128i sum3 = _mm_add_epi64(sum1, sum2);              // add
    return (uint64_t)_emulate_movq(sum3);
}

// function add_saturated: add element by element, unsigned with saturation
static inline Vec4ui add_saturated(Vec4ui const a, Vec4ui const b) {
    Vec4ui sum = a + b;
    Vec4ui aorb = Vec4ui(a | b);
    Vec4ui overflow = Vec4ui(sum < aorb);                  // overflow if a + b < (a | b)
    return Vec4ui(sum | overflow);                         // return 0xFFFFFFFF if overflow
}

// function sub_saturated: subtract element by element, unsigned with saturation
static inline Vec4ui sub_saturated(Vec4ui const a, Vec4ui const b) {
    Vec4ui diff = a - b;
    Vec4ui underflow = Vec4ui(diff > a);                   // underflow if a - b > a
    return _mm_andnot_si128(underflow, diff);              // return 0 if underflow
}

// function max: a > b ? a : b
static inline Vec4ui max(Vec4ui const a, Vec4ui const b) {
    return _mm_max_epu32(a, b);
}

// function min: a < b ? a : b
static inline Vec4ui min(Vec4ui const a, Vec4ui const b) {
    return _mm_min_epu32(a, b);
}


/*****************************************************************************
*
*          Vector of 2 64-bit signed integers
*
*****************************************************************************/

class Vec2q : public Vec128b {
public:
    // Default constructor:
    Vec2q() = default;
    // Constructor to broadcast the same value into all elements:
    Vec2q(int64_t i) {
        xmm = _mm_set1_epi64x(i);
    }
    // Constructor to build from all elements:
    Vec2q(int64_t i0, int64_t i1) {
        xmm = _mm_set_epi64x(i1, i0);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec2q(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec2q & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Type cast operator to convert to __m128i used in intrinsics
    operator __m128i() const {
        return xmm;
    }
    // Member function to load from array (unaligned)
    Vec2q & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec2q & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Partial load. Load n elements and set the rest to 0
    Vec2q & load_partial(int n, void const * p) {

        switch (n) {
        case 0:
            *this = 0;  break;
        case 1:
            // intrinsic for movq is missing in some compilers (_mm_cvtsi64_si128)
            *this = Vec2q(*(int64_t const*)p, 0);  break;
        case 2:
            load(p);  break;
        default:
            break;
        }
        return *this;
    }
    // Partial store. Store n elements
    void store_partial(int n, void * p) const {
        if (n == 1) {
            int64_t q[2];
            store(q);
            *(int64_t*)p = q[0];
        }
        else if (n > 1) {
            store(p);
        }
    }
    // cut off vector to n elements. The last 2-n elements are set to zero
    Vec2q & cutoff(int n) {
        *this = Vec16c(xmm).cutoff(n * 8);
        return *this;
    }
    // Member function to change a single element in vector
    // Note: This function is inefficient. Use load function if changing more than one element
    Vec2q const insert(int index, int64_t value) {
        union {
            __m128i m;
            int64_t ii;
        } u;
        u.ii = value;
        __m128i v = _mm_loadl_epi64(&u.m);
        if (index == 0) {
            v = _mm_unpacklo_epi64(v, v);
            xmm = _mm_unpackhi_epi64(v, xmm);
        }
        else {  // index = 1
            xmm = _mm_unpacklo_epi64(xmm, v);
        }
        return *this;
    }
    // Member function extract a single element from vector
    int64_t extract(int index) const {
        int64_t x[2];
        store(x);
        return x[index & 1];
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    int64_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int size() {
        return 2;
    }
    static constexpr int elementtype() {
        return 10;
    }
};


/*****************************************************************************
*
*          Vec2qb: Vector of 2 Booleans for use with Vec2q and Vec2uq
*
*****************************************************************************/


// Broad boolean masks for the compatibility backend.
class Vec2qb : public Vec2q {
public:
    // Default constructor:
    Vec2qb() = default;
    // Constructor to build from all elements:
    Vec2qb(bool x0, bool x1) {
        xmm = Vec2q(-int64_t(x0), -int64_t(x1));
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec2qb(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec2qb & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Constructor to broadcast scalar value:
    Vec2qb(bool b) : Vec2q(-int64_t(b)) {
    }
    // Assignment operator to broadcast scalar value:
    Vec2qb & operator = (bool b) {
        *this = Vec2qb(b);
        return *this;
    }
    Vec2qb & insert(int index, bool a) {
        Vec2q::insert(index, -(int64_t)a);
        return *this;
    }
    // Member function extract a single element from vector
    bool extract(int index) const {
        return Vec2q::extract(index) != 0;
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    bool operator [] (int index) const {
        return extract(index);
    }
    // Member function to change a bitfield to a boolean vector
    Vec2qb & load_bits(uint8_t a) {
        __m128i b1 = _mm_set1_epi8((int8_t)a);  // broadcast byte
        __m128i m1 = constant4ui<1, 1, 2, 2>();
        __m128i c1 = _mm_and_si128(b1, m1); // isolate one bit in each byte
        xmm = _mm_cmpgt_epi32(c1, _mm_setzero_si128());  // compare with 0 (64 bit compare requires SSE4.1)
        return *this;
    }
    static constexpr int elementtype() {
        return 3;
    }
    // Prevent constructing from int, etc.
    Vec2qb(int b) = delete;
    Vec2qb & operator = (int x) = delete;
};



/*****************************************************************************
*
*          Define operators for Vec2qb
*
*****************************************************************************/


// vector operator & : bitwise and
static inline Vec2qb operator & (Vec2qb const a, Vec2qb const b) {
    return Vec2qb(Vec128b(a) & Vec128b(b));
}
static inline Vec2qb operator && (Vec2qb const a, Vec2qb const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec2qb & operator &= (Vec2qb & a, Vec2qb const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec2qb operator | (Vec2qb const a, Vec2qb const b) {
    return Vec2qb(Vec128b(a) | Vec128b(b));
}
static inline Vec2qb operator || (Vec2qb const a, Vec2qb const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec2qb & operator |= (Vec2qb & a, Vec2qb const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec2qb operator ^ (Vec2qb const a, Vec2qb const b) {
    return Vec2qb(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec2qb & operator ^= (Vec2qb & a, Vec2qb const b) {
    a = a ^ b;
    return a;
}

// vector operator == : xnor
static inline Vec2qb operator == (Vec2qb const a, Vec2qb const b) {
    return Vec2qb(a ^ (~b));
}

// vector operator != : xor
static inline Vec2qb operator != (Vec2qb const a, Vec2qb const b) {
    return Vec2qb(a ^ b);
}

// vector operator ~ : bitwise not
static inline Vec2qb operator ~ (Vec2qb const a) {
    return Vec2qb(~Vec128b(a));
}

// vector operator ! : element not
static inline Vec2qb operator ! (Vec2qb const a) {
    return ~a;
}

// vector function andnot
static inline Vec2qb andnot(Vec2qb const a, Vec2qb const b) {
    return Vec2qb(andnot(Vec128b(a), Vec128b(b)));
}

// horizontal_and. Returns true if all elements are true
static inline bool horizontal_and(Vec2qb const a) {
    return _mm_movemask_epi8(a) == 0xFFFF;
}

// horizontal_or. Returns true if at least one element is true
static inline bool horizontal_or(Vec2qb const a) {
    return !_mm_testz_si128(a, a);
}




/*****************************************************************************
*
*          Operators for Vec2q
*
*****************************************************************************/

// vector operator + : add element by element
static inline Vec2q operator + (Vec2q const a, Vec2q const b) {
    return _mm_add_epi64(a, b);
}
// vector operator += : add
static inline Vec2q & operator += (Vec2q & a, Vec2q const b) {
    a = a + b;
    return a;
}

// postfix operator ++
static inline Vec2q operator ++ (Vec2q & a, int) {
    Vec2q a0 = a;
    a = a + 1;
    return a0;
}
// prefix operator ++
static inline Vec2q & operator ++ (Vec2q & a) {
    a = a + 1;
    return a;
}

// vector operator - : subtract element by element
static inline Vec2q operator - (Vec2q const a, Vec2q const b) {
    return _mm_sub_epi64(a, b);
}
// vector operator - : unary minus
static inline Vec2q operator - (Vec2q const a) {
    return _mm_sub_epi64(_mm_setzero_si128(), a);
}
// vector operator -= : subtract
static inline Vec2q & operator -= (Vec2q & a, Vec2q const b) {
    a = a - b;
    return a;
}

// postfix operator --
static inline Vec2q operator -- (Vec2q & a, int) {
    Vec2q a0 = a;
    a = a - 1;
    return a0;
}
// prefix operator --
static inline Vec2q & operator -- (Vec2q & a) {
    a = a - 1;
    return a;
}

// vector operator * : multiply element by element
static inline Vec2q operator * (Vec2q const a, Vec2q const b) {
    // Split into 32-bit multiplies
    __m128i bswap = _mm_shuffle_epi32(b, 0xB1);            // b0H,b0L,b1H,b1L (swap H<->L)
    __m128i prodlh = _mm_mullo_epi32(a, bswap);            // a0Lb0H,a0Hb0L,a1Lb1H,a1Hb1L, 32 bit L*H products
    __m128i zero = _mm_setzero_si128();                    // 0
    __m128i prodlh2 = _mm_hadd_epi32(prodlh, zero);        // a0Lb0H+a0Hb0L,a1Lb1H+a1Hb1L,0,0
    __m128i prodlh3 = _mm_shuffle_epi32(prodlh2, 0x73);    // 0, a0Lb0H+a0Hb0L, 0, a1Lb1H+a1Hb1L
    __m128i prodll = _mm_mul_epu32(a, b);                  // a0Lb0L,a1Lb1L, 64 bit unsigned products
    __m128i prod = _mm_add_epi64(prodll, prodlh3);         // a0Lb0L+(a0Lb0H+a0Hb0L)<<32, a1Lb1L+(a1Lb1H+a1Hb1L)<<32
    return  prod;
}

// vector operator *= : multiply
static inline Vec2q & operator *= (Vec2q & a, Vec2q const b) {
    a = a * b;
    return a;
}

// vector operator << : shift left
static inline Vec2q operator << (Vec2q const a, int32_t b) {
    return _mm_sll_epi64(a, _mm_cvtsi32_si128(b));
}

// vector operator <<= : shift left
static inline Vec2q & operator <<= (Vec2q & a, int32_t b) {
    a = a << b;
    return a;
}

// vector operator >> : shift right arithmetic
static inline Vec2q operator >> (Vec2q const a, int32_t b) {
    __m128i bb, shi, slo, sra2;
    if (b <= 32) {
        bb = _mm_cvtsi32_si128(b);               // b
        shi = _mm_sra_epi32(a, bb);              // a >> b signed dwords
        slo = _mm_srl_epi64(a, bb);              // a >> b unsigned qwords
    }
    else {  // b > 32
        bb = _mm_cvtsi32_si128(b - 32);          // b - 32
        shi = _mm_srai_epi32(a, 31);             // sign of a
        sra2 = _mm_sra_epi32(a, bb);             // a >> (b-32) signed dwords
        slo = _mm_srli_epi64(sra2, 32);          // a >> (b-32) >> 32 (second shift unsigned qword)
    }
    return _mm_blend_epi16(slo, shi, 0xCC);
}

// vector operator >>= : shift right arithmetic
static inline Vec2q & operator >>= (Vec2q & a, int32_t b) {
    a = a >> b;
    return a;
}

// vector operator == : returns true for elements for which a == b
static inline Vec2qb operator == (Vec2q const a, Vec2q const b) {
    return _mm_cmpeq_epi64(a, b);
}

// vector operator != : returns true for elements for which a != b
static inline Vec2qb operator != (Vec2q const a, Vec2q const b) {
    return Vec2qb(Vec2q(~(a == b)));
}

// vector operator < : returns true for elements for which a < b
static inline Vec2qb operator < (Vec2q const a, Vec2q const b) {
    return Vec2qb(Vec2q(_mm_cmpgt_epi64(b, a)));
}

// vector operator > : returns true for elements for which a > b
static inline Vec2qb operator > (Vec2q const a, Vec2q const b) {
    return b < a;
}

// vector operator >= : returns true for elements for which a >= b (signed)
static inline Vec2qb operator >= (Vec2q const a, Vec2q const b) {
    return Vec2qb(Vec2q(~(a < b)));
}

// vector operator <= : returns true for elements for which a <= b (signed)
static inline Vec2qb operator <= (Vec2q const a, Vec2q const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec2q operator & (Vec2q const a, Vec2q const b) {
    return Vec2q(Vec128b(a) & Vec128b(b));
}
static inline Vec2q operator && (Vec2q const a, Vec2q const b) {
    return a & b;
}
// vector operator &= : bitwise and
static inline Vec2q & operator &= (Vec2q & a, Vec2q const b) {
    a = a & b;
    return a;
}

// vector operator | : bitwise or
static inline Vec2q operator | (Vec2q const a, Vec2q const b) {
    return Vec2q(Vec128b(a) | Vec128b(b));
}
static inline Vec2q operator || (Vec2q const a, Vec2q const b) {
    return a | b;
}
// vector operator |= : bitwise or
static inline Vec2q & operator |= (Vec2q & a, Vec2q const b) {
    a = a | b;
    return a;
}

// vector operator ^ : bitwise xor
static inline Vec2q operator ^ (Vec2q const a, Vec2q const b) {
    return Vec2q(Vec128b(a) ^ Vec128b(b));
}
// vector operator ^= : bitwise xor
static inline Vec2q & operator ^= (Vec2q & a, Vec2q const b) {
    a = a ^ b;
    return a;
}

// vector operator ~ : bitwise not
static inline Vec2q operator ~ (Vec2q const a) {
    return Vec2q(~Vec128b(a));
}

// vector operator ! : logical not, returns true for elements == 0
static inline Vec2qb operator ! (Vec2q const a) {
    return a == Vec2q(_mm_setzero_si128());
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 8; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec2q select(Vec2qb const s, Vec2q const a, Vec2q const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec2q if_add(Vec2qb const f, Vec2q const a, Vec2q const b) {
    return a + (Vec2q(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec2q if_sub(Vec2qb const f, Vec2q const a, Vec2q const b) {
    return a - (Vec2q(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec2q if_mul(Vec2qb const f, Vec2q const a, Vec2q const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline int64_t horizontal_add(Vec2q const a) {
    __m128i sum1 = _mm_unpackhi_epi64(a, a);               // high element
    __m128i sum2 = _mm_add_epi64(a, sum1);                 // sum
    return _emulate_movq(sum2);
}

// function max: a > b ? a : b
static inline Vec2q max(Vec2q const a, Vec2q const b) {
    return select(a > b, a, b);
}

// function min: a < b ? a : b
static inline Vec2q min(Vec2q const a, Vec2q const b) {
    return select(a < b, a, b);
}

// function abs: a >= 0 ? a : -a
static inline Vec2q abs(Vec2q const a) {
    __m128i sign = _mm_cmpgt_epi64(_mm_setzero_si128(), a);// 0 > a
    __m128i inv  = _mm_xor_si128(a, sign);                 // invert bits if negative
    return         _mm_sub_epi64(inv, sign);               // add 1
}

// function abs_saturated: same as abs, saturate if overflow
static inline Vec2q abs_saturated(Vec2q const a) {
    __m128i absa = abs(a);                                 // abs(a)
    __m128i overfl = _mm_cmpgt_epi64(_mm_setzero_si128(), absa);// 0 > a
    return           _mm_add_epi64(absa, overfl);          // subtract 1 if 0x8000000000000000
}

// function rotate_left all elements
// Use negative count to rotate right
static inline Vec2q rotate_left(Vec2q const a, int b) {
    __m128i left = _mm_sll_epi64(a, _mm_cvtsi32_si128(b & 0x3F));    // a << b
    __m128i right = _mm_srl_epi64(a, _mm_cvtsi32_si128((uint32_t(0) - uint32_t(b)) & 0x3F));// a >> (64 - b)
    __m128i rot = _mm_or_si128(left, right);                         // or
    return  (Vec2q)rot;
}


/*****************************************************************************
*
*          Vector of 2 64-bit unsigned integers
*
*****************************************************************************/

class Vec2uq : public Vec2q {
public:
    // Default constructor:
    Vec2uq() = default;
    // Constructor to broadcast the same value into all elements:
    Vec2uq(uint64_t i) {
        xmm = Vec2q((int64_t)i);
    }
    // Constructor to build from all elements:
    Vec2uq(uint64_t i0, uint64_t i1) {
        xmm = Vec2q((int64_t)i0, (int64_t)i1);
    }
    // Constructor to convert from type __m128i used in intrinsics:
    Vec2uq(__m128i const x) {
        xmm = x;
    }
    // Assignment operator to convert from type __m128i used in intrinsics:
    Vec2uq & operator = (__m128i const x) {
        xmm = x;
        return *this;
    }
    // Member function to load from array (unaligned)
    Vec2uq & load(void const * p) {
        xmm = _mm_loadu_si128((__m128i const*)p);
        return *this;
    }
    // Member function to load from array (aligned)
    Vec2uq & load_a(void const * p) {
        xmm = _mm_load_si128((__m128i const*)p);
        return *this;
    }
    // Member function to change a single element in vector
    Vec2uq const insert(int index, uint64_t value) {
        Vec2q::insert(index, (int64_t)value);
        return *this;
    }
    // Member function extract a single element from vector
    uint64_t extract(int index) const {
        return (uint64_t)Vec2q::extract(index);
    }
    // Extract a single element. Use store function if extracting more than one element.
    // Operator [] can only read an element, not write.
    uint64_t operator [] (int index) const {
        return extract(index);
    }
    static constexpr int elementtype() {
        return 11;
    }
};

// Define operators for this class

// vector operator + : add
static inline Vec2uq operator + (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec2q(a) + Vec2q(b));
}

// vector operator - : subtract
static inline Vec2uq operator - (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec2q(a) - Vec2q(b));
}

// vector operator * : multiply element by element
static inline Vec2uq operator * (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec2q(a) * Vec2q(b));
}

// vector operator >> : shift right logical all elements
static inline Vec2uq operator >> (Vec2uq const a, uint32_t b) {
    return _mm_srl_epi64(a, _mm_cvtsi32_si128((int)b));
}

// vector operator >> : shift right logical all elements
static inline Vec2uq operator >> (Vec2uq const a, int32_t b) {
    return a >> (uint32_t)b;
}

// vector operator >>= : shift right logical
static inline Vec2uq & operator >>= (Vec2uq & a, int b) {
    a = a >> b;
    return a;
}

// vector operator << : shift left all elements
static inline Vec2uq operator << (Vec2uq const a, uint32_t b) {
    return Vec2uq((Vec2q)a << (int32_t)b);
}

// vector operator << : shift left all elements
static inline Vec2uq operator << (Vec2uq const a, int32_t b) {
    return Vec2uq((Vec2q)a << b);
}

// vector operator > : returns true for elements for which a > b (unsigned)
static inline Vec2qb operator > (Vec2uq const a, Vec2uq const b) {
    __m128i sign64 = constant4ui<0, 0x80000000, 0, 0x80000000>();
    __m128i aflip = _mm_xor_si128(a, sign64);              // flip sign bits to use signed compare
    __m128i bflip = _mm_xor_si128(b, sign64);
    Vec2q   cmp = _mm_cmpgt_epi64(aflip, bflip);
    return Vec2qb(cmp);
}

// vector operator < : returns true for elements for which a < b (unsigned)
static inline Vec2qb operator < (Vec2uq const a, Vec2uq const b) {
    return b > a;
}

// vector operator >= : returns true for elements for which a >= b (unsigned)
static inline Vec2qb operator >= (Vec2uq const a, Vec2uq const b) {
    return  Vec2qb(Vec2q(~(b > a)));
}

// vector operator <= : returns true for elements for which a <= b (unsigned)
static inline Vec2qb operator <= (Vec2uq const a, Vec2uq const b) {
    return b >= a;
}

// vector operator & : bitwise and
static inline Vec2uq operator & (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec128b(a) & Vec128b(b));
}
static inline Vec2uq operator && (Vec2uq const a, Vec2uq const b) {
    return a & b;
}

// vector operator | : bitwise or
static inline Vec2uq operator | (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec128b(a) | Vec128b(b));
}
static inline Vec2uq operator || (Vec2uq const a, Vec2uq const b) {
    return a | b;
}

// vector operator ^ : bitwise xor
static inline Vec2uq operator ^ (Vec2uq const a, Vec2uq const b) {
    return Vec2uq(Vec128b(a) ^ Vec128b(b));
}

// vector operator ~ : bitwise not
static inline Vec2uq operator ~ (Vec2uq const a) {
    return Vec2uq(~Vec128b(a));
}

// Functions for this class

// Select between two operands. Corresponds to this pseudocode:
// for (int i = 0; i < 2; i++) result[i] = s[i] ? a[i] : b[i];
static inline Vec2uq select(Vec2qb const s, Vec2uq const a, Vec2uq const b) {
    return selectb(s, a, b);
}

// Conditional add: For all vector elements i: result[i] = f[i] ? (a[i] + b[i]) : a[i]
static inline Vec2uq if_add(Vec2qb const f, Vec2uq const a, Vec2uq const b) {
    return a + (Vec2uq(f) & b);
}

// Conditional sub: For all vector elements i: result[i] = f[i] ? (a[i] - b[i]) : a[i]
static inline Vec2uq if_sub(Vec2qb const f, Vec2uq const a, Vec2uq const b) {
    return a - (Vec2uq(f) & b);
}

// Conditional mul: For all vector elements i: result[i] = f[i] ? (a[i] * b[i]) : a[i]
static inline Vec2uq if_mul(Vec2qb const f, Vec2uq const a, Vec2uq const b) {
    return select(f, a * b, a);
}

// Horizontal add: Calculates the sum of all vector elements. Overflow will wrap around
static inline uint64_t horizontal_add(Vec2uq const a) {
    return (uint64_t)horizontal_add((Vec2q)a);
}

// function max: a > b ? a : b
static inline Vec2uq max(Vec2uq const a, Vec2uq const b) {
    return select(a > b, a, b);
}

// function min: a < b ? a : b
static inline Vec2uq min(Vec2uq const a, Vec2uq const b) {
    return select(a > b, b, a);
}


/*****************************************************************************
*
*          Vector permute functions
*
******************************************************************************
*
* These permute functions can reorder the elements of a vector and optionally
* set some elements to zero.
*
* The indexes are inserted as template parameters in <>.
* These indexes must be constants.
* Each template parameter is an index to the element you want to select.
* An index of -1 will generate zero.
* An index of V_DC means don't care. This gives the best instruction that
* fits the remaining indexes
*
* Example:
* Vec4i a(10,11,12,13);        // a is (10,11,12,13)
* Vec4i b, c;
* b = permute4<0,0,2,2>(a);    // b is (10,10,12,12)
* c = permute4<3,2,-1,-1>(a);  // c is (13,12, 0, 0)
*
* A lot of the code here is metaprogramming aiming to find the instructions
* that best fits the template parameters and instruction set.
* The final optimized code will contain only one or a few instructions.
* Higher instruction sets may give you more efficient code.
*
*****************************************************************************/

// permute Vec2q
template <int i0, int i1>
static inline Vec2q permute2(Vec2q const a) {
    constexpr int indexs[2] = { i0, i1 };                  // indexes as array
    __m128i y = a;                                         // result
    // get flags for possibilities that fit the permutation pattern
    constexpr uint64_t flags = perm_flags<Vec2q>(indexs);

    static_assert((flags & perm_outofrange) == 0, "Index out of range in permute function");

    if constexpr ((flags & perm_allzero) != 0) return _mm_setzero_si128();  // just return zero

    constexpr bool fit_shleft  = (flags & perm_shleft)  != 0;
    constexpr bool fit_shright = (flags & perm_shright) != 0;
    constexpr bool fit_punpckh = (flags & perm_punpckh) != 0;
    constexpr bool fit_punpckl = (flags & perm_punpckl) != 0;
    constexpr bool fit_zeroing = (flags & perm_zeroing) != 0;

    if constexpr ((flags & perm_perm) != 0) {              // permutation needed
        // try to fit various instructions

        if constexpr (fit_shleft && fit_zeroing) {
            // pslldq does both permutation and zeroing. if zeroing not needed use punpckl instead
            return _mm_bslli_si128(a, 8);
        }
        if constexpr (fit_shright && fit_zeroing) {
            // psrldq does both permutation and zeroing. if zeroing not needed use punpckh instead
            return _mm_bsrli_si128(a, 8);
        }
        if constexpr (fit_punpckh) {       // fits punpckhi
            y = _mm_unpackhi_epi64(a, a);
        }
        else if constexpr (fit_punpckl) {  // fits punpcklo
            y = _mm_unpacklo_epi64(a, a);
        }
        else {  // needs general permute
            y = _mm_shuffle_epi32(a, i0 * 0x0A + i1 * 0xA0 + 0x44);
        }
    }
    if constexpr (fit_zeroing) {
        // additional zeroing needed
        if constexpr (i0 == -1) {
            y = _mm_unpackhi_epi64(_mm_setzero_si128(), y);
        }
        else if constexpr (i1 == -1) {
            y = _mm_unpacklo_epi64(y, _mm_setzero_si128());
        }
    }
    return y;
}

template <int i0, int i1>
static inline Vec2uq permute2(Vec2uq const a) {
    return Vec2uq(permute2 <i0, i1>((Vec2q)a));
}

// permute Vec4i
template <int i0, int i1, int i2, int i3>
static inline Vec4i permute4(Vec4i const a) {
    constexpr int indexs[4] = {i0, i1, i2, i3};            // indexes as array
    __m128i y = a;                                         // result

    // get flags for possibilities that fit the permutation pattern
    constexpr uint64_t flags = perm_flags<Vec4i>(indexs);

    static_assert((flags & perm_outofrange) == 0, "Index out of range in permute function");

    if constexpr ((flags & perm_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & perm_perm) != 0) {              // permutation needed

        if constexpr ((flags & perm_largeblock) != 0) {
            // use larger permutation
            constexpr EList<int, 2> L = largeblock_perm<4>(indexs); // permutation pattern
            y = permute2 <L.a[0], L.a[1]> (Vec2q(a));
            if (!(flags & perm_addz)) return y;            // no remaining zeroing
        }
        else if constexpr ((flags & perm_shleft) != 0) {   // fits pslldq
            y = _mm_bslli_si128(a, (16-(flags >> perm_rot_count)) & 0xF);
            if (!(flags & perm_addz)) return y;            // no remaining zeroing
        }
        else if constexpr ((flags & perm_shright) != 0) {  // fits psrldq
            y = _mm_bsrli_si128(a, (flags >> perm_rot_count) & 0xF);
            if (!(flags & perm_addz)) return y;            // no remaining zeroing
        }
        else if constexpr ((flags & perm_zeroing) != 0) {
            // Do both permutation and zeroing with PSHUFB instruction
            // (bm is constexpr rather than const to make sure it is calculated at compile time)
            constexpr EList <int8_t, 16> bm = pshufb_mask<Vec4i>(indexs);
            return _mm_shuffle_epi8(a, Vec4i().load(bm.a));
        }
        else if constexpr ((flags & perm_punpckh) != 0) {  // fits punpckhi
            y = _mm_unpackhi_epi32(a, a);
        }
        else if constexpr ((flags & perm_punpckl) != 0) {  // fits punpcklo
            y = _mm_unpacklo_epi32(a, a);
        }
        else if constexpr ((flags & perm_rotate) != 0) {   // fits palignr
            y = _mm_alignr_epi8(a, a, (flags >> perm_rot_count) & 0xF);
        }
        else {  // needs general permute
            y = _mm_shuffle_epi32(a, (i0 & 3) | (i1 & 3) << 2 | (i2 & 3) << 4 | (i3 & 3) << 6);
        }
    }
    if constexpr ((flags & perm_zeroing) != 0) {
        // additional zeroing needed
        constexpr EList <int32_t, 4> bm = zero_mask_broad<Vec4i>(indexs);
        y = _mm_and_si128(Vec4i().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3>
static inline Vec4ui permute4(Vec4ui const a) {
    return Vec4ui(permute4 <i0, i1, i2, i3>(Vec4i(a)));
}


// permute Vec8s
template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7>
static inline Vec8s permute8(Vec8s const a) {
    // indexes as array
    constexpr int indexs[8] = { i0, i1, i2, i3, i4, i5, i6, i7 };
    // get flags for possibilities that fit the permutation pattern
    constexpr uint64_t flags   = perm_flags<Vec8s>(indexs);
    constexpr uint64_t flags16 = perm16_flags<Vec8s>(indexs);

    constexpr bool fit_zeroing = (flags & perm_zeroing) != 0;// needs additional zeroing
    constexpr bool L2L = (flags16 & 1) != 0;               // from low  to low  64-bit part
    constexpr bool H2H = (flags16 & 2) != 0;               // from high to high 64-bit part
    constexpr bool H2L = (flags16 & 4) != 0;               // from high to low  64-bit part
    constexpr bool L2H = (flags16 & 8) != 0;               // from low  to high 64-bit part
    constexpr uint8_t pL2L = uint8_t(flags16 >> 32);       // low  to low  permute pattern
    constexpr uint8_t pH2H = uint8_t(flags16 >> 40);       // high to high permute pattern
    constexpr uint8_t noperm = 0xE4;                       // pattern for no permute

    __m128i y = a;                                         // result

    static_assert((flags & perm_outofrange) == 0, "Index out of range in permute function");

    if constexpr ((flags & perm_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & perm_perm) != 0) {
        // permutation needed

        if constexpr ((flags & perm_largeblock) != 0) {
            // use larger permutation
            constexpr EList<int, 4> L = largeblock_perm<8>(indexs); // permutation pattern
            y = permute4 <L.a[0], L.a[1], L.a[2], L.a[3]> (Vec4i(a));
            if (!(flags & perm_addz)) return y;            // no remaining zeroing
        }
        else if constexpr ((flags & perm_shleft) != 0 && (flags & perm_addz) == 0) {// fits pslldq
            return _mm_bslli_si128(a, (16-(flags >> perm_rot_count)) & 0xF);
        }
        else if constexpr ((flags & perm_shright) != 0 && (flags & perm_addz) == 0) {// fits psrldq
            return _mm_bsrli_si128(a, (flags >> perm_rot_count) & 0xF);
        }
        else if constexpr ((flags & perm_broadcast) != 0 && (flags & perm_zeroing) == 0 && (flags >> perm_rot_count & 0xF) == 0) {
            y = _mm_shufflelo_epi16(a, 0);                 // broadcast of first element
            return _mm_unpacklo_epi64(y, y);
        }
        else if constexpr ((flags & perm_zext) != 0 && (flags & perm_punpckl) != 0 && zero_mask<8>(indexs) == 0x55) {  // fits zero extension
            return _mm_unpacklo_epi16(a, _mm_setzero_si128 ());
        }
        else if constexpr ((flags & perm_punpckh) != 0 && zero_mask<8>(indexs) == 0x55) {  // fits zero extension of high high elements
            return _mm_unpackhi_epi16(a, _mm_setzero_si128 ());
        }
        else if constexpr (fit_zeroing) {
            // Do both permutation and zeroing with PSHUFB instruction
            constexpr EList <int8_t, 16> bm = pshufb_mask<Vec8s>(indexs);
            return _mm_shuffle_epi8(a, Vec8s().load(bm.a));
        }
        else if constexpr ((flags & perm_punpckh) != 0) {  // fits punpckhi
            y = _mm_unpackhi_epi16(a, a);
        }
        else if constexpr ((flags & perm_punpckl) != 0) {  // fits punpcklo
            y = _mm_unpacklo_epi16(a, a);
        }
        else if constexpr ((flags & perm_rotate) != 0) {   // fits palignr
            y = _mm_alignr_epi8(a, a, (flags >> perm_rot_count) & 0xF);
        }
        else if constexpr (!H2L && !L2H) {                 // no crossing of 64-bit boundary
            if constexpr (L2L && pL2L != noperm) {
                y = _mm_shufflelo_epi16(y, pL2L);          // permute low 64-bits
            }
            if constexpr (H2H && pH2H != noperm) {
                y = _mm_shufflehi_epi16(y, pH2H);          // permute high 64-bits
            }
        }
        else {  // needs general permute
            constexpr EList <int8_t, 16> bm = pshufb_mask<Vec8s>(indexs);
            y = _mm_shuffle_epi8(a, Vec8s().load(bm.a));
            return y;  // _mm_shuffle_epi8 also does zeroing
        }
    }
    if constexpr (fit_zeroing) {
        // additional zeroing needed
        constexpr EList <int16_t, 8> bm = zero_mask_broad<Vec8s>(indexs);
        y = _mm_and_si128(Vec8s().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7>
static inline Vec8us permute8(Vec8us const a) {
    return Vec8us(permute8 <i0, i1, i2, i3, i4, i5, i6, i7>(Vec8s(a)));
}

// permute Vec16c
template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7,
    int i8, int i9, int i10, int i11, int i12, int i13, int i14, int i15 >
    static inline Vec16c permute16(Vec16c const a) {

    // indexes as array
    constexpr int indexs[16] = { i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15 };
    // get flags for possibilities that fit the permutation pattern
    constexpr uint64_t flags = perm_flags<Vec16c>(indexs);

    constexpr bool fit_zeroing = (flags & perm_zeroing) != 0;// needs additional zeroing

    __m128i y = a;                                         // result

    static_assert((flags & perm_outofrange) == 0, "Index out of range in permute function");

    if constexpr ((flags & perm_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & perm_perm) != 0) {
        // permutation needed

        if constexpr ((flags & perm_largeblock) != 0) {
            // use larger permutation
            constexpr EList<int, 8> L = largeblock_perm<16>(indexs); // permutation pattern
            y = permute8 <L.a[0], L.a[1], L.a[2], L.a[3], L.a[4], L.a[5], L.a[6], L.a[7] > (Vec8s(a));
            if (!(flags & perm_addz)) return y;            // no remaining zeroing
        }
        else if constexpr ((flags & perm_shleft) != 0) {   // fits pslldq
            y = _mm_bslli_si128(a, (16-(flags >> perm_rot_count)) & 0xF);
            if ((flags & perm_addz) == 0) return y;
        }
        else if constexpr ((flags & perm_shright) != 0) {  // fits psrldq
            y = _mm_bsrli_si128(a, (flags >> perm_rot_count) & 0xF);
            if ((flags & perm_addz) == 0) return y;
        }
        else if constexpr (fit_zeroing) {
            // Do both permutation and zeroing with PSHUFB instruction
            constexpr EList <int8_t, 16> bm = pshufb_mask<Vec16c>(indexs);
            return _mm_shuffle_epi8(a, Vec16c().load(bm.a));
        }
        else if constexpr ((flags & perm_punpckh) != 0) {  // fits punpckhi
            y = _mm_unpackhi_epi8(a, a);
        }
        else if constexpr ((flags & perm_punpckl) != 0) {  // fits punpcklo
            y = _mm_unpacklo_epi8(a, a);
        }
        else if constexpr ((flags & perm_rotate) != 0) {   // fits palignr
            y = _mm_alignr_epi8(a, a, (flags >> perm_rot_count) & 0xF);
        }
        else {  // needs general permute
            constexpr EList <int8_t, 16> bm = pshufb_mask<Vec16c>(indexs);
            y = _mm_shuffle_epi8(a, Vec16c().load(bm.a));
            return y;  // _mm_shuffle_epi8 also does zeroing
        }
    }
    if constexpr (fit_zeroing) {
        // additional zeroing needed
        constexpr EList <int8_t, 16> bm = zero_mask_broad<Vec16c>(indexs);
        y = _mm_and_si128(Vec16c().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7,
    int i8, int i9, int i10, int i11, int i12, int i13, int i14, int i15 >
    static inline Vec16uc permute16(Vec16uc const a) {
    return Vec16uc(permute16 <i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15>(Vec16c(a)));
}


/*****************************************************************************
*
*          Vector blend functions
*
******************************************************************************
*
* These blend functions can mix elements from two different vectors of N
* elements eadh and optionally set some elements to zero.
*
* The N indexes are inserted as template parameters in <>.
* These indexes must be compile-time constants. Each template parameter
* selects an element from one of the input vectors a and b.
* An index in the range 0 .. N-1 selects the corresponding element from a.
* An index in the range N .. 2*N-1 selects an element from b.
* An index with the value -1 gives zero in the corresponding element of
* the result.
* An index with the value V_DC means don't care. The code will select the
* optimal sequence of instructions that fits the remaining indexes.
*
* Example:
* Vec4i a(100,101,102,103);         // a is (100, 101, 102, 103)
* Vec4i b(200,201,202,203);         // b is (200, 201, 202, 203)
* Vec4i c;
* c = blend4<1,4,-1,7> (a,b);       // c is (101, 200,   0, 203)
*
* A lot of the code here is metaprogramming aiming to find the instructions
* that best fit the template parameters and instruction set. The metacode
* will be reduced out to leave only a few vector instructions in release
* mode with optimization on.
*****************************************************************************/

// permute and blend Vec2q
template <int i0, int i1>
static inline Vec2q blend2(Vec2q const a, Vec2q const b) {
    int constexpr indexs[2] = { i0, i1 };                  // indexes as array
    __m128i y = a;                                         // result
    constexpr uint64_t flags = blend_flags<Vec2q>(indexs); // get flags for possibilities that fit the index pattern

    static_assert((flags & blend_outofrange) == 0, "Index out of range in blend function");

    if constexpr ((flags & blend_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & blend_b) == 0) {                // nothing from b. just permute a
        return permute2 <i0, i1> (a);
    }
    if constexpr ((flags & blend_a) == 0) {                // nothing from a. just permute b
        return permute2 <i0<0 ? i0 : i0&1, i1<0 ? i1 : i1&1> (b);
    }

    if constexpr ((flags & (blend_perma | blend_permb)) == 0) {// no permutation, only blending
        y = _mm_blend_epi16 (a, b, ((i0 & 2) ? 0x0F : 0) | ((i1 & 2) ? 0xF0 : 0));
    }
    // check if pattern fits special cases
    else if constexpr ((flags & blend_punpcklab) != 0) {
        y = _mm_unpacklo_epi64 (a, b);
    }
    else if constexpr ((flags & blend_punpcklba) != 0) {
        y = _mm_unpacklo_epi64 (b, a);
    }
    else if constexpr ((flags & blend_punpckhab) != 0) {
        y = _mm_unpackhi_epi64 (a, b);
    }
    else if constexpr ((flags & blend_punpckhba) != 0) {
        y = _mm_unpackhi_epi64 (b, a);
    }
    else if constexpr ((flags & blend_rotateab) != 0) {
        y = _mm_alignr_epi8(a, b, flags >> blend_rotpattern);
    }
    else if constexpr ((flags & blend_rotateba) != 0) {
        y = _mm_alignr_epi8(b, a, flags >> blend_rotpattern);
    }
#if ALLOW_FP_PERMUTE  // allow floating point permute instructions on integer vectors
    else if constexpr ((flags & blend_shufab) != 0) {      // use floating point instruction shufpd
        y = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b), (flags >> blend_shufpattern) & 3));
    }
    else if constexpr ((flags & blend_shufba) != 0) {      // use floating point instruction shufpd
        y = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(b), _mm_castsi128_pd(a), (flags >> blend_shufpattern) & 3));
    }
#endif
    else { // No special cases. permute a and b separately, then blend.
           // This will not occur if ALLOW_FP_PERMUTE is true
        constexpr bool dozero = false;
        constexpr EList<int, 4> L = blend_perm_indexes<2, (int)dozero>(indexs); // get permutation indexes
        __m128i ya = permute2<L.a[0], L.a[1]>(a);
        __m128i yb = permute2<L.a[2], L.a[3]>(b);
        y = _mm_blend_epi16 (ya, yb, ((i0 & 2) ? 0x0F : 0) | ((i1 & 2) ? 0xF0 : 0));
    }

    if constexpr ((flags & blend_zeroing) != 0) {          // additional zeroing needed
        constexpr EList <int64_t, 2> bm = zero_mask_broad<Vec2q>(indexs);
        y = _mm_and_si128(Vec2q().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1>
static inline Vec2uq blend2(Vec2uq const a, Vec2uq const b) {
    return Vec2uq(blend2 <i0, i1>(Vec2q(a), Vec2q(b)));
}


// permute and blend Vec4i
template <int i0, int i1, int i2, int i3>
static inline Vec4i blend4(Vec4i const a, Vec4i const b) {
    int constexpr indexs[4] = { i0, i1, i2, i3 };          // indexes as array
    __m128i y = a;                                         // result
    constexpr uint64_t flags = blend_flags<Vec4i>(indexs); // get flags for possibilities that fit the index pattern

    constexpr bool blendonly = (flags & (blend_perma | blend_permb)) == 0; // no permutation, only blending

    static_assert((flags & blend_outofrange) == 0, "Index out of range in blend function");

    if constexpr ((flags & blend_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & blend_b) == 0) {                // nothing from b. just permute a
        return permute4 <i0, i1, i2, i3> (a);
    }
    if constexpr ((flags & blend_a) == 0) {                // nothing from a. just permute b
        return permute4 < i0<0?i0:i0&3, i1<0?i1:i1&3, i2<0?i2:i2&3, i3<0?i3:i3&3> (b);
    }
    if constexpr ((flags & blend_largeblock) != 0) {       // fits blending with larger block size
        constexpr EList<int, 2> L = largeblock_indexes<4>(indexs);
        y = blend2 <L.a[0], L.a[1]> (Vec2q(a), Vec2q(b));
        if constexpr ((flags & blend_addz) == 0) {
            return y;                                      // any zeroing has been done by larger blend
        }
    }
    // check if pattern fits special cases
    else if constexpr ((flags & blend_punpcklab) != 0) {
        y = _mm_unpacklo_epi32 (a, b);
    }
    else if constexpr ((flags & blend_punpcklba) != 0) {
        y = _mm_unpacklo_epi32 (b, a);
    }
    else if constexpr ((flags & blend_punpckhab) != 0) {
        y = _mm_unpackhi_epi32 (a, b);
    }
    else if constexpr ((flags & blend_punpckhba) != 0) {
        y = _mm_unpackhi_epi32 (b, a);
    }
    else if constexpr ((flags & blend_rotateab) != 0) {
        y = _mm_alignr_epi8(a, b, flags >> blend_rotpattern);
    }
    else if constexpr ((flags & blend_rotateba) != 0) {
        y = _mm_alignr_epi8(b, a, flags >> blend_rotpattern);
    }
#if ALLOW_FP_PERMUTE  // allow floating point permute instructions on integer vectors
    else if constexpr ((flags & blend_shufab) != 0 && !blendonly) { // use floating point instruction shufps
        y = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), uint8_t(flags >> blend_shufpattern)));
    }
    else if constexpr ((flags & blend_shufba) != 0 && !blendonly) { // use floating point instruction shufps
        y = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a), uint8_t(flags >> blend_shufpattern)));
    }
#endif
    else { // No special cases. permute a and b separately, then blend.
        constexpr bool dozero = false;
        Vec4i ya = a, yb = b;   // a and b permuted
        constexpr EList<int, 8> L = blend_perm_indexes<4, (int)dozero>(indexs); // get permutation indexes
        if constexpr ((flags & blend_perma) != 0 || dozero) {
            ya = permute4 <L.a[0], L.a[1], L.a[2], L.a[3]>(a);
        }
        if constexpr ((flags & blend_permb) != 0 || dozero) {
            yb = permute4 <L.a[4], L.a[5], L.a[6], L.a[7]>(b);
        }
        constexpr uint8_t mm = ((i0 & 4) ? 0x03 : 0) | ((i1 & 4) ? 0x0C : 0) | ((i2 & 4) ? 0x30 : 0) | ((i3 & 4) ? 0xC0 : 0);
        y = _mm_blend_epi16 (ya, yb, mm);
    }
    if constexpr ((flags & blend_zeroing) != 0) {          // additional zeroing needed
        constexpr EList <int32_t, 4> bm = zero_mask_broad<Vec4i>(indexs);
        y = _mm_and_si128(Vec4i().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3>
static inline Vec4ui blend4(Vec4ui const a, Vec4ui const b) {
    return Vec4ui(blend4<i0, i1, i2, i3>(Vec4i(a), Vec4i(b)));
}


// permute and blend Vec8s
template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7>
static inline Vec8s blend8(Vec8s const a, Vec8s const b) {
    int constexpr indexs[8] = { i0, i1, i2, i3, i4, i5, i6, i7 };  // indexes as array
    __m128i y = a;                                         // result
    constexpr uint64_t flags = blend_flags<Vec8s>(indexs); // get flags for possibilities that fit the index pattern

    static_assert((flags & blend_outofrange) == 0, "Index out of range in blend function");

    if constexpr ((flags & blend_allzero) != 0) return _mm_setzero_si128();

    if constexpr ((flags & blend_b) == 0) {                // nothing from b. just permute a
        return permute8 <i0, i1, i2, i3, i4, i5, i6, i7> (a);
    }
    if constexpr ((flags & blend_a) == 0) {                // nothing from a. just permute b
        return permute8 < i0<0?i0:i0&7, i1<0?i1:i1&7, i2<0?i2:i2&7, i3<0?i3:i3&7,
                          i4<0?i4:i4&7, i5<0?i5:i5&7, i6<0?i6:i6&7, i7<0?i7:i7&7 > (b);
    }
    if constexpr ((flags & blend_largeblock) != 0) {       // fits blending with larger block size
        constexpr EList<int, 4> L = largeblock_indexes<8>(indexs);
        y = blend4 <L.a[0], L.a[1], L.a[2], L.a[3]> (Vec4i(a), Vec4i(b));
        if constexpr ((flags & blend_addz) == 0) {
            return y;                                      // any zeroing has been done by larger blend
        }
    }
    // check if pattern fits special cases
    else if constexpr ((flags & blend_punpcklab) != 0) {
        y = _mm_unpacklo_epi16 (a, b);
    }
    else if constexpr ((flags & blend_punpcklba) != 0) {
        y = _mm_unpacklo_epi16 (b, a);
    }
    else if constexpr ((flags & blend_punpckhab) != 0) {
        y = _mm_unpackhi_epi16 (a, b);
    }
    else if constexpr ((flags & blend_punpckhba) != 0) {
        y = _mm_unpackhi_epi16 (b, a);
    }
    else if constexpr ((flags & blend_rotateab) != 0) {
        y = _mm_alignr_epi8(a, b, flags >> blend_rotpattern);
    }
    else if constexpr ((flags & blend_rotateba) != 0) {
        y = _mm_alignr_epi8(b, a, flags >> blend_rotpattern);
    }

    else { // No special cases.
        // full blend instruction not available,
        // permute a and b separately, then blend.
        constexpr bool dozero = (flags & blend_zeroing) != 0;
        Vec8s ya = a, yb = b;   // a and b permuted
        constexpr EList<int, 16> L = blend_perm_indexes<8, (int)dozero>(indexs); // get permutation indexes
        if constexpr ((flags & blend_perma) != 0 || dozero) {
            ya = permute8 <L.a[0], L.a[1], L.a[2], L.a[3], L.a[4], L.a[5], L.a[6], L.a[7]> (a);
        }
        if constexpr ((flags & blend_permb) != 0 || dozero) {
            yb = permute8 <L.a[8], L.a[9], L.a[10], L.a[11], L.a[12], L.a[13], L.a[14], L.a[15]> (b);
        }
        if constexpr (dozero) {  // unused elements are zero
            return _mm_or_si128(ya, yb);
        }
        else { // blend ya and yb

        constexpr uint8_t mm = ((i0 & 8) ? 0x01 : 0) | ((i1 & 8) ? 0x02 : 0) | ((i2 & 8) ? 0x04 : 0) | ((i3 & 8) ? 0x08 : 0) |
                               ((i4 & 8) ? 0x10 : 0) | ((i5 & 8) ? 0x20 : 0) | ((i6 & 8) ? 0x40 : 0) | ((i7 & 8) ? 0x80 : 0);
        y = _mm_blend_epi16 (ya, yb, mm);
        }
    }
    if constexpr ((flags & blend_zeroing) != 0) {          // additional zeroing needed after special cases
        constexpr EList <int16_t, 8> bm = zero_mask_broad<Vec8s>(indexs);
        y = _mm_and_si128(Vec8s().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7>
static inline Vec8us blend8(Vec8us const a, Vec8us const b) {
    return Vec8us(blend8<i0, i1, i2, i3, i4, i5, i6, i7>(Vec8s(a), Vec8s(b)));
}


// permute and blend Vec16c
template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7,
    int i8, int i9, int i10, int i11, int i12, int i13, int i14, int i15 >
    static inline Vec16c blend16(Vec16c const a, Vec16c const b) {
    int constexpr indexs[16] = { i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15 };          // indexes as array
    __m128i y = a;                                         // result
    constexpr uint64_t flags = blend_flags<Vec16c>(indexs);// get flags for possibilities that fit the index pattern

    static_assert((flags & blend_outofrange) == 0, "Index out of range in blend function");

    if constexpr ((flags & blend_allzero) != 0) return _mm_setzero_si128();

    else if constexpr ((flags & blend_b) == 0) {           // nothing from b. just permute a
        return permute16 <i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15> (a);
    }
    else if constexpr ((flags & blend_a) == 0) {           // nothing from a. just permute b
        constexpr EList<int, 32> L = blend_perm_indexes<16, 2>(indexs); // get permutation indexes
        return permute16 < L.a[16], L.a[17], L.a[18], L.a[19], L.a[20], L.a[21], L.a[22], L.a[23],
            L.a[24], L.a[25], L.a[26], L.a[27], L.a[28], L.a[29], L.a[30], L.a[31] > (b);
    }
    else if constexpr ((flags & blend_rotateab) != 0) {
        y = _mm_alignr_epi8(a, b, flags >> blend_rotpattern);
    }
    else if constexpr ((flags & blend_rotateba) != 0) {
        y = _mm_alignr_epi8(b, a, flags >> blend_rotpattern);
    }
    else if constexpr ((flags & blend_largeblock) != 0) {  // fits blending with larger block size
        constexpr EList<int, 8> L = largeblock_indexes<16>(indexs);
        y = blend8 <L.a[0], L.a[1], L.a[2], L.a[3], L.a[4], L.a[5], L.a[6], L.a[7] > (Vec8s(a), Vec8s(b));
        if constexpr ((flags & blend_addz) == 0) {
            return y;                                      // any zeroing has been done by larger blend
        }
    }
    // check if pattern fits special cases
    else if constexpr ((flags & blend_punpcklab) != 0) {
        y = _mm_unpacklo_epi8 (a, b);
    }
    else if constexpr ((flags & blend_punpcklba) != 0) {
        y = _mm_unpacklo_epi8 (b, a);
    }
    else if constexpr ((flags & blend_punpckhab) != 0) {
        y = _mm_unpackhi_epi8 (a, b);
    }
    else if constexpr ((flags & blend_punpckhba) != 0) {
        y = _mm_unpackhi_epi8 (b, a);
    }
    else { // No special cases. Full permute needed

        // full blend instruction not available,
        // permute a and b separately, then blend.
        constexpr bool dozero = true;
        Vec16c ya = a, yb = b;   // a and b permuted
        constexpr EList<int, 32> L = blend_perm_indexes<16, (int)dozero>(indexs); // get permutation indexes
        if constexpr ((flags & blend_perma) != 0 || dozero) {
            ya = permute16 <L.a[0], L.a[1], L.a[2], L.a[3], L.a[4], L.a[5], L.a[6], L.a[7],
                L.a[8], L.a[9], L.a[10], L.a[11], L.a[12], L.a[13], L.a[14], L.a[15]> (a);
        }
        if constexpr ((flags & blend_permb) != 0 || dozero) {
            yb = permute16 <L.a[16], L.a[17], L.a[18], L.a[19], L.a[20], L.a[21], L.a[22], L.a[23],
                L.a[24], L.a[25], L.a[26], L.a[27], L.a[28], L.a[29], L.a[30], L.a[31]> (b);
        }
        if constexpr (dozero) {  // unused fields in ya and yb are zero
            return _mm_or_si128(ya, yb);
        }
        else {
        }
    }
    if constexpr ((flags & blend_zeroing) != 0) {          // additional zeroing needed
        constexpr EList <int8_t, 16> bm = zero_mask_broad<Vec16c>(indexs);
        y = _mm_and_si128(Vec16c().load(bm.a), y);
    }
    return y;
}

template <int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7,
    int i8, int i9, int i10, int i11, int i12, int i13, int i14, int i15 >
    static inline Vec16uc blend16(Vec16uc const a, Vec16uc const b) {
    return Vec16uc(blend16<i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15>(Vec16c(a), Vec16c(b)));
}


/*****************************************************************************
*
*          Vector lookup functions
*
******************************************************************************
*
* These functions use vector elements as indexes into a table.
* The table is given as one or more vectors or as an array.
*
* This can be used for several purposes:
*  - table lookup
*  - permute or blend with variable indexes
*  - blend from more than two sources
*  - gather non-contiguous data
*
* An index out of range may produce any value - the actual value produced is
* implementation dependent and may be different for different instruction
* sets. An index out of range does not produce an error message or exception.
*
* Example:
* Vec4i a(2,0,0,3);           // index a is (  2,   0,   0,   3)
* Vec4i b(100,101,102,103);   // table b is (100, 101, 102, 103)
* Vec4i c;
* c = lookup4 (a,b);          // c is (102, 100, 100, 103)
*
*****************************************************************************/

static inline Vec16c lookup16(Vec16c const index, Vec16c const table) {
    return _mm_shuffle_epi8(table, index);
}

static inline Vec16c lookup32(Vec16c const index, Vec16c const table0, Vec16c const table1) {
    Vec16c r0 = _mm_shuffle_epi8(table0, index + 0x70);           // make negative index for values >= 16
    Vec16c r1 = _mm_shuffle_epi8(table1, (index ^ 0x10) + 0x70);  // make negative index for values <  16
    return r0 | r1;
}

template <int n>
static inline Vec16c lookup(Vec16c const index, void const * table) {
    if constexpr (n <= 0) return 0;
    else {
    if constexpr (n <= 16) return lookup16(index, Vec16c().load_partial(n < 16 ? n : 16, table));
    if constexpr (n <= 32) return lookup32(index, Vec16c().load_partial(n < 16 ? n : 16, table), Vec16c().load_partial(n - 16, (int8_t const*)table + 16));
    // n > 32. Limit index
    Vec16uc index1;
    if constexpr (n == INT_MAX || n > 256) {
        index1 = index;
    }
    else if constexpr ((n & (n - 1)) == 0) {
        // n is a power of 2, make index modulo n
        index1 = Vec16uc(index) & uint8_t(n - 1);
    }
    else {
        // n is not a power of 2, limit to n-1
        index1 = min(Vec16uc(index), uint8_t(n - 1));
    }
    uint8_t ii[16];  index1.store(ii);
    int8_t  rr[16];
    for (int j = 0; j < 16; j++) {
        rr[j] = ((int8_t*)table)[ii[j]];
    }
    return Vec16c().load(rr);
    } // n > 0
}

static inline Vec8s lookup8(Vec8s const index, Vec8s const table) {
    return _mm_shuffle_epi8(table, index * 0x202 + 0x100);
}

static inline Vec8s lookup16(Vec8s const index, Vec8s const table0, Vec8s const table1) {
    Vec8s r0 = _mm_shuffle_epi8(table0, Vec16c(index * 0x202) + Vec16c(Vec8s(0x7170)));
    Vec8s r1 = _mm_shuffle_epi8(table1, Vec16c(index * 0x202 ^ 0x1010) + Vec16c(Vec8s(0x7170)));
    return r0 | r1;
}

template <int n>
static inline Vec8s lookup(Vec8s const index, void const * table) {
    if constexpr (n <= 0) return 0;
    else {
    if constexpr (n <= 8) return lookup8(index, Vec8s().load_partial(n < 8 ? n : 8, table));
    if constexpr (n <= 16) return lookup16(index, Vec8s().load_partial(n < 8 ? n : 8, table), Vec8s().load_partial(n - 8, (int16_t const*)table + 8));
    // n > 16. Limit index
    Vec8us index1;
    if constexpr (n == INT_MAX || n > 65536) {
        index1 = index;
    }
    else if constexpr ((n & (n - 1)) == 0) {
        // n is a power of 2, make index modulo n
        index1 = Vec8us(index) & (n - 1);
    }
    else {
        // n is not a power of 2, limit to n-1
        index1 = min(Vec8us(index), n - 1);
    }
    uint16_t ii[8];  index1.store(ii);
    return Vec8s(((int16_t*)table)[ii[0]], ((int16_t*)table)[ii[1]], ((int16_t*)table)[ii[2]], ((int16_t*)table)[ii[3]],
        ((int16_t*)table)[ii[4]], ((int16_t*)table)[ii[5]], ((int16_t*)table)[ii[6]], ((int16_t*)table)[ii[7]]);
    } // n > 0
}


static inline Vec4i lookup4(Vec4i const index, Vec4i const table) {
    return _mm_shuffle_epi8(table, index * 0x04040404 + 0x03020100);
}

static inline Vec4i lookup8(Vec4i const index, Vec4i const table0, Vec4i const table1) {
    // return Vec4i(lookup16(Vec8s(index * 0x20002 + 0x10000), Vec8s(table0), Vec8s(table1)));
    Vec4i r0 = _mm_shuffle_epi8(table0, Vec16c(index * 0x04040404) + Vec16c(Vec4i(0x73727170)));
    Vec4i r1 = _mm_shuffle_epi8(table1, Vec16c(index * 0x04040404 ^ 0x10101010) + Vec16c(Vec4i(0x73727170)));
    return r0 | r1;
}

static inline Vec4i lookup16(Vec4i const index, Vec4i const table0, Vec4i const table1, Vec4i const table2, Vec4i const table3) {
    Vec16c aa = Vec16c(Vec4i(0x73727170));
    Vec4i r0 = _mm_shuffle_epi8(table0, Vec16c((index) * 0x04040404) + aa);
    Vec4i r1 = _mm_shuffle_epi8(table1, Vec16c((index ^ 4) * 0x04040404) + aa);
    Vec4i r2 = _mm_shuffle_epi8(table2, Vec16c((index ^ 8) * 0x04040404) + aa);
    Vec4i r3 = _mm_shuffle_epi8(table3, Vec16c((index ^ 12) * 0x04040404) + aa);
    return (r0 | r1) | (r2 | r3);

}

template <int n>
static inline Vec4i lookup(Vec4i const index, void const * table) {
    if constexpr (n <= 0) return 0;
    else {
    if constexpr (n <= 4) return lookup4(index, Vec4i().load_partial(n < 4 ? n : 4, table));
    if constexpr (n <= 8) return lookup8(index, Vec4i().load_partial(n < 4 ? n : 4, table), Vec4i().load_partial(n - 4, (int32_t const*)table + 4));
    // n > 8. Limit index
    Vec4ui index1;
    if constexpr (n == INT_MAX) {
        index1 = index;
    }
    else if constexpr ((n & (n - 1)) == 0) {
        // n is a power of 2, make index modulo n
        index1 = Vec4ui(index) & (n - 1);
    }
    else {
        // n is not a power of 2, limit to n-1
        index1 = min(Vec4ui(index), n - 1);
    }
    uint32_t ii[4];  index1.store(ii);
    return Vec4i(((int32_t*)table)[ii[0]], ((int32_t*)table)[ii[1]], ((int32_t*)table)[ii[2]], ((int32_t*)table)[ii[3]]);
    } // n > 0
}


static inline Vec2q lookup2(Vec2q const index, Vec2q const table) {
    return _mm_shuffle_epi8(table, index * 0x0808080808080808ll + 0x0706050403020100ll);
}

template <int n>
static inline Vec2q lookup(Vec2q const index, void const * table) {
    if constexpr (n <= 0) return 0;
    else {
    // n > 0. Limit index
    Vec2uq index1;
    if constexpr (n == INT_MAX) {
        index1 = index;
    }
    else if constexpr ((n & (n - 1)) == 0) {
        // n is a power of 2, make index modulo n
        index1 = Vec2uq(index) & (n - 1);
    }
    else {
        // n is not a power of 2, limit to n-1.
        // There is no 64-bit min instruction, but we can use the 32-bit unsigned min,
        // since n is a 32-bit integer
        index1 = Vec2uq(min(Vec2uq(index), constant4ui<n - 1, 0, n - 1, 0>()));
    }
    uint32_t ii[4];  index1.store(ii);  // use only lower 32 bits of each index
    int64_t const * tt = (int64_t const *)table;
    return Vec2q(tt[ii[0]], tt[ii[2]]);
    } // n > 0
}


/*****************************************************************************
*
*          Byte shifts
*
*****************************************************************************/

// Function shift_bytes_up: shift whole vector left by b bytes.
template <unsigned int b>
static inline Vec16c shift_bytes_up(Vec16c const a) {
    if constexpr (b < 16) {
        return _mm_alignr_epi8(a, _mm_setzero_si128(), 16 - b);
    }
    else {
        return _mm_setzero_si128();                       // zero
    }
}

// Function shift_bytes_down: shift whole vector right by b bytes
template <unsigned int b>
static inline Vec16c shift_bytes_down(Vec16c const a) {
    if constexpr (b < 16) {
        return _mm_alignr_epi8(_mm_setzero_si128(), a, b);
    }
    else {
        return _mm_setzero_si128();
    }
}


/*****************************************************************************
*
*          Gather functions with fixed indexes
*
*****************************************************************************/
// find lowest and highest index
template <int N>
constexpr int min_index(const int (&a)[N]) {
    int ix = a[0];
    for (int i = 1; i < N; i++) {
        if (a[i] < ix) ix = a[i];
    }
    return ix;
}

template <int N>
constexpr int max_index(const int (&a)[N]) {
    int ix = a[0];
    for (int i = 1; i < N; i++) {
        if (a[i] > ix) ix = a[i];
    }
    return ix;
}

// Load elements from array a with indices i0, i1, i2, i3
template <int i0, int i1, int i2, int i3>
static inline Vec4i gather4i(void const * a) {
    int constexpr indexs[4] = { i0, i1, i2, i3 }; // indexes as array
    constexpr int imin = min_index(indexs);
    constexpr int imax = max_index(indexs);
    static_assert(imin >= 0, "Negative index in gather function");

    if constexpr (imax - imin <= 3) {
        // load one contiguous block and permute
        if constexpr (imax > 3) {
            // make sure we don't read past the end of the array
            Vec4i b = Vec4i().load((int32_t const *)a + imax - 3);
            return permute4<i0 - imax + 3, i1 - imax + 3, i2 - imax + 3, i3 - imax + 3>(b);
        }
        else {
            Vec4i b = Vec4i().load_partial(imax - imin + 1, (int32_t const *)a + imin);
            return permute4<i0 - imin, i1 - imin, i2 - imin, i3 - imin>(b);
        }
    }
    if constexpr ((i0<imin + 4 || i0>imax - 4) && (i1<imin + 4 || i1>imax - 4) && (i2<imin + 4 || i2>imax - 4) && (i3<imin + 4 || i3>imax - 4)) {
        // load two contiguous blocks and blend
        Vec4i b = Vec4i().load((int32_t const *)a + imin);
        Vec4i c = Vec4i().load((int32_t const *)a + imax - 3);
        constexpr int j0 = i0 < imin + 4 ? i0 - imin : 7 - imax + i0;
        constexpr int j1 = i1 < imin + 4 ? i1 - imin : 7 - imax + i1;
        constexpr int j2 = i2 < imin + 4 ? i2 - imin : 7 - imax + i2;
        constexpr int j3 = i3 < imin + 4 ? i3 - imin : 7 - imax + i3;
        return blend4<j0, j1, j2, j3>(b, c);
    }
    // Use the bounded compatibility lookup.
    return lookup<imax + 1>(Vec4i(i0, i1, i2, i3), a);
}

// Load elements from array a with indices i0, i1
template <int i0, int i1>
static inline Vec2q gather2q(void const * a) {
    constexpr int imin = i0 < i1 ? i0 : i1;
    constexpr int imax = i0 > i1 ? i0 : i1;
    static_assert(imin >= 0, "Negative index in gather function");

    if constexpr (imax - imin <= 1) {
        // load one contiguous block and permute
        if constexpr (imax > 1) {
            // make sure we don't read past the end of the array
            Vec2q b = Vec2q().load((int64_t const *)a + imax - 1);
            return permute2<i0 - imax + 1, i1 - imax + 1>(b);
        }
        else {
            Vec2q b = Vec2q().load_partial(imax - imin + 1, (int64_t const *)a + imin);
            return permute2<i0 - imin, i1 - imin>(b);
        }
    }
    return Vec2q(((int64_t*)a)[i0], ((int64_t*)a)[i1]);
}


/*****************************************************************************
*
*          Vector scatter functions with fixed indexes
*
******************************************************************************
*
* These functions write the elements of a vector to arbitrary positions in an
* array in memory. Each vector element is written to an array position
* determined by an index. An element is not written if the corresponding
* index is out of range.
* The indexes can be specified as constant template parameters or as an
* integer vector.
*
* The scatter functions are useful if the data are distributed in a sparce
* manner into the array. If the array is dense then it is more efficient
* to permute the data into the right positions and then write the whole
* permuted vector into the array.
*
* Example:
* Vec8q a(10,11,12,13,14,15,16,17);
* int64_t b[16] = {0};
* scatter<0,2,14,10,1,-1,5,9>(a,b); // b = (10,14,11,0,0,16,0,0,0,17,13,0,0,0,12,0)
*
*****************************************************************************/

template <int i0, int i1, int i2, int i3>
static inline void scatter(Vec4i const data, void * destination) {
    int32_t * arr = (int32_t*)destination;
    const int index[4] = { i0,i1,i2,i3 };
    for (int i = 0; i < 4; i++) {
        if (index[i] >= 0) arr[index[i]] = data[i];
    }
}

template <int i0, int i1>
static inline void scatter(Vec2q const data, void * destination) {
    int64_t* arr = (int64_t*)destination;
    if (i0 >= 0) arr[i0] = data[0];
    if (i1 >= 0) arr[i1] = data[1];
}


/*****************************************************************************
*
*          Scatter functions with variable indexes
*
*****************************************************************************/

static inline void scatter(Vec4i const index, uint32_t limit, Vec4i const data, void * destination) {
    int32_t* arr = (int32_t*)destination;
    for (int i = 0; i < 4; i++) {
        if (uint32_t(index[i]) < limit) arr[index[i]] = data[i];
    }
}

static inline void scatter(Vec2q const index, uint32_t limit, Vec2q const data, void * destination) {
    int64_t* arr = (int64_t*)destination;
    if (uint64_t(index[0]) < uint64_t(limit)) arr[index[0]] = data[0];
    if (uint64_t(index[1]) < uint64_t(limit)) arr[index[1]] = data[1];
}


/*****************************************************************************
*
*          Functions for conversion between integer sizes
*
*****************************************************************************/

// Extend 8-bit integers to 16-bit integers, signed and unsigned

// Function extend_low : extends the low 8 elements to 16 bits with sign extension
static inline Vec8s extend_low(Vec16c const a) {
    __m128i sign = _mm_cmpgt_epi8(_mm_setzero_si128(), a);  // 0 > a
    return         _mm_unpacklo_epi8(a, sign);              // interleave with sign extensions
}

// Function extend_high : extends the high 8 elements to 16 bits with sign extension
static inline Vec8s extend_high(Vec16c const a) {
    __m128i sign = _mm_cmpgt_epi8(_mm_setzero_si128(), a);  // 0 > a
    return         _mm_unpackhi_epi8(a, sign);              // interleave with sign extensions
}

// Function extend_low : extends the low 8 elements to 16 bits with zero extension
static inline Vec8us extend_low(Vec16uc const a) {
    return    _mm_unpacklo_epi8(a, _mm_setzero_si128());    // interleave with zero extensions
}

// Function extend_high : extends the high 8 elements to 16 bits with zero extension
static inline Vec8us extend_high(Vec16uc const a) {
    return    _mm_unpackhi_epi8(a, _mm_setzero_si128());    // interleave with zero extensions
}

// Extend 16-bit integers to 32-bit integers, signed and unsigned

// Function extend_low : extends the low 4 elements to 32 bits with sign extension
static inline Vec4i extend_low(Vec8s const a) {
    __m128i sign = _mm_srai_epi16(a, 15);                   // sign bit
    return         _mm_unpacklo_epi16(a, sign);             // interleave with sign extensions
}

// Function extend_high : extends the high 4 elements to 32 bits with sign extension
static inline Vec4i extend_high(Vec8s const a) {
    __m128i sign = _mm_srai_epi16(a, 15);                   // sign bit
    return         _mm_unpackhi_epi16(a, sign);             // interleave with sign extensions
}

// Function extend_low : extends the low 4 elements to 32 bits with zero extension
static inline Vec4ui extend_low(Vec8us const a) {
    return    _mm_unpacklo_epi16(a, _mm_setzero_si128());   // interleave with zero extensions
}

// Function extend_high : extends the high 4 elements to 32 bits with zero extension
static inline Vec4ui extend_high(Vec8us const a) {
    return    _mm_unpackhi_epi16(a, _mm_setzero_si128());   // interleave with zero extensions
}

// Extend 32-bit integers to 64-bit integers, signed and unsigned

// Function extend_low : extends the low 2 elements to 64 bits with sign extension
static inline Vec2q extend_low(Vec4i const a) {
    __m128i sign = _mm_srai_epi32(a, 31);                   // sign bit
    return         _mm_unpacklo_epi32(a, sign);             // interleave with sign extensions
}

// Function extend_high : extends the high 2 elements to 64 bits with sign extension
static inline Vec2q extend_high(Vec4i const a) {
    __m128i sign = _mm_srai_epi32(a, 31);                   // sign bit
    return         _mm_unpackhi_epi32(a, sign);             // interleave with sign extensions
}

// Function extend_low : extends the low 2 elements to 64 bits with zero extension
static inline Vec2uq extend_low(Vec4ui const a) {
    return    _mm_unpacklo_epi32(a, _mm_setzero_si128());   // interleave with zero extensions
}

// Function extend_high : extends the high 2 elements to 64 bits with zero extension
static inline Vec2uq extend_high(Vec4ui const a) {
    return    _mm_unpackhi_epi32(a, _mm_setzero_si128());   // interleave with zero extensions
}

// Compress 16-bit integers to 8-bit integers, signed and unsigned, with and without saturation

// Function compress : packs two vectors of 16-bit integers into one vector of 8-bit integers
// Overflow wraps around
static inline Vec16c compress(Vec8s const low, Vec8s const high) {
    __m128i mask = _mm_set1_epi32(0x00FF00FF);            // mask for low bytes
    __m128i lowm = _mm_and_si128(low, mask);               // bytes of low
    __m128i highm = _mm_and_si128(high, mask);              // bytes of high
    return  _mm_packus_epi16(lowm, highm);                  // unsigned pack
}

// Function compress : packs two vectors of 16-bit integers into one vector of 8-bit integers
// Signed, with saturation
static inline Vec16c compress_saturated(Vec8s const low, Vec8s const high) {
    return  _mm_packs_epi16(low, high);
}

// Function compress : packs two vectors of 16-bit integers to one vector of 8-bit integers
// Unsigned, overflow wraps around
static inline Vec16uc compress(Vec8us const low, Vec8us const high) {
    return  Vec16uc(compress((Vec8s)low, (Vec8s)high));
}

// Function compress : packs two vectors of 16-bit integers into one vector of 8-bit integers
// Unsigned, with saturation
static inline Vec16uc compress_saturated(Vec8us const low, Vec8us const high) {
    __m128i maxval = _mm_set1_epi32(0x00FF00FF);           // maximum value
    __m128i low1 = _mm_min_epu16(low, maxval);             // upper limit
    __m128i high1 = _mm_min_epu16(high, maxval);           // upper limit
    return            _mm_packus_epi16(low1, high1);       // this instruction saturates from signed 32 bit to unsigned 16 bit
}

// Compress 32-bit integers to 16-bit integers, signed and unsigned, with and without saturation

// Function compress : packs two vectors of 32-bit integers into one vector of 16-bit integers
// Overflow wraps around
static inline Vec8s compress(Vec4i const low, Vec4i const high) {
    __m128i mask = _mm_set1_epi32(0x0000FFFF);             // mask for low words
    __m128i lowm = _mm_and_si128(low, mask);               // bytes of low
    __m128i highm = _mm_and_si128(high, mask);             // bytes of high
    return  _mm_packus_epi32(lowm, highm);                 // unsigned pack
}

// Function compress : packs two vectors of 32-bit integers into one vector of 16-bit integers
// Signed with saturation
static inline Vec8s compress_saturated(Vec4i const low, Vec4i const high) {
    return  _mm_packs_epi32(low, high);                    // pack with signed saturation
}

// Function compress : packs two vectors of 32-bit integers into one vector of 16-bit integers
// Overflow wraps around
static inline Vec8us compress(Vec4ui const low, Vec4ui const high) {
    return Vec8us(compress((Vec4i)low, (Vec4i)high));
}

// Function compress : packs two vectors of 32-bit integers into one vector of 16-bit integers
// Unsigned, with saturation
static inline Vec8us compress_saturated(Vec4ui const low, Vec4ui const high) {
    __m128i maxval = _mm_set1_epi32(0x0000FFFF);           // maximum value
    __m128i low1 = _mm_min_epu32(low, maxval);             // upper limit
    __m128i high1 = _mm_min_epu32(high, maxval);           // upper limit
    return            _mm_packus_epi32(low1, high1);       // this instruction saturates from signed 32 bit to unsigned 16 bit
}

// Compress 64-bit integers to 32-bit integers, signed and unsigned, with and without saturation

// Function compress : packs two vectors of 64-bit integers into one vector of 32-bit integers
// Overflow wraps around
static inline Vec4i compress(Vec2q const low, Vec2q const high) {
    __m128i low2 = _mm_shuffle_epi32(low, 0xD8);           // low dwords of low  to pos. 0 and 32
    __m128i high2 = _mm_shuffle_epi32(high, 0xD8);         // low dwords of high to pos. 0 and 32
    return  _mm_unpacklo_epi64(low2, high2);               // interleave
}

// Function compress : packs two vectors of 64-bit integers into one vector of 32-bit integers
// Signed, with saturation
// This function is very inefficient unless the SSE4.2 instruction set is supported
static inline Vec4i compress_saturated(Vec2q const low, Vec2q const high) {
    Vec2q maxval = _mm_set_epi32(0, 0x7FFFFFFF, 0, 0x7FFFFFFF);
    Vec2q minval = _mm_set_epi32(-1, 0x80000000, -1, 0x80000000);
    Vec2q low1 = min(low, maxval);
    Vec2q high1 = min(high, maxval);
    Vec2q low2 = max(low1, minval);
    Vec2q high2 = max(high1, minval);
    return compress(low2, high2);
}

// Function compress : packs two vectors of 32-bit integers into one vector of 16-bit integers
// Overflow wraps around
static inline Vec4ui compress(Vec2uq const low, Vec2uq const high) {
    return Vec4ui(compress((Vec2q)low, (Vec2q)high));
}

// Function compress : packs two vectors of 64-bit integers into one vector of 32-bit integers
// Unsigned, with saturation
static inline Vec4ui compress_saturated(Vec2uq const low, Vec2uq const high) {
    __m128i zero = _mm_setzero_si128();
    __m128i lowzero = _mm_cmpeq_epi32(low, zero);          // for each dword is zero
    __m128i highzero = _mm_cmpeq_epi32(high, zero);        // for each dword is zero
    __m128i mone = _mm_set1_epi32(-1);                     // FFFFFFFF
    __m128i lownz = _mm_xor_si128(lowzero, mone);          // for each dword is nonzero
    __m128i highnz = _mm_xor_si128(highzero, mone);        // for each dword is nonzero
    __m128i lownz2 = _mm_srli_epi64(lownz, 32);            // shift down to low dword
    __m128i highnz2 = _mm_srli_epi64(highnz, 32);          // shift down to low dword
    __m128i lowsatur = _mm_or_si128(low, lownz2);          // low, saturated
    __m128i hisatur = _mm_or_si128(high, highnz2);         // high, saturated
    return  Vec4ui(compress(Vec2q(lowsatur), Vec2q(hisatur)));
}



/*****************************************************************************
*
*          Integer division operators
*
******************************************************************************
*
* The instruction set does not support integer vector division. Instead, we
* are using a method for fast integer division based on multiplication and
* shift operations. This method is faster than simple integer division if the
* same divisor is used multiple times.
*
* All elements in a vector are divided by the same divisor. It is not possible
* to divide different elements of the same vector by different divisors.
*
* The parameters used for fast division are stored in an object of a
* Divisor class. This object can be created implicitly, for example in:
*        Vec4i a, b; int c;
*        a = b / c;
* or explicitly as:
*        a = b / Divisor_i(c);
*
* It takes more time to compute the parameters used for fast division than to
* do the division. Therefore, it is advantageous to use the same divisor object
* multiple times. For example, to divide 80 unsigned short integers by 10:
*
*        uint16_t dividends[80], quotients[80];         // numbers to work with
*        Divisor_us div10(10);                          // make divisor object for dividing by 10
*        Vec8us temp;                                   // temporary vector
*        for (int i = 0; i < 80; i += 8) {              // loop for 4 elements per iteration
*            temp.load(dividends+i);                    // load 4 elements
*            temp /= div10;                             // divide each element by 10
*            temp.store(quotients+i);                   // store 4 elements
*        }
*
* The parameters for fast division can also be computed at compile time. This is
* an advantage if the divisor is known at compile time. Use the const_int or const_uint
* macro to do this. For example, for signed integers:
*        Vec8s a, b;
*        a = b / const_int(10);
* Or, for unsigned integers:
*        Vec8us a, b;
*        a = b / const_uint(10);
*
* The division of a vector of 16-bit integers is faster than division of a vector
* of other integer sizes.
*
*
* Mathematical formula, used for signed division with fixed or variable divisor:
* (From T. Granlund and P. L. Montgomery: Division by Invariant Integers Using Multiplication,
* Proceedings of the SIGPLAN 1994 Conference on Programming Language Design and Implementation.
* http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.1.2556 )
* x = dividend
* d = abs(divisor)
* w = integer word size, bits
* L = ceil(log2(d)) = bit_scan_reverse(d-1)+1
* L = max(L,1)
* m = 1 + 2^(w+L-1)/d - 2^w                      [division should overflow to 0 if d = 1]
* sh1 = L-1
* q = x + (m*x >> w)                             [high part of signed multiplication with 2w bits]
* q = (q >> sh1) - (x<0 ? -1 : 0)
* if (divisor < 0) q = -q
* result = trunc(x/d) = q
*
* Mathematical formula, used for unsigned division with variable divisor:
* (Also from T. Granlund and P. L. Montgomery)
* x = dividend
* d = divisor
* w = integer word size, bits
* L = ceil(log2(d)) = bit_scan_reverse(d-1)+1
* m = 1 + 2^w * (2^L-d) / d                      [2^L should overflow to 0 if L = w]
* sh1 = min(L,1)
* sh2 = max(L-1,0)
* t = m*x >> w                                   [high part of unsigned multiplication with 2w bits]
* result = floor(x/d) = (((x-t) >> sh1) + t) >> sh2
*
* Mathematical formula, used for unsigned division with fixed divisor:
* (From Terje Mathisen, unpublished)
* x = dividend
* d = divisor
* w = integer word size, bits
* b = floor(log2(d)) = bit_scan_reverse(d)
* f = 2^(w+b) / d                                [exact division]
* If f is an integer then d is a power of 2 then go to case A
* If the fractional part of f is < 0.5 then go to case B
* If the fractional part of f is > 0.5 then go to case C
* Case A:  [shift only]
* result = x >> b
* Case B:  [round down f and compensate by adding one to x]
* result = ((x+1)*floor(f)) >> (w+b)             [high part of unsigned multiplication with 2w bits]
* Case C:  [round up f, no compensation for rounding error]
* result = (x*ceil(f)) >> (w+b)                  [high part of unsigned multiplication with 2w bits]
*
*
*****************************************************************************/

// encapsulate parameters for fast division on vector of 4 32-bit signed integers
class Divisor_i {
protected:
    __m128i multiplier;                                    // multiplier used in fast division
    __m128i shift1;                                        // shift count used in fast division
    __m128i sign;                                          // sign of divisor
public:
    Divisor_i() = default;                                 // Default constructor
    Divisor_i(int32_t d) {                                 // Constructor with divisor
        set(d);
    }
    Divisor_i(int m, int s1, int sgn) {                    // Constructor with precalculated multiplier, shift and sign
        multiplier = _mm_set1_epi32(m);
        shift1 = _mm_cvtsi32_si128(s1);
        sign = _mm_set1_epi32(sgn);
    }
    void set(int32_t d) {                                  // Set or change divisor, calculate parameters
        int32_t sh, m;
        const uint32_t d1 = d < 0 ? uint32_t(0) - uint32_t(d) : uint32_t(d);
        if (uint32_t(d) == 0x80000000u) {                  // fix overflow for this special case
            m = 0x80000001;
            sh = 30;
        }
        else if (d1 > 1) {
            sh = (int)bit_scan_reverse(uint32_t(d1 - 1));  // shift count = ceil(log2(d1))-1 = (bit_scan_reverse(d1-1)+1)-1
            m = int32_t((int64_t(1) << (32 + sh)) / d1 - ((int64_t(1) << 32) - 1)); // calculate multiplier
        }
        else {
            m = 1;                                         // for d1 = 1
            sh = 0;
            if (d == 0) m /= d;                            // provoke error here if d = 0
        }
        multiplier = _mm_set1_epi32(m);                    // broadcast multiplier
        shift1 = _mm_cvtsi32_si128(sh);                    // shift count
        //sign = _mm_set1_epi32(d < 0 ? -1 : 0);           // bug in VS2019, 32 bit release. Replace by this:
        if (d < 0) sign = _mm_set1_epi32(-1); else sign = _mm_set1_epi32(0);  // sign of divisor
    }
    __m128i getm() const {                                 // get multiplier
        return multiplier;
    }
    __m128i gets1() const {                                // get shift count
        return shift1;
    }
    __m128i getsign() const {                              // get sign of divisor
        return sign;
    }
};

// encapsulate parameters for fast division on vector of 4 32-bit unsigned integers
class Divisor_ui {
protected:
    __m128i multiplier;                                    // multiplier used in fast division
    __m128i shift1;                                        // shift count 1 used in fast division
    __m128i shift2;                                        // shift count 2 used in fast division
public:
    Divisor_ui() = default;                                // Default constructor
    Divisor_ui(uint32_t d) {                               // Constructor with divisor
        set(d);
    }
    Divisor_ui(uint32_t m, int s1, int s2) {               // Constructor with precalculated multiplier and shifts
        multiplier = _mm_set1_epi32((int32_t)m);
        shift1 = _mm_setr_epi32(s1, 0, 0, 0);
        shift2 = _mm_setr_epi32(s2, 0, 0, 0);
    }
    void set(uint32_t d) {                                 // Set or change divisor, calculate parameters
        uint32_t L, L2, sh1, sh2, m;
        switch (d) {
        case 0:
            m = sh1 = sh2 = 1 / d;                         // provoke error for d = 0
            break;
        case 1:
            m = 1; sh1 = sh2 = 0;                          // parameters for d = 1
            break;
        case 2:
            m = 1; sh1 = 1; sh2 = 0;                       // parameters for d = 2
            break;
        default:                                           // general case for d > 2
            L = bit_scan_reverse(d - 1) + 1;               // ceil(log2(d))
            L2 = L < 32 ? uint32_t(1) << L : uint32_t(0);            // 2^L, overflow to 0 if L = 32
            m = 1 + uint32_t((uint64_t(L2 - d) << 32) / d);// multiplier
            sh1 = 1;  sh2 = L - 1;                         // shift counts
        }
        multiplier = _mm_set1_epi32((int32_t)m);
        shift1 = _mm_setr_epi32((int32_t)sh1, 0, 0, 0);
        shift2 = _mm_setr_epi32((int32_t)sh2, 0, 0, 0);
    }
    __m128i getm() const {                                 // get multiplier
        return multiplier;
    }
    __m128i gets1() const {                                // get shift count 1
        return shift1;
    }
    __m128i gets2() const {                                // get shift count 2
        return shift2;
    }
};


// encapsulate parameters for fast division on vector of 8 16-bit signed integers
class Divisor_s {
protected:
    __m128i multiplier;                                    // multiplier used in fast division
    __m128i shift1;                                        // shift count used in fast division
    __m128i sign;                                          // sign of divisor
public:
    Divisor_s() = default;                                 // Default constructor
    Divisor_s(int16_t d) {                                 // Constructor with divisor
        set(d);
    }
    Divisor_s(int16_t m, int s1, int sgn) {                // Constructor with precalculated multiplier, shift and sign
        multiplier = _mm_set1_epi16(m);
        shift1 = _mm_setr_epi32(s1, 0, 0, 0);
        sign = _mm_set1_epi32(sgn);
    }
    void set(int16_t d) {                                  // Set or change divisor, calculate parameters
        const uint32_t d1 = d < 0 ? uint32_t(0) - uint32_t(d) : uint32_t(d);
        int32_t sh, m;
        if (uint16_t(d) == 0x8000u) {                      // fix overflow for this special case
            m = 0x8001;
            sh = 14;
        }
        else if (d1 > 1) {
            sh = (int32_t)bit_scan_reverse(uint32_t(d1-1));// shift count = ceil(log2(d1))-1 = (bit_scan_reverse(d1-1)+1)-1
            m = ((int32_t(1) << (16 + sh)) / d1 - ((int32_t(1) << 16) - 1)); // calculate multiplier
        }
        else {
            m = 1;                                         // for d1 = 1
            sh = 0;
            if (d == 0) m /= d;                            // provoke error here if d = 0
        }
        multiplier = _mm_set1_epi16(int16_t(m));           // broadcast multiplier
        shift1 = _mm_setr_epi32(sh, 0, 0, 0);              // shift count
        sign = _mm_set1_epi32(d < 0 ? -1 : 0);             // sign of divisor
    }
    __m128i getm() const {                                 // get multiplier
        return multiplier;
    }
    __m128i gets1() const {                                // get shift count
        return shift1;
    }
    __m128i getsign() const {                              // get sign of divisor
        return sign;
    }
};


// encapsulate parameters for fast division on vector of 8 16-bit unsigned integers
class Divisor_us {
protected:
    __m128i multiplier;                                    // multiplier used in fast division
    __m128i shift1;                                        // shift count 1 used in fast division
    __m128i shift2;                                        // shift count 2 used in fast division
public:
    Divisor_us() = default;                                // Default constructor
    Divisor_us(uint16_t d) {                               // Constructor with divisor
        set(d);
    }
    Divisor_us(uint16_t m, int s1, int s2) {               // Constructor with precalculated multiplier and shifts
        multiplier = _mm_set1_epi16((int16_t)m);
        shift1 = _mm_setr_epi32(s1, 0, 0, 0);
        shift2 = _mm_setr_epi32(s2, 0, 0, 0);
    }
    void set(uint16_t d) {                                 // Set or change divisor, calculate parameters
        uint16_t L, L2, sh1, sh2, m;
        switch (d) {
        case 0:
            m = sh1 = sh2 = 1u / d;                        // provoke error for d = 0
            break;
        case 1:
            m = 1; sh1 = sh2 = 0;                          // parameters for d = 1
            break;
        case 2:
            m = 1; sh1 = 1; sh2 = 0;                       // parameters for d = 2
            break;
        default:                                           // general case for d > 2
            L = (uint16_t)bit_scan_reverse(d - 1u) + 1u;   // ceil(log2(d))
            L2 = uint16_t(1 << L);                         // 2^L, overflow to 0 if L = 16
            m = 1u + uint16_t((uint32_t(L2 - d) << 16) / d); // multiplier
            sh1 = 1;  sh2 = L - 1u;                        // shift counts
        }
        multiplier = _mm_set1_epi16((int16_t)m);
        shift1 = _mm_setr_epi32((int32_t)sh1, 0, 0, 0);
        shift2 = _mm_setr_epi32((int32_t)sh2, 0, 0, 0);
    }
    __m128i getm() const {                                 // get multiplier
        return multiplier;
    }
    __m128i gets1() const {                                // get shift count 1
        return shift1;
    }
    __m128i gets2() const {                                // get shift count 2
        return shift2;
    }
};


// vector operator / : divide each element by divisor

// vector of 4 32-bit signed integers
static inline Vec4i operator / (Vec4i const a, Divisor_i const d) {
    __m128i t1 = _mm_mul_epi32(a, d.getm());               // 32x32->64 bit signed multiplication of a[0] and a[2]
    __m128i t2 = _mm_srli_epi64(t1, 32);                   // high dword of result 0 and 2
    __m128i t3 = _mm_srli_epi64(a, 32);                    // get a[1] and a[3] into position for multiplication
    __m128i t4 = _mm_mul_epi32(t3, d.getm());              // 32x32->64 bit signed multiplication of a[1] and a[3]
    __m128i t7 = _mm_blend_epi16(t2, t4, 0xCC);
    __m128i t8 = _mm_add_epi32(t7, a);                     // add
    __m128i t9 = _mm_sra_epi32(t8, d.gets1());             // shift right arithmetic
    __m128i t10 = _mm_srai_epi32(a, 31);                   // sign of a
    __m128i t11 = _mm_sub_epi32(t10, d.getsign());         // sign of a - sign of d
    __m128i t12 = _mm_sub_epi32(t9, t11);                  // + 1 if a < 0, -1 if d < 0
    return        _mm_xor_si128(t12, d.getsign());         // change sign if divisor negative
}

// vector of 4 32-bit unsigned integers
static inline Vec4ui operator / (Vec4ui const a, Divisor_ui const d) {
    __m128i t1 = _mm_mul_epu32(a, d.getm());               // 32x32->64 bit unsigned multiplication of a[0] and a[2]
    __m128i t2 = _mm_srli_epi64(t1, 32);                   // high dword of result 0 and 2
    __m128i t3 = _mm_srli_epi64(a, 32);                    // get a[1] and a[3] into position for multiplication
    __m128i t4 = _mm_mul_epu32(t3, d.getm());              // 32x32->64 bit unsigned multiplication of a[1] and a[3]
    __m128i t7 = _mm_blend_epi16(t2, t4, 0xCC);            // blend two results
    __m128i t8 = _mm_sub_epi32(a, t7);                     // subtract
    __m128i t9 = _mm_srl_epi32(t8, d.gets1());             // shift right logical
    __m128i t10 = _mm_add_epi32(t7, t9);                   // add
    return        _mm_srl_epi32(t10, d.gets2());           // shift right logical
}

// vector of 8 16-bit signed integers
static inline Vec8s operator / (Vec8s const a, Divisor_s const d) {
    __m128i t1 = _mm_mulhi_epi16(a, d.getm());             // multiply high signed words
    __m128i t2 = _mm_add_epi16(t1, a);                     // + a
    __m128i t3 = _mm_sra_epi16(t2, d.gets1());             // shift right arithmetic
    __m128i t4 = _mm_srai_epi16(a, 15);                    // sign of a
    __m128i t5 = _mm_sub_epi16(t4, d.getsign());           // sign of a - sign of d
    __m128i t6 = _mm_sub_epi16(t3, t5);                    // + 1 if a < 0, -1 if d < 0
    return        _mm_xor_si128(t6, d.getsign());          // change sign if divisor negative
}

// vector of 8 16-bit unsigned integers
static inline Vec8us operator / (Vec8us const a, Divisor_us const d) {
    __m128i t1 = _mm_mulhi_epu16(a, d.getm());             // multiply high unsigned words
    __m128i t2 = _mm_sub_epi16(a, t1);                     // subtract
    __m128i t3 = _mm_srl_epi16(t2, d.gets1());             // shift right logical
    __m128i t4 = _mm_add_epi16(t1, t3);                    // add
    return        _mm_srl_epi16(t4, d.gets2());            // shift right logical
}


// vector of 16 8-bit signed integers
static inline Vec16c operator / (Vec16c const a, Divisor_s const d) {
    // expand into two Vec8s
    Vec8s low = extend_low(a) / d;
    Vec8s high = extend_high(a) / d;
    return compress(low, high);
}

// vector of 16 8-bit unsigned integers
static inline Vec16uc operator / (Vec16uc const a, Divisor_us const d) {
    // expand into two Vec8s
    Vec8us low = extend_low(a) / d;
    Vec8us high = extend_high(a) / d;
    return compress(low, high);
}

// vector operator /= : divide
static inline Vec8s & operator /= (Vec8s & a, Divisor_s const d) {
    a = a / d;
    return a;
}

// vector operator /= : divide
static inline Vec8us & operator /= (Vec8us & a, Divisor_us const d) {
    a = a / d;
    return a;
}

// vector operator /= : divide
static inline Vec4i & operator /= (Vec4i & a, Divisor_i const d) {
    a = a / d;
    return a;
}

// vector operator /= : divide
static inline Vec4ui & operator /= (Vec4ui & a, Divisor_ui const d) {
    a = a / d;
    return a;
}

// vector operator /= : divide
static inline Vec16c & operator /= (Vec16c & a, Divisor_s const d) {
    a = a / d;
    return a;
}

// vector operator /= : divide
static inline Vec16uc & operator /= (Vec16uc & a, Divisor_us const d) {
    a = a / d;
    return a;
}

/*****************************************************************************
*
*          Integer division 2: divisor is a compile-time constant
*
*****************************************************************************/

// Divide Vec4i by compile-time constant
template <int32_t d>
static inline Vec4i divide_by_i(Vec4i const x) {
    static_assert(d != 0, "Integer division by zero");     // Error message if dividing by zero
    if constexpr (d == 1) return  x;
    if constexpr (d == -1) return -x;
    if constexpr (uint32_t(d) == 0x80000000u) return select(x == Vec4i(0x80000000), Vec4i(1), Vec4i(0)); // prevent overflow when changing sign
    constexpr uint32_t d1 = d > 0 ? uint32_t(d) : uint32_t(0) - uint32_t(d);// compile-time abs(d). (force GCC compiler to treat d as 32 bits, not 64 bits)
    if constexpr ((d1 & (d1 - 1)) == 0) {
        // d1 is a power of 2. use shift
        constexpr int k = bit_scan_reverse_const(d1);
        __m128i sign;
        if constexpr (k > 1) sign = _mm_srai_epi32(x, k-1); else sign = x;// k copies of sign bit
        __m128i bias = _mm_srli_epi32(sign, 32 - k);       // bias = x >= 0 ? 0 : k-1
        __m128i xpbias = _mm_add_epi32(x, bias);           // x + bias
        __m128i q = _mm_srai_epi32(xpbias, k);             // (x + bias) >> k
        if (d > 0)      return q;                          // d > 0: return  q
        return _mm_sub_epi32(_mm_setzero_si128(), q);      // d < 0: return -q
    }
    // general case
    constexpr int32_t sh = bit_scan_reverse_const(uint32_t(d1) - 1); // ceil(log2(d1)) - 1. (d1 < 2 handled by power of 2 case)
    constexpr int32_t mult = int(1 + (uint64_t(1) << (32 + sh)) / uint32_t(d1) - (int64_t(1) << 32));   // multiplier
    const Divisor_i div(mult, sh, d < 0 ? -1 : 0);
    return x / div;
}

// define Vec4i a / const_int(d)
template <int32_t d>
static inline Vec4i operator / (Vec4i const a, NAMESPACEPREFIX Const_int_t<d>) {
    return divide_by_i<d>(a);
}

// define Vec4i a / const_uint(d)
template <uint32_t d>
static inline Vec4i operator / (Vec4i const a, Const_uint_t<d>) {
    static_assert(d < 0x80000000u, "Dividing signed by overflowing unsigned");
    return divide_by_i<int32_t(d)>(a);                     // signed divide
}

// vector operator /= : divide
template <int32_t d>
static inline Vec4i & operator /= (Vec4i & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}

// vector operator /= : divide
template <uint32_t d>
static inline Vec4i & operator /= (Vec4i & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}


// Divide Vec4ui by compile-time constant
template <uint32_t d>
static inline Vec4ui divide_by_ui(Vec4ui const x) {
    static_assert(d != 0, "Integer division by zero");     // Error message if dividing by zero
    if constexpr (d == 1) return x;                        // divide by 1
    constexpr int b = bit_scan_reverse_const(d);           // floor(log2(d))
    if constexpr ((uint32_t(d) & (uint32_t(d) - 1)) == 0) {
        // d is a power of 2. use shift
        return    _mm_srli_epi32(x, b);                    // x >> b
    }
    // general case (d > 2)
    constexpr uint32_t mult = uint32_t((uint64_t(1) << (b+32)) / d); // multiplier = 2^(32+b) / d
    constexpr uint64_t rem = (uint64_t(1) << (b+32)) - uint64_t(d)*mult; // remainder 2^(32+b) % d
    constexpr bool round_down = (2 * rem < d);             // check if fraction is less than 0.5
    constexpr uint32_t mult1 = round_down ? mult : mult + 1;
    // do 32*32->64 bit unsigned multiplication and get high part of result
    const __m128i multv = Vec2uq(uint64_t(mult1));         // zero-extend mult and broadcast
    __m128i t1 = _mm_mul_epu32(x, multv);                  // 32x32->64 bit unsigned multiplication of x[0] and x[2]
    if constexpr (round_down) {
        t1 = _mm_add_epi64(t1, multv);                     // compensate for rounding error. (x+1)*m replaced by x*m+m to avoid overflow
    }
    __m128i t2 = _mm_srli_epi64(t1, 32);                   // high dword of result 0 and 2
    __m128i t3 = _mm_srli_epi64(x, 32);                    // get x[1] and x[3] into position for multiplication
    __m128i t4 = _mm_mul_epu32(t3, multv);                 // 32x32->64 bit unsigned multiplication of x[1] and x[3]
    if constexpr (round_down) {
        t4 = _mm_add_epi64(t4, multv);                     // compensate for rounding error. (x+1)*m replaced by x*m+m to avoid overflow
    }
    __m128i t7 = _mm_blend_epi16(t2, t4, 0xCC);            // blend two results
    Vec4ui q = _mm_srli_epi32(t7, b);                      // shift right by b
    return q;                                              // no overflow possible
}

// define Vec4ui a / const_uint(d)
template <uint32_t d>
static inline Vec4ui operator / (Vec4ui const a, Const_uint_t<d>) {
    return divide_by_ui<d>(a);
}

// define Vec4ui a / const_int(d)
template <int32_t d>
static inline Vec4ui operator / (Vec4ui const a, Const_int_t<d>) {
    static_assert(d < 0x80000000u, "Dividing unsigned integer by negative value is ambiguous");
    return divide_by_ui<d>(a);                             // unsigned divide
}

// vector operator /= : divide
template <uint32_t d>
static inline Vec4ui & operator /= (Vec4ui & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}

// vector operator /= : divide
template <int32_t d>
static inline Vec4ui & operator /= (Vec4ui & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}


// Divide Vec8s by compile-time constant
template <int d>
static inline Vec8s divide_by_i(Vec8s const x) {
    constexpr int16_t d0 = int16_t(d);                     // truncate d to 16 bits
    static_assert(d0 != 0, "Integer division by zero");    // Error message if dividing by zero
    if constexpr (d0 == 1) return  x;                      // divide by  1
    if constexpr (d0 == -1) return -x;                     // divide by -1
    if constexpr (uint16_t(d0) == 0x8000u) return select(x == Vec8s(0x8000), Vec8s(1), Vec8s(0));// prevent overflow when changing sign
    // if (d > 0x7FFF || d < -0x8000) return 0;            // not relevant when d truncated to 16 bits
    const uint32_t d1 = d0 > 0 ? uint32_t(d0) : uint32_t(0) - uint32_t(d0);// compile-time abs(d). (force GCC compiler to treat d as 32 bits, not 64 bits)
    if constexpr ((d1 & (d1 - 1)) == 0) {
        // d is a power of 2. use shift
        constexpr int k = bit_scan_reverse_const(uint32_t(d1));
        __m128i sign;
        if constexpr (k > 1) sign = _mm_srai_epi16(x, k-1); else sign = x;// k copies of sign bit
        __m128i bias = _mm_srli_epi16(sign, 16 - k);       // bias = x >= 0 ? 0 : k-1
        __m128i xpbias = _mm_add_epi16(x, bias);           // x + bias
        __m128i q = _mm_srai_epi16(xpbias, k);             // (x + bias) >> k
        if (d0 > 0)  return q;                             // d0 > 0: return  q
        return _mm_sub_epi16(_mm_setzero_si128(), q);      // d0 < 0: return -q
    }
    // general case
    const int L = bit_scan_reverse_const(uint16_t(d1 - 1)) + 1;// ceil(log2(d)). (d < 2 handled above)
    const int16_t mult = int16_t(1 + (1u << (15 + L)) / uint32_t(d1) - 0x10000);// multiplier
    const int shift1 = L - 1;
    const Divisor_s div(mult, shift1, d0 > 0 ? 0 : -1);
    return x / div;
}

// define Vec8s a / const_int(d)
template <int d>
static inline Vec8s operator / (Vec8s const a, Const_int_t<d>) {
    return divide_by_i<d>(a);
}

// define Vec8s a / const_uint(d)
template <uint32_t d>
static inline Vec8s operator / (Vec8s const a, Const_uint_t<d>) {
    static_assert(d < 0x8000u, "Dividing signed by overflowing unsigned");
    return divide_by_i<int(d)>(a);                         // signed divide
}

// vector operator /= : divide
template <int32_t d>
static inline Vec8s & operator /= (Vec8s & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}

// vector operator /= : divide
template <uint32_t d>
static inline Vec8s & operator /= (Vec8s & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}


// Divide Vec8us by compile-time constant
template <uint32_t d>
static inline Vec8us divide_by_ui(Vec8us const x) {
    constexpr uint16_t d0 = uint16_t(d);                   // truncate d to 16 bits
    static_assert(d0 != 0, "Integer division by zero");    // Error message if dividing by zero
    if constexpr (d0 == 1) return x;                       // divide by 1
    constexpr int b = bit_scan_reverse_const(d0);          // floor(log2(d))
    if constexpr ((d0 & (d0 - 1u)) == 0) {
        // d is a power of 2. use shift
        return  _mm_srli_epi16(x, b);                      // x >> b
    }
    // general case (d > 2)
    constexpr uint16_t mult = uint16_t((1u << uint32_t(b+16)) / d0); // multiplier = 2^(32+b) / d
    constexpr uint32_t rem = (uint32_t(1) << uint32_t(b + 16)) - uint32_t(d0) * mult;// remainder 2^(32+b) % d
    constexpr bool round_down = (2u * rem < d0);           // check if fraction is less than 0.5
    Vec8us x1 = x;
    if (round_down) {
        x1 = x1 + 1u;                                      // round down mult and compensate by adding 1 to x
    }
    constexpr uint16_t mult1 = round_down ? mult : mult + 1;
    const __m128i multv = _mm_set1_epi16((int16_t)mult1);  // broadcast mult
    __m128i xm = _mm_mulhi_epu16(x1, multv);               // high part of 16x16->32 bit unsigned multiplication
    Vec8us q = _mm_srli_epi16(xm, (int)b);                 // shift right by b
    if constexpr (round_down) {
        Vec8sb overfl = (x1 == (Vec8us)_mm_setzero_si128());// check for overflow of x+1
        return select(overfl, Vec8us(uint16_t(mult1 >> (uint16_t)b)), q); // deal with overflow (rarely needed)
    }
    else {
        return q;                                          // no overflow possible
    }
}

// define Vec8us a / const_uint(d)
template <uint32_t d>
static inline Vec8us operator / (Vec8us const a, Const_uint_t<d>) {
    return divide_by_ui<d>(a);
}

// define Vec8us a / const_int(d)
template <int d>
static inline Vec8us operator / (Vec8us const a, Const_int_t<d>) {
    static_assert(d >= 0, "Dividing unsigned integer by negative is ambiguous");
    return divide_by_ui<d>(a);                             // unsigned divide
}

// vector operator /= : divide
template <uint32_t d>
static inline Vec8us & operator /= (Vec8us & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}

// vector operator /= : divide
template <int32_t d>
static inline Vec8us & operator /= (Vec8us & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}


// define Vec16c a / const_int(d)
template <int d>
static inline Vec16c operator / (Vec16c const a, Const_int_t<d>) {
    // expand into two Vec8s
    Vec8s low = extend_low(a) / Const_int_t<d>();
    Vec8s high = extend_high(a) / Const_int_t<d>();
    return compress(low, high);
}

// define Vec16c a / const_uint(d)
template <uint32_t d>
static inline Vec16c operator / (Vec16c const a, Const_uint_t<d>) {
    static_assert (uint8_t(d) < 0x80u, "Dividing signed integer by overflowing unsigned");
    return a / Const_int_t<d>();                           // signed divide
}

// vector operator /= : divide
template <int32_t d>
static inline Vec16c & operator /= (Vec16c & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}
// vector operator /= : divide
template <uint32_t d>
static inline Vec16c & operator /= (Vec16c & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}

// define Vec16uc a / const_uint(d)
template <uint32_t d>
static inline Vec16uc operator / (Vec16uc const a, Const_uint_t<d>) {
    // expand into two Vec8usc
    Vec8us low = extend_low(a) / Const_uint_t<d>();
    Vec8us high = extend_high(a) / Const_uint_t<d>();
    return compress(low, high);
}

// define Vec16uc a / const_int(d)
template <int d>
static inline Vec16uc operator / (Vec16uc const a, Const_int_t<d>) {
    static_assert (int8_t(d) >= 0, "Dividing unsigned integer by negative is ambiguous");
    return a / Const_uint_t<d>();                          // unsigned divide
}

// vector operator /= : divide
template <uint32_t d>
static inline Vec16uc & operator /= (Vec16uc & a, Const_uint_t<d> b) {
    a = a / b;
    return a;
}

// vector operator /= : divide
template <int32_t d>
static inline Vec16uc & operator /= (Vec16uc & a, Const_int_t<d> b) {
    a = a / b;
    return a;
}


/*****************************************************************************
*
*          Boolean <-> bitfield conversion functions
*
*****************************************************************************/



// to_bits: convert boolean vector to integer bitfield
static inline uint16_t to_bits(Vec16cb const x) {
    return (uint16_t)_mm_movemask_epi8(x);
}

// to_bits: convert boolean vector to integer bitfield
static inline uint8_t to_bits(Vec8sb const x) {
    __m128i a = _mm_packs_epi16(x, x);  // 16-bit words to bytes
    return (uint8_t)_mm_movemask_epi8(a);
}

// to_bits: convert boolean vector to integer bitfield
static inline uint8_t to_bits(Vec4ib const x) {
    int r = _mm_movemask_ps(_mm_castsi128_ps(x));
    return r & 0x0F;
}

// to_bits: convert boolean vector to integer bitfield
static inline uint8_t to_bits(Vec2qb const x) {
    int r = _mm_movemask_pd(_mm_castsi128_pd(x));
    return r & 3;
}


#ifdef VCL_NAMESPACE
}
#endif

#endif  // VECTORI128_COMPAT_NEON_H
