# M2 return handoff: preflight and ARM planning — 19 November 2026

## Purpose and scope

Resume on the native Mac M2 from the published, integrated architecture separation.
Preserve Agner Fog's original x86 implementation. Prepare a reviewable proposal
for one bounded ARM/Neon improvement using retained evidence.

This handoff covers repository inspection/fetch, a fresh detached worktree,
source/dependency/tool verification, read-only maintenance checks, runner planning
and a new planning document. It does not select an implementation change, run a
native correctness matrix, compile new consumer/code-generation probes, run
benchmarks or claim performance gains. Those belong to the subsequently selected
task. Compatibility stays default; Direct Neon stays explicitly experimental.

The Windows integration and push are complete. `origin/master` was verified at
`5e039b01b49abc00548123763be85898932e3ad8`. This handoff is later documentation;
its publication commit, when available, must be recorded separately. It does not
change the source checkpoint below or acquire earlier native results.

## Read first

Read the applicable `AGENTS.md`, protected root `README.md`,
[fork guide](../../README-vcl2-xa64.md), [contributing rules](../../CONTRIBUTING.md),
[integration plan](../integration-plan.md), [validation index](README.md),
[ARM/Neon lessons](../neon-arm-lessons.md), [source ownership](../backend-layout.md),
[backend coverage](../backends.md) and [numerical contracts](../numerical-contracts.md).

Then read the [integration acceptance](architecture-separation-integration-2026-11-19.md),
[completion report](architecture-separation-integration-completed-2026-11-19.md),
[Windows matrix](windows-amd-2026-11-19-arch-separation.md),
[Windows triage](windows-amd-2026-11-19-arch-separation-triage.md),
[M2 cleanup result](mac-arm-2026-11-18-arm-header-cleanup.md) and
[publication comparison](arch-separation-publication-2026-11-18/README.md).
Older handoffs and dated pending notes are historical; do not rerun the original
migration or restore retired x86 fixes. The ARM lessons include older checkpoint
descriptions; use the dated M2 cleanup report for the later ARM result.

## Source identities and actual outcomes

| Identity | Files | Fingerprint / result |
| --- | ---: | --- |
| Integrated planning base `5e039b01b49abc00548123763be85898932e3ad8`, tree `ea90c1b40fc50936db7c3509aaa11ab51c02d890` | 564 | `fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06`; exact Git/raw-byte preflight identity, no native run at this commit |
| Windows-tested publication `2d251bc72bcc861edad371499fa732e8eb45a5f7`, tree `8c16f0135a597db27b963dff6c241b533a5b91b4` | 497 | `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2`; correctness gate failed/incomplete |
| Mac-tested frozen cleanup-01 capture; tested commit/tree null | 474 | `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`; 8,036 CTest passes and eight examples |

Fingerprint input is the complete file inventory sorted by UTF-8 relative path:
concatenate `path + NUL + raw SHA256 hex + newline`, then SHA256 that UTF-8 input.
The 564-file identity was checked against every Git blob on Windows when preparing
this handoff. It excludes this later handoff and all ignored `build/` artifacts.

Windows retained **5,387 passed, 249 failed, 76 not run / 5,712 registered**,
plus twelve passing examples. All 325 nonpassing entries were classified. All
76 compile failures reproduced through originals; twenty paired runtime checks
matched failure status. No selected facade-specific failure was found. This is
targeted equivalence evidence, not a universal correctness pass. Documented lookup
read-ahead exceeds the exact-boundary guarantee demanded by some fork tests;
other inherited issues include genuine valid-input/UB/Debug-runtime findings.
Preserve those distinctions and the failed gate.

The original Mac capture ZIP hash remains
`3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`.
The Windows publication capture ZIP hash remains
`6b2f943cea502830c7c0929e61b15e63a335e8f2453edc51ed44c3119b7127f4`.
Later metadata commits do not relabel either capture. Compare complete inventories
if needed; do not compare only their intersection or regenerate old manifests.
No transfer of ignored Windows build files is needed for this handoff.

