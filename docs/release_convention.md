# VCL2 XA64 release convention

Decided 22 November 2026. The first fork release targets **VCL2 XA64 1.0.0**,
with **`xa64-v1.0.0-rc.1`** available as the first release candidate and
**`xa64-v1.0.0`** as the final release. This document records the agreed naming,
release description and treatment of known limitations. Select candidate or
final status and the exact commit when authorizing publication. Recording this
convention does not create a tag or authorize publishing a release.

## Names and versions

Use **`xa64-vMAJOR.MINOR.PATCH`** for fork release tags. The `xa64-` prefix
distinguishes them from the original VCL's `v2.*` tags. Use
**`VCL2 XA64 MAJOR.MINOR.PATCH — Description`** for release titles.

| Purpose | Example tag |
| --- | --- |
| First release candidate | `xa64-v1.0.0-rc.1` |
| Further candidate for 1.0.0 | `xa64-v1.0.0-rc.2` |
| First final release | `xa64-v1.0.0` |
| Compatible fix | `xa64-v1.0.1` |
| Compatible feature addition | `xa64-v1.1.0` |
| Optional future release candidate | `xa64-v1.1.0-rc.1` |
| Further candidate for that release | `xa64-v1.1.0-rc.2` |
| Breaking public API change | `xa64-v2.0.0` |

