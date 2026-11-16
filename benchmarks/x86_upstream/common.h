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
#pragma once
#include <cstddef>
#include <cstdint>

constexpr int capacity = 16384;
constexpr int recurrence_steps = 4096;
struct Inputs {
    alignas(64) float f[capacity], fg[capacity], fh[capacity];
    alignas(64) double d[capacity], dg[capacity], dh[capacity];
    alignas(64) int32_t i[256];
};
struct Outputs {
    alignas(64) float f[capacity];
    alignas(64) double d[capacity];
    alignas(64) int32_t i[capacity];
};
using Kernel = void (*)(Inputs const*, Outputs*);
struct Case {
    char const* name;
    char const* operation;
    char const* type;
    int bits, chains, count;
    Kernel kernel;
};
Case const* cases(size_t& count);
