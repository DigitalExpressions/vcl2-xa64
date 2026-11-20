# Copyright 2026 DigitalExpressions Sweden
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)
project(VCLSimdCapture LANGUAGES CXX)
set(VCL_CAPTURE_ISA sse2 CACHE STRING "sse2, avx2, avx512 or neon")
set(VCL_CAPTURE_MAX_WIDTH 512 CACHE STRING "Logical maximum: 128, 256, 512")
if(NOT VCL_CAPTURE_MAX_WIDTH MATCHES "^(128|256|512)$")
  message(FATAL_ERROR "Unsupported logical width")
endif()
if(NOT VCL_CAPTURE_ISA MATCHES "^(sse2|avx2|avx512|neon)$")
  message(FATAL_ERROR "Unsupported capture ISA")
endif()
set(VCL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(VCL_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
add_subdirectory(../.. library)
add_executable(simd_capture capture.cpp)
target_link_libraries(simd_capture PRIVATE vcl::vcl)
if(NOT VCL_BACKEND STREQUAL "3")
  target_compile_definitions(simd_capture PRIVATE MAX_VECTOR_SIZE=${VCL_CAPTURE_MAX_WIDTH})
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(simd_capture PRIVATE /fp:strict)
  if(VCL_CAPTURE_ISA STREQUAL "avx2")
    target_compile_options(simd_capture PRIVATE /arch:AVX2)
  elseif(VCL_CAPTURE_ISA STREQUAL "avx512")
    target_compile_options(simd_capture PRIVATE /arch:AVX512)
  endif()
else()
  target_compile_options(simd_capture PRIVATE -ffp-contract=off -frounding-math)
  if(VCL_CAPTURE_ISA STREQUAL "avx2")
    target_compile_options(simd_capture PRIVATE -mavx2 -mfma)
  elseif(VCL_CAPTURE_ISA STREQUAL "avx512")
    target_compile_options(simd_capture PRIVATE -mavx512f -mavx512vl -mavx512bw -mavx512dq -mfma)
  elseif(VCL_CAPTURE_ISA STREQUAL "sse2")
    target_compile_options(simd_capture PRIVATE -msse2)
  else()
    target_compile_options(simd_capture PRIVATE -mcpu=apple-m1)
  endif()
endif()
if(VCL_SANITIZE)
  target_compile_options(simd_capture PRIVATE -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)
  target_link_options(simd_capture PRIVATE -fsanitize=undefined)
endif()