## Non-negotiable source and evidence boundaries

- Keep all 28 original root files, all 28 `vendor/vectorclass/` files and
  `vendor/vectorclass.json` byte-exact, including inventory and line endings.
  Upstream pin is `667577495efaa36a10cbb5d6cb8c942c23913848`, tree
  `36aee120c28c9c64e62903bfc81139003e37cedd`. Never edit, normalize, format,
  regenerate or refresh these files to address a failure. On mismatch, retain
  the offending bytes and stop to investigate; do not update accepted hashes.
- Also protect `sse2neon.h`, `vendor/sse2neon.json` and
  `vendor/sse2neon/sse2neon.h`: 60 protected/dependency paths in total.
- The public include is `#include "vcl2_xa64.h"`. The x86 graph selects only
  pristine originals and neutral policy; ARM must not include original x86
  headers. Retired adaptation patches and `backends/x86/` stay inactive.
- Authoritative ARM implementations are root `*_neon.h` and
  `instrset_detect_neon.cpp`. The old generator is retired: `--check` is read-only
  and `--write` rejects. Do not recreate canonical duplicates or edit dependencies.
- Preserve all local changes, branches, worktrees, raw evidence and tool/testbench
  installations. Do not reset, stash automatically, clean, prune or delete them.
  Windows cleanup does not authorize any Mac cleanup. Leave the legacy sibling
  `vcl2-arm64`, its `port-tmp` workspace and external consumers alone.
- Keep API/ABI/namespace/width settings, lane order, notices, fixtures, generators,
  seeds, numerical gates, FP/rounding flags, strict UBSan, guard/canary controls,
  60-second case deadlines and concurrency three intact.
- Do not run timings, ASan, hosted CI, translated/emulated tests, x86 binaries on
  ARM or external application/SDK tests. Do not commit, push, merge, create PRs,
  retire branches, install/update tools or clean evidence under this handoff.
  Earlier publication approvals apply to completed actions.

## Stage A: preserve the host and create an exact worktree

Use the existing `vcl2-xa64` repository on the Mac. Do not assume it is clean,
on `master`, or that a branch/worktree was removed on both hosts.

Before fetching, create a unique session directory under ignored `build/` and
record the current branch, HEAD/tree, full status/untracked inventory, staged and
unstaged binary diffs, refs, worktrees, remotes and saved local Git configuration.
Keep raw paths and any private local details in that ignored directory. Leave
all original working files and evidence in place. Record command arguments,
actual exits and stdout/stderr for each following step; stop on a failed gate.

The following shell variables illustrate the checkout route; choose a new session
ID if any destination exists. Do not overwrite an existing directory. Verify that
`origin` is `https://github.com/DigitalExpressions/vcl2-xa64.git` before fetching.

```sh
TASK_REPO="$(git rev-parse --show-toplevel)"
TASK_BASE=5e039b01b49abc00548123763be85898932e3ad8
TASK_ID="m2-arm-planning-$(date -u +%Y%m%dT%H%M%SZ)-01"
TASK_SESSION="$TASK_REPO/build/$TASK_ID"
mkdir -p "$TASK_REPO/build"
mkdir "$TASK_SESSION"
```

Record the pre-fetch state above in `$TASK_SESSION`, then fetch without pruning:

```sh
git -c gc.auto=0 fetch --no-prune origin
git merge-base --is-ancestor "$TASK_BASE" origin/master
git rev-parse origin/master
git -c core.autocrlf=false -c core.eol=lf worktree add --detach \
  "$TASK_SESSION/checkout" "$TASK_BASE"
cd "$TASK_SESSION/checkout"
TASK_EVIDENCE="$PWD/build/preflight-01"
mkdir -p "$TASK_EVIDENCE"
git rev-parse HEAD
git rev-parse 'HEAD^{tree}'
git status --porcelain
```

