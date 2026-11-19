# Native ARM validation and retained evidence

## Dependency audit published and cleanup completed — 19 November 2026

The [publication/cleanup report](dependency-audit-publication-2026-11-19.md)
records the verified ledger/deduplication push at
`2f9837dcd5fe3d5b94c5abc5af82dedf54c54eb9` and removal of 52.6 MiB from eight
generated control/smoke build directories after complete archive coverage checks.
Originals, dependency bytes, source captures, fixtures, tools, testbench, history
and evidence remain preserved. [Portable receipts](dependency-audit-history-2026-11-19/publication/README.md)
retain actual counts without changing earlier native results or their failed
Windows full gate. No new native matrix or benchmark was run.

## Dependency revision ledgers and VCL deduplication — 19 November 2026

The [new layout/provenance record](dependency-audit-history-2026-11-19.md) covers
the root [x86](../../audit_x86_history.md) and [ARM64 compatibility](../../audit_arm64_history.md)
ledgers, relocation of the unchanged VCL hash manifest, removal of duplicate
vendor originals and verified offline reconstruction of a comparison snapshot.
The sse2neon dependency bytes are unchanged; its actual upstream snapshot and
retained representation differences are now recorded. This is not a new native
matrix or benchmark pass, and historical source inventories remain unchanged.

## Four-blend integration and next M2 task — 19 November 2026

The maintainer approved the [four-blend integration](blend4-integration-2026-11-19.md)
and publication of its [Windows results](windows-blend4-handoff-2026-11-19/results/README.md).
The full gate remains failed/incomplete: 5,387 passed, 249 failed, 76 not run of
5,712, plus twelve passing examples. The selected preservation checks passed;
Agner's original x86 files remain exact. The
[M2 performance follow-up](m2-trs-inverse-follow-up-2026-11-19.md) starts from
integrated implementation/report commit `ccde9e43674b77975d1f375d1ecff02aa761aef3`
to investigate and recover the measured 2.2% / 2.7% TRS-inverse costs while
preserving transpose gains. Those costs remain unresolved. The first push was
verified at `a526fc4bd5ffd21d436893d4522cae469d3a7e5f`; cleanup removed about
15.43 GiB and two temporary worktrees after complete coverage checks. See the
[portable completion receipts](windows-blend4-handoff-2026-11-19/integration/README.md).
Earlier entries retain their historical scope and source identities.

## Native Windows four-blend follow-up — 19 November 2026

The [new Windows report](windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md) records the exact frozen Git candidate:
5,387 passed, 249 failed, 76 not run of 5,712, plus twelve examples. The full gate
remains failed/incomplete. All case statuses match history; seven additional
Debug timeout outcomes were checked against current control failures and paired
RTC diagnostics. The bounded assessment passed: 96 isolated cases and 96 selected
code comparisons, with protected originals/flags/controls intact. The M2
small-transpose gain versus TRS-inverse elapsed cost remains a retain/reject
decision for the maintainer. No source fix, timing, cleanup or publication occurred.

## Published master and M2 return preparation — 19 November 2026

The maintainer separately approved pushing the integrated master. The push
completed and `origin/master` was verified at
`5e039b01b49abc00548123763be85898932e3ad8`. The
[new M2 handoff](m2-arm-planning-handoff-2026-11-19.md) prepares a fresh
worktree at that exact base, source/dependency/tool preflight and a bounded ARM
proposal using retained evidence. Its initial scope is planning, with no runtime
change, native matrix, new timings, cleanup or publication. The earlier native
results and failed Windows gate retain their separate identities.

## Integration and cleanup completed — 19 November 2026

The [completion report](architecture-separation-integration-completed-2026-11-19.md) records local master
integration, exact protected-source checks and verified cleanup of about 15 GiB.
The Windows gate remains failed/incomplete; its reports and source identities
are unchanged. Original archives, frozen source, tools, pinned testbench, branches
and unrelated work remain. Next: choose a bounded ARM/Neon task from this base.

## Approved integration — 19 November 2026

The maintainer approved local integration and evidence-backed cleanup under the
[dated acceptance decision](architecture-separation-integration-2026-11-19.md). X86 acceptance preserves
the pinned original implementation and verifies the selected facade boundary.
The Windows gate remains failed/incomplete: 5,387 passed, 249 failed and 76 not
run out of 5,712 instances, plus twelve passing examples. Historical results and
source identities remain unchanged. Further development starts with a bounded
ARM/Neon task; no x86 repair or new runtime change is included.

