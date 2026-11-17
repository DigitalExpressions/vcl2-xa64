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

#ifndef VCL_TEST_TRUNCATE_I64_REGRESSION_H
#define VCL_TEST_TRUNCATE_I64_REGRESSION_H

#include "vectorclass.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace vcl_test {
#ifdef VCL_NAMESPACE
using namespace VCL_NAMESPACE;
#endif

// Literal IEEE-754 inputs and independently specified integer results. Avoid
// a scalar FP-to-integer cast in the oracle, including on Windows (64-bit
// long double is not a wider reference). Invalid conversions use INT64_MIN,
// matching native CVTTPD2QQ and the existing 128-bit adaptation contract.
template<class V> int truncate_i64_regression() {
    struct Case { uint64_t raw; int64_t expected; };
    const Case cases[] = {
        {0x0000000000000000ull, 0}, {0x8000000000000000ull, 0},
        {0x0000000000000001ull, 0}, {0x8000000000000001ull, 0},
        {0x3ffc000000000000ull, 1}, {0xbffc000000000000ull, -1},
        {0x4004000000000000ull, 2}, {0xc004000000000000ull, -2},
        {0x43dfffffffffffffull, INT64_C(9223372036854774784)},
        {0x43e0000000000000ull, INT64_MIN},
        {0x43e0000000000001ull, INT64_MIN},
        {0xc3dfffffffffffffull, -INT64_C(9223372036854774784)},
        {0xc3e0000000000000ull, INT64_MIN},
        {0xc3e0000000000001ull, INT64_MIN},
        {0x7fefffffffffffffull, INT64_MIN},
        {0xffefffffffffffffull, INT64_MIN},
        {0x7ff0000000000000ull, INT64_MIN},
        {0xfff0000000000000ull, INT64_MIN},
        {0x7ff8000000001234ull, INT64_MIN},
        {0xfff8000000001234ull, INT64_MIN},
        {0x7ff0000000000001ull, INT64_MIN},
        {0xfff0000000000001ull, INT64_MIN}
    };
    constexpr int count = int(sizeof(cases) / sizeof(cases[0]));
    const unsigned saved = get_control_word();
    int failures = 0;
    // Rotate heterogeneous inputs through every lane, under all four rounding
    // modes: truncation must stay toward zero regardless of the FP environment.
    for (unsigned mode = 0; mode < 4; ++mode) {
        set_control_word(0x1f80u | (mode << 13));
        for (int start = 0; start < count; ++start) {
            double input[V::size()];
            int64_t output[V::size()];
            for (int lane = 0; lane < V::size(); ++lane) {
                const uint64_t raw = cases[(start + lane) % count].raw;
                std::memcpy(&input[lane], &raw, sizeof(raw));
            }
            V value; value.load(input);
            truncatei(value).store(output);
            for (int lane = 0; lane < V::size(); ++lane) {
                const Case &test = cases[(start + lane) % count];
                if (output[lane] != test.expected) {
                    if (failures < 8)
                        std::fprintf(stderr,
                            "truncate_i64 lanes=%d mode=%u lane=%d raw=%016llx expected=%lld actual=%lld\n",
                            V::size(), mode, lane,
                            static_cast<unsigned long long>(test.raw),
                            static_cast<long long>(test.expected),
                            static_cast<long long>(output[lane]));
                    ++failures;
                }
            }
        }
    }
    set_control_word(saved);
    return failures;
}
} // namespace vcl_test
#endif
