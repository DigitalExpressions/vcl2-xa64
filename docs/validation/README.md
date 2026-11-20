# Native validation status and procedures

| Architecture | Documentation |
| --- | --- |
| ARM64 / macOS | [Native M2 reports and decisions](arm64/README.md) |
| x86 / Windows | [Windows runner, reports and results](x86/README.md) |
| Shared | [Native workflow](native-workflow.md), [report template](report-template.md), [cross-platform comparison procedure](cross-platform/README.md) |

## Current master - 20 November 2026

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

- [19 November four-blend correctness report](arm64/mac-arm-2026-11-19-blend4.md):
  8,036 selected cases plus eight examples passed across compatibility maximum
  512 and Direct Neon maxima 128/256/512, each Release and strict UBSan Debug.
  The tested implementation is a 571-file uncommitted capture, fingerprint
  3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36;
  tested commit/tree are null. [Portable results](arm64/m2-blend4-2026-11-19/README.md)
  retain source identities, names, flags, controls and checksums.
- [Four-blend acceptance](arm64/blend4-integration-2026-11-19.md) records the
  retained implementation, performance costs and separate Windows evidence.
- [Matched M2 performance report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md)
  and [current-master reference checkpoint](../benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md)
  retain workload-specific results, slower rows and variability. Future timing
  comparisons need fresh matched sessions.

Obsolete Mac sessions, source copies, test binaries and archives identified by
[the closed cleanup record](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md#completed-local-workspace-cleanup)
were discarded on 20 November. Their original receipts describe historical
availability; exact discarded binaries/raw trials cannot be reinspected locally.
The current portable results above remain available.

## Windows gate and architecture preservation

The [20 November integrated x86 harness report](x86/windows-x86-harness-integration-2026-11-20.md)
records native AMD Windows validation of its 594-file integrated capture:

| Profile | Passed | Failed | Not run | Registered |
| --- | ---: | ---: | ---: | ---: |
| upstream-optimized | 2,737 | 81 | 38 | 2,856 |
| strict-validation | 2,739 | 79 | 38 | 2,856 |
| strict-ubsan | 1,322 | 73 | 33 | 1,428 |

All fifteen canonical examples pass. All 7,140 registered case statuses,
including failures, timeouts and unavailable executables, remain recorded.
The two optimized MSVC signed-zero failures remain failed. All original runtime
files remain byte-exact; these profiles do not replace the earlier full gate.

The retained full gate stays **failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. Bounded preservation,
include/code checks and RTC diagnostics do not convert failures into passes.

- [Architecture matrix](x86/windows-amd-2026-11-19-arch-separation.md) and
  [targeted triage](x86/windows-amd-2026-11-19-arch-separation-triage.md).
- [Four-blend Windows return](x86/windows-blend4-handoff-2026-11-19/results/README.md)
  and [architecture acceptance](architecture-separation-integration-2026-11-19.md).
- [Portable integrated harness results](x86/windows-x86-harness-integration-2026-11-20/README.md).
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

On 20 November the maintainer requested retiring local documents whose
relevance is now historical. **179 files** were retired with citations redirected
to immutable Git objects at e55c54a. Each retired path was verified in that
published source; complete associated portable packets were retired together.
The earlier benchmark and validation folder moves remain in the working tree.

Historical sources:

- [Pre-separation 16 November comparison packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/cross-platform/simd-parity-20261116),
  [source-byte audit](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/source-byte-audit-2026-11-16-bd7807e.md),
  [Mac report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-2026-11-16-bd7807e-final.md),
  [Windows report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-amd-2026-11-16-bd7807e-final.md)
  and [validator follow-up](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-2026-11-16-bd7807e-validator-followup.md).
  These establish the old checkpoint's 32 selected comparisons and 135-path
  provenance only. They provide no validation of today's separated backends.
- [Completed architecture integration/cleanup](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/architecture-separation-integration-completed-2026-11-19.md)
  and [receipts](https://github.com/DigitalExpressions/vcl2-xa64/tree/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/architecture-separation-integration-2026-11-19).
  The acceptance decision and unresolved gate remain local above.
- [Dependency audit](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19.md),
  [publication/cleanup](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-publication-2026-11-19.md)
  and [portable receipts](https://github.com/DigitalExpressions/vcl2-xa64/tree/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19).
  Current provenance remains in the [x86 ledger](../../audit_x86_history.md),
  [ARM64 ledger](../../audit_arm64_history.md) and protected audit manifests.
- [Superseded Windows harness report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20.md)
  and [complete packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20).
  Their source identities, failures and comparisons remain in frozen Git; the
  current integrated report and evidence remain local.
- [Mac cleanup history](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md),
  [Windows archive-recovery report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/arm-strategy-cleanup-2026-11-18.md)
  and [archive map](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/arm-strategy-cleanup-2026-11-18.json).
  These retain original reasons, counts, hashes and recovery locations.

Only local documentation and portable historical packets were retired.
Raw Windows evidence/archives, reusable tools, canonical tests, source captures,
installed dependencies and unrelated work were preserved. All retained portable
payloads keep their original bytes; affected navigation/checksum entries were
updated. No native matrix, timing, runtime change, commit or push occurred.

Before-retirement SHA-256 identities for retained inventories changed by
navigation or availability edits (after the earlier folder moves):

| Inventory | Before-retirement SHA-256 |
| --- | --- |
| `docs/validation/arm64/m2-blend4-2026-11-19/checksums.sha256` | `4a7c75a525997d3f86cc9c04a90415e226af24f9e4d3b699b59f6a23a2f6921e` |
| `docs/validation/x86/windows-blend4-handoff-2026-11-19/integration/checksums.sha256` | `04872759590b594cdc8a7f7a2e911f8b8772a5855bbeb563a49bf5eadf0248cc` |

## Windows documentation cleanup

The earlier retirement of **81 tracked Windows documentation files** is recorded
in the [frozen cleanup index](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/README.md#windows-documentation-cleanup).
Its original findings, packets and checksum identities remain reachable there.
The later historical retirement above also closes its former local exceptions
for pre-separation comparisons and archive-recovery documentation. Current
Windows procedures and the retained separated-backend gates remain local.

## Architecture folders - 20 November 2026

Current Mac/M2 records use `arm64/`; Windows records use `x86/`; shared
procedures use this level and `cross-platform/`. The earlier
[relocation receipt](relocation-2026-11-20.json) describes the intermediate layout
before historical retirement. Its original hashes retain that stage's meaning.
