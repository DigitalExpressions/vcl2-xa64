# x86 build and test instructions

## Compiler policy and availability on master

The maintainer selected optimized native x64 MSVC and GNU-driver Clang builds
on 20 November 2026, following Agner Fog's
[compiler guidance](https://github.com/vectorclass/manual/blob/c18341d00171bfe6d816ce7b24800c9d164f15aa/vcl_performance.tex)
and the [pinned upstream test script](https://github.com/vectorclass/testbench/blob/a7293702385e5169a5f58b83177f2a43cd59139b/runtest.sh).
Clang O2 is our reproducible starting point; the upstream script uses O3 and
the manual discusses both. No benchmark selected this policy.

| Selection | Ordinary compiler policy |
| --- | --- |
| MSVC optimized Release | `/O2 /std:c++17 /fp:precise /fp:except-`; no `/RTC*` or `/Od` |
| Clang optimized Release | `-O2 -std=c++17 -fno-trapping-math -fno-math-errno` |
| Strict numerical comparison | retain `/fp:strict` or Clang no-contraction/rounding controls |
| Strict UBSan | separate GNU-driver Clang Debug with disabled recovery, frame pointers and verified release CRT |
| MSVC RTC | optional future diagnostics; deferred until noninteractive reporting is validated |

**Availability:** the profile-aware harness was integrated and published on
master at 2237ffb. The Windows runner defaults to `upstream-optimized`; `--sanitize`
selects `strict-ubsan`. Python `--profile`, PowerShell `-Profile` and CMake
`VCL_TEST_PROFILE` select explicit policies. Direct CMake retains
`strict-validation` as its portable default, so Release alone does not select
the optimized test policy. Use the profile option rather than global flag
overrides. MSVC Debug/RTC requests are rejected before test execution.
See the [integration/validation record](validation/x86/windows-x86-harness-integration-2026-11-20.md)
for the exact integrated capture and actual results, and the
[Windows runner guide](validation/x86/windows-runner.md) for controls and evidence.
The latest [21 November native report](validation/x86/windows-x86-validation-2026-11-21.md)
records the subsequent clean-source collection and compiler/profile/ISA table;
its correctness gate remains failed/incomplete.
The [earlier harness validation](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20.md)
retains its own frozen source identity; it is not a new pass assigned to master.

## Native setup and original source

Use native AMD Windows x64, an x64 Visual Studio developer shell, native x64
Python 3.9+, CMake 3.20+, CTest with JUnit and Ninja. Inspect installed tools
before installing anything. Set `$nativeClang` to an inspected GNU-style
`clang++` executable reporting a Windows MSVC-ABI x64 target. Do not substitute
clang-cl, WSL or emulation. Machine-specific paths belong under ignored build.
Keep Windows source/build paths short. The integration record retains an ABI
probe failure caused by a deeply nested path; a shorter byte-identical frozen
copy resolved setup without source, flag or test changes.

Include `#include "vcl2_xa64.h"`. It forwards to Agner's original x86 headers;
ARM headers must not enter the compiled x86 include graph. Preserve all 28
original root files, [VCL audit manifest](../audit/vectorclass-2.02.04.json) and
root `sse2neon.h` byte-exact. The required [sse2neon audit](../audit/sse2neon-2272251.json)
pins that sole copy; the vendor catalog is retired. Do not repair
original temporaries or normalize source bytes to address test failures.

Inspect actual HEAD, changes, refs and worktrees. Validate an approved clean
source or complete immutable capture in fresh output directories. Reuse pristine
testbench `a7293702385e5169a5f58b83177f2a43cd59139b`, verifying revision,
cleanliness and all three pinned source hashes. Never overwrite an existing
dependency. Follow [native integrity/capture procedures](validation/native-workflow.md).
Reject unexpected inherited compiler-policy settings such as `CL`, `_CL_` and
`CXXFLAGS`, rather than changing saved machine configuration.

## Compile the facade example with both compilers

From the source root in an x64 developer PowerShell, choose a new output
directory and check every exit code. These recipes passed during the separate
frozen native Windows validation; that does not assign a pass to this later
documentation tree.

```powershell
$runRoot = Join-Path (Get-Location) ('build/x86-example-' + [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssfffZ'))
New-Item -ItemType Directory -Path $runRoot -ErrorAction Stop | Out-Null
$msvcObject = Join-Path $runRoot 'msvc.obj'
$msvcExample = Join-Path $runRoot 'msvc.exe'
$clangExample = Join-Path $runRoot 'clang.exe'

cl.exe /nologo /std:c++17 /EHsc /O2 /fp:precise /fp:except- /I. tests/vector_arithmetic.cpp "/Fo$msvcObject" "/Fe$msvcExample"
if ($LASTEXITCODE -ne 0) { throw 'MSVC compilation failed' }
& $msvcExample
if ($LASTEXITCODE -ne 0) { throw 'MSVC example failed' }

& $nativeClang -m64 -std=c++17 -O2 -fno-trapping-math -fno-math-errno -I. tests/vector_arithmetic.cpp -o $clangExample
if ($LASTEXITCODE -ne 0) { throw 'Clang compilation failed' }
& $clangExample
if ($LASTEXITCODE -ne 0) { throw 'Clang example failed' }
```

Both should print `2 5 10 17` and be native x64 PE executables. Ordinary vector
operations are header-only. Runtime feature queries also need unchanged
`instrset_detect.cpp` compiled/linked, or CMake's `vcl::runtime`. Preserve ABI,
namespace and width settings across translation units.

Choose target flags for the deployment CPU. Before executing higher-ISA tests,
independently probe CPU **and OS/XCR0 support**, then verify the original runtime
detector and emitted flags. Do not assume AVX2/AVX-512 support from the build host.

## Optimized full-suite workflow

`$verifiedTestbench` is the inspected pristine pin above. Use an approved clean checkout; authorized
uncommitted work instead requires a complete matching immutable source capture
and its `--capture-manifest`. Preserve both source and validator identities.

```powershell
# Optimized correctness, strict comparison and sanitizer results stay separate.
python -B tools/run_windows_validation.py --profile upstream-optimized --driver msvc --mode Release --testbench-dir $verifiedTestbench --plan
python -B tools/run_windows_validation.py --profile upstream-optimized --driver msvc --mode Release --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile upstream-optimized --driver clang --mode Release --compiler $nativeClang --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile strict-validation --driver msvc --mode Release --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile strict-validation --driver clang --mode Release --compiler $nativeClang --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile strict-ubsan --driver clang --mode Debug --sanitize --compiler $nativeClang --testbench-dir $verifiedTestbench
```

Each pinned profile registers 1,348 cases plus one example. Both optimized
baselines total 2,696 plus two examples. Supported AVX2/FMA and AVX-512
F/DQ/BW/VL/FMA local profiles add 160 plus four examples across both compilers:
fully supported optimized total 2,856 plus six. Strict Release comparisons stay
separate; strict UBSan totals 1,428 plus three with both higher-ISA profiles.
These are inventory expectations, not passing counts or new results on master.

Keep every registered case, independent fixture/reference, seed, numerical gate,
protected-page/canary control, 60-second deadline and concurrency three intact.
Ordinary tests must not acquire fast/finite-math assumptions. The existing
classification-only stress target retains its separate flags. Strict comparisons
preserve original no-contraction/rounding controls.

## Separate diagnostics and failure reporting

Clang UBSan requires the verified static release CRT `MultiThreaded`, even in
Debug, O0, undefined sanitizer, disabled recovery, frame pointers and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Require positive exit zero
and intentional signed-overflow exit one with its diagnostic and no continuation.
Missing controls/runtimes remain gaps. MSVC Debug is not UBSan coverage.

MSVC Debug/RTC requests are rejected by the current runner and test profile
configuration. Callback/popup-handler work is deferred. Future RTC investigation needs explicit
opt-in after validated noninteractive reporting is available. `NoInitFailures`
means a confirmed RTC uninitialized-use diagnostic, not proof of author intent
or a correctness waiver. Do not relabel numerical/UBSan failures by case name.

The historical Windows gate remains **failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. Record new results separately
at their actual source and flags. Preserve every failure, missing binary,
timeout, first return packet and separately logged build-abort continuation.
Do not replay completed cases or weaken gates. Retain raw logs, input/output
bits, source captures, flags, binaries and checksums under ignored build;
portable reports exclude machine paths and binaries. No benchmark, ASan,
hosted CI, emulation or external consumer work is implied.
