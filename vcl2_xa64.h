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

// Compile-time platform selection only; operations remain direct inline VCL APIs.
#ifndef VCL2_XA64_H
#define VCL2_XA64_H
#include "vcl_config.h"
#if VCL_ARCH_ARM64
#ifdef VECTORCLASS_H
#error Original x86 headers cannot be mixed with the ARM family
#endif
#include "vectorclass_neon.h"
#include "vectormath_exp_neon.h"
#include "vectormath_trig_neon.h"
#include "vectormath_hyp_neon.h"
#else
#ifdef VECTORCLASS_NEON_H
#error ARM headers cannot be mixed with original x86 headers
#endif
#include "vectorclass.h"
#include "vectormath_exp.h"
#include "vectormath_trig.h"
#include "vectormath_hyp.h"
#endif
#endif
