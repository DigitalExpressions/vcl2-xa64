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

#ifndef VECTORCLASS_NEON_H
#define VECTORCLASS_NEON_H 20204
#include "vcl_config.h"
#if !VCL_ARCH_ARM64
#error ARM headers require an ARM64 compiler target
#endif
#if VCL_BACKEND == VCL_BACKEND_NEON
#include "vectori128_neon.h"
#include "vectorf128_neon.h"
#else
#include "instrset_compat_neon.h"
#include "vectori128_compat_neon.h"
#include "vectorf128_compat_neon.h"
#endif
#if MAX_VECTOR_SIZE >= 256
#include "vectori256e_neon.h"
#include "vectorf256e_neon.h"
#endif
#if MAX_VECTOR_SIZE >= 512
#include "vectori512e_neon.h"
#include "vectorf512e_neon.h"
#include "vectori512se_neon.h"
#endif
#include "vector_convert_neon.h"
#endif
