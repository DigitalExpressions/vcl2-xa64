# Backend boundaries and x86 optimization plan

Requested 2026-11-16. Keep one public API and repository, with compile-time
x86 and ARM/NEON implementations. Optimize measured x86 paths without removing
correctness fixes or changing ARM defaults. Initial implementation excluded
publication; on 2026-11-16 the maintainer authorized committing and pushing the
prepared work for the M2 handoff. Merge/PR/release/cleanup are not authorized.
Native Intel remains omitted. Native M2 execution completed at the approved
publication checkpoint; see the dated result below.

The maintainer clarified that the architectural target is a pristine upstream
copy behind an explicit adaptation layer. The initial optimization-only scope
was too narrow. Keep an untouched pinned upstream vendor tree, versioned
adaptation patches/backend sources, and reproducible headers used by applications.
Existing correctness fixes remain explicit adaptations rather than edits to
the canonical upstream copy. The maintainer approved preserving direct header
usage without a required generation step. Current optimization validation
is a frozen baseline for the separation and must retain its original source IDs.

## Starting state and preserved work

The active checkout is `master` at
`44f974ac31e1448e0beb5157d2bd8d669f5c518e`, tree
`77f68cb6590ed95d6a812d7dff3436aff8f73b66`. Its changes since the earlier
`ade9d9b` benchmark baseline are documentation/instructions only. Previously
prepared benchmark files/results are staged, with related README/index edits
unstaged. That index and all historical reports/evidence were preserved during
implementation. Publication is now specifically authorized for this prepared
work; unrelated changes remain protected. Applicable instructions are in `AGENTS.md`.

Baseline: [upstream comparison](benchmarks/x86-upstream-2026-11-16-ade9d9b.md).
The largest candidates are MSVC classification (especially 512-bit FP64) and
512-bit FP32 exponential. SSE2 rounding has a demonstrated correctness fix;
retain it and the integer-conversion sentinel policy.

## Execution and restart checklist

- [x] Inspect HEAD/tree/status/worktrees and applicable instructions.
- [x] Save starting patches/index inventories under ignored `build/`.
- [x] Audit classification and exponential source/assembly. Classify edits as
      architecture adaptation, correctness fix, compiler workaround or refactor.
      Document the backend boundary; avoid duplicating the whole library.
- [x] Build isolated MSVC x86 candidate implementations in fresh ignored source
      snapshots. Use direct intrinsic register/mask operations where wrapper
      composition causes measured overhead. Preserve MSVC FPCLASS regression
      coverage and `/fp:fast` classification behavior.
- [x] Check screening candidates against independent classification/rounding references,
      existing regression tests and unchanged ordinary/finite-math FP modes.
      Retain every rejected candidate and its source hashes/logs.
- [x] Keep only justified changes. Benchmark frozen candidate bytes against the
      pre-change fork and pinned upstream, using the same harness/ISA/FP options
      and alternating repeated pairs. Identify dirty source by base commit,
      exact reviewed patch and source manifest, never by an invented commit.
- [x] Run native Windows MSVC Release/Debug and Clang Release/strict UBSan Debug
      with the pinned upstream selection; exercise changed higher-ISA paths,
      guard-page controls, fixtures and unchanged 60-second deadlines.
- [x] Record portable before/after results, raw archive checksums, coverage
      gaps and any remaining regressions. Preserve the original benchmark data.
- [x] Prepare exact candidate/patch verification and a native M2 handoff covering
      compatibility 512 and Direct NEON 128/256/512 Release/strict UBSan. Do not
      label that pending native run as passed from Windows compilation.
- [x] Complete the clarified separation: pristine upstream bytes and pin/hash
      inventory; separate adaptation patch/backend sources; deterministic
      regeneration/check tooling. Preserve public header names and direct use.
      Prove that generated compiled headers match the frozen optimized bytes;
      validate source provenance, stale-output detection and consumer usage.
- [x] Review source/doc diffs, links, whitespace and unchanged existing index.
      The complete result was left uncommitted for review; the maintainer has
      now authorized its commit/push for M2 validation.
