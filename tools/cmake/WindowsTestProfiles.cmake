# Copyright 2026 DigitalExpressions Sweden
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

# Explicit test policy; the interface library never exports these options.
set(VCL_TEST_PROFILE "strict-validation" CACHE STRING "Test compiler policy")
set_property(CACHE VCL_TEST_PROFILE PROPERTY STRINGS
  upstream-optimized strict-validation strict-ubsan rtc-diagnostics)
if(NOT VCL_TEST_PROFILE MATCHES "^(upstream-optimized|strict-validation|strict-ubsan|rtc-diagnostics)$")
  message(FATAL_ERROR "Unknown VCL_TEST_PROFILE: ${VCL_TEST_PROFILE}")
endif()
if(VCL_TEST_PROFILE STREQUAL "rtc-diagnostics")
  message(FATAL_ERROR "RTC diagnostics are deferred: noninteractive reporting is not implemented")
endif()
if(NOT VCL_TEST_PROFILE STREQUAL "strict-validation")
  if(NOT WIN32 OR VCL_TARGET_ARCH STREQUAL "arm64" OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This test profile requires native Windows x64")
  endif()
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(MSVC|Clang)$" OR
     (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    message(FATAL_ERROR "Use MSVC or GNU-driver clang++ for Windows test profiles")
  endif()
endif()
if(VCL_TEST_PROFILE STREQUAL "upstream-optimized")
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_CONFIGURATION_TYPES OR VCL_SANITIZE)
    message(FATAL_ERROR "upstream-optimized requires single-config Release without sanitizers")
  endif()
  string(TOLOWER "${CMAKE_CXX_FLAGS}" vcl_global_flags)
  if(vcl_global_flags MATCHES "[-/](rtc|od|o[0-3szg]|fp:|ffast-math|ofast|ffinite-math-only|ffp-contract|frounding-math)")
    message(FATAL_ERROR "Conflicting global compiler policy; use profile options, not CMAKE_CXX_FLAGS")
  endif()
  # CMake initializes Clang Release with O3. Select O2 once in this directory
  # and its children, without forcing a saved cache or consumer's parent scope.
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    string(REGEX REPLACE "(^| )-O[0-3szg]( |$)" " " CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(STRIP "${CMAKE_CXX_FLAGS_RELEASE}" CMAKE_CXX_FLAGS_RELEASE)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O2")
  endif()
elseif(VCL_TEST_PROFILE STREQUAL "strict-ubsan")
  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR NOT CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT VCL_SANITIZE)
    message(FATAL_ERROR "strict-ubsan requires GNU-driver Clang Debug with VCL_SANITIZE=ON")
  endif()
  if(NOT CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded")
    message(FATAL_ERROR "Windows strict UBSan requires the verified MultiThreaded release CRT")
  endif()
endif()
if(WIN32 AND VCL_SANITIZE AND VCL_TEST_PROFILE STREQUAL "strict-validation")
  message(FATAL_ERROR "Select strict-ubsan explicitly for Windows sanitizer execution")
endif()
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND VCL_BUILD_TESTS AND
   (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_CONFIGURATION_TYPES))
  message(FATAL_ERROR "MSVC RTC test execution is deferred; use Release or noninteractive diagnostics after implementation")
endif()