The version portion follows [Semantic Versioning](https://semver.org/): patch
for compatible fixes, minor for compatible additions, and major for incompatible
public API changes. A final 1.0 release establishes the documented public API
and support commitments. Published release versions and tags must remain
immutable; subsequent changes receive a new version.

The fork version is independent of **upstream VCL 2.02.04**. Keep the upstream
version and [source pin](../audit/vectorclass-2.02.04.json) visible in release
notes. Do not edit protected upstream version definitions to match a fork tag.

## First-release decision and future candidates

The first release may be **candidate 1.0.0-rc.1** or **final 1.0.0**, with the
documented API and support scope defined by [backend coverage](backends.md) and
[numerical contracts](numerical-contracts.md). Use `xa64-v1.0.0-rc.1` for candidate
publication and mark it as a prerelease; use `xa64-v1.0.0` for final publication.
The known limitations below are accepted within that declared release scope;
the Windows correctness gate retains its actual failed/incomplete status.

For future versions, a release candidate is available when seeking feedback
before confirming the intended API and support contract. Mark those candidate
releases as prereleases.

The Windows failure total alone does not determine candidate versus final
status. Preserved upstream limitations do not automatically imply that the fork
as a whole is nonfunctional. They remain real correctness and coverage limits
that release notes must explain. Direct Neon remains explicitly experimental
in the final 1.0.0 release; that version does not silently
promote every backend or API to production support.

## Describe x86 preservation and Windows findings accurately

Lead with the fact that the fork uses **Agner Fog's unchanged VCL 2.02.04 x86
implementation**. All 28 protected original files remain byte-for-byte identical
to [upstream revision `667577495efaa36a10cbb5d6cb8c942c23913848`](https://github.com/vectorclass/version2/commit/667577495efaa36a10cbb5d6cb8c942c23913848).
Link to [Agner's repository](https://github.com/vectorclass/version2), the local
audit pin and [backend ownership](backend-layout.md). ARM64 implementations are
maintained separately; the fork's facade, build and validation infrastructure
are identified separately from those original implementation files.

The [22 November native x86 comparison](benchmarks/x86/x86-upstream-2026-11-22.md)
provides bounded upstream/fork evidence:

- 816 comparison rows and 1,632 independent finite-workload benchmark checks
  completed, with zero output-hash disagreements.
- No row was consistently slower or faster across all four paired invocations
  under the declared comparison rule. This is workload-specific evidence,
  rather than a whole-library equivalence or statistical-significance claim.
- Targeted SSE2 FP64 rounding failures reproduced in both pristine upstream and
  the fork under both compilers: 42 failed lane checks per SSE2 source/compiler
  selection. These are separate from the passing finite benchmark checks.

The broader [Windows correctness collection](validation/x86/windows-x86-validation-2026-11-22.md)
remains **failed/incomplete: 6,796 passed, 235 failed and 109 not run of 7,140
registered cases, plus fifteen passing examples**. Explain the result:

- Failures include numerical edge cases and strict UBSan diagnostics in original
  headers, including signed integer overflow. Upstream provenance does not make
  an observed correctness or undefined-behavior issue harmless.
- Not-run cases represent unavailable executables following build failures.
  They are coverage gaps and must not be counted as passes.
- Case counts repeat across compiler, profile and ISA configurations. The 235
  failed instances do not mean 235 distinct defects, nor do these results alone
  diagnose a fault in AMD hardware.
- Source identity and selected matching checks support x86 preservation. Do not
  claim that every full-suite failure has been individually reproduced against
  pristine upstream; that complete one-to-one comparison has not been established.
- Keep the gate failed/incomplete. Passing examples, matching outputs, inherited
  behavior and successful benchmark checks do not convert it into a full pass.

Use current evidence for future releases. Keep older dated checkpoints and
their gate counts separate; do not transfer their passes to a release commit.

## First-release description

Release title: **VCL2 XA64 1.0.0 — Unchanged x86 VCL and native ARM64 support**.
For the first candidate, use **VCL2 XA64 1.0.0-rc.1 — Unchanged x86 VCL and native ARM64 support**
and identify it as a release candidate.
The following release description uses the 22 November evidence. Verify that
it applies to the selected release commit before publication.

VCL2 XA64 combines Agner Fog's original VCL 2.02.04 x86 implementation with
native ARM64 support through the shared `vcl2_xa64.h` interface. The protected
x86 implementation remains byte-for-byte unchanged from the pinned upstream
source.

ARM64 offers sse2neon compatibility by default and an explicitly enabled,
experimental Direct Neon backend. The supported common surface includes portable
integer, FP32 and FP64 operations with logical 128/256/512-bit vectors. Neon
hardware registers remain 128 bits. Native FP16, SVML and ARM64EC are outside
the supported common surface.

Native Apple M2 validation passed **8,036 cases plus eight arithmetic smoke
runs**, covering compatibility maximum 512 and Direct Neon maxima 128/256/512
in Release and strict nonrecovering UBSan Debug. The published performance session
contains **160 comparisons: 65 Direct Neon leads, 94 within the documented 5%
band and one sse2neon lead**. Results depend on operation, layout, logical width
and working-set size; the comparison band is separate from statistical significance.
[M2 correctness](validation/arm64/mac-arm-validation-2026-11-22.md) ·
[M2 performance](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md).

Native AMD Windows comparisons against pristine upstream produced matching
outputs across the selected finite benchmark workloads, with no consistent
performance regression under the recorded protocol. Targeted rounding limitations
also reproduced in both sources. The broader Windows suite exercises numerical
edge cases, strict floating-point contracts and undefined-behavior checks,
recording limitations of the unchanged implementation under the tested compiler
profiles alongside unavailable build targets. The gate remains failed/incomplete:
**6,796 passed, 235 failed and 109 not run, plus fifteen passing examples**.
These findings document the validated scope and remaining limits; they do not
establish that the library as a whole is nonfunctional.
[Windows correctness](validation/x86/windows-x86-validation-2026-11-22.md) ·
[Upstream/fork comparison](benchmarks/x86/x86-upstream-2026-11-22.md).

Validation reports identify the exact tested source commits: M2 correctness and
performance at `c120ded4321a087339a952ef0671fb89c456ca14`, and the Windows
collections above at `08f2d2903d9bfdfdb22aa187329e99ce0f44d177`. Later reporting
corrections and release publication do not establish a new native pass or
cross-host parity at a release tag.

All credit for the original Vector Class Library belongs to **Agner Fog**.
ARM64 compatibility also builds on earlier compatibility work and the
**sse2neon contributors**. Original licenses and attribution are preserved;
see [provenance and third-party notices](third-party.md).

## Publication scope

Before publishing `xa64-v1.0.0-rc.1` or `xa64-v1.0.0`, select the exact release commit and review its
documented evidence and remaining gaps against the agreed support scope.
The naming convention and target version are decided; candidate versus final
status is selected explicitly for publication. Follow the
[root README standards](validation/arm64/results-publication-checklist.md#root-readme-performance-table-standard)
when updating its summaries. Tagging, creating or publishing a release, committing
and pushing require task-specific maintainer authorization. Preparing this
document performs none of those actions.
