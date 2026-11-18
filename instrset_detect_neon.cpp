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

#include "vcl2_xa64.h"
#ifdef VCL_NAMESPACE
namespace VCL_NAMESPACE {
#endif
// Legacy detector reports x86 machine capability, not translated SSE support.
int instrset_detect(void) { return 0; }
bool hasFMA3(void) { return false; }
bool hasFMA4(void) { return false; }
bool hasXOP(void) { return false; }
bool hasAVX512ER(void) { return false; }
bool hasAVX512VBMI(void) { return false; }
bool hasAVX512VBMI2(void) { return false; }
bool hasF16C(void) { return false; }
bool hasAVX512FP16(void) { return false; }
#ifdef VCL_NAMESPACE
}
#endif
