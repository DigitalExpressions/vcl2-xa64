# Native validation status and procedures

## Current master — 20 November 2026

Keep runtime d796768; the frozen TRS-recovery candidate 3b4af0d is declined
unchanged. The [completed TRS review](trs-inverse-decision-2026-11-20.md)
records inverse recovery, material dot-product costs and unresolved variability.
Master retains the accepted four-blend implementation, its small-transpose gain
and known inverse cost. Only the primary master worktree remains on the Mac;
branches and source history are retained. No new native pass follows from cleanup.

The [pre-19 November Mac documentation cleanup](mac-arm-documentation-cleanup-2026-11-20.md)
removes superseded reports, bundles and execution plans. The few older records
listed below remain because they support retained source/comparison claims.
Historical links use frozen Git revisions; they are not pending tasks.

## Latest retained native M2 validation

- [19 November four-blend correctness report](mac-arm-2026-11-19-blend4.md):
  8,036 selected cases plus eight examples passed across compatibility maximum
  512 and Direct Neon maxima 128/256/512, each Release and strict UBSan Debug.
  The result belongs to its 571-file uncommitted capture, fingerprint
  3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36;
  tested commit/tree are null. [Portable results](m2-blend4-2026-11-19/README.md)
  preserve names, source identities, flags, controls and checksums.
- [Accepted integration](blend4-integration-2026-11-19.md): relates the reviewed
  implementation and Windows results to integration; no later commit inherits
  a native pass merely from publication.
- [Matched M2 performance report](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19.md)
  and [current-master reference checkpoint](../benchmarks/standard-math-master-checkpoint-2026-11-19/README.md):
  retain workload-specific timing, slower rows and variability separately from
  correctness. New work needs fresh matched timings, not ratios against history.

Raw Mac sessions, source copies, test binaries and archives named in historical
reports were deliberately discarded where listed by the 20 November cleanup.
Their original receipts describe historical availability. Git-retained portable
records remain; exact deleted binaries/raw trials cannot be inspected locally.

## Windows gate and architecture preservation

The full gate stays **failed/incomplete: 5,387 passed, 249 failed, 76 not run
of 5,712, plus twelve passing examples**. Bounded movement, include/code and RTC
diagnostics do not convert failures or timeouts into passes.

- [Architecture matrix](windows-amd-2026-11-19-arch-separation.md) and
  [targeted triage](windows-amd-2026-11-19-arch-separation-triage.md).
- [Four-blend Windows return](windows-blend4-handoff-2026-11-19/results/README.md)
  and [integration acceptance](architecture-separation-integration-2026-11-19.md).
- [Architecture integration completion](architecture-separation-integration-completed-2026-11-19.md)
  and [dependency audit/publication](dependency-audit-publication-2026-11-19.md).
- [Windows preservation return for declined TRS candidate](https://github.com/DigitalExpressions/vcl2-xa64/tree/b5a671a72fe59888ce0a661741a2b65c6b02a4b6/docs/validation/trs-inverse-current-master-2026-11-19).

## Reusable procedures and contracts

Use [native workflow](native-workflow.md), [test guidance](../../tests/README.md),
[benchmark correctness contracts](../../benchmarks/README.md#correctness-contracts), [numerical contracts](../numerical-contracts.md),
[backend ownership](../backend-layout.md), [Windows runner](windows-runner.md)
and [report template](report-template.md) for a newly agreed frozen source.
Keep independent fixtures, seeds, numerical gates, FP flags, strict nonrecovering
UBSan, actual-page guards/canaries, 60-second deadlines and concurrency three.
Direct Neon is experimental; compatibility stays default. Physical Intel,
unselected hosts/toolchains/APIs, native FP16, ARM64EC, ASan and consumer workload
validation remain outside the documented scope. Do not resume retired handoffs.

## Older records still needed

- [Shared Windows/Mac corpus procedures and completed comparison](cross-platform/README.md),
  [Mac captures](cross-platform/simd-parity-20261116/mac-m2/README.md),
  [validator follow-up](cross-platform/simd-parity-20261116/comparisons/mac-validator-02/README.md)
  and [source-byte audit](source-byte-audit-2026-11-16-bd7807e.md): unique evidence
  for the 32 selected cross-host pairings and reconciled 135-path inventories.
  These prove only the recorded checkpoint, not cross-host parity at master.
- [Mixed Windows cleanup record](arm-strategy-cleanup-2026-11-18.md): retains
  Windows archive/recovery information beyond this Mac documentation scope.
- [Retained x86 findings](../x86_findings_and_fix_candidates.md) index earlier
  Windows investigations. Windows reports and checksummed payloads are retained.
