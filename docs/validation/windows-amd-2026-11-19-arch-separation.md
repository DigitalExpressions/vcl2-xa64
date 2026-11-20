# Native AMD Windows architecture separation — 19 November 2026

## Outcome

**Correctness gate failed/incomplete.** All twelve selected profiles were collected on
unchanged frozen source. The facade/include separation is evaluated separately from
inherited original-x86 behavior and test/API compatibility failures. No original,
test, numerical gate or runtime fix was changed to obtain a pass.

Baseline: **5231 passed, 133 failed,
28 not run, 0 skipped** out of
5,392 registered case instances. Higher ISA: **156 passed,
116 failed, 48 not run,
0 skipped** out of 320 registered instances.
Timeouts: 1 (included in
failed counts). Missing executables are not counted as executed or passing.
All 12 separate arithmetic examples passed with `2 5 10 17`.

## Source identity and publication comparison

- Approved publication commit: `2d251bc72bcc861edad371499fa732e8eb45a5f7`.
- Publication tree: `8c16f0135a597db27b963dff6c241b533a5b91b4`.
- Windows immutable published-source capture: **497 files**,
  fingerprint `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2`.
- Windows source ZIP SHA256: `6b2f943cea502830c7c0929e61b15e63a335e8f2453edc51ed44c3119b7127f4`.
- Original Mac cleanup-01 capture: **474 files**,
  fingerprint `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
  Its source ZIP remains `3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`.
  Its base was `1a5226446f14e1f4d26216ae688b40ccc9b0fa03`, tree
  `e5b8579b6292cce2ea6a67b8cdf8027ec228d7bc`, not its tested implementation.

Fetched origin without pruning or changing branches. Created a fresh detached
worktree at the approved commit using command-scoped `core.autocrlf=false` and
`core.eol=lf`; saved configuration did not change. HEAD and cleanliness passed
before capture. Every one of the 497 raw checkout files matched its exact Git blob;
there were **zero newline transformations**. All 474 Mac paths reconciled:
463 exact, 11 listed documentation/metadata differences; 23 added report paths.
Runtime, tests, tools, fixtures, dependencies and build files are identical.

All execution used the gitless immutable capture and its explicit manifest.
The capture tool's generic schema is named `frozen-uncommitted-source`; here its
base commit identifies an exact clean publication, with an empty tracked patch
and no added/deleted source paths. No fake HEAD or source commit was created.
Mac passes remain tied to the Mac capture; this is no new cross-host corpus run.

## Native environment, controls and integrity

Native AMD Ryzen 9 7950X, Windows 11 Pro 10.0.26200, native x64 Python 3.11.
`IsWow64Process2` established process machine 0 and native machine `0x8664`.
MSVC 19.50.35723 (installed toolset 14.50.35717), followed by GNU-driver Clang
20.1.8, target `x86_64-pc-windows-msvc`. CMake/CTest 4.2.1, Ninja 1.12.1.
No tools were installed. Testbench stayed clean at
`a7293702385e5169a5f58b83177f2a43cd59139b`; all three pinned CPP hashes matched
before and after. Generated include-adapted testbench copies stayed under `build/`.

The independent native CPUID/XCR0 probe established AVX2/FMA and
AVX-512F/DQ/BW/VL with OS state support (`XCR0=0xe7`) before any higher-ISA
execution. Runtime consumers linked `vcl::runtime` and used the original detector.
All ordinary native binary records were verified as x64 PE (`0x8664`). Actual
page size was 4,096 bytes. Every positive guard control exited zero, and all four
negative read/write boundary controls per profile returned `0xc0000005`.
Clang Debug positive UBSan controls exited zero; deliberate signed overflow
exited **1** with the required diagnostic and no continuation.

All 28 protected root originals, all 28 vendor originals,
`vendor/vectorclass.json`, sse2neon alias/dependency and its pin retained exact
raw bytes and mtimes at every recorded phase checkpoint. Upstream pin remains
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`.
Full capture verification passed before/after all phases. Read-only integrity,
repository, header-integrity negative controls and source-capture controls passed.
Negative controls modified disposable copies only; the retired writer rejected.

