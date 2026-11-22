# VCL2 XA64 - (x86/ARM64 Neon)

This is DigitalExpressions’ fork of Agner Fog’s **Vector Class Library 2**,
with ARM64 support alongside the existing x86 backend. It provides the familiar
VCL C++ vector types and operators for application-independent SIMD code.

**Application code using the supported VCL API is x86/ARM64 agnostic:** include
`vcl2_xa64.h` and use the same types, functions and source code on both
architectures. The library selects the appropriate backend at compile time,
without architecture-specific branches in your application code.

ARM64 offers two backends: **sse2neon compatibility**, selected by default, and
**Direct Neon**, an explicitly enabled experimental implementation. Both execute
native ARM64 instructions. Logical 128/256/512-bit vectors use one/two/four
128-bit Neon registers. The existing x86 backend selects instructions according
to compiler target flags.

The tested native surface includes the portable integer/FP32/FP64 core, masks,
movement, conversions and the documented in-tree numerical functions.
[Backend support](docs/backends.md) and [numerical contracts](docs/numerical-contracts.md)
define the limits. Direct Neon remains experimental. The
[validation index](docs/validation/README.md) distinguishes native Mac M2 and
AMD Windows evidence from emulation and untested platforms/toolchains.

### Acknowledgement:
**ALL** credit for the original x86 Vector Class Library belongs to **Agner Fog**.
We are grateful for the expertise and dedication behind his work, which provides
the foundation for this ARM64 port.

