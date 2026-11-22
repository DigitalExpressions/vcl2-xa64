# Native validation status and procedures

## Native M2 collection and local artifact availability — 22 November 2026

On 22 November 2026 the authorized local Mac build reset discarded 40,072 files totaling 4,649,286,799 bytes, including historical M2 archives, source captures, raw logs, binaries, benchmark output dumps and the Clang study outputs. Earlier identifiers, hashes and numerical results describe their original checkpoints. Their old local payloads are no longer available. The fresh collection used clean commit `c120ded4321a087339a952ef0671fb89c456ca14`, with 8,036 cases and eight smokes passed, followed by 18 timing invocations and all 160 report rows.

[Fresh correctness report](arm64/mac-arm-validation-2026-11-22.md) · [Fresh performance report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)


| Architecture | Documentation |
| --- | --- |
| ARM64 / macOS | [Native M2 reports and decisions](arm64/README.md) |
| x86 / Windows | [Windows runner, reports and results](x86/README.md) |
| Shared | [Native workflow](native-workflow.md), [report template](report-template.md), [cross-platform comparison procedure](cross-platform/README.md) |

After a completed Mac validation and performance round, follow the
[ARM64 results publication checklist](arm64/results-publication-checklist.md)
for copy destinations, evidence, checksums and README updates.
For a completed native Windows round and any requested separate performance
tests, use the [x86 results publication checklist](x86/results-publication-checklist.md).
At the start of each new x86 validation request, ask whether to include
performance tests and record the answer; earlier rounds' choices do not carry over.

## Current master - 22 November 2026

Master retains runtime d796768 and the accepted four-blend implementation,
including its small-transpose gain and accepted 2.2% / 2.7% TRS-inverse costs.
The [completed TRS review](arm64/trs-inverse-decision-2026-11-20.md) declines
candidate 3b4af0d unchanged. No old handoff is a pending task.

Applications include `vcl2_xa64.h`. Use [backend ownership](../backend-layout.md)
for the separated include graphs and protected original bytes. Every result
below belongs to its recorded source; a later documentation commit acquires
no native pass. Fresh cross-platform validation requires a new frozen source,
verified architecture-specific include graphs and the declared shared contracts.

## Latest retained native M2 validation