- [x] Execute the approved `a099098` checkpoint natively on M2: 8,036/8,036
      cases and eight examples passed on the first attempt. Preserve pristine
      vendor bytes, generation controls and before/after integrity records.
      Publication of the Mac report/status documentation was authorized on 2026-11-17.

## Acceptance

Public headers/names, lane order, ABI settings, attribution and explicit Direct
NEON opt-in stay intact. Ordinary numerical gates, special values, rounding,
conversion boundaries, independent fixtures, guard controls and strict UBSan
must pass their selected inventories. No fast-math shortcuts in ordinary tests,
weakened tests, changed expected outputs, or hidden failed attempts.

Performance conclusions apply to named workloads/compiler/ISA/source bytes.
Retain slower and unstable rows. Inspect called helper bodies, not only wrapper
instruction counts. Do not promise universal upstream-equivalent performance
or extrapolate AMD results to Intel or ARM.

## Current restart point

The scoped implementation, Windows confirmation and native M2 pass are complete.
Read the [layout guide](backend-layout.md),
[native report](validation/windows-amd-2026-11-16-x86-layout.md)
and [performance report](benchmarks/x86-optimization-2026-11-16.md), plus the
[M2 follow-up](validation/mac-arm-2026-11-17-a099098-x86-layout.md).

- Pristine original VCL: 28 exact files under `vendor/vectorclass/`, pinned to
  `667577495efaa36a10cbb5d6cb8c942c23913848`; separate canonical patches/backend
  sources reproduce all 35 shipped runtime products exactly. Eight generation
  integrity controls and four root-only consumer checks pass.
- Frozen optimized runtime: base `44f974ac31e1448e0beb5157d2bd8d669f5c518e`
  plus patch `b954c2894ecac21ad59b3ca3d9ff10401f01fb8ac5552e96eeb4dc300c5855e4`,
  source fingerprint `1da758d8e17b4f50b60fca687a5dae2f52065b317aa71936f72466ae7375fc45`.
  The separate layout provenance identifies added canonical sources/tooling;
  no later whole repository revision inherits this tested source identity.
- Four full native Windows profiles pass 5,392/5,392 cases. Paired higher-ISA
  selections each have 190/320 passes and 130/320 failures on baseline and
  candidate, with no additional failed names. Existing Clang AVX2 UBSan failure
  and higher-ISA oracle/correctness gaps remain failing; no gate was weakened.
- Full before/after and original-upstream experiments retain 1,632 comparisons
  and 91,392 samples. Targeted MSVC workloads improve roughly 9.2x/10x versus
  the pre-change fork. All slower rows remain, with separate layout diagnostics.
- Raw root: `build/x86-optimization-20261116/`. `00-start/` preserves the initial
  index/patches; `01-classification/` and `02-exp-inline/` preserve screens;
  `03-frozen-source/` preserves exact source; `04-before-after/`, `05-upstream/`,
  `06-higher-paired/`, `07-root-only-consumer/` and both alignment attempts retain
  their records. The 2,741-entry compact supplement has SHA256
  `10c4c5bd728594c0df46766f406df6f2516d403364c75ab598c01bf3371bb109`.

No commits/pushes occurred during measurement or implementation; the maintainer
subsequently authorized publication of the prepared source/reports for M2. The
handoff publication commit is the clean combined Mac checkpoint, resolved as
documented in the handoff. Original Windows source identities remain unchanged.
No merge, PR, branch retirement or evidence cleanup is authorized. Keep the
detached candidate worktree and original evidence until cleanup is approved.

The M2 followed the [approved handoff](validation/mac-arm-x86-optimization-handoff.md)
at commit `a0990984d97e5232e36db5316f421b983b155b1c`, tree
`3a9815449ae8fd930821a2301a5d3744a99cc464`: all eight profiles passed without
failures, skips, timeouts or retries. The 28 pristine vendor files and pin
manifest retained their hashes and timestamps; all eight generation controls
and before/after checks passed. Original Windows source identities and failures
remain unchanged. The maintainer authorized publishing the Mac report/status
documentation on 2026-11-17.

Next, triage the retained Windows higher-ISA failures, distinguishing library
bugs from oracle ISA assumptions, before claiming broad higher-ISA readiness. Further source
changes need new candidate IDs, checks and evidence; do not reuse these passes.
