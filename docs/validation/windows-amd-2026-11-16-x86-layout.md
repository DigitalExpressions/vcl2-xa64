# Native Windows x86 optimization and source-layout follow-up

The pristine-source layout and targeted MSVC optimization are implemented.
Baseline native correctness and both performance confirmations are complete.
Higher-ISA selections retain pre-existing failures; native M2 is pending.
Read the [performance report](../benchmarks/x86-optimization-2026-11-16.md)
for the measured gains and remaining slower cases.

## Source identity

The tested optimized snapshot is base
`44f974ac31e1448e0beb5157d2bd8d669f5c518e`, tree
`77f68cb6590ed95d6a812d7dff3436aff8f73b66`, plus the uncommitted five-file patch
`b954c2894ecac21ad59b3ca3d9ff10401f01fb8ac5552e96eeb4dc300c5855e4`.
Its initial/final source fingerprint is
`1da758d8e17b4f50b60fca687a5dae2f52065b317aa71936f72466ae7375fc45`.
This is not an executed pass of the unmodified base commit or a later commit.
The [patch and selected inventory](x86-optimization-2026-11-16/README.md)
preserve original source identifiers.

After the maintainer clarified the architecture target, a pristine upstream
snapshot and explicit adaptation/backend layer were added. All **35 shipped
runtime files reproduce the tested optimized bytes exactly**. The new
[layout provenance](x86-optimization-2026-11-16/source-layout.json) separately
identifies canonical patches, backend sources and generation/check tooling.
The whole repository now contains additional files not present in the native
test snapshot. Do not transfer earlier whole-tree validation claims to them.
See the [layout guide](../backend-layout.md) and
[execution plan](../x86-backend-optimization-plan.md).

The main checkout started on `master` with previously staged benchmark work.
Its index was preserved; runtime validation used a detached ignored worktree.
No commits, pushes, merges, PRs or branch retirement were performed.

## Native host and tools

AMD Ryzen 9 7950X, Windows 11 Pro 10.0.26200, native x64 Python 3.11; MSVC
19.50.35723 first and GNU-driver Clang 20.1.8 targeting Windows x64. Native
execution uses `IsWow64Process2` and PE machine `0x8664`, without WSL/QEMU or
translation. The pristine upstream testbench remains pinned at
`a7293702385e5169a5f58b83177f2a43cd59139b`, with original source hash controls.
Physical Intel is omitted.

## Full baseline-ISA matrix of the optimized source

| Driver | Mode / sanitizer | Registered | Passed | Failed | Skipped | Timed out |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| MSVC | Release, no sanitizer | 1348 | 1348 | 0 | 0 | 0 |
| MSVC | Debug, no sanitizer | 1348 | 1348 | 0 | 0 | 0 |
| Clang | Release, no sanitizer | 1348 | 1348 | 0 | 0 | 0 |
| Clang | Debug, strict UBSan/static release CRT | 1348 | 1348 | 0 | 0 | 0 |
| Total | | **5392** | **5392** | **0** | **0** | **0** |

All four profiles passed on the first native attempt. Four examples produced
`2 5 10 17`. Initial/final source fingerprints agree. MSVC Debug is distinct
from Clang UBSan coverage. The selected pinned upstream targets use SSE4.2;
local baseline targets retain their original ISA selection.

Controls retain native runtime queries, actual-page writable guards and all
four intentional read/write boundary faults, independent fixtures/configuration
checks, canaries, selected rounding flags and all original numerical gates.
Ordinary MSVC targets use `/fp:strict`, classification alone `/fp:fast`. Clang
retains no implicit contraction/rounding controls and classification-only
finite-math stress. Strict UBSan requires a passing positive control, intentional
signed-overflow exit `1` with its diagnostic, disabled recovery, frame pointers
and halt-on-error; it is not inferred from successful compilation. All CTest
deadlines remain 60 seconds and concurrency remains three. No benchmark timings
are collected by the native correctness runner.

## Additional higher-ISA findings

The first MSVC AVX2 Release local selection registered/executed 40 cases:
27 passed and 13 failed, with no skips. A fresh unmodified-runtime diagnostic
at base `44f974a` reproduces the same 13 failing case names. The candidate is
not relabeled as passing this selection. Paired diagnostics used identical
expanded classification oracles on both runtime snapshots for AVX2/AVX-512,
both compilers and Release/Debug (strict UBSan for Clang Debug). The baseline
runtime is unmodified `44f974a`; its test source includes the new mixed-lane
oracle. The candidate uses the frozen patch.

