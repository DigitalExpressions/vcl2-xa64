# Windows x86 test harness integration - 20 November 2026

## Outcome and integration scope

The seven owned CMake/runner/profile-control files are integrated in local
master's working tree, byte-exact with the earlier validated harness. Windows
routine testing defaults to optimized MSVC/GNU-driver Clang Release; strict
Release comparisons and strict Clang UBSan remain separately named. MSVC Debug/
RTC is deferred and rejected before test execution. No popup handler is installed.
Agner's original x86 files and all 98 C++ source/header files remain unchanged.

The integrated frozen source completed all fifteen selected native AMD Windows
configurations: **7,140 registered cases plus fifteen passing canonical examples**.
The new full collection remains nonpassing, with every failure/not-run outcome
preserved below. This is not an all-API correctness or upstream-official-test claim.

The source is an uncommitted capture based on master
`8067ff3e4f6d52849261308b31f70542a0d9a10f`, tree `9391853629a92e05c3c93ee193398410ed140627`.
Its 594-file fingerprint is `d86cf67d40f938082c6887e2095524389d7c8d961277112e77a886e46cbcc184`;
patch SHA256 `411ffb86d7d894da2c13cf2d490b281225111678fdb0dd0994350ea1002f8e94`. Tested commit/tree are null: the
base does not identify the uncommitted tested implementation. A later commit or
report edit does not inherit a native pass. The complete
[source comparison](windows-x86-harness-integration-2026-11-20/source-comparison.json) proves all prior executable
inputs and the seven harness files match the earlier 20 November checkpoint.
Existing metadata checkout-EOL differences are identified analytically; no
protected byte was normalized. The final report/index metadata follows execution.

The [earlier report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20.md) and its portable
bundle retain their original bytes and separate 656-file fingerprint
`e6d83a55d7c8f535b928d4cfc709cad63b383c071b82cacb7d37aeb500877230`.
Current master keeps the declined-TRS decision recorded by the 20 November
documentation cleanup; its runtime was not replaced. No M2 performance judgment
or execution occurred here. The earlier report's planning state is historical.

## Complete new results

| Profile | Registered | Passed | Failed | Not run | Timeouts | Examples passed |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| upstream-optimized | 2856 | 2737 | 81 | 38 | 0 | 6 |
| strict-validation | 2856 | 2739 | 79 | 38 | 0 | 6 |
| strict-ubsan | 1428 | 1322 | 73 | 33 | 0 | 3 |

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| msvc | upstream-optimized | baseline | 1348 | 1329 | 16 | 3 | 0 |
| msvc | strict-validation | baseline | 1348 | 1331 | 14 | 3 | 0 |
| clang | upstream-optimized | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-validation | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-ubsan | baseline | 1348 | 1284 | 53 | 11 | 0 |
| msvc | upstream-optimized | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | upstream-optimized | avx512 | 40 | 13 | 27 | 0 | 0 |
| msvc | strict-validation | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | strict-validation | avx512 | 40 | 13 | 27 | 0 | 0 |
| clang | upstream-optimized | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | upstream-optimized | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-validation | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | strict-validation | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-ubsan | avx2 | 40 | 19 | 9 | 12 | 0 |
| clang | strict-ubsan | avx512 | 40 | 19 | 11 | 10 | 0 |

No cases are skipped; timeout counts are included in failures when present.
Each full pinned baseline registers 1,348 cases, including six Python fixture/
configuration checks. Each supported higher-ISA local profile registers 40.
Every canonical case keeps its 60-second deadline, concurrency three, seeds,
numerical gates, rounding/FP policy and protected-memory/canary controls.
All examples print `2 5 10 17`. The 6 setup smoke examples and two
standalone compiler-recipe consumers are additional controls, not canonical totals.

[Complete comparison with the prior harness](windows-x86-harness-integration-2026-11-20/integrated-vs-prior-harness.json)
finds **0 case-status differences across all 7,140 registrations**;
names are compared as complete inventories, not intersections.
[Optimized/strict comparisons](windows-x86-harness-integration-2026-11-20/optimized-vs-strict.json) preserve the
two optimized MSVC baseline signed-zero failures, `fp_helpers` and
`fp_helpers_namespace`; no waiver or changed expectation is applied. Captured
stdout differences remain recorded separately. Matching statuses are not proof
of universal output-bit identity. Portable JUnit stdout may be truncated;
full process and LastTest logs remain in raw evidence.