- [22 November current-master correctness report](arm64/mac-arm-validation-2026-11-22.md):
  8,036 cases plus eight arithmetic smoke runs passed at clean commit
  `c120ded4321a087339a952ef0671fb89c456ca14`, tree
  `b02601fc5a04dbdc41e224c1e04d2b987955150a`, using Apple Clang 21 / SDK 26.5.
  Compatibility maximum 512 and Direct Neon maxima 128/256/512 each passed
  Release and strict nonrecovering UBSan Debug. The separate
  [full standard-math report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
  and [supporting bundle](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/checksums.sha256)
  retain all 160 rows, controls and exact source identities.
- [Earlier 21 November correctness report](arm64/mac-arm-validation-2026-11-21.md):
  8,036 selected cases plus eight arithmetic smoke runs passed at clean commit
  `dc802e466663d61d9e219bd7c7351aa8ac312204`, tree
  `7eaae08ad9b0a813ace5c74aff9be6ff2fc7630b`, using Apple Clang 21 / SDK 26.5.
  Compatibility maximum 512 and Direct Neon maxima 128/256/512 each passed
  Release and strict nonrecovering UBSan Debug, with zero failures, skips,
  timeouts or retries. The [full standard-math report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
  followed separately; source, controls, integrity and archive receipts share
  its [supporting bundle](../benchmarks/arm64/standard-math-master-2026-11-21/checksums.sha256).
- [Four-blend acceptance](arm64/blend4-integration-2026-11-19.md) records the
  retained implementation, performance costs and separate Windows evidence.
- [19 November matched M2 performance report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md)
  and [historical reference checkpoint](../benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md)
  retain workload-specific results, slower rows and variability. Future timing
  comparisons need fresh matched sessions.

The 21 November native archives, benchmark builds and focused Clang study were
discarded in the authorized 22 November reset. Its
[dated availability update](arm64/mac-arm-validation-2026-11-21.md#native-m2-collection-and-local-artifact-availability--22-november-2026)
supersedes the earlier recovery receipts. Fresh 22 November artifacts have their
own source and availability records. Earlier retired campaigns keep their own
availability limits in frozen Git.

## Windows gate and architecture preservation

The latest [22 November native Windows report](x86/windows-x86-validation-2026-11-22.md)
records validation of clean commit `08f2d2903d9bfdfdb22aa187329e99ce0f44d177`:
**6,796 passed, 235 failed and 109 not run out of 7,140 registrations**, plus
fifteen passing canonical examples, with zero timeouts or skipped cases.
All 7,140 named registrations reconciled with the 20 November collection;
six FP-helper statuses changed in the two MSVC AVX2 rows. Optimized Release,
strict Release and strict UBSan remain separately reported. The correctness
gate remains failed/incomplete; raw evidence and archive receipts are identified
in the dated report. Later documentation commits do not inherit this execution.

The earlier [21 November report](x86/windows-x86-validation-2026-11-21.md)
retains its own tested source `dc802e4`, 6,798 passed, 233 failed and 109 not run,
with all 7,140 case statuses matching the 20 November collection.

The historical 20 November Windows test-profile collection recorded native AMD
validation of its 594-file integrated capture:

| Profile | Passed | Failed | Not run | Registered |
| --- | ---: | ---: | ---: | ---: |
| upstream-optimized | 2,737 | 81 | 38 | 2,856 |
| strict-validation | 2,739 | 79 | 38 | 2,856 |
| strict-ubsan | 1,322 | 73 | 33 | 1,428 |

All fifteen canonical examples passed. All 7,140 registered case statuses,
including failures, timeouts and unavailable executables, were recorded.
The two optimized MSVC signed-zero failures remain failed. The completed
integration report and its portable bundle are retired from the current checkout;
their original records remain in Git history. All original runtime files remain
byte-exact; these profiles do not replace the earlier full gate.

The historical full gate stays **failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. Bounded preservation,
include/code checks and RTC diagnostics do not convert failures into passes.

- [Frozen separation matrix, triage and evidence](#x86-separation-documentation-retirement).
- [Current backend acceptance rules](../backend-layout.md#validation-and-source-identity).
- [Latest portable Windows results](x86/windows-x86-validation-2026-11-22/README.md).
- [Frozen Windows return for the declined TRS candidate](https://github.com/DigitalExpressions/vcl2-xa64/tree/b5a671a72fe59888ce0a661741a2b65c6b02a4b6/docs/validation/trs-inverse-current-master-2026-11-19).

## Reusable procedures and contracts

Use the [x86 build and test guide](../x86-build-and-test.md),
[Windows runner](x86/windows-runner.md), [native workflow](native-workflow.md),
[test guidance](../../tests/README.md), [numerical contracts](../numerical-contracts.md),
[benchmark correctness contracts](../../benchmarks/README.md#correctness-contracts)
and [report template](report-template.md) for a newly agreed frozen source.
The Windows runner defaults to optimized Release; strict Release comparisons
and strict Clang UBSan remain separate, and MSVC RTC is deferred and rejected.

Keep independent fixtures, seeds, numerical/FP gates, strict nonrecovering
UBSan, actual-page guards/canaries, 60-second deadlines and concurrency three.
Direct Neon stays experimental. Physical Intel, unselected hosts/toolchains/APIs,
native FP16, ARM64EC, ASan and external consumer validation remain outside
recorded coverage. Tests, corpora, comparators and runners remain maintained.

## Historical document retirement

Completed comparisons, dependency-audit paperwork and cleanup chronology use
frozen Git references. The [full historical index](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/README.md#historical-document-retirement)
preserves the original reasons, counts, hashes and evidence locations. Folder
moves and their navigation updates are committed. Retired results keep their
original source identities and do not establish validation of a later tree.

## Windows documentation cleanup

The [frozen cleanup index](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/README.md#windows-documentation-cleanup)
records the earlier retirement of obsolete Windows documentation. Current native
Windows reports, compiler policy, runners and publication checklist remain
local; failures and coverage limits remain explicit. No Windows evidence or
status label was changed by this ARM documentation consolidation.

## x86 separation documentation retirement

The completed job's 60 local documentation files were retired on 21 November.
The [full retirement record](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/README.md#x86-separation-documentation-retirement)
preserves their frozen report/bundle links, verified byte identities and raw
archive coverage. Current acceptance limits and failed gate outcomes remain
summarized above; that retirement removed no runtime, test or raw build evidence.

## ARM64 documentation retirement

After the 21 November full native pass, the older M2 correctness report and its
complete 13-file packet were retired locally. Their
[frozen report](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/arm64/mac-arm-2026-11-19-blend4.md)
and [complete packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/6458749cd9405025251aae112d962053577cfdc5/docs/validation/arm64/m2-blend4-2026-11-19)
retain the original source, cases, controls and hashes. The useful bit-level
contract and accepted costs remain in the [blend decision](arm64/blend4-integration-2026-11-19.md).
The intermediate [relocation receipt](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/relocation-2026-11-20.json)
is also historical. All 15 retired files were verified byte-exact in that
published source before removal. Retained 19 November benchmark comparisons,
the final TRS decision and the live cross-platform procedure remain available.
No runtime, test, numerical gate or raw evidence was removed in this pass.

## Documentation layout

Current Mac/M2 records use `arm64/`; Windows records use `x86/`; shared
procedures use this level and `cross-platform/`. Raw host evidence remains
under ignored `build/`; Git contains the documented portable records and
relative links. Use the architecture publication checklists for new results.

## Build reset and fresh Windows collection — 2026-11-22

Fresh native AMD Windows collection at clean 08f2d2903d9bfdfdb22aa187329e99ce0f44d177: 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen passing examples. The gate remains failed/incomplete.
The [reset report](x86/windows-x86-validation-2026-11-22.md) records the deliberately discarded prior local Windows payloads and freshly reconstructed resources. Earlier archive-retention statements in historical Windows records no longer establish local availability on this host. Other hosts were not changed. [Separate performance](../benchmarks/x86/x86-upstream-2026-11-22.md) followed its own checks.