Ordinary MSVC tests retained `/fp:strict`; only classification stress used
`/fp:fast`. Clang retained `-ffp-contract=off`, selected rounding options and
classification-only `-ffinite-math-only`. Clang Debug used strict nonrecovering
UBSan, frame pointers, static release CRT (`MultiThreaded`) and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. MSVC Debug was unsanitized.
All registered deadlines remained 60 seconds, build/test concurrency three.
Independent fixtures, seeds, tolerances and numerical gates stayed unchanged.

## Actual inventory and attempts

| Profile | Registered | Passed | Failed | Not run | Skipped | Timed out |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| continuation01-clang-debug | 1348 | 1284 | 53 | 11 | 0 | 0 |
| continuation01-clang-release | 1348 | 1328 | 9 | 11 | 0 | 0 |
| continuation01-msvc-debug | 1348 | 1288 | 57 | 3 | 0 | 1 |
| continuation01-msvc-release | 1348 | 1331 | 14 | 3 | 0 | 0 |
| higher01-clang-debug-avx2 | 40 | 19 | 9 | 12 | 0 | 0 |
| higher01-clang-debug-avx512 | 40 | 19 | 11 | 10 | 0 | 0 |
| higher01-clang-release-avx2 | 40 | 24 | 4 | 12 | 0 | 0 |
| higher01-clang-release-avx512 | 40 | 19 | 11 | 10 | 0 | 0 |
| higher01-msvc-debug-avx2 | 40 | 22 | 16 | 2 | 0 | 0 |
| higher01-msvc-debug-avx512 | 40 | 16 | 24 | 0 | 0 | 0 |
| higher01-msvc-release-avx2 | 40 | 24 | 14 | 2 | 0 | 0 |
| higher01-msvc-release-avx512 | 40 | 13 | 27 | 0 | 0 | 0 |

Each original baseline runner failed at compilation, before suite execution.
Those four attempts and their original packets remain intact. A separately
identified `continuation01` invocation used the same build files, source,
flags and full inventory, with Ninja `-k 0` to collect independent build edges.
It did not retry a CTest case, remove targets or change the acceptance criteria.
CTest then accounted for every registered name, including missing executables.
Higher-ISA profiles similarly retained all failures with complete 40-case lists.
The original build-abort attempts add zero suite executions to the totals above.

Preparation failures in the external controller and the existing shell policy
rejection are preserved in the attempt inventory. They ran no native suite and
did not change the frozen source or machine execution policy.

## Facade versus direct original

**12 compiler/mode/ISA pairs** compared a consumer including only
`vcl2_xa64.h` with a direct-original consumer including the same original
vector/exp/trig/hyperbolic surface. Native MSVC and Clang, Release/Debug,
SSE2/AVX2/AVX-512 were covered. Include traces reached original runtime/math
headers without `_neon.h`, sse2neon, adaptation or backend helpers.
Outputs matched each other and independent exact arithmetic/math expectations;
actual result bits are retained. Instruction bytes matched in every pair.
The original raw code/symbol-name comparison passed nine pairs and recorded
three MSVC Debug differences (eleven compiler-assigned string-label names each).
A separately hashed offline validator resolved every defined relocation to its
actual section/offset: code relocations and all runtime section bytes/relocations
matched in all twelve pairs. MSVC `.debug$S` and `.chks64` source metadata differed
and are reported separately; the whole-object-section comparison remains false
for those six MSVC pairs. Clang whole-object sections/relocations matched.
Original raw-name results, objects, disassembly and all comparison records remain
retained. No native case was rerun for this offline analysis. This verifies these generated functions,
not every possible API, all compiler flags or application performance.

## Failures and interpretation

Compile and runtime diagnostics, complete registered names, actual outputs and
JUnit status are in the [portable bundle](windows-arch-separation-2026-11-19/README.md).
Retained original-x86 findings are [separately documented](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md).
The wide unsigned-qword complement assertion is a retained test/API expectation
that does not hold for the original SSE2 composition; it is not evidence of a
new architecture-selection defect. Clang's signed-minimum constant-division
diagnostic originates in original `vectori128.h`: the later constexpr negation
still requires a valid constant expression despite an earlier conditional return.
Other compilation/behavior differences remain actual failures; an upstream or
compiler defect is not inferred solely from a fork expectation or source rewrite.

