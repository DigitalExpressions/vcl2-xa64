# Application-independent regression tests

The suite detects changes in VCL behavior: arithmetic and masks, integer
division, conversions, permutations, gather/scatter, protected-page memory
boundaries, namespace/ABI contracts, 256/512 composition, FP environment and
bounded numerical functions. Independent scalar/Decimal references provide
expected values. No application-specific test or proprietary dependency is part
of this suite. Protected-page tests use POSIX memory APIs on macOS/Linux and
VirtualAlloc/VirtualProtect on native Windows, retaining inaccessible boundaries.

The `edges` case also calls `truncate_i64_regression.h`: independent literal
IEEE-754 input bits and integer expectations for FP64 truncation at every
available 128/256/512-bit width. Mixed lanes rotate zeros, subnormals, fractions,
int64 boundaries, infinities and both NaN signs through all four rounding modes.
The oracle never casts an invalid floating-point value to an integer.

The maintainer's available native hosts are a Mac M2 (macOS ARM64) and an AMD
Ryzen 9 7950X (Windows x86-64). Use the default `master` branch on both hosts
or a clean worktree at the approved validation commit. Completed port/audit
branches were retired and are not test dependencies. Follow the shared
[agent instructions](../AGENTS.md); preserve unrelated local work and branches.
See [the native validation workflow](../docs/validation/native-workflow.md) for
branch synchronization, the in-tree Mac runner, evidence capture and coverage
labels. Physical Intel testing is omitted for now.

For current native Windows toolchain steps, controls and evidence capture, use
the [x86 build and test instructions](../docs/x86-build-and-test.md) and
[Windows runner guide](../docs/validation/x86/windows-runner.md). Label physical
CPU coverage accurately.
The [validation index](../docs/validation/README.md) identifies the completed
Mac/Windows checkpoint, comparator follow-up, source audit and coverage limits.
It also preserves links to the earlier Windows attempts and separately labeled
ARM/POSIX emulation. Use each dated report's exact source and profile scope.

Run from the repository root with CMake 3.20+, Python 3.9+ and Clang/GCC:

```sh
cmake -S . -B build/compat-release -DCMAKE_BUILD_TYPE=Release -DVCL_BUILD_TESTS=ON -DVCL_BACKEND=2
cmake --build build/compat-release -j 3
ctest --test-dir build/compat-release --output-on-failure --timeout 60 -j 3

cmake -S . -B build/neon512-release -DCMAKE_BUILD_TYPE=Release -DVCL_BUILD_TESTS=ON \
  -DVCL_BACKEND=3 -DVCL_ENABLE_EXPERIMENTAL_NEON=ON -DVCL_NATIVE_MAX_VECTOR_SIZE=512
cmake --build build/neon512-release -j 3
ctest --test-dir build/neon512-release --output-on-failure --timeout 60 -j 3
```

Use separate Direct Neon builds for widths 128 and 256. Compatibility and x86
local builds exercise all configured widths. Explicit ARM backends require an
ARM64 compiler target. For strict UBSan use a separate Debug directory with
`-DVCL_SANITIZE=ON` and run with
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Sanitizer recovery is disabled.
The documented Apple-host ASan startup hang is not retried. Native Windows
ASan is outside the recorded native validation selection.

## Native Windows

For automatic evidence capture and independent native/guard-page/UBSan
controls, prefer `tools/run_windows_validation.py` as described in the
[Windows runner guide](../docs/validation/x86/windows-runner.md). The manual commands below remain useful
for individual profiles and investigations.

Use an x64 Native Tools developer PowerShell with MSVC, CMake, Ninja and Python
available. Choose fresh build directories and check each command's exit code:

```powershell
cmake -S . -B build/windows-msvc-release -G Ninja -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=Release -DVCL_TEST_PROFILE=upstream-optimized -DVCL_BACKEND=1 -DVCL_BUILD_TESTS=ON -DVCL_BUILD_EXAMPLES=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/windows-msvc-release --parallel 3 --verbose
ctest --test-dir build/windows-msvc-release --show-only=json-v1
ctest --test-dir build/windows-msvc-release --output-on-failure --no-tests=error --timeout 60 --parallel 3 --output-junit results.xml
```

These commands explicitly select optimized MSVC `/O2 /fp:precise /fp:except-`.
The Windows runner defaults to `upstream-optimized`; direct CMake retains the
portable `strict-validation` default, so Release alone is insufficient. GNU-driver
Clang optimized Release uses `-O2 -fno-trapping-math -fno-math-errno`.
MSVC Debug/RTC requests are rejected until noninteractive reporting is validated.

