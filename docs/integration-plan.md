# Current integration status — 22 November 2026

The current backend layout is integrated into master. The retained
runtime is d796768b30cb375ea62afa72dcab061b1a3e54ca; documentation cleanup does
not establish a fresh native pass at a later commit.

Applications include `vcl2_xa64.h`. The 28 original VCL files remain exact
pinned upstream bytes: 27 at root and the upstream README under `audit/`.
Root `README.md` is the fork guide. ARM implementations live in root `*_neon.h` files and
`instrset_detect_neon.cpp`; compatibility stays default and Direct Neon remains
an explicit experimental opt-in. See [ownership](backend-layout.md), the
[x86 pin](../audit_x86_history.md) and
[ARM64 dependency history](../audit_arm64_history.md).

Master retains the [accepted four-blend implementation](validation/arm64/blend4-integration-2026-11-19.md),
its small-transpose gain and known inverse cost. The later frozen TRS-recovery
candidate 3b4af0d is declined unchanged; the [completed decision](validation/arm64/trs-inverse-decision-2026-11-20.md)
records its measured costs and remaining uncertainty.

The latest [22 November Windows collection](validation/x86/windows-x86-validation-2026-11-22.md)
at clean commit `08f2d2903d9bfdfdb22aa187329e99ce0f44d177` remains
**failed/incomplete: 6,796 passed, 235 failed and 109 not run of 7,140,
plus fifteen passing examples**. All named registrations reconciled with the
20 November collection; six FP-helper statuses changed in the two MSVC AVX2
rows. There were zero timeouts or skipped cases.

The earlier [21 November collection](validation/x86/windows-x86-validation-2026-11-21.md)
retains its own tested source `dc802e4`, 6,798 passed, 233 failed and 109 not run,
with all case statuses matching the 20 November collection.

The historical 19 November Windows gate remains **failed/incomplete: 5,387
passed, 249 failed, 76 not run of 5,712, plus twelve passing examples**.
Bounded x86 preservation and matching diagnostics do not convert that gate
into a pass. Retained records:

- [Current backend ownership and acceptance rules](backend-layout.md#validation-and-source-identity).
- [21 November Windows validation](validation/x86/windows-x86-validation-2026-11-21.md),
  with the earlier [separation records retained in frozen Git](validation/README.md#x86-separation-documentation-retirement).
- [22 November M2 correctness](validation/arm64/mac-arm-validation-2026-11-22.md) and
  [current-master performance results](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md),
  with the [earlier 21 November checkpoint](validation/arm64/mac-arm-validation-2026-11-21.md)
  retained at its own tested source.
- [Dependency audit/publication](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-publication-2026-11-19.md).

Each result keeps its exact tested source; separate native passes do not prove
new cross-host parity. Original x86 limitations remain; the x86 repair campaign
is retired. Canonical tests, fixtures, numerical/FP gates, guard controls, tools
and pinned dependencies remain in place.

## Restart point

The [x86 MSVC/Clang build and test guide](x86-build-and-test.md) records the
20 November optimized compiler policy and standalone recipes. The owned
profile-aware harness is integrated and published on master, with
explicit strict Release/UBSan profiles and RTC rejection. The completed
20 November integration records are retired from the current checkout and
remain in Git history with their original source identities.
Agner's originals and all runtime source remain unchanged.
The maintainer subsequently approved commit and push; the harness was published
at 2237ffbdc23b23cda4358acf29cd494b2d63ad8c. The separately requested
[Windows documentation cleanup](validation/README.md#windows-documentation-cleanup)
retires obsolete pre-19 November reports while preserving current instructions,
source proofs and raw evidence. Later publication needs its own authorization.
The latest clean-source collection is recorded in the
[22 November report](validation/x86/windows-x86-validation-2026-11-22.md), with
every failed/not-run outcome retained and optimized Release, strict Release and
UBSan totals separated. Earlier dated reports keep their own tested source
identities and remain nonpassing collections.

New runtime work needs an agreed bounded scope, frozen source and fresh relevant
native validation under the [maintained workflow](validation/native-workflow.md).
Use the [validation index](validation/README.md) and [ARM lessons](neon-arm-lessons.md).
The [20 November cleanup report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md)
records earlier discarded Mac campaigns. The 21 November native worktree was
initially archived, but those archives, performance builds and the Clang study
were discarded in the authorized 22 November reset. Its
[dated availability update](validation/arm64/mac-arm-validation-2026-11-21.md#native-m2-collection-and-local-artifact-availability--22-november-2026)
records that loss; fresh 22 November results retain their own evidence.
Completed planning/publication paperwork remains in frozen
Git. No old handoff is a pending task. Commits, pushes, integrations and branch
retirement require task-specific authorization; this cleanup performs none.

## Fresh Windows reset checkpoint — 2026-11-22

Fresh native AMD Windows collection at clean 08f2d2903d9bfdfdb22aa187329e99ce0f44d177: 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen passing examples. The gate remains failed/incomplete.
The [new report](validation/x86/windows-x86-validation-2026-11-22.md) identifies this source separately from dc802e4 and records the authorized loss of all old local build payloads. Historical result values remain unchanged; their hashes cannot reconstruct discarded local evidence.

## Native M2 collection and local artifact availability — 22 November 2026

On 22 November 2026 the authorized local Mac build reset discarded 40,072 files totaling 4,649,286,799 bytes, including historical M2 archives, source captures, raw logs, binaries, benchmark output dumps and the Clang study outputs. Earlier identifiers, hashes and numerical results describe their original checkpoints. Their old local payloads are no longer available. The fresh collection used clean commit `c120ded4321a087339a952ef0671fb89c456ca14`, with 8,036 cases and eight smokes passed, followed by 18 timing invocations and all 160 report rows.

[Fresh correctness report](validation/arm64/mac-arm-validation-2026-11-22.md) · [Fresh performance report](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
