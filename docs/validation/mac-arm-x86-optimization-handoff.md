# Native M2 handoff for the x86 optimization candidate

Windows preparation date: 2026-11-16. Native ARM execution is **pending**.
Read `AGENTS.md`, the [optimization plan](../x86-backend-optimization-plan.md),
[native workflow](native-workflow.md) and [report template](report-template.md).
Preserve unrelated work and all previous validation evidence.

## Mandatory pristine-upstream rule

**`vendor/vectorclass/` must remain untouched on the M2 at all times.** Its 28
files are the exact original VCL2 snapshot at
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Do not edit code, format files,
normalize line endings, regenerate into this directory, replace its files or
update its pin/hash inventory to make a test pass. Do not modify this directory
while diagnosing ARM, compiler, sanitizer or numerical failures.

Run the generator's read-only check and Git vendor-diff check before and after
the matrix, retaining both records under ignored `build/`. The generator always
requires exact vendor bytes and original Git blobs/tree, even when it permits
explicit CRLF/LF comparison of generated root products. On any vendor mismatch,
preserve the failed check and stop; never silently repair the vendor snapshot
or relax its hashes. A separately authorized upstream-update task is the only
exception, and is outside this validation handoff.

The integrity self-test intentionally corrupts isolated fixture copies under
ignored `build/` to prove rejection. That is permitted only for those copies;
it never authorizes changes to the checkout's canonical `vendor/vectorclass/`.

ARM fixes belong in canonical `backends/arm64/` sources or explicit adaptation
patches. Shared fork policy/helpers belong in `adaptation/include/`. Regeneration
may update shipped root products only, after the pristine check passes. Preserve
the initial failed run, prepare a reviewed fix and obtain a new approved clean
checkpoint for rerun; do not edit vendor code or numerical gates as a shortcut.

## Published source and safe Mac checkout

The maintainer authorized committing/pushing the prepared source, tooling,
portable Windows evidence and this strengthened M2 handoff on 2026-11-16.
The commit containing this handoff version is the approved Mac source; that
publication does not relabel earlier Windows runs as tests of the new commit.
Prefer the exact publication SHA supplied by Windows. If only this file path
was supplied, resolve its publication from `origin/master` after fetching:

```sh
git status --short --branch
git fetch origin
M2_VALIDATION_COMMIT=$(git log -1 --format=%H origin/master -- docs/validation/mac-arm-x86-optimization-handoff.md)
git show --no-patch --format=fuller "$M2_VALIDATION_COMMIT"
M2_TESTBENCH_DIR="$(git rev-parse --show-toplevel)/build/_deps/testbench"
M2_SHORT_COMMIT=$(git rev-parse --short=12 "$M2_VALIDATION_COMMIT")
mkdir -p build
git worktree add --detach "build/mac-m2-handoff-$M2_SHORT_COMMIT" "$M2_VALIDATION_COMMIT"
cd "build/mac-m2-handoff-$M2_SHORT_COMMIT"
git status --short --branch
```

An existing directory/worktree must not be overwritten: inspect/reuse it only
if it has the exact approved commit and is clean, otherwise choose a fresh
suffix. Do not reset or discard primary-checkout work, rename branches or require
retired port branches. Read the checked-out `AGENTS.md`, linked plan/workflow/
template and this file before execution. For an older checkout without this
file, first read it using
`git show origin/master:docs/validation/mac-arm-x86-optimization-handoff.md`.
Keep this frozen worktree and original evidence until cleanup is authorized.

## Source to review and freeze

Base commit: `44f974ac31e1448e0beb5157d2bd8d669f5c518e`.
The [five-file candidate patch](x86-optimization-2026-11-16/candidate.patch) has
SHA256 `b954c2894ecac21ad59b3ca3d9ff10401f01fb8ac5552e96eeb4dc300c5855e4`.
The [source inventory](x86-optimization-2026-11-16/candidate-source.json) records
the exact selected library/test/build/runner bytes plus separately identified
LF-normalized hashes. Full Git-export fingerprint:
`1da758d8e17b4f50b60fca687a5dae2f52065b317aa71936f72466ae7375fc45`.
Those identifiers describe the original uncommitted Windows test snapshot.
The approved publication commit additionally includes the canonical source
layout and handoff documentation. Preserve the original test identifiers and
compare the published files as described below; do not label the base commit
alone or the publication commit as the original Windows executed source.

The library changes are guarded to native MSVC x86. ARM compatibility defaults,
Direct NEON opt-in, numerical policies, rounding and conversion fixes remain.
The shared classification test adds mixed lanes and deterministic integer-bit
oracles; that new test coverage still needs native M2 execution on both backends.

