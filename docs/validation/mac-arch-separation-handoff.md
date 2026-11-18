# Mac M2 handoff: implement and validate architecture separation

Prepared 18 November 2026. The maintainer approved the sequence: publish the
reviewed documentation, implement on native Mac M2, validate ARM there, then
validate the same frozen implementation on native AMD Windows. The present
publication is documentation only, based on Windows `master` at
`e41b0776d9e1d8ba51576bf2a17529a013753900` before this publication.

## Scope and authority

Execute the [architecture separation plan](../arch_separation_plan.md).
The agreed public include is `#include "vcl2_xa64.h"`.
Implementation and native ARM correctness validation are the next Mac task.
Keep compatibility as the default and Direct Neon as explicit experimental
opt-in. Preserve the supported portable integer/FP32/FP64 vector and in-tree
math surface; native FP16, Intel SVML and a Direct-Neon-default switch are not
part of this migration. Restore all 28 original inventory paths, including
the original README; relocate fork usage guidance to `README-vcl2-xa64.md`.

Only step 1, publication of the documentation/handoff, has commit/push approval.
The Mac implementation, tests and reports must remain uncommitted until reviewed
and separately approved. Do not push, merge, create PRs, rename `master`, retire
branches, contact upstream, clean evidence or run benchmarks. The x86 findings
are retained for later research, not instructions to apply x86 fixes.

This handoff defines the migration exception to the current generated-root
maintenance instructions: first preserve/audit the old canonical sources, then
retire their writers to protected root paths. Thereafter edit the authoritative
ARM `_neon.h` family. Do not mix old and new ownership rules or leave multiple
editable copies. Existing instructions still govern the pre-migration checkpoint.

## Read before changing anything

- Applicable `AGENTS.md`, root README and `CONTRIBUTING.md`.
- [Architecture separation plan](../arch_separation_plan.md), including all 28
  file dispositions, collision handling and acceptance checks.
- [Integration status](../integration-plan.md), [ARM lessons](../neon-arm-lessons.md)
  and [retained x86 findings](../x86_findings_and_fix_candidates.md).
- [Existing source layout](../backend-layout.md), [backend limits](../backends.md),
  [numerical contracts](../numerical-contracts.md) and [test guidance](../../tests/README.md).
- [Native workflow](native-workflow.md), [report template](report-template.md),
  [validation index](README.md) and [latest M2 result](mac-arm-2026-11-17-ab5bb87-correctness.md).

The native workflow describes the existing runner. Adapt its include/provenance
handling where separation requires it; preserve its numerical/guard/sanitizer
controls. Old execution prompts are retired and do not replace this handoff.

## 1. Inspect and establish the published base

1. Inspect OS/CPU, native arm64 Python/Apple Clang, Rosetta state, tools,
   branch/HEAD/tree, status, worktrees, dependency checkouts and previous evidence.
   Record the original state under a fresh ignored `build/` session directory.
2. Fetch `origin` without resets, automatic stashes, overwrites or pruning.
   Use the exact documentation publication SHA supplied with the Mac prompt;
   verify that it contains this handoff and the plan. Do not substitute a moving
   branch tip or assume another machine's branch/worktree state.
3. Create a new topic branch from that publication, for example
   `arch-separation-m2`. If its name/path already exists, inspect and reuse only
   if appropriate, otherwise choose a new name. Use a fresh worktree if needed
   to preserve unrelated changes. Do not delete existing worktrees or branches.
4. Identify existing native tools and pristine testbench
   `a7293702385e5169a5f58b83177f2a43cd59139b`; verify Git revision, cleanliness and
   source hashes before reusing it. Never overwrite another dependency checkout.
5. Before migration, run the existing read-only generation/repository checks.
   Record all 28 vendor hashes and the exact pin-inventory bytes. Preserve the
   old canonical patches/backend sources in Git and capture any local edits.
   Windows-local raw archives described in the x86 document are not assumed
   present on the Mac; their absence does not authorize inventing replacements.

## 2. Protect originals and implement the separated include graph

