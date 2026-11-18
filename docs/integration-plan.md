# ARM / Neon strategy reset

On **18 November 2026**, the maintainer requested cleanup and a complete strategy
change, retaining what was learned about Neon on Mac ARM. The next implementation
strategy is recorded in the [architecture separation plan](arch_separation_plan.md):
pristine original VCL files at root for x86, separate `_neon.h` files for ARM,
and the agreed compile-time-selected public include `vcl2_xa64.h`.
The maintainer approved publishing the documentation, then implementing and
validating on Mac M2, followed by same-source native AMD Windows verification.
The [Mac implementation handoff](validation/mac-arch-separation-handoff.md)
defines the initial scope. No source implementation has started here.

The [retained x86 findings and fix candidates](x86_findings_and_fix_candidates.md)
preserve prior discoveries, canonical repair references, source identifiers and
evidence recovery steps for optional future x86 work or upstream discussion.
This does not resume the retired campaign or permit changes to protected originals.

Start with [ARM64 / Neon lessons](neon-arm-lessons.md). This publication contains
documentation and workspace-cleanup records, with **no library, adaptation,
backend, vendor, test, fixture, CMake or tool source changes**. Existing x86 fixes
stay in the checkout for now; the Mac migration removes them from the x86 path
by restoring originals, without resuming their repair/optimization campaign.

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
- Keep `master`, remotes, branches and published history intact. Step 1 has
  commit/push approval for the prepared documentation only. Mac source/results
  publication, merges, PRs and branch retirement are not authorized.
- Remove completed local worktrees and rebuildable output only after verified
  evidence retention. Preserve the installed tools and primary pinned testbench.

## Evidence worth carrying forward

The latest [native M2 report](validation/mac-arm-2026-11-17-ab5bb87-correctness.md)
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

1. Publish the reviewed cleanup documentation, separation plan, retained x86
   findings and Mac handoff on `master`; approval granted on 18 November 2026.
2. On Mac, fetch the exact documentation publication and execute the handoff
   on a new topic branch, preserving local work. Keep vendor/pin exact; restore
   protected originals and separate ARM headers behind `vcl2_xa64.h`.
3. Freeze the complete uncommitted implementation and run fresh native M2
   correctness checks. Prepare portable results and the Windows return handoff.
4. Review the concrete implementation/results before any source commit/push.
   Then validate the same exact implementation on native AMD Windows, MSVC first
   and Clang second. Preserve inherited upstream failures as actual findings.
5. Measure target-specific performance only under separate explicit scope.

No implementation or new benchmark work has started under the strategy reset.
The Windows source remains unchanged by this documentation publication. The
cleanup receipt records its original pre-publication state and remains historical.