Select `strict-validation` explicitly for MSVC `/fp:strict` comparisons:
rounding-mode changes, NaNs and
signed zero remain observable, and implicit contraction is disabled. Only
`fp_classification` uses `/fp:fast`, a broader MSVC stress option than GNU
`-ffinite-math-only`; it is not an exact flag equivalent. Strict Clang uses
`-ffp-contract=off` and selected tests use `-frounding-math`; clang-cl forwards
these options with `/clang:`. Its classification target retains precisely
`-ffinite-math-only`.

Use native x64 GNU-driver `clang++` for the optimized and UBSan profiles, checking
its default target before configuring. Existing clang-cl strict syntax support
is separate from the GNU-driver runner. Syntax probes use temporary sources and each driver's
options, and carry the configured global/ISA flags. Positive controls reject
unsupported options before a negative contract can be counted as passing.

MSVC does not provide UBSan. For GNU-style native Clang, first verify that an
intentional-UB probe stops with the expected diagnostic. Then use
`-DVCL_TEST_PROFILE=strict-ubsan -DVCL_SANITIZE=ON` in a separate Debug build and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`.
The tested Visual Studio Clang 20.1.8 runtime requires the static release CRT:
add `-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded` even in Debug. Debug compilation
still uses `-O0` and debug information; this does not claim Debug CRT coverage.
Other installed runtimes may have different constraints. Sanitizer compilation
or link failure is an unavailable gate, never a pass. The sanitizer harness
requires the GNU driver; clang-cl is covered without UBSan.

## Optional pinned upstream matrix

```sh
sh tools/fetch_testbench.sh build/_deps/testbench
cmake -S . -B build/upstream-neon512 -DCMAKE_BUILD_TYPE=Release -DVCL_BUILD_TESTS=ON \
  -DVCL_BACKEND=3 -DVCL_ENABLE_EXPERIMENTAL_NEON=ON -DVCL_NATIVE_MAX_VECTOR_SIZE=512 \
  -DVCL_TESTBENCH_DIR="$PWD/build/_deps/testbench"
cmake --build build/upstream-neon512 -j 3
ctest --test-dir build/upstream-neon512 --output-on-failure --timeout 60 -j 3
```

The revision is `a7293702385e5169a5f58b83177f2a43cd59139b`; CMake checks its Git
revision and source hashes. The separately fetched testbench retains its GPL-3.0
license. Generated copies retain that provenance; expected-value adapters repair
defined-domain scalar references without modifying the original checkout,
vector functions, input generation or numerical tolerances. Embedded matching
fragments in the adapter/configuration originate in that testbench. The selected
matrix is intentionally smaller than the entire upstream suite.

The upstream matrix targets SSE4.2 for x86. Use a separately configured local
SSE2 build for minimum-ISA coverage. Physical Intel, unselected AMD/ARM profiles,
FP16 and unavailable x86 ISAs remain separate coverage gates; native AMD coverage
is documented in the validation index.

On Windows, fetch the same revision with Git as described in the runner guide and
pass its path with `-DVCL_TESTBENCH_DIR=build/_deps/testbench`. MSVC uses
`/arch:SSE4.2` when supported; older MSVC retains default SSE2 compiler codegen
with explicit `INSTRSET=6` intrinsics for the pinned targets. GNU-style Clang
uses `-msse4.2`, and clang-cl uses `/clang:-msse4.2`. The ordinary local targets
remain at the baseline unless global target flags explicitly select another ISA.
Verify CPU and OS state support before executing these or higher-ISA binaries.

## Independent fixtures

The existing edge cases also run `boundary_regression.h` and
`truncate_i64_regression.h`: protected-page table ends, compact-mask division,
full-width shifts, composition literals and FP64 conversion boundaries under
all four rounding modes. These assertions retain the original case inventory,
deadlines and numerical gates. Compact x86 mask expansion and native AVX-512
fixed-nearest conversion are checked according to their actual API contracts.
See the [validation index](../docs/validation/README.md) for recorded native
results and remaining gaps. Each pass belongs to its exact frozen source;
retired Windows/Mac plans do not authorize another execution or repair campaign.

`generate_*_reference.py --check` regenerates the committed fixtures and checks
agreement at two Decimal precisions. The extended x86 fixture generator uses
exact pinned seed-1 keys in its header when no capture directory is supplied.
Changed keys require explicit regeneration. Unknown selected x86 reference
inputs fail closed. `configuration.py` includes negative compilation controls
and header/backend isolation checks. Test cases and original 60-second deadlines
are preserved. See [numerical contracts](../docs/numerical-contracts.md).
