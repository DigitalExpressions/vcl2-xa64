# VCL2 XA64 release convention

Decided 22 November 2026. The first fork publication is **VCL2 XA64
1.0.0-rc.1**, tagged **`xa64-v1.0.0-rc.1`** and marked as a **prerelease**.
The tag targets the documentation commit containing this finalized convention
and the approved release description below. The maintainer authorized updating
and pushing this convention before creating the candidate. Final
**`xa64-v1.0.0`** remains a separate future publication.

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

The first publication is **candidate 1.0.0-rc.1**, with the documented API and
support scope defined by [backend coverage](backends.md) and
[numerical contracts](numerical-contracts.md). Publish `xa64-v1.0.0-rc.1` as a
prerelease. Use `xa64-v1.0.0` for a separately authorized final publication.
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

- **Tag:** `xa64-v1.0.0-rc.1`
- **Title:** **VCL2 XA64 1.0.0-rc.1 — Unchanged x86 VCL and native ARM64 support**
- **Status:** Prerelease

The following release description is approved for the first candidate.

<!-- release-notes:start -->
This first release candidate of **VCL2 XA64** brings native ARM64 support to Agner Fog’s Vector Class Library through the shared `vcl2_xa64.h` interface.

The **x86 implementation remains byte-for-byte identical to upstream VCL 2.02.04**. ARM64 implementations are maintained separately, preserving the original x86 sources and attribution.

**Included**

- Shared VCL types and supported APIs across x86 and ARM64.
- Default ARM64 compatibility backend using **sse2neon**.
- Explicitly enabled, experimental **Direct Neon** backend.
- Portable integer, FP32 and FP64 operations with logical 128/256/512-bit vectors.
- Documented numerical contracts, source-integrity checks and native validation procedures.

**Native Apple M2 validation**

The recorded matrix passed **8,036 cases plus eight arithmetic smoke runs**, covering compatibility and Direct Neon in Release and strict nonrecovering UBSan Debug.

The performance collection contains **160 comparisons: 65 Direct Neon leads, 94 within the documented 5% band, and one sse2neon lead**. Gains depend on operation, layout, logical width and working-set size; these classifications are separate from statistical significance.

**x86 preservation and known limitations**

Native AMD Windows comparisons against pristine upstream completed **1,632 independent finite-workload benchmark checks with zero output-hash disagreements**, across 816 comparison rows. No row showed a consistent performance regression under the recorded protocol. Targeted SSE2 rounding failures reproduced in both sources.

The broader Windows correctness gate remains **failed/incomplete: 6,796 passed, 235 failed and 109 not run of 7,140 cases**, plus fifteen passing examples.

**Note:** Findings include numerical **edge cases**, UBSan diagnostics in original headers and unavailable build targets. Counts repeat across configurations and do not represent 235 distinct defects. Not every failure has been individually compared against pristine upstream.

**Release scope**

Direct Neon remains experimental. Native FP16, SVML and ARM64EC are outside the supported common surface. Validation results belong to the exact source checkpoints identified in their reports; this release publication does not establish a new native pass.

This candidate invites feedback on integration, the shared interface and the documented backend behavior before final 1.0.

All credit for the original Vector Class Library belongs to **Agner Fog**. Original licenses, earlier compatibility-work attribution and **sse2neon contributor** notices are preserved.
<!-- release-notes:end -->

## Validation source identities

The release description refers to the
[M2 correctness report](validation/arm64/mac-arm-validation-2026-11-22.md) and
[M2 performance report](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
at `c120ded4321a087339a952ef0671fb89c456ca14`, and the
[Windows correctness report](validation/x86/windows-x86-validation-2026-11-22.md)
and [upstream/fork comparison](benchmarks/x86/x86-upstream-2026-11-22.md)
at `08f2d2903d9bfdfdb22aa187329e99ce0f44d177`. Reporting corrections and
publication do not establish a new native pass or cross-host parity at the tag.
See [provenance and third-party notices](third-party.md) for retained attribution.

## Publication scope

The maintainer selected the first candidate, its title, the release description
above and the documentation update commit as its tag target on 22 November 2026.
Push that commit to the fork's `origin/master` before publishing the candidate.
Verify the tag target and exact release body, mark the release as a prerelease,
and preserve the original source and evidence identities.

Future candidates and the final release require their own explicit version,
commit and publication authorization. Follow the
[root README standards](validation/arm64/results-publication-checklist.md#root-readme-performance-table-standard)
when updating its summaries. Recording a convention alone does not authorize
tagging, releases, commits or pushes; use the maintainer's task-specific request.