We also thank the [sse2neon team](https://github.com/DLTcollab/sse2neon) and its
contributors. This fork uses their library to translate SSE intrinsics into
ARM Neon instructions in the default compatibility backend. Direct Neon implements
the supported VCL API directly with ARM Neon intrinsics, enabling ARM-specific
optimizations that can improve performance over the sse2neon compatibility backend.

## VCL2 x86 implementation

**The x86 backend uses Agner Fog's unchanged VCL 2.02.04 implementation**,
byte-for-byte identical to the [pinned upstream source](audit/vectorclass-2.02.04.json).
Upstream: [Agner Fog's Vector Class Library](https://github.com/vectorclass/version2),
revision [`667577495efaa36a10cbb5d6cb8c942c23913848`](https://github.com/vectorclass/version2/commit/667577495efaa36a10cbb5d6cb8c942c23913848).
ARM64 implementations are maintained separately and do not modify the original
x86 code. Repository integrity checks verify all 28 protected original files;
see [backend ownership](docs/backend-layout.md#protected-originals).

## Direct Neon vs sse2neon

### Performance

Direct Neon shows its strongest gains in **FP32 batch processing**, with matching
components stored in separate arrays (SoA). The
[Apple M2 / Clang 21 / SDK 26.5 measurements](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
from 22 November 2026 show these throughput ratios against sse2neon:

Each range in the batch table spans logical vector widths of 128/256/512 bits
and arrays of 128 or 4,096 objects.

| Winner | Meaning | Direct Neon / sse2neon ratio |
| --- | --- | --- |
| 🟣 <span style="color: #6f42c1;">**Direct Neon wins**</span> | Beyond the 5% margin | Above 1.05× |
| 🔵 <span style="color: #0969da;">**Within 5%**</span> | Approximately equal throughput | 0.95–1.05×, inclusive |
| 🟠 <span style="color: #bc4c00;">**sse2neon wins**</span> | Beyond the 5% margin | Below 0.95× |

Markers and colors describe the measured throughput ratio, not statistical
significance.

<br>

| FP32 batch operation throughput | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span> |
| --- | ---: |
| 4D dot product | 🔵 **<span style="color: #0969da;">1.01×</span>**–🟣 **<span style="color: #6f42c1;">1.51×</span>** |
| 3D cross product | 🔵 **<span style="color: #0969da;">1.03×</span>**–🟣 **<span style="color: #6f42c1;">1.19×</span>** |
| Vector length | 🟣 **<span style="color: #6f42c1;">1.66×</span>**–🟣 **<span style="color: #6f42c1;">2.01×</span>** |
| Vector normalization | 🟣 **<span style="color: #6f42c1;">1.40×</span>**–🟣 **<span style="color: #6f42c1;">1.70×</span>** |
| 4×4 matrix multiplication | 🟣 **<span style="color: #6f42c1;">1.07×</span>**–🟣 **<span style="color: #6f42c1;">1.24×</span>** |
| 4×4 matrix–vector multiplication | 🔵 **<span style="color: #0969da;">1.03×</span>**–🟣 **<span style="color: #6f42c1;">1.66×</span>** |
| 4×4 matrix transpose | 🟠 **<span style="color: #bc4c00;">0.91×</span>**–🟣 **<span style="color: #6f42c1;">1.43×</span>** |
| TRS matrix inverse | 🔵 **<span style="color: #0969da;">1.04×</span>**–🟣 **<span style="color: #6f42c1;">1.18×</span>** |

Endpoint colors show their respective classifications; intermediate shapes
can fall within the 5% band. For batch transpose at 128 objects, sse2neon leads
by **10.0%** at logical 256 bits, while Direct Neon leads by **42.7%** at logical
512 bits.

**Per-object processing (AoS)** keeps each vector or matrix's components
together. The same FP32 operations from the same M2 checkpoint show how the
comparison changes with the layout and number of objects:

<br>

| Per-object operation | FP | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span>, 128 objects | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span>, 4,096 objects |
| --- | ---: | ---: | ---: |
| 4D dot product | 32 | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 **<span style="color: #0969da;">1.017×</span>** |
| 3D cross product | 32 | 🟣 **<span style="color: #6f42c1;">1.466×</span>** | 🟣 **<span style="color: #6f42c1;">1.494×</span>** |
| Vector length | 32 | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 **<span style="color: #0969da;">1.044×</span>** |
| Vector normalization | 32 | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🟣 **<span style="color: #6f42c1;">1.052×</span>** |
| 4×4 matrix multiplication | 32 | 🔵 **<span style="color: #0969da;">1.028×</span>** | 🔵 **<span style="color: #0969da;">1.021×</span>** |
| 4×4 matrix–vector multiplication | 32 | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 **<span style="color: #0969da;">1.007×</span>** |
| 4×4 matrix transpose | 32 | 🔵 **<span style="color: #0969da;">0.981×</span>** | 🔵 **<span style="color: #0969da;">1.042×</span>** |
| TRS matrix inverse | 32 | 🔵 **<span style="color: #0969da;">0.959×</span>** | 🔵 **<span style="color: #0969da;">1.019×</span>** |

**FP64 per-object transpose:** 🔵 **<span style="color: #0969da;">1.041×</span>** at 128 objects and
🔵 **<span style="color: #0969da;">0.983×</span>** at 4,096 objects; both are within the 5% band.

Ratios above 1 favor Direct Neon; below 1 favor sse2neon. The full report records the invocation ranges and complete measured inventory.

[Latest full standard-math results — 22 November 2026](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)

The [21 November report](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
retains the separate Clang 17 versus 21 compiler investigation.

FP32 per-object 3D cross products delivered **46.6%–49.4% higher throughput**.
Across all 160 rows, Direct Neon led in **65**, **94** were within 5%, and
sse2neon led in **1**. Some FP64 SIMD rows were slower than scalar; the full
report retains those costs. Gains depend on layout, logical vector width and
working-set size; these measurements describe kernels rather than whole applications.

### Coverage and backend choice

Both backends support the portable integer/FP32/FP64 core, masks, memory operations,
conversions and supported vector math, with logical 128/256/512-bit vectors.
[Latest native M2 correctness run](docs/validation/arm64/mac-arm-validation-2026-11-22.md)
covers compatibility maximum 512 and Direct Neon maxima 128/256/512, each in
Release and strict UBSan Debug. Native FP16 and SVML are outside this shared ARM
surface; see [backend coverage](docs/backends.md) for API and interoperability limits.

**sse2neon remains the default** and supports SSE-style interoperability.
**Direct Neon requires experimental opt-in** and is especially promising for
FP32 batch workloads and cross products.

## Use this fork

The library is header-only for ordinary vector operations. Use **C++17 or newer**
and add this repository to your compiler’s include path. Compile
`instrset_detect_neon.cpp` on ARM or unchanged `instrset_detect.cpp` on x86
if you call runtime feature queries; CMake `vcl::runtime` selects it for you.

```sh
git clone https://github.com/DigitalExpressions/vcl2-xa64.git
cd vcl2-xa64
```

For development and validation instructions, see [contributing](CONTRIBUTING.md)
and the [developer documentation](docs/README.md).

### Direct VCL usage

The following example uses VCL directly, without `dex-gfxmath`. It includes
`vcl2_xa64.h` and operates on VCL's `Vec4f` SIMD lanes; it does not use
GfxMath's spatial vector or matrix types.

Save this as `example.cpp`:

```cpp
#include "vcl2_xa64.h"
#include <cstdio>

int main() {
    float input[4] = {1.f, 2.f, 3.f, 4.f};
    float output[4];
    Vec4f x;
    x.load(input);
    Vec4f y = x * x + Vec4f(1.f);
    y.store(output);
    std::printf("%.0f %.0f %.0f %.0f\n",
                output[0], output[1], output[2], output[3]);
}
```

On Apple Silicon, use the default compatibility backend:

```sh
xcrun clang++ -std=c++17 -O3 -arch arm64 -mcpu=apple-m1 \
  -ffp-contract=off -I. example.cpp -o example
./example
# 2 5 10 17
```

To select the experimental direct backend:

```sh
xcrun clang++ -std=c++17 -O3 -arch arm64 -mcpu=apple-m1 \
  -ffp-contract=off -I. \
  -DVCL_BACKEND=VCL_BACKEND_NEON -DVCL_ENABLE_EXPERIMENTAL_NEON=1 \
  -DMAX_VECTOR_SIZE=128 example.cpp -o example-neon
./example-neon
```

Define these switches before any VCL header is included. Direct Neon also supports
`-DMAX_VECTOR_SIZE=256` and `512`. To explicitly select compatibility, use
`-DVCL_BACKEND=VCL_BACKEND_SSE2NEON`; its default maximum width is 512, with
128 and 256 also selectable. Its precise division, sqrt and min/max options
default to enabled.

On x86, the original backend is selected automatically from compiler target
macros. Use the [x86 MSVC/Clang build and test instructions](docs/x86-build-and-test.md)
for native Windows setup, complete standalone commands and full-suite workflow
availability. Optimized Release uses `/O2 /fp:precise /fp:except-` with MSVC,
without `/RTC1`, and `-O2 -fno-trapping-math -fno-math-errno` with GNU-driver
Clang. Both use C++17 or newer. The Windows runner defaults to the optimized
profile; select strict comparisons and Clang UBSan separately. Direct CMake
retains its portable strict default, so select `VCL_TEST_PROFILE` explicitly for
optimized tests. MSVC Debug/RTC execution is deferred and rejected.

Choose deployment target flags deliberately and verify CPU **and OS** support
before higher-ISA validation execution. `INSTRSET` retains its x86
meaning; direct Neon uses the separate backend switch. On ARM,
`instrset_detect()` reports zero x86 capability and `has_neon()` reports Neon.

### Optional: using dex GfxMath

GfxMath adds spatial vectors and transformation matrices on top of VCL.
These examples use an external `dex-gfxmath` checkout with its VCL backend
integration applied; it is optional and is not a library or test dependency. Add both this repository and `../dex-gfxmath/Source` to
your include paths. GfxMath requires **C++20** and **`MAX_VECTOR_SIZE=512`**,
including on ARM64, because its public aliases include wider logical vectors.

Include this setup once, then place the small examples below inside `main()`
or another function:

```cpp
#define GFXMATH_VEC3
#define GFXMATH_VEC4
#include "GfxMath.h"
```

**Vector arithmetic, lengths, dot and cross products:**

```cpp
const dex::vec3f position(1.f, 2.f, 3.f);
const dex::vec3f velocity(2.f, 0.f, 0.f);
const auto nextPosition = position + velocity * 0.5f; // (2, 2, 3)
const float speed = velocity.length();               // 2
const auto direction = velocity.normalized();
const float alignment = direction.dot(dex::vec3f(1.f, 0.f, 0.f));
const auto normal = dex::vec3f(1.f, 0.f, 0.f)
                        .cross(dex::vec3f(0.f, 1.f, 0.f)); // (0, 0, 1)
```

**Update four independent 3D positions with the same expression:**

```cpp
const dex::vec3f4 positions(dex::float4(1.f, 2.f, 3.f, 4.f),
                           dex::float4(0.f), dex::float4(0.f));
const dex::vec3f4 velocities(dex::float4(1.f),
                            dex::float4(2.f), dex::float4(0.f));
const auto advanced = positions + velocities * dex::float4(0.5f);
float resultX[4];
advanced.x.store(resultX); // 1.5, 2.5, 3.5, 4.5
```

`vec3f` represents one XYZ vector. `vec3f4` represents four independent XYZ
vectors: each of its `x`, `y` and `z` members holds four SIMD lanes.
`dex::float4` is GfxMath's alias for VCL's `Vec4f`.

**Transform a point and a direction:**

```cpp
const auto move = dex::Matrix4f::translation(dex::vec3f(10.f, 0.f, 0.f));
const auto movedPoint = move.transformPoint(dex::vec3f(1.f, 2.f, 3.f)); // (11, 2, 3)
const auto movedDirection = move.transformDir(dex::vec3f(1.f, 0.f, 0.f)); // (1, 0, 0)
```

Points include translation; directions ignore it. Use matrix factories or
`dex::Matrix4f(1)` for identity; default construction leaves entries uninitialized.

**Compose transforms and convert back to local coordinates:**

```cpp
const auto translation = dex::Matrix4f::translation(dex::vec3f(10.f, 0.f, 0.f));
const auto scale = dex::Matrix4f::scale(2.f);
const auto model = translation.multipliedWith(scale);
const auto worldPoint = model.transformPoint(dex::vec3f(1.f, 2.f, 3.f)); // (12, 4, 6)
const auto localPoint = model.inversed().transformPoint(worldPoint); // approx. (1, 2, 3)
```

Matrices store columns and act on column vectors: `A.multipliedWith(B)` applies
`B` first, then `A`. The inverse helper supports affine transforms with nonzero,
orthogonal scaled basis columns, as above; it is not a general inverse for
sheared or projective matrices. Use `vec3d` and `Matrix4d` for double precision.

**The application code is x86/ARM64 agnostic:** these expressions and type names
stay the same when compiled for either target. VCL selects the x86 backend on
x86 and sse2neon compatibility on ARM64 by default. Explicitly selecting Direct
Neon changes the SIMD implementation behind the GfxMath types; no architecture
branches or `_mm_*`/Neon intrinsics are needed in these snippets. Wider logical
types compose 128-bit registers on Neon. Recompile for each target and keep
backend/width settings consistent across translation units exchanging vectors.
This is source portability; floating-point results retain the documented
backend-specific numerical limits.

For a source file containing the setup and examples, build with the native
compiler for your target, from this repository's root:

```sh
clang++ -std=c++20 -O3 -ffp-contract=off -DMAX_VECTOR_SIZE=512 \
  -I. -I../dex-gfxmath/Source gfxmath_example.cpp -o gfxmath_example
```

On ARM64, append `-DVCL_BACKEND=3 -DVCL_ENABLE_EXPERIMENTAL_NEON=1` to opt into
Direct Neon. The direct backend remains experimental; the default ARM64
compatibility backend and the x86 backend use the same example source.


## Optional CMake integration

Direct header inclusion needs no build system. CMake 3.20+ exposes `vcl::vcl`
and the separately compiled `vcl::runtime` target for capability queries.
Tests and benchmarks are opt-ins; a normal library configuration
does not build them. No CPU-specific compiler flags are exported to consumers.

```cmake
add_subdirectory(path/to/vcl2-xa64)
target_link_libraries(my_application PRIVATE vcl::vcl)
# Link vcl::runtime as well only when using runtime capability queries.
```

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DVCL_BUILD_TESTS=ON
cmake --build build -j 3
ctest --test-dir build --output-on-failure --timeout 60 -j 3
```

For Direct Neon add `-DVCL_BACKEND=3 -DVCL_ENABLE_EXPERIMENTAL_NEON=ON`
and select `-DVCL_NATIVE_MAX_VECTOR_SIZE=128`, `256` or `512`.
See [testing](tests/README.md) for strict UBSan and the optional pinned upstream
matrix, and [contributing](CONTRIBUTING.md) for regression expectations.

`VCL_BUILD_TESTS=ON` also builds the arithmetic smoke program `vcl_example`
under `build/tests/`; the native validation runners execute it separately.
The usage examples above can be compiled directly with their shown commands.

## Validation and performance

See the [validation index](docs/validation/README.md) for tested source revisions,
native coverage and known failures. The full Windows correctness gate remains
failed/incomplete.

The [benchmark guide](benchmarks/README.md) documents workloads, correctness
checks and timing methodology.

## Repository and attribution

Upstream-compatible headers, runtime detector and dispatch examples stay at the
root. `tests/`, `benchmarks/`, `docs/` and `tools/` contain maintained
development assets. Temporary work, binaries and local evidence are ignored.

VCL retains its [Apache license](LICENSE), existing source notices and
[upstream changelog](changelog.txt). The bundled sse2neon header retains its MIT
notice. See [provenance and third-party notices](docs/third-party.md) and
[backend ownership](docs/backend-layout.md).

Existing ARM consumers must migrate unsuffixed original VCL includes to
`vcl2_xa64.h` in their own repositories.

## Upstream resources

- [Agner Fog's Vector Class Library](https://github.com/vectorclass/version2)
- [Upstream releases](https://github.com/vectorclass/version2/releases)
- [VCL manual (PDF)](https://github.com/vectorclass/manual/raw/master/vcl_manual.pdf)
- [Upstream add-on packages](https://github.com/vectorclass/add-on)
- [Getting-started video](https://www.youtube.com/watch?v=TKjYdLIMTrI), by Christopher Rose

## Latest Windows reset validation — 2026-11-22

Fresh native AMD Windows collection at clean 08f2d2903d9bfdfdb22aa187329e99ce0f44d177: 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen passing examples. The gate remains failed/incomplete.
The [report](docs/validation/x86/windows-x86-validation-2026-11-22.md) records the complete compiler/profile/ISA table, controls and intentionally discarded historical local evidence. [Separate original-VCL performance validation](docs/benchmarks/x86/x86-upstream-2026-11-22.md) retains its own source, flags, checks and actual outcome. ARM results above keep their original checkpoints.
