# Native ARM validation and retained evidence

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
