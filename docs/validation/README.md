# Native validation status and procedures

## Current master — 20 November 2026

Keep runtime d796768; the frozen TRS-recovery candidate 3b4af0d is declined
unchanged. The [completed TRS review](trs-inverse-decision-2026-11-20.md)
records inverse recovery, material dot-product costs and unresolved variability.
Master retains the accepted four-blend implementation, its small-transpose gain
and known inverse cost. Inspect actual refs and worktrees before new work;
retired validation branches are not prerequisites. No new native pass follows
from cleanup.

The [pre-19 November Mac documentation cleanup](mac-arm-documentation-cleanup-2026-11-20.md)
removes superseded reports, bundles and execution plans. The few older records
listed below remain because they support retained source/comparison claims.
Historical links use frozen Git revisions; they are not pending tasks.

The [Windows x86 documentation cleanup](#windows-documentation-cleanup)
retires 81 tracked files, with old x86 findings and comparison results retained
through frozen Git. Current Windows instructions and 19–20 November results
remain; the older exceptions below retain unique
comparison, source or evidence-recovery information.
Historical reports and bundle indexes preserve their original execution-time
status, including pending notes later closed by follow-ups. Use this index and
the maintained procedures for today's status; do not resume old handoffs.

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

The [20 November integrated x86 harness report](windows-x86-harness-integration-2026-11-20.md)
records fresh native AMD Windows validation of the 594-file integrated capture:

| Profile | Passed | Failed | Not run | Registered |
| --- | ---: | ---: | ---: | ---: |
| upstream-optimized | 2,737 | 81 | 38 | 2,856 |
| strict-validation | 2,739 | 79 | 38 | 2,856 |
| strict-ubsan | 1,322 | 73 | 33 | 1,428 |

All fifteen canonical examples pass; timeouts remain separately
reported in the complete results. All 7,140 registered case statuses are compared
with the earlier 20 November capture. The two optimized MSVC signed-zero failures
remain failed, and all original runtime files remain byte-exact. These new
profiles do not replace the historical four-profile gate below. The earlier
20 November report and portable bundle retain their exact source/checksums.

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

Use the [x86 MSVC/Clang build and test guide](../x86-build-and-test.md) and
[integrated harness record](windows-x86-harness-integration-2026-11-20.md) for
current profiles and actual results. The Windows runner defaults to optimized
Release; strict comparisons and UBSan stay separate, and MSVC RTC is rejected.
The [earlier 20 November harness results](windows-upstream-optimized-2026-11-20.md)
retain their original capture and checksums; publication does not transfer passes.

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
  the verified recovery map and checksums for retained Windows raw evidence.
- [16 November Windows checkpoint](windows-amd-2026-11-16-bd7807e-final.md):
  identifies the native suite and corpus source behind the selected cross-host
  comparison. Its original pending notes are historical; use the completed
  comparison and validator follow-up above for the final outcome.
- [Retained x86 findings](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md) index earlier
  Windows investigations through frozen historical links. The retired patched
  x86 campaign is separate from today's pristine-original path.
- The retired [16 November optimization report](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-optimization-2026-11-16.md),
  [benchmark packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-optimization-2026-11-16)
  and [candidate source packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/validation/x86-optimization-2026-11-16)
  remain in frozen Git. Their local copies were removed with approval. These
  records concern the old patched implementation, not current-master performance.
- The earlier [upstream-comparison report](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-upstream-2026-11-16-ade9d9b.md)
  and [12-file packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-upstream-2026-11-16-ade9d9b-final)
  were also retired locally with approval. Their measured checkpoint remains in
  frozen Git; the reusable comparison guide and tools remain in the checkout.

## Windows documentation cleanup

On 20 November 2026 the maintainer requested removal of obsolete pre-19 November
Windows x86 validation material, then approved consolidating three further
documents, retiring the local x86 findings index and removing the closed
optimization campaign. This retires **81 tracked files**: five 15 November Windows reports,
the 16 November layout report, two 17 November correctness reports and their
25-file bundles, the retired optimization-plan stub, two bundle README
indexes and the x86 findings document, followed by the 31 remaining files in the
16 November optimization report, benchmark packet and candidate source packet,
then the 13-file earlier upstream-comparison report/packet.
The standalone cleanup draft was folded into this section. Original tracked documents remain in
[frozen Git at 2237ffb](https://github.com/DigitalExpressions/vcl2-xa64/tree/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs).

The removed campaigns concern the earlier patched x86 implementation and retired
generated-root layout. Current pristine-original results and maintained build
instructions supersede them for new work. Useful x86 findings remain indexed
above through frozen Git; unique cross-host proofs and archive recovery
receipts remain locally for the stated purposes. All four archives in the 18 November
cleanup receipt passed renewed SHA-256 and ZIP CRC verification. Raw evidence
and failures were preserved; old source-manifest path strings still describe
their original frozen trees.

The optimization report and both complete packets, including their checksum
inventories, are now retired together. Their original measurements, labels,
patches and source identities remain in frozen Git. Raw archives, reusable
benchmark tools and current M2 checkpoints were preserved. The earlier Windows
comparison report and its complete packet were subsequently retired together;
their original records remain in frozen Git.
Original navigation identities for the two retired packets:

| Bundle | Original README SHA-256 | Original checksum-inventory SHA-256 |
| --- | --- | --- |
| docs/benchmarks/x86-optimization-2026-11-16 | 11ccf7bae073c22c7805baa662d3360f3cc88f0cf3f95c70648f240e24b5b518 | 508f97cf490f49d402713232a6d433077adead17214bb72821e47fb81ae771cf |
| docs/validation/x86-optimization-2026-11-16 | fe3cb7c4461f88636f61e1fe28b65621fe625b495ab731779d4282ad35c240a5 | 00c2a1c3b7da9c597b1a37a42276497c016869f71114ecee3c7acaf1b70dbd00 |

The 19 November architecture report replaces only its findings link with the
frozen historical reference; only that report's entry in the companion checksum
inventory was refreshed. Original report SHA-256:
86906115517f4618a9c4b269a76788a4713a94f3314ce2f20db8e96308531e32.
Original checksum-inventory SHA-256:
9316c53d5c16597e81534fcf9aafc397094a43ee596fa71ff248f4c2ca566c07.
Both originals remain at 2237ffb. All recorded results, captured source identities
and other payload/hash entries are unchanged.

Historical citations resolve through frozen Git; current links use maintained
guides and retained results. Attribute rules exclusive to deleted files were
retired. Protected originals, runtime code, tests, fixtures, tools, dependencies
and saved Git configuration are unchanged. Repository/link, whitespace,
retained-checksum and read-only integrity checks cover this documentation-only
change. No native matrix or timing was run. The Windows gate stays
failed/incomplete as recorded above; cleanup establishes no new native pass.