The maintainer subsequently approved pristine upstream/adaptation separation
while retaining direct header-only usage. The [layout guide](../backend-layout.md)
describes the new canonical directories and generation controls. The shipped
35 runtime files reproduce the frozen optimized bytes exactly; canonical copies,
patches and generation tooling are additional maintained sources. The original
five-file patch/source inventory remains a historical pre-layout identifier.
The [layout provenance](x86-optimization-2026-11-16/source-layout.json) and
[Windows report](windows-amd-2026-11-16-x86-layout.md) must accompany the approved
combined commit; do not treat the original whole-tree fingerprint as describing
all files added by the separation. In particular the repository checker now
also checks generation, and has a separately recorded tooling hash.
For the approved combined checkout, compare shipped products and new tooling
against `source-layout.json` (including its explicit LF-normalized digests).
That tooling record supersedes the old `tools/check_repository.py` hash in the
pre-layout inventory. Other unchanged selected runtime/test paths retain their
original hashes. Require pristine upstream bytes to match exactly; do not
normalize that canonical vendor snapshot to hide a discrepancy.

The Mac runner requires the clean approved publication commit resolved above.
Record its actual full commit/tree as the new Mac checkpoint. Compare all
selected inventory paths with the approved checkout.
For a path with different raw bytes, explicitly compare the supplied
`lf_normalized_sha256` against SHA256 of bytes with CRLF replaced by LF; report
the transformation and retain the raw hash. Any other mismatch is a source gap
to investigate before execution. Documentation/benchmark-tool additions in a
later commit do not change the original Windows tested source identifiers.

Do not bypass the runner's clean-source requirement, commit locally without
approval, or label a patch application as a tested commit. Fetching a branch
without the approved candidate is insufficient.

## Native execution

Use native macOS arm64 Python 3.9+, Apple Clang, CMake 3.20+, CTest and Ninja or
make. Verify Rosetta is off, the actual compiler/SDK and the pristine testbench
pin `a7293702385e5169a5f58b83177f2a43cd59139b`. Reuse installed resources; never
overwrite an existing dependency checkout. Reuse `M2_TESTBENCH_DIR` from the
primary checkout; if it does not exist, the pinned fetch script can create it.
If it exists but is not the pristine pinned checkout, stop and preserve it.
From the clean approved worktree, with the shell variables above retained:

```sh
if [ ! -e "$M2_TESTBENCH_DIR" ]; then
  sh tools/fetch_testbench.sh "$M2_TESTBENCH_DIR"
fi
mkdir -p build
M2_AUDIT_DIR=$(mktemp -d build/mac-m2-pristine.XXXXXX)
python3 -B tools/regenerate_headers.py --check --json "$M2_AUDIT_DIR/before.json"
git diff --exit-code "$M2_VALIDATION_COMMIT" -- vendor/vectorclass/ vendor/vectorclass.json
python3 -B tools/check_header_generation.py
python3 -B tools/run_native_arm_validation.py --plan
python3 -B tools/run_native_arm_validation.py \
  --expected-commit "$M2_VALIDATION_COMMIT" \
  --testbench-dir "$M2_TESTBENCH_DIR"
```

Require each prerequisite command to succeed before continuing. Record the
native runner's actual exit code. Even if the matrix fails, perform these
read-only post-run integrity checks and retain their results:

```sh
python3 -B tools/regenerate_headers.py --check --json "$M2_AUDIT_DIR/after.json"
git diff --exit-code "$M2_VALIDATION_COMMIT" -- vendor/vectorclass/ vendor/vectorclass.json
git status --short --branch
```

Require unchanged pristine vendor bytes and a clean frozen checkout. A matrix
failure remains a failure even if the integrity check succeeds; a vendor change
invalidates the run and must be reported explicitly. Existing Windows higher-ISA
failures remain separate findings and must not be hidden by an ARM pass.

Run compatibility maximum 512 and Direct NEON maxima 128/256/512, each Release
and strict nonrecovering UBSan Debug: expected 8,036 CTest case instances plus
eight examples. Preserve independent fixtures, mixed/broadcast classification,
actual-page guard controls, canaries, original numerical gates and 60-second
deadlines, concurrency three, no implicit contraction and the separate
classification-only finite-math mode. Require valid native Mach-O binaries,
runtime checks and passing UBSan positive/intentional-overflow controls. Preserve
SIGABRT/exit-1 observations; do not edit exit codes. Do not retry the documented
ASan startup hang. Do not collect benchmark timings as part of correctness.

Retain every attempt and its source/compiler/flag inventories, raw logs, JUnit,
manifests, binary hashes and checksummed evidence archive under ignored `build/`.
Add a new dated portable report/index entry using the template, with exact
approved commit/tree and the relationship to the original Windows patch.
Report actual registered/pass/fail/skip/timeout counts and remaining gaps.
No commit, push, merge, PR or publication without separate user authorization.
