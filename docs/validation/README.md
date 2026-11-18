# Native ARM validation and retained evidence

Current planning starts with [ARM64 / Neon lessons](../neon-arm-lessons.md) and
the [strategy reset](../integration-plan.md), requested on 18 November 2026.
The old x86 repair/optimization campaign is retired. No source was changed by
the cleanup, and no previous pass establishes a future design's correctness.
The approved next implementation and native-check sequence is in the
[Mac architecture separation handoff](mac-arch-separation-handoff.md).
M2 execution and subsequent same-source Windows verification are pending.

## Latest native M2 checkpoint

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