## Native Windows failure triage — 19 November 2026

The [dated triage](windows-amd-2026-11-19-arch-separation-triage.md)
classifies all 249 failed and 76 unavailable original instances. All 76 missing
targets reproduce compile failures through pristine originals; 20 runtime pairs
reproduce matching failure statuses. No selected facade-specific regression was
found. The MSVC Debug timeout was traced to a modal RTC report for original
uninitialized branch temporaries. Documented lookup read-ahead and other contract
differences are separated from inherited valid-input issues. The original gate
remains failed; acceptance scope must be reviewed before integration. Protected
source and old evidence remain exact. No source fix or publication occurred.

## Native AMD Windows follow-up — 19 November 2026

The [new Windows report](windows-amd-2026-11-19-arch-separation.md)
records approved publication `2d251bc` on unchanged frozen source:
baseline 5231/5,392 passed, 133 failed,
28 not run; higher ISA 156/320 passed,
116 failed, 48 not run.
The correctness gate is failed/incomplete. Protected originals/vendor/pins stayed
exact, and 12 facade/direct-original code/output/include pairs passed.
Mac/publication source identities remain separate. This dated follow-up supersedes
earlier pending Windows status; historical text and evidence remain unchanged.
No source fix or publication action was performed.

Current planning starts with [ARM64 / Neon lessons](../neon-arm-lessons.md) and
the [strategy reset](../integration-plan.md), requested on 18 November 2026.
The old x86 repair/optimization campaign is retired. The original strategy
cleanup was documentation only; the later architecture implementation and ARM
header cleanup below have their own frozen-source results.
The approved next implementation and native-check sequence is in the
[Mac architecture separation handoff](mac-arch-separation-handoff.md).
The [new M2 report](mac-arm-2026-11-18-arm-header-cleanup.md) records actual
uncommitted-source validation; native Windows verification remains pending.

## ARM header cleanup — 18 November 2026

The [follow-up M2 report](mac-arm-2026-11-18-arm-header-cleanup.md) records **8,036/8,036** native CTest passes
and **8/8** examples after removing obsolete x86 workarounds/branches from nine
ARM headers. All eight Release/strict UBSan profiles passed with zero failures,
skips or timeouts. The [audit](arm-header-cleanup-audit-2026-11-18.md) distinguishes
removed x86 fixes from the SSE-shaped sse2neon interface and ARM fixes retained.
All 18 preprocessing comparisons are byte-identical before/after; originals,
vendor and pin inventory stayed exact. New frozen fingerprint:
`5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`. [Portable evidence/checksums](arm-header-cleanup-m2-2026-11-18/README.md) identify its own run.
The implementation/reports are published on `arch-separation-m2`; Windows is pending via the updated
[return handoff](windows-arch-separation-handoff.md). Earlier candidate 04 and
all its evidence remain historical, without transferring its result.

## Architecture-separated M2 candidate — 18 November 2026

Candidate 04 passed 8,036/8,036 native CTest instances and
8/8 examples across all eight Release/strict UBSan profiles.
The [dated report](mac-arm-2026-11-18-arch-separation.md) and
[portable inventory/checksums](arch-separation-m2-2026-11-18/README.md) identify
base `1a52264` separately from the 454-file frozen implementation fingerprint
`cef817d707baa6033fe70c223538c17daea4152ca85cd16a274e12ea490633a2`.
All 28 restored root originals, vendor bytes and pin inventory stayed exact;
ARM traces/installed-header consumers and guard/strict UBSan controls passed.
The pre-run rejection and two interrupted build-failed captures remain retained.
No earlier pass was transferred or numerical gate relaxed. See the report for
all actual attempts; this is not a pristine first-attempt migration.

Candidate 04 was uncommitted when tested; it is retained as historical evidence.
The later cleanup implementation is published on the topic branch; master is unchanged.
The [Windows return handoff](windows-arch-separation-handoff.md) requires the same
reviewed source, original-byte/include isolation and honest inherited-failure
reporting. The [publication source comparison](arch-separation-publication-2026-11-18/README.md)
records the later documentation differences. No benchmark or evidence cleanup was performed.

## Earlier native M2 checkpoint — 17 November

