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

#ifndef VCL_TEST_SCALAR_BITS_H
#define VCL_TEST_SCALAR_BITS_H
#include <cstdint>
namespace vcl_test {
// Independent scalar references, including zero; no ISA-specific intrinsics.
inline int scalar_popcount(uint64_t bits) {
    int count=0;
    while(bits) { count+=int(bits&1); bits>>=1; }
    return count;
}
inline int scalar_first_bit(uint64_t bits) {
    if(!bits) return -1;
    int first=0;
    while(!(bits&1)) { ++first; bits>>=1; }
    return first;
}
}
#endif