Require the exact base commit/tree in the table and an empty source status.
These checkout settings are command-scoped; do not save Git configuration.
Do not advance or switch the original Mac checkout. Even if `origin/master`
later moves, this worktree stays pinned to the named base. Record the fetched
tip and handoff-document revision separately. Read this later handoff from the
supplied copy or with `git show origin/master:docs/validation/m2-arm-planning-handoff-2026-11-19.md`
after it is published; it is deliberately absent from the pinned base worktree.

## Stage B: source, tools and dependency preflight

Before writing a planning document, inventory every tracked source path in the
new worktree. Require exact raw bytes equal to its Git blob, a complete 564-file
inventory and the planning-base fingerprint above. A newline-only difference is
a failure, not permission to normalize protected files. Save per-path SHA256,
size and the complete inventory under `$TASK_EVIDENCE`.

Snapshot raw SHA256 and nanosecond modification times for all 60 protected paths
before checks and require the same bytes, inventory and times afterward. Verify
the pin JSON against the Git blob as well as checking each vendor/root file.
Validate the checksummed Mac, publication, Windows and integration portable
bundles without changing any recorded manifest, checksum or dated report.

Create and verify a new complete source capture before adding planning metadata:

```sh
python3 -B tools/capture_source.py --output "$TASK_EVIDENCE/source-capture-01"
python3 -B tools/capture_source.py \
  --checkout "$TASK_EVIDENCE/source-capture-01/source" \
  --verify-manifest "$TASK_EVIDENCE/source-capture-01/capture.json"
```

Require 564 files, the expected fingerprint, the exact base commit/tree, an empty
tracked patch and no added/deleted source paths. Preserve the ZIP, archive receipt,
manifest and patch. The tool's generic schema name `frozen-uncommitted-source`
does not mean this clean base is dirty or that execution occurred.

Verify native Apple M2/macOS, `uname -m`/Python architecture `arm64`, Rosetta state,
Apple Clang target, available arm64 tool slices and actual page size. Inspect
installed Python 3.9+, Apple Clang, CMake 3.20+, CTest JUnit support and Ninja/make;
reuse them without installation/configuration changes. Historical tool versions
and the old 16,384-byte page measurement are evidence, not assumptions about today.
Keep absolute tool/dependency paths and inventories under ignored `build/`.

Locate the existing pristine testbench in retained local resources. Verify HEAD
`a7293702385e5169a5f58b83177f2a43cd59139b`, cleanliness and all three pinned
`SOURCE_HASHES` in `tools/run_native_arm_validation.py`. It can remain outside
the new worktree and later be selected with `--testbench-dir`. Never overwrite
an existing checkout. If a tool/dependency is missing, preserve findings and
report the gap; do not silently download or replace it during this preflight.

Run only the maintenance checks and runner plan below, saving each command,
exit and complete output under `$TASK_EVIDENCE`:

```sh
python3 -B tools/regenerate_headers.py --check
python3 -B tools/check_repository.py
python3 -B tools/check_header_generation.py
python3 -B tools/run_native_arm_validation.py --plan
```

The integrity controls modify disposable copies under ignored `build/`; actual
originals remain untouched. Keep their failing negative-control inputs/results.
The plan should register eight profiles / 8,036 cases, with 60-second deadlines.
A successful plan is not a native pass and does not verify tools or testbench by
itself; perform their separate checks above. Do not invoke the runner without
`--plan` as part of this handoff. Reverify all protected hashes/mtimes and the
captured source fingerprint afterward. Use Python `-B` for every script/import
to avoid creating bytecode in source snapshots.

## Stage C: prepare the next ARM task for review

Use the [retained M2 measurements](../benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md)
and [reading guide](../benchmarks/standard_math_reading_guide.md), the later M2
correctness report and current source to identify two or three bounded candidate
improvements. Existing timings belong to their recorded source, not the separated
base. Inspect source/read existing artifacts; do not run new timings or probes.