Every nonpassing case is accounted for below (details and outputs in JSON):

- `composition512` (failed): `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `composition512` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-clang-release-avx2`, `higher01-msvc-debug-avx2`, `higher01-msvc-release-avx2`.
- `composition512_namespace` (failed): `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `composition512_namespace` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-clang-release-avx2`, `higher01-msvc-debug-avx2`, `higher01-msvc-release-avx2`.
- `core_512` (failed): `higher01-msvc-release-avx512`.
- `core_namespace` (failed): `higher01-msvc-release-avx512`.
- `edges` (failed): `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `edges` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `exp_log` (failed): `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `exp_log_namespace` (failed): `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `exp_log_signed_zero` (failed): `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `fp_classification` (failed): `continuation01-clang-release`, `continuation01-msvc-release`, `higher01-clang-release-avx2`, `higher01-msvc-release-avx512`.
- `fp_helpers` (failed): `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `fp_helpers` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `fp_helpers_namespace` (failed): `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `fp_helpers_namespace` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `fp_helpers_signed_zero` (failed): `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `fp_helpers_signed_zero` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `helpers_Vec2d_25` (failed): `continuation01-clang-debug`.
- `helpers_Vec4d_25` (failed): `continuation01-clang-debug`.
- `helpers_Vec8d_25` (failed): `continuation01-clang-debug`.
- `hyperbolic` (failed): `continuation01-msvc-debug`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `hyperbolic_Vec16f_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec16f_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec16f_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec16f_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec2d_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec2d_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec2d_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec2d_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec2d_305` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4d_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4d_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4d_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4d_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4d_305` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4f_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4f_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4f_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4f_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec4f_305` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8d_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8d_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8d_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8d_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8d_305` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8f_300` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8f_302` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8f_303` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8f_304` (failed): `continuation01-msvc-debug`.
- `hyperbolic_Vec8f_305` (failed): `continuation01-msvc-debug`.
- `hyperbolic_namespace` (failed): `continuation01-msvc-debug`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `hyperbolic_signed_zero` (failed): `continuation01-msvc-debug`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `integer_slice` (failed): `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `integer_slice` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `integer_slice_namespace` (failed): `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `integer_slice_namespace` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `inverse_pi` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `inverse_pi_namespace` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `inverse_pi_signed_zero` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx512`.
- `math_Vec16f_300` (failed): `continuation01-msvc-debug`.
- `math_Vec2d_200` (failed): `continuation01-clang-debug`.
- `math_Vec2d_300` (failed): `continuation01-msvc-debug`.
- `math_Vec4d_200` (failed): `continuation01-clang-debug`.
- `math_Vec4d_300` (failed): `continuation01-msvc-debug`.
- `math_Vec4f_300` (failed): `continuation01-msvc-debug`.
- `math_Vec8d_200` (failed): `continuation01-clang-debug`.
- `math_Vec8d_300` (failed): `continuation01-msvc-debug`.
- `math_Vec8f_300` (failed): `continuation01-msvc-debug`.
- `movement_slice` (failed): `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `movement_slice` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `movement_slice_namespace` (failed): `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `movement_slice_namespace` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `power_roots` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-release-avx2`.
- `power_roots_namespace` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-release-avx2`.
- `power_roots_signed_zero` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-clang-debug-avx2`, `higher01-msvc-release-avx2`.
- `powerroots_Vec2d_500` (failed): `continuation01-clang-debug`.
- `powerroots_Vec2d_501` (failed): `continuation01-clang-debug`.
- `powerroots_Vec4d_500` (failed): `continuation01-clang-debug`.
- `powerroots_Vec8d_500` (failed): `continuation01-clang-debug`.
- `trig` (failed): `continuation01-clang-debug`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `trig_Vec2d_200` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_201` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_202` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_203` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_204` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_205` (failed): `continuation01-msvc-debug`.
- `trig_Vec2d_206` (failed): `continuation01-msvc-debug`.
- `trig_Vec2d_210` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_211` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_212` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_213` (failed): `continuation01-clang-debug`.
- `trig_Vec2d_214` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_200` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_201` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_202` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_203` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_204` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_205` (failed): `continuation01-msvc-debug`.
- `trig_Vec4d_206` (failed): `continuation01-msvc-debug`.
- `trig_Vec4d_210` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_211` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_212` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_213` (failed): `continuation01-clang-debug`.
- `trig_Vec4d_214` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_200` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_201` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_202` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_203` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_204` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_205` (failed): `continuation01-msvc-debug`.
- `trig_Vec8d_206` (failed): `continuation01-msvc-debug`.
- `trig_Vec8d_210` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_211` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_212` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_213` (failed): `continuation01-clang-debug`.
- `trig_Vec8d_214` (failed): `continuation01-clang-debug`.
- `trig_namespace` (failed): `continuation01-clang-debug`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `trig_signed_zero` (failed): `continuation01-clang-debug`, `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `upstream_Vec4i_5` (failed): `continuation01-clang-debug`.
- `upstream_Vec8i_5` (failed): `continuation01-clang-debug`.
- `upstream_Vec8uq_109` (notrun): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`.
- `wide_composition` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `wide_composition` (notrun): `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.
- `wide_composition_namespace` (failed): `continuation01-clang-debug`, `continuation01-clang-release`, `continuation01-msvc-debug`, `continuation01-msvc-release`, `higher01-msvc-debug-avx2`, `higher01-msvc-debug-avx512`, `higher01-msvc-release-avx2`, `higher01-msvc-release-avx512`.
- `wide_composition_namespace` (notrun): `higher01-clang-debug-avx2`, `higher01-clang-debug-avx512`, `higher01-clang-release-avx2`, `higher01-clang-release-avx512`.

## Evidence, preservation and remaining gaps

Run ID: `windows-arch-separation-20261119-ws18-01`. All raw records and generated
inputs remain under the active checkout's ignored `build/ws18-01/`.
The immutable source is `cap/source/`, with `cap/capture.json` and source ZIP.
Original baseline packets remain under their individual capture build directories.
The complete compact raw packet is `windows-arch-separation-evidence.zip`:
SHA256 **`e7999de002dd09191fd3c2a3d12b6a3f819a3a71bfb89cc660f9ad041f572876`**. Its 6216 recorded entries passed
CRC and every per-entry SHA256 verification; the adjacent file inventory and
receipt identify exact coverage. Portable metadata has its own checksums and
contains no absolute machine paths or compiled binaries. Raw archives are local
audit records and are not transferred by a future Git publication.

All original build directories, compiled artifacts, failed attempts and prior
evidence remain. No benchmark, ASan, hosted CI, WSL/emulation, external consumer
change, original-file repair, commit, push, merge, PR or branch retirement occurred.
Primary master remained unchanged; review documents were prepared only in the
detached worktree after frozen execution. Saved repository configuration stayed
unchanged. Correctness is not fully validated because of the listed failures and
unbuilt cases. Physical Intel, other hosts/toolchains, native FP16/SVML,
unselected APIs and application integrations remain outside coverage.

## Post-execution inspection artifact

A final inspection omitted Python `-B` and created
`tools/__pycache__/capture_source.cpython-311.pyc` after all native executions.
Capture verification rejected the extra file. No original source, protected
root/vendor/pin byte or test result changed. The generated cache was retained
with its original path, timestamp and SHA256 under ignored `build/`; no evidence
was deleted. Complete 497-file capture verification then passed again with the
same fingerprint. No native test was rerun or pass transferred.
The [supplement receipt](windows-arch-separation-2026-11-19/post-review-supplement.json)
records `post-review-evidence-supplement.zip`, SHA256
`823ced8f40623153f0c7c94a0a0d8a573cb00c3b24af0bf7b104e9f03a74123b`. Its CRC and all recorded hashes passed.
The supplement also retains the earlier review draft/checksums. The original
6,216-file raw packet and its SHA256 remain unchanged.