**Never edit, format, normalize or regenerate `vendor/vectorclass/` or change
`vendor/vectorclass.json`.** The reference stays at upstream
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`.

Implement the plan in its documented dependency order:

1. Produce an old-to-new path/include/guard map. Classify ARM requirements
   separately from x86-only repairs. Do not blindly append a suffix to every diff.
2. Update maintenance tools/checkout attributes so they cannot overwrite or
   newline-convert protected originals. Retire the adapted-root generation path
   before restoring originals. Keep one authoritative editable ARM implementation
   per public header; eliminate duplicate active/generated ownership.
3. Preserve the ARM implementations as `_neon.h` counterparts. Reconcile the
   existing `vectori128_neon.h`, `vectorf128_neon.h` and `instrset_neon.h` without
   overwriting distinct implementations. Compatibility-private headers may use
   names such as `vectori128_compat_neon.h`.
4. Rewire the complete ARM dependency graph, including common math, conversion
   and wide-composition headers, even where their bodies matched upstream.
   Separate guards and includes must prevent a return to original x86 headers.
   Keep compatibility/Direct behavior and logical width settings intact.
5. Copy all 28 pinned originals to their original root paths with exact raw
   bytes. Protect those paths too: Neon changes never edit them. Keep vendor
   as the reference; do not move it away. Preserve notices and dependencies.
6. Add `vcl2_xa64.h`: automatic compiler-target selection, original root headers
   on x86, ARM family on ARM64. No per-operation wrapper or runtime architecture
   branch. Reject unsupported targets, invalid overrides and initially ARM64EC
   explicitly. Retain namespace/width consistency between translation units.
7. Select any compiled runtime-query source per architecture, keeping `.cpp`
   implementations as `.cpp`. Original x86 `instrset_detect.cpp` stays unchanged;
   preserve required ARM capability queries in an ARM-owned implementation.
8. Update owned examples, CMake, tests, capture tools and header-isolation probes
   to the facade and new layout. Keep the external pinned testbench pristine;
   adapt generated copies under ignored `build/`. Do not modify application/SDK
   consumers outside this repository. Update AGENTS/developer instructions and
   integrity tooling to describe the implemented ownership rule consistently.

Once copied, each protected root file must match its corresponding vendor hash
before and after every validation phase. Keep the original pin inventory exact.
On an unexpected mismatch, preserve evidence and stop to investigate; do not
update expected hashes or accept newline equivalence. Integrity checks must
detect deliberate changes to disposable copies under `build/`, including a
newline-only change, without altering real originals.

## 3. Freeze an exact test candidate without committing it

Use a new run ID, such as `arch-separation-m2-<actual-UTC-timestamp>`, and a new
attempt ID for every source revision/invocation. Do not test a moving checkout
and describe it as a frozen commit.

The current `run_native_arm_validation.py` requires a clean Git checkout and
rejects an implementation left uncommitted. The present approval does not allow
a source commit merely to satisfy that guard. Make an explicit captured-source
validation mode, or use equivalent recorded native profile invocations against
an immutable captured source, with these requirements:

- Preserve the default clean-source guard. Do not spoof clean status, misuse
  `--expected-commit`, omit new files, or label the base commit/tree as tested.
- Record the publication/base commit and tree separately from the complete
  implementation patch, added/deleted paths and exact captured file inventory.
  Include all build-affecting headers, sources, tests, tools, fixtures and build
  files, including untracked additions; exclude `.git`, builds and unrelated work.
- Archive the actual source bytes and patch/new files under ignored `build/`.
  Verify the captured source against a reviewed per-path SHA256 inventory before
  and after the run. Fingerprint sorted UTF-8 entries as
  `relative_path + NUL + file_SHA256_hex + newline`; specify the ordering.
- Build only that frozen capture. Identify the exact runner/validator bytes
  separately. Preserve native-host checks, controls, complete registration,
  numerical flags, timeouts, failure collection and checksummed packaging.
- After any runtime change, freeze a new candidate and repeat relevant checks
  and the full baseline profiles. Never attach older passes to newer source.

This is permission to adapt validation provenance for the uncommitted migration,
not to weaken source identity or numerical checks. If complete capture cannot be
established, report the specific gap rather than claim a full native pass.

## 4. Native M2 acceptance

First run focused include/build/integrity checks, then the full native matrix.

- Verify arm64 processes and Mach-O executables, Apple Clang/version/target,
  selected SDK, actual page size, Python/build tools and absence of Rosetta.
- Check all 28 root originals, all 28 vendor files and the pin inventory exactly.
  Check that no active maintenance writer can modify a protected original.
- Save compiler include traces proving ARM uses its family, not protected x86
  headers. Build the portable consumer using only `vcl2_xa64.h`, including vector
  arithmetic, supported math and optional runtime queries. Preserve direct-use
  installation/header isolation without an application generation prerequisite.
- Execute compatibility maximum 512 and Direct Neon maxima 128/256/512, each
  Release and strict nonrecovering UBSan Debug: eight full profiles. The previous
  inventory was 8,036 CTest instances plus eight examples; reconcile any deliberate
  registration changes by full names. Do not drop failing cases or call a smaller
  local-only selection the full matrix.
- Preserve independent fixtures, seeds, exceptional bit patterns, tolerances,
  selected FP/rounding flags, no implicit contraction, finite-math stress isolation,
  canaries and exact-page protected-memory controls. CTest/process correctness
  deadlines remain 60 seconds and build/test concurrency three.
- Require a passing UBSan positive control and the expected intentional-overflow
  diagnostic with no continuation. Apple negative exit may be `-6` or `1`;
  retain the actual value. Do not retry the documented ASan startup hang.
- Preserve failures, setup attempts, logs, diagnostics and complete case counts.
  No old 8,036-pass checkpoint transfers to the new layout. No benchmark timing
  or cross-platform bitwise-equivalence claim is part of this task.

Do not run x86 binaries through Rosetta as Windows proof. Mac structural checks
can establish original-byte preservation; native MSVC/Clang validation remains
pending on Windows until actually executed there.

## 5. Portable results and Windows return handoff

Add a dated report using [the shared template](report-template.md), for example
`docs/validation/mac-arm-YYYY-MM-DD-arch-separation.md`, and a portable companion
directory with `checksums.sha256`. Preserve all existing historical reports and
checksum inventories. Update the validation index and integration status.

Include base commit/tree, implementation patch and complete source fingerprint,
per-path source hashes, runner hashes, vendor/root integrity, tool versions,
full registered/pass/fail/skip/timeout inventories, actual flags and control
results, attempts and unresolved gaps. Raw archives, logs, source captures and
binaries remain ignored under `build/`; export only portable metadata without
absolute machine paths or credentials. State which raw archive lives on which
host and its checksum. Do not claim Windows-local archives were checked on Mac.

Prepare `docs/validation/windows-arch-separation-handoff.md` with the exact
candidate identity and native Windows commands. The next phase must:

1. Use the same reviewed implementation bytes, including all runner/test changes.
   After separate source-publication approval, select an exact published commit
   and prove its relation to the Mac capture; do not relabel the Mac's tested
   source identifiers. If transferring a capture, verify its inventory instead.
2. Run MSVC first, then native GNU-driver Clang: baseline Release/Debug profiles,
   strict UBSan for Clang Debug, and relevant supported higher-ISA checks. Probe
   CPU/OS support and distinguish original-upstream behavior from fork contracts.
3. Prove x86 include traces resolve protected root originals without ARM headers,
   sse2neon or retired fixes. Compare facade/direct-original outputs and generated
   code under matched flags; no timing or new x86 repair campaign is authorized.
4. Preserve inherited numerical/UBSan/page-boundary failures as actual failures,
   with references to the x86 findings. No padding, gate relaxation, silent skips
   or protected-file edits may turn these into a nominal pass.

Finish on Mac with the implementation summary, exact candidate identifiers,
native outcomes, source-integrity results, report/archive locations and remaining
Windows or other gaps. Leave implementation/results uncommitted for review;
request no further approval until that concrete work is ready. Do not publish
or clean up evidence under the step-1 documentation approval.

## Reusable Mac task prompt

Use the publication SHA reported when this handoff is pushed:

```text
Fetch origin while preserving local work, branches, worktrees and evidence.
Confirm documentation publication <PUBLICATION_SHA> is available and read
the handoff from that exact commit. Read applicable AGENTS.md and execute:
docs/validation/mac-arch-separation-handoff.md

Implement docs/arch_separation_plan.md on native Mac M2, then run its native
ARM correctness validation. Use #include "vcl2_xa64.h" as the common entry point.
Keep vendor/vectorclass/ and its pin inventory untouched; restored original
root files must stay byte-exact and be used only by the x86 path. Keep ARM
adaptations in the separate _neon.h family and preserve current backend defaults.

Preserve numerical gates, FP flags, UBSan/guard controls, 60-second correctness
deadlines and concurrency three. Capture the complete uncommitted tested source
with before/after hashes; do not pretend the clean base was tested. Save portable
reports/checksums and prepare the exact-source Windows return handoff. Keep raw
evidence under ignored build/ and preserve every failure/attempt.

No benchmark timings, x86 fix campaign, source commit, push, merge, PR, branch
retirement, upstream contact or evidence cleanup is authorized. Finish with
actual results, source identity, integrity checks, report locations and gaps.
```