Rank candidates by evidence strength, practical scope and numerical/ABI risk;
recommend one. For each, identify the precise API/operation, ARM backend(s),
logical widths and ARM-owned files involved. Explain what existing evidence shows,
what remains a hypothesis and why the change could matter. Do not promise a gain
or select a winner from wrapper instruction counts alone.

For the recommended candidate, specify the supported input domain, NaN/Inf/
signed-zero/subnormal/conversion/rounding/FMA behavior as applicable, acceptance
gates and independent references. Separate ISA/contract differences from defects.
Keep stronger ARM memory/conversion guarantees intact even where original x86
has a narrower contract. Preserve the pristine x86 include path and neutral
selection policy; any need to change shared policy must be explicitly scoped.

Prepare a new dated proposal, for example
`docs/arm-next-step-proposal-<actual-date>.md`, in the detached worktree. Report
the preflight identity separately from the later dirty planning-document tree.
Include inspected evidence/source IDs, candidates and recommendation, exact
proposed file scope, open questions, acceptance criteria and a future validation
plan. Future runtime/test/runner changes need a new frozen source checkpoint,
fresh relevant checks and the full native M2 baseline: compatibility maximum 512
and Direct Neon maxima 128/256/512, each Release and strict nonrecovering UBSan
Debug, retaining guard/UBSan controls, original deadlines and concurrency three.
Performance measurement and consumer integration remain separate scope.

Run documentation/repository and whitespace checks after preparing the proposal;
reverify protected paths and the untouched captured source. Keep all raw evidence,
failed preparation attempts, generated control inputs, logs, source/tool/dependency
inventories and checksums under ignored `build/`. Package a checksummed portable
preflight summary without machine paths or binaries. Retain the worktree and all
evidence; this handoff authorizes no cleanup or publication.

Finish with a concise readiness report: exact HEAD/tree/fingerprint, real
preflight checks and gaps, protected-source integrity, preserved evidence paths,
the recommended bounded ARM task and the proposal location. End at this reviewable
proposal. No runtime change or new native/performance result is implied.

## Copyable M2 prompt

```text
Prepare the native Mac M2 for the next ARM/Neon task in vcl2-xa64.

Read applicable AGENTS.md. Inspect local Git state, changes, branches, worktrees
and evidence, then fetch origin without pruning or discarding anything.
Read docs/validation/m2-arm-planning-handoff-2026-11-19.md from the supplied
copy or via git show origin/master:docs/validation/m2-arm-planning-handoff-2026-11-19.md
after its documentation publication. Follow that handoff's preflight/planning
scope; do not execute an older migration handoff.

Create a fresh detached worktree at 5e039b01b49abc00548123763be85898932e3ad8
using command-scoped core.autocrlf=false and core.eol=lf. Preserve the original
checkout and saved Git configuration. Require tree
ea90c1b40fc50936db7c3509aaa11ab51c02d890, 564 source files and fingerprint
fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06.

Protect all 28 original root files, vendor/vectorclass/ and its pin byte-exact;
also verify the sse2neon dependency. Reuse and verify installed native tools and
the pinned pristine testbench. Run only the handoff's maintenance checks and
runner --plan; preserve raw evidence under ignored build/ and all failures.
Keep the separate Mac-tested and Windows-tested identities and the failed
Windows gate unchanged.

Use retained M2 evidence to prepare a dated proposal comparing two or three
bounded ARM improvements and recommending one with exact scope, API/numerical
contract and future validation criteria. Keep compatibility default and Direct
Neon experimental. Finish with the proposal and preflight readiness report.

Do not change runtime/tests/fixtures/runners, run the native matrix or benchmarks,
use ASan/hosted CI/emulation, alter consumers, clean evidence/worktrees/branches,
or commit/push/merge/create PRs. No new implementation is selected yet.
```