The [17 November correctness report](mac-arm-2026-11-17-ab5bb87-correctness.md)
tested `ab5bb8754a33724e05a170fa2867475077efd023`, tree
`48376a53dfa5f5f2139651e3d44f5e1a9ed841b1`: **8,036/8,036 CTest passes plus
eight examples**. Compatibility maximum 512 and Direct Neon maxima 128/256/512
each passed Release and strict nonrecovering UBSan Debug without retries,
skips or timeouts. Direct Neon remains experimental.

The [portable M2 bundle](mac-arm-correctness-2026-11-17-ab5bb87/README.md)
retains complete test inventories, raw source hashes, flags, guard/sanitizer
controls, vendor snapshots and archive receipts. All 28 vendor files remained
byte-exact. The report records native arm64 execution, 16,384-byte pages,
Apple UBSan's actual negative exit and the ASan coverage gap.

Raw Mac archives remain on the Mac as recorded by its report. This Windows
cleanup retains portable Mac evidence; it does not claim to have removed or
verified the Mac's host-local archives. Original reports/checksums are unchanged.

## Other ARM findings to preserve

- [Earlier layout checkpoint](mac-arm-2026-11-17-a099098-x86-layout.md): M2
  correctness at `a099098`, with pristine-vendor and generation evidence.
- [Frozen shared checkpoint](mac-arm-2026-11-16-bd7807e-final.md) and
  [validator follow-up](mac-arm-2026-11-16-bd7807e-validator-followup.md): native
  M2 execution and all 32 selected cross-host pairings under explicit policies.
- [Source-byte audit](source-byte-audit-2026-11-16-bd7807e.md) and
  [source inventories](cross-platform/simd-parity-20261116/source-manifests/README.md):
  verified newline reconstructions, preserving original fingerprints.
- [Original native Mac report](mac-arm-2026-11-15.md): earlier execution and its
  sleep-related replay, kept as a historical checkpoint.
- [Shared corpus and policies](cross-platform/README.md): reusable comparisons
  with independent references and declared FP differences, not a universal
  bitwise-equality claim.
- [M2 performance report](../benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md),
  [historical measurements](../benchmarks/STANDARD_MATH_RESULTS_2026-11-13.md)
  and [reading guide](../benchmarks/standard_math_reading_guide.md).

## Procedures and current limits

The [native workflow](native-workflow.md), [test guidance](../../tests/README.md)
and [report template](report-template.md) remain reference procedures. Choose
an agreed frozen source before a new run. Old execution handoffs are retired;
they are not a request to test or continue x86 fixes during this reset.
Preserve original numerical gates, independent fixtures, exceptional inputs,
60-second deadlines, concurrency three and actual-page guard controls.
New source, API, compiler or backend changes need relevant fresh native checks.

Coverage is limited to recorded sources, selected APIs and hosts. Native FP16,
ASan on the documented Mac host, exception-mask emulation, complete scalar-libm
semantics and application-level 3D workloads were not established by these passes.

## Historical x86 records

These remain provenance only, separate from the new ARM agenda:

- [Retained x86 findings and fix candidates](../x86_findings_and_fix_candidates.md):
  evidence-qualified discoveries, existing remedies, limitations, archive recovery
  and suggestions for future upstream review; no new x86 execution is authorized.

- [Windows layout checkpoint](windows-amd-2026-11-16-x86-layout.md),
  [ISA triage](windows-amd-2026-11-17-isa-triage.md) and
  [correctness completion](windows-amd-2026-11-17-correctness-completion.md).
- [Original Windows review](windows-amd-2026-11-15-823ca2a.md),
  [AVX-512 report](windows-avx512-2026-11-15.md) and
  [frozen Windows counterpart](windows-amd-2026-11-16-bd7807e-final.md).
- [Initial x86 performance report](../benchmarks/x86-upstream-2026-11-16-ade9d9b.md)
  and [older optimization report](../benchmarks/x86-optimization-2026-11-16.md).
- [Windows procedure reference](windows-runner.md) and
  [historical comparison protocol](../benchmarks/x86-upstream-comparison.md).

The unpublished final Windows performance report/data and prior uncommitted
documentation are retained in the local pre-cleanup documentation archive.
See the [18 November cleanup record](arm-strategy-cleanup-2026-11-18.md) for
archive locations and worktree/output removals. No commit or push is performed
by this cleanup.
