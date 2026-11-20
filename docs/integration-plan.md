# Current integration status — 20 November 2026

Architecture separation is complete and integrated into master. The retained
runtime is d796768b30cb375ea62afa72dcab061b1a3e54ca; documentation cleanup does
not establish a fresh native pass at a later commit.

Applications include `vcl2_xa64.h`. The 28 original VCL root files remain exact
pinned upstream bytes. ARM implementations live in root `*_neon.h` files and
`instrset_detect_neon.cpp`; compatibility stays default and Direct Neon remains
an explicit experimental opt-in. See [ownership](backend-layout.md), the
[file map](arch-separation-map.json), [x86 pin](../audit_x86_history.md) and
[ARM64 dependency history](../audit_arm64_history.md).

Master retains the [accepted four-blend implementation](validation/arm64/blend4-integration-2026-11-19.md),
its small-transpose gain and known inverse cost. The later frozen TRS-recovery
candidate 3b4af0d is declined unchanged; the [completed decision](validation/arm64/trs-inverse-decision-2026-11-20.md)
records its measured costs and remaining uncertainty.

The full Windows gate remains **failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. Bounded x86 preservation
and matching diagnostics do not convert that gate into a pass. Retained records:

- [Architecture integration acceptance](validation/architecture-separation-integration-2026-11-19.md)
  and [completion](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/architecture-separation-integration-completed-2026-11-19.md).
- [Windows matrix](validation/x86/windows-amd-2026-11-19-arch-separation.md) and
  [targeted triage](validation/x86/windows-amd-2026-11-19-arch-separation-triage.md).
- [19 November M2 correctness](validation/arm64/mac-arm-2026-11-19-blend4.md) and
  [current-master timing checkpoint](benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md).
- [Dependency audit/publication](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-publication-2026-11-19.md).

Each result keeps its exact tested source; separate native passes do not prove
new cross-host parity. Original x86 limitations remain; the x86 repair campaign
is retired. Canonical tests, fixtures, numerical/FP gates, guard controls, tools
and pinned dependencies remain in place.

## Restart point

The [x86 MSVC/Clang build and test guide](x86-build-and-test.md) records the
20 November optimized compiler policy and standalone recipes. The owned
profile-aware harness is integrated and published on master, with
explicit strict Release/UBSan profiles and RTC rejection. The
[integration record](validation/x86/windows-x86-harness-integration-2026-11-20.md)
identifies fresh validation of the integrated capture; earlier results keep
their source identities. Agner's originals and all runtime source remain unchanged.
The maintainer subsequently approved commit and push; the harness was published
at 2237ffbdc23b23cda4358acf29cd494b2d63ad8c. The separately requested
[Windows documentation cleanup](validation/README.md#windows-documentation-cleanup)
retires obsolete pre-19 November reports while preserving current instructions,
source proofs and raw evidence. Later publication needs its own authorization.
Fresh collection is complete: 7,140 registered cases plus fifteen passing
canonical examples, with every failed/not-run outcome retained. The linked
integration report separates the optimized, strict Release and UBSan totals
and preserves both tested source identities. This remains a nonpassing collection.

New runtime work needs an agreed bounded scope, frozen source and fresh relevant
native validation under the [maintained workflow](validation/native-workflow.md).
Use the [validation index](validation/README.md) and [ARM lessons](neon-arm-lessons.md).
Obsolete local Mac artifacts and completed planning/publication paperwork were
retired without replacement archives; the [cleanup report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md)
records current availability. History remains in Git. No old handoff is a pending
task. Commits, pushes, integrations and branch retirement require task-specific
authorization; this cleanup performs none.