| Driver / ISA | Mode | Baseline passed / failed | Candidate passed / failed | New failed case names |
| --- | --- | --- | --- | ---: |
| MSVC / AVX2 | Release | 27 / 13 | 27 / 13 | 0 |
| MSVC / AVX2 | Debug | 31 / 9 | 31 / 9 | 0 |
| Clang / AVX2 | Release | 30 / 10 | 30 / 10 | 0 |
| Clang / AVX2 | strict UBSan Debug | 25 / 15 | 25 / 15 | 0 |
| MSVC / AVX-512 | Release | 17 / 23 | 17 / 23 | 0 |
| MSVC / AVX-512 | Debug | 20 / 20 | 20 / 20 | 0 |
| Clang / AVX-512 | Release | 20 / 20 | 20 / 20 | 0 |
| Clang / AVX-512 | strict UBSan Debug | 20 / 20 | 20 / 20 | 0 |

Each row registers/executes 40 cases per source. The candidate selection totals
190 passes and 130 failures; baseline has the same counts and failed names.
There are zero skips/timeouts. These **failing** results are not clean higher-ISA
validation, and matching failed names alone is not an exhaustive equivalence
proof. Existing AVX-512 `exp_log` failure diagnostics also match between sources.

Clang AVX2 Debug reports NaN-to-`long long` undefined conversion in
`vectorf256.h:2066` on both sources. That entire header is unchanged from
original upstream: both original and pre-change fork have Git blob
`b6d8e4592624d3047e4c793a585cefc7f6b6e96b`. No suppression, expected-value edit or
numerical-policy change was made. The new mixed-lane oracle also exposes an
existing Clang AVX2 finite-math classification failure in Release. These need
separate correctness/oracle triage before a broad higher-ISA readiness claim.

The first collector stopped on the initial AVX2 failure; the paired collector
retained all 16 profile results and exited nonzero because the UBSan failure
remained. After confirming the same existing failure category and no additional
failed case names, a separate performance phase ran only independently checked
benchmark kernels. It does not turn any failing CTest result into a pass.

Failures include movement/composition and signed-zero or special-value
expectations in FP helpers, power/roots and trig. No expectations, tolerances
or policies were changed to suppress them. Matching failed names alone cannot
prove every diagnostic is identical; retained logs remain authoritative.
These additional selections do not invalidate the separately identified
baseline passes, and those passes do not imply a clean higher-ISA matrix.

## Integrity and portable evidence

The [pristine upstream inventory](../../vendor/vectorclass.json) verifies all
28 original files and reconstructs upstream tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Generation applies explicit patches
without fuzz, then copies canonical backend sources to the existing root include
layout. Eight controls passed: clean generation, vendor mutation, extra vendor
path, patch-context mismatch, output path escape, hand-edited output, changed
canonical backend and explicit line-ending behavior. Canonical separation does
not require applications to run generation. Native M2 remains pending; use the
[handoff](mac-arm-x86-optimization-handoff.md) after a clean source is approved.

Four further MSVC/Clang consumer builds/runs at SSE2/AVX-512 use only the 35
shipped root files in an isolated namespace. They pass without canonical vendor,
adaptation or backend directories in the application package. Their
[portable record](x86-optimization-2026-11-16/consumer-check.json) is separate
from CTest counts and performance samples.

All raw work is retained under `build/x86-optimization-20261116/`; the full
native profiles are under its detached `native-candidate/build/` directory.
Each compact packet retains logs, manifests, patches, binary hashes, compiler
commands, inventories and JUnit; rebuildable executables remain host-local.
Generated probe sources and other records not covered by compact packets are
included in a separately checksummed supplement, verified across 2,741 entries.
Its [receipt](x86-optimization-2026-11-16/supplement-receipt.json) records
`compact-supplement.zip`, SHA256
`10c4c5bd728594c0df46766f406df6f2516d403364c75ab598c01bf3371bb109`.
Original historical
reports, source manifests and benchmark evidence are preserved.

| Profile / `return-evidence.zip` directory suffix | SHA256 |
| --- | --- |
| `windows-native-msvc-release-20261116T205104640725Z` | `723199e3dea8a5cadb542b6755667b0f787a65e4abf8f237a5ac09998a5f59e2` |
| `windows-native-msvc-debug-20261116T205617687415Z` | `bfab32cce59b1f1dca4607123407545fa7688790b701a570f7ace77bcd0ca03c` |
| `windows-native-clang-release-20261116T210148091926Z` | `06210c16e715ab8fb3d11c900187d18356853ee5d099e64d419ec19d4dd5bc6c` |
| `windows-native-clang-debug-20261116T210622338932Z` | `ed726f4e7ff2350395c2ed0cab5e303c32e95d49d57ff37cd55074ea1224e793` |
