# ARM / Neon strategy reset

On **18 November 2026**, the maintainer requested cleanup and a complete strategy
change, retaining what was learned about Neon on Mac ARM. The next implementation
strategy is recorded in the [architecture separation plan](arch_separation_plan.md):
pristine original VCL files at root for x86, separate `_neon.h` files for ARM,
and the agreed compile-time-selected public include `vcl2_xa64.h`.
The maintainer approved publishing the documentation, then implementing and
validating on Mac M2, followed by same-source native AMD Windows verification.
The [Mac implementation handoff](validation/mac-arch-separation-handoff.md)
defines the initial scope. The `arch-separation-m2` topic publication implements
that separation. The [new M2 report](validation/mac-arm-2026-11-18-arm-header-cleanup.md) records actual
frozen-candidate results after ARM header cleanup, retained attempts and the pending native Windows gate.
No old native pass transfers to the candidate.

The [retained x86 findings and fix candidates](x86_findings_and_fix_candidates.md)
preserve prior discoveries, canonical repair references, source identifiers and
evidence recovery steps for optional future x86 work or upstream discussion.
This does not resume the retired campaign or permit changes to protected originals.

Start with [ARM64 / Neon lessons](neon-arm-lessons.md). The original `1a52264` publication was documentation only. The Mac
implementation restores all originals and separates ARM ownership without
resuming the x86 repair/optimization campaign.

## Current decisions

- Focus the next planning discussion on Mac ARM/Neon behavior, architecture
  boundaries and useful tests. Retire the x86 correctness/optimization agenda.
- Keep the pristine upstream directory and pin inventory untouched. Preserve
  attribution, dependency notices and the distinction between current code and
  any future design.
- Preserve reusable ARM tests, independent references, numerical gates,
  guard-page controls and native evidence. Do not weaken expected results.
- Retain factual dated reports and their checksum inventories. Old handoffs
  and execution plans are historical and do not authorize new runs or fixes.
- Publish the reviewed implementation, cleanup and M2 reports on
  `arch-separation-m2`, as subsequently approved by the maintainer. Keep `master`,
  remotes and published history intact. Merges, PRs, branch retirement and
  additional source changes are outside this publication.
- Remove completed local worktrees and rebuildable output only after verified
  evidence retention. Preserve the installed tools and primary pinned testbench.

## Evidence worth carrying forward

The earlier [native M2 report](validation/mac-arm-2026-11-17-ab5bb87-correctness.md)
records 8,036 passes and eight examples at `ab5bb87`, across compatibility and
all three Direct Neon maxima, Release/strict UBSan. Its
[portable evidence](validation/mac-arm-correctness-2026-11-17-ab5bb87/README.md)
preserves the exact source, inventories, flags and limitations. These passes
do not transfer to a future implementation.

The [M2 vector/matrix measurements](benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md)
and [numerical contracts](numerical-contracts.md) retain useful ARM findings.
The [validation index](validation/README.md) keeps selected cross-host source
and policy evidence separate from universal bitwise identity. Original port
integration is recorded in the [historical review](integration-review.md).

The [cleanup report](validation/arm-strategy-cleanup-2026-11-18.md) records local
removals, retained raw archives, source snapshots and integrity checks. The
unpublished Windows performance report/data is archived locally, rather than
promoted into the new ARM documentation agenda.

## Restart point

The approved implementation/results publication is on `arch-separation-m2`;
primary `master` remains at documentation publication `1a52264`. The
[M2 cleanup result](validation/mac-arm-2026-11-18-arm-header-cleanup.md) records
cleanup candidate 01: 8,036/8,036 native CTest instances and 8/8 examples across
all eight Release/strict UBSan profiles. Candidate 04 is an earlier layout result.

The [publication inventory](validation/arch-separation-publication-2026-11-18/README.md)
compares every captured path. Runtime, tests, tools, fixtures and build files
match the M2 capture exactly; publication differences are documentation and
metadata. The Mac did not execute the later publication commit itself.

Use the [Windows return handoff](validation/windows-arch-separation-handoff.md)
at the exact published commit supplied with the Windows prompt. It supports a
fresh detached Git worktree and separate published-source capture; the original
Mac source ZIP remains an alternative. Record actual Windows identity and every
verified metadata/newline difference. Preserve inherited original-x86 failures;
no repairs, relaxed gates or benchmark timings are part of this sequence.
Raw evidence, failed attempts, worktrees and tools are retained. No merge, PR,
branch retirement or evidence deletion is part of this publication.
