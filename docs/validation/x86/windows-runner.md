# Manual native Windows compilation and testing

## Current policy - 21 November 2026

At the start of each new x86 validation request, ask the maintainer whether to
include separate performance tests; an explicit choice in that request already
answers the question. Record it in the plan and report. Do not infer the answer
from previous rounds or start timing while the choice is pending. Use the
[results publication checklist](results-publication-checklist.md) to prepare
completed correctness results and any requested performance results.

Routine testing uses optimized native x64 MSVC and GNU-driver Clang, following
Agner's [compiler guidance](https://github.com/vectorclass/manual/blob/c18341d00171bfe6d816ce7b24800c9d164f15aa/vcl_performance.tex)
and the [pinned testbench script](https://github.com/vectorclass/testbench/blob/a7293702385e5169a5f58b83177f2a43cd59139b/runtest.sh).
Clang O2 is the chosen reproducible level; the upstream script uses O3.
The latest
[22 November native collection](windows-x86-validation-2026-11-22.md) at
clean `08f2d29` records 6,796 passed, 235 failed and 109 not run of 7,140,
plus fifteen passing examples;
it remains failed/incomplete. The historical full Windows
gate remains failed/incomplete: 5,387 passed, 249 failed, 76 not run of 5,712,
plus twelve passing examples. A profile change does not relabel that evidence.

| Profile | Selection | Ordinary test compiler policy |
| --- | --- | --- |
| upstream-optimized | runner default; MSVC/Clang Release | MSVC O2/precise/except-; Clang O2/no-trapping-math/no-math-errno |
| strict-validation | explicit Release comparison | retained strict FP/no-contraction/rounding controls |
| strict-ubsan | Clang Debug with --sanitize | nonrecovering UBSan, frame pointers, static release CRT |
| rtc-diagnostics | deferred and rejected before execution | noninteractive reporting has not been implemented |

Ordinary tests do not use fast/finite-math assumptions. The existing
classification-only stress target retains its separate fast/finite-math flags.
Numerical expectations, fixtures, seeds and controls are unchanged. The owned
profile helper exports no compiler options through the public interface target.
Direct CMake retains strict-validation as its portable default; explicitly select
upstream-optimized for routine Windows tests.

## Native tools and source preflight

Use native AMD x64 Windows, native x64 Python 3.9+, CMake 3.20+, CTest supporting
JUnit, Ninja and an x64 Visual Studio developer shell. Inspect installed tools
before setup; do not substitute WSL, emulation or clang-cl. GNU-style clang++
must report a Windows MSVC-ABI x64 target. Use inspected paths for `$nativeClang`
and `$verifiedTestbench`; machine paths belong under ignored build.

The [PowerShell wrapper](../../../tools/run_windows_validation.ps1) can initialize
the installed x64 developer environment. Do not change machine-wide execution
policy. Use the Python entry point from an existing developer shell if needed.
Both the wrapper and Python runner accept explicit profile selection.

Read [AGENTS.md](../../../AGENTS.md), [validation status](../README.md) and
[integration status](../../integration-plan.md). Inspect actual HEAD, status,
worktrees and remotes. A new validation run uses an approved clean source or a
complete immutable captured source, not moving edits. Protect all 28 original
files (27 at root and the upstream README under `audit/`), the VCL audit manifest
and root `sse2neon.h` byte-exactly. The required
`audit/sse2neon-2272251.json` pins the sole dependency copy; `vendor/` is retired.
Reuse the pristine testbench at `a7293702385e5169a5f58b83177f2a43cd59139b`,
checking cleanliness and all three pinned source hashes. Never overwrite an
existing dependency. Run repository/original-integrity checks before and after.

## Compile a simple facade consumer

The existing example uses `#include "vcl2_xa64.h"`. These standalone templates
are for an inspected source root and native developer shell; choose a fresh run
directory and check each command's exit code. Both standalone recipes and the
CMake examples were verified in the dated report; these are not extra full-suite
passes. The retained traces also verify facade/original x86 includes and no ARM.

```powershell
$runRoot = Join-Path (Get-Location) ('build/example-' + [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssfffZ'))
New-Item -ItemType Directory -Path $runRoot -ErrorAction Stop | Out-Null
$msvcObject = Join-Path $runRoot 'msvc.obj'
$msvcExample = Join-Path $runRoot 'msvc.exe'
$clangExample = Join-Path $runRoot 'clang.exe'

cl.exe /nologo /std:c++17 /EHsc /O2 /fp:precise /fp:except- /I. tests/vector_arithmetic.cpp "/Fo$msvcObject" "/Fe$msvcExample"
if ($LASTEXITCODE -ne 0) { throw 'MSVC example compilation failed' }
& $msvcExample
if ($LASTEXITCODE -ne 0) { throw 'MSVC example failed' }

& $nativeClang -m64 -std=c++17 -O2 -fno-trapping-math -fno-math-errno -I. tests/vector_arithmetic.cpp -o $clangExample
if ($LASTEXITCODE -ne 0) { throw 'Clang example compilation failed' }
& $clangExample
if ($LASTEXITCODE -ne 0) { throw 'Clang example failed' }
```

Both should print `2 5 10 17` and be native x64 PE executables. For runtime
capability queries, also link the unchanged instrset_detect.cpp, or use the
existing `vcl::runtime` CMake target. No global AVX2 deployment assumption is made.

## Run the complete pinned profiles

From an approved clean checkout in a native x64 developer shell:

```powershell
python -B tools/run_windows_validation.py --driver msvc --mode Release --plan
python -B tools/run_windows_validation.py --driver msvc --mode Release --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --driver clang --mode Release --compiler $nativeClang --testbench-dir $verifiedTestbench

python -B tools/run_windows_validation.py --profile strict-validation --driver msvc --mode Release --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile strict-validation --driver clang --mode Release --compiler $nativeClang --testbench-dir $verifiedTestbench
python -B tools/run_windows_validation.py --profile strict-ubsan --driver clang --mode Debug --sanitize --compiler $nativeClang --testbench-dir $verifiedTestbench
```

For an explicitly authorized uncommitted implementation, first freeze it with
`tools/capture_source.py --output <new-ignored-capture-directory>`. Run the tools
inside that capture's source directory with `--capture-manifest` pointing to
its matching capture.json. Preserve the patch, manifest, added files and source
ZIP; do not edit the captured source or reuse an old manifest for new edits.
Dirty source without an explicit complete capture is rejected.

PowerShell examples, when local execution policy already permits the wrapper:

```powershell
./tools/run_windows_validation.ps1 -Driver msvc -Mode Release -Upstream
./tools/run_windows_validation.ps1 -Driver clang -Mode Release -Profile strict-validation -Upstream
./tools/run_windows_validation.ps1 -Driver clang -Mode Debug -Sanitize -Profile strict-ubsan -Upstream
```

Each full pinned profile registers 1,348 cases and one separate example; without
the testbench it registers the existing 40 local cases. Two optimized baselines
total 2,696 cases plus two examples. Separately CPU/OS-gated AVX2/AVX-512 local
profiles add 160 plus four examples when fully supported: routine total 2,856
plus six. Strict Release has the same separate selection; strict UBSan totals
1,428 plus three with full higher-ISA support. These are registration counts,
not predicted passes. Unsupported profiles remain explicit gaps.

The runner audits emitted compiler commands before suite build/execution,
rejects inherited compiler-policy environment variables, and rejects incompatible
profile selections and MSVC Debug/RTC execution. Keep all numerical gates,
fixture checks, original 60-second case deadlines and concurrency three.
Probe CPU and OS/XCR0 before higher-ISA execution; the baseline runner rejects
blanket AVX overrides. Use fresh explicitly configured local builds for supported
higher profiles, preserving the actual ISA and FP flags in their evidence.

## Controls, failures and evidence

Every dated Windows validation report must include the compiler/profile/ISA
results table specified by the [report template](../report-template.md), with
Registered, Passed, Failed, Not run and Timeouts columns for every selected
configuration, alongside the profile summary. Reconcile both tables with the
retained case inventories; count examples separately and state whether timeouts
are included in failures. The
[21 November report](windows-x86-validation-2026-11-21.md) shows this format.

The runner verifies native Windows/x64 process and PE identity, runtime detection,
actual page size, a passing writable guard control and all four protected
read/write fault controls returning `0xc0000005`. Strict GNU-driver Clang Debug
uses the verified MultiThreaded release CRT, -O0, debug information, undefined
sanitizer, disabled recovery, frame pointers and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Positive control must exit zero;
intentional signed overflow must exit one with its diagnostic and no continuation.
MSVC Debug is not UBSan coverage. Missing controls/runtimes are gaps.

`NoInitFailures` means a captured RTC uninitialized-use diagnostic. The retained
[policy and sites](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20/noinit-policy.json) do not
infer author intent or harmlessness from a case name or uninitialized constructor.
RTC is not executed by the current routine profiles and is not converted to a
passing count. No callback/private desktop route is installed. A future RTC run
requires separate noninteractive implementation and validation first.

Every invocation creates a unique ignored `build/windows-native-<profile>-.../`
directory and retains commands/exits, flags, complete inventories, source proofs,
compiler/binary hashes, configure/build/test logs, JUnit, controls and example.
The [shared evidence module](../../../tools/validation_evidence.py) packages the
first runner return packet on success/failure after capture starts. Preserve
that packet and any build-abort continuation separately. Missing binaries stay
not run. Do not replay completed cases or weaken gates to improve counts.
Full process/LastTest logs remain authoritative when JUnit output is truncated.

Keep raw records, source captures, generated control/probe inputs and all failed
attempts under ignored build. Portable metadata excludes machine paths and
binaries. Follow the [retention policy](../native-workflow.md#reports-and-cleanup)
before any separately authorized cleanup. No hosted CI, ASan, benchmark,
emulation, external consumer or automatic commit/push/merge belongs to this run.
Mac M2 uses the [native Mac workflow](../native-workflow.md).