The historical Windows gate remains **failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples per historical source**.
These fresh profile results do not overwrite it. `NoInitFailures` describes
confirmed historical RTC uninitialized-use diagnostics, not intentional/safe
behavior inferred from a case name. RTC was not executed or converted to a pass.

## Native controls, flags and integrity

Native AMD Ryzen 9 7950X/Windows 11 x64, MSVC 19.50.35723 and GNU-driver Clang
20.1.8 targeting Windows MSVC-ABI x64 were verified, with native Python 3.11,
CMake/CTest 4.2.1 and Ninja 1.12.1. Installed tools and runtimes were reused and
hash-checked. Testbench `a7293702385e5169a5f58b83177f2a43cd59139b` remains
clean with all three source hashes verified before/after. No dependency update
or saved tool/repository configuration change occurred.

MSVC optimized ordinary targets use O2/C++17/precise/except- without RTC/Od.
Clang uses O2/C++17/no-trapping-math/no-math-errno. Strict profiles keep their
original FP/no-contraction/rounding controls. Fast/finite-math flags remain
classification-only stress. Actual compiler commands were audited before suite
build/execution. CPU CPUID and OS/XCR0 probes permitted AVX2/FMA and AVX-512
F/DQ/BW/VL/FMA; fresh probes and the original runtime detector preceded execution.

All fifteen native x64 PE/runtime/guard control sets pass, using actual 4,096-byte
pages, positive writable access and four expected protected access violations.
All three UBSan sets have positive exit zero, intentional overflow exit one,
the expected diagnostic and no continuation, with MultiThreaded release CRT,
O0 Debug, disabled recovery, frame pointers and halt-on-error settings.
Actual dependency graphs verify facade forwarding to root originals without
ARM/Neon/sse2neon, adaptation or inactive-backend headers. The existing testbench
adapter uses the facade too; this is not a new full pristine-consumer matrix.

[Integrity proofs](windows-x86-harness-integration-2026-11-20/integrity.json) cover protected bytes/inventory and
nanosecond mtimes, all runtime and harness files, both immutable replicas,
the earlier capture/records, tools/testbench, HEAD, branches/tags/worktrees and
saved repository configuration. Origin was confirmed as the DigitalExpressions
fork and fetched without pruning; no runtime/candidate merge or consumer edit occurred.

## Preserved failed setup and build attempts

The first profile-control attempt used a deeply nested immutable capture path.
MSVC's CMake ABI probe exceeded Windows path length, leaving pointer-size
detection unavailable; the native-x64 guard rejected configure. That failed
attempt, partial build and one passing development smoke example remain intact.
It executed **zero canonical cases**. A shorter frozen replica verified against
all 594 source hashes resolved setup without source, flag or test changes.
Both complete profile-control sets then passed. The initial PowerShell capture
shell reported native-stderr warning records; its completed source ZIP, CRC and
every source/archive-entry hash were independently verified before execution.

First build-abort return packets/false summaries are retained unchanged. Each
affected build has a separately logged Ninja `-k 0` continuation; all missing
binaries remain not run. No completed canonical case was replayed. Setup
positive/negative controls and standalone consumers are separately inventoried.

## Date scope, evidence and review

Only the seven 20 November harness changes, the earlier 20 November report/
portable records, and current instructions/results were integrated. No report,
execution plan or retired x86 fix older than 19 November was restored. Existing
protected upstream originals/pins, the frozen testbench and canonical fixtures
are necessary unchanged inputs. Older path names inside the exact earlier
source manifest identify its historical inventory; their files were not restored.

[Portable results and checksums](windows-x86-harness-integration-2026-11-20/README.md) include complete case statuses,
nonpassing captured outputs, source/flag/control/include/binary proofs and failed
setup metadata. The [raw archive receipt](windows-x86-harness-integration-2026-11-20/raw-evidence-receipt.json)
identifies `build/ui20-01-raw-2026-11-20.zip`, verified by CRC and every entry hash.
Raw evidence, both source replicas, first packets, generated inputs and binaries
remain under ignored `build/ui20-01/`. Compiled payloads are retained on disk
with exhaustive hashes; portable results contain no machine paths or binaries.

Commit approval is pending. No cleanup, commit, push, PR, benchmark, ASan,
emulation, hosted CI, ARM execution or external consumer change was performed.
Physical Intel, other toolchains/APIs and performance remain outside this run.
