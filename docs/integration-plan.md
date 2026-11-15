# Integrating the ARM port into master

This is the durable integration plan and completion record. Read it and the
[validation index](validation/README.md) when resuming after a pause or context
reset. Keep the checklist and resume notes current; preserve exact tested
source identifiers rather than treating a branch name as validation evidence.

## Decisions and scope

- Keep the default branch named `master`; do not rename it to `main`.
- The maintainer approved merging PR #1 into `master` on 2026-11-15 and
  explicitly requested keeping the branches. The merge is complete; retain
  `neon-port` and `windows-native-validation`.
- **Manual native validation only**, on the maintainer's Mac M2/macOS ARM64
  and AMD Ryzen 9 7950X/Windows x86-64. On 2026-11-15 the maintainer replaced
  the initial hosted-CI proposal with this preference. Remove hosted test
  workflows; pushes/PRs must not start cloud test matrices.
- Keep Direct Neon experimental and explicitly enabled. Preserve default ARM
  compatibility, existing x86 selection, public API/ABI boundaries, attribution,
  numerical gates, fixture checks, guard pages and 60-second deadlines.
- Physical Intel testing is omitted. No benchmark timings are collected.
  Raw evidence stays under ignored `build/` and transfers separately.
- Complete developer documentation cleanup before considering branch retirement.
  Preserve historical validation reports and benchmark results unchanged.

## Checklist

### 1. Repeatable manual native validation

- [x] Retain the native Mac runner and add Windows MSVC Release/Debug plus
  GNU-driver Clang Release/strict UBSan Debug with matching runtime/CRT controls.
- [x] Add source/patch manifests, native x64 PE checks, runtime consumers,
  independent guard-page controls and checksummed evidence packets.
- [x] Remove the hosted GitHub Actions workflow and the unpublished CI prototype.
- [x] Restore original serial configuration probes after removing the hosted
  timeout workaround; preserve all flag controls and contracts.
- [x] Verify the final manual Windows entry points after the scope change and
  record their actual source and results. Reuse the existing Mac report only
  with its original identifiers and a precise source comparison.

### 2. Complete port review and evidence

- [x] Review the entire port diff from `master`, including backend selection,
  x86 changes, API/ABI boundaries, build exports, attribution and scalar oracles.
- [x] Reproduce and fix the GNU/Clang i386 CPUID assembly-selection regression;
  retain upstream/failing/fixed compilation probes and native Windows checks.
- [x] Record review findings and limits in [the integration review](integration-review.md).
- [x] Attach the native Mac report, historical Windows review, new manual runner
  results and exact source comparisons to the draft integration PR.

### 3. Developer documentation cleanup

- [x] Add a [developer documentation index](README.md) and
  [manual Windows runner guide](validation/windows-runner.md).
- [x] Correct stale AMD coverage, ASan host labels, current instructions and
  branch-transition descriptions. `master` is now the common development base.
- [x] Preserve dated reports, numerical fixtures and benchmark data.
- [x] Run final repository/link/whitespace checks and review the documentation
  diff. No branch may be retired before this cleanup is complete.

### 4. Integration PR and approval

- [x] Commit/push reviewed preparation changes on `neon-port` and create/update
  a draft PR targeting `master`, with actual validation, limitations and this plan.
- [x] Present the PR for the maintainer's merge decision. A draft PR does not
  authorize merging or deleting branches.
- [x] After explicit approval, merge PR #1 into `master`, preserve both branches
  and fast-forward the local Windows `master` to the merge commit.
- [x] Verify the merge tree equals the reviewed PR head tree; update current
  developer instructions to use `master`. Documentation changes do not require
  rerunning the native matrix or relabeling earlier results.
- [ ] Update the Mac checkout to `master` without overwriting unrelated work.
  Record any new native validation at its actual source checkpoint.
- Keep both branches as requested. Branch retirement and release publication
  require separate approval. Track original VCL via `upstream`; use short-lived
  topic branches for future features, fixes and upstream imports.

## Resume notes

- Preparation started clean on `neon-port` at
  `08c3fe899ab9306c11eb58f101aece392c5d7374`; the original `master` was
  `667577495efaa36a10cbb5d6cb8c942c23913848`. No applicable `AGENTS.md` was found.
- Existing evidence: native Mac 8,036 cases at `465436d`, passed after replay;
  native AMD historical final 5,627 cases at `823ca2a` plus identified doc changes.
  Preserve those reports; they are different whole Git trees.
- New local preparation snapshots: first 160/160 cases; CPUID-corrected pinned
  selection 2,776/2,776; parallel configuration prototype 159/160 due to a shared
  MSVC PDB; isolated-directory prototype 160/160. All attempts are retained under
  `build/windows-native-*-20261115T*/` and their checksummed combined packet under
  `build/master-integration-20261115/`. The parallel prototype was then removed
  when manual-only validation was selected; do not label it as final source.
- Historical hosted ARM failures were configuration timeouts. An old compatibility
  Debug job remained stuck in CTest for over 40 minutes; its state was retained
  and obsolete run `43757884045` was cancelled. These are historical failures,
  not a requirement to run hosted CI under the new manual-only policy.
- GitHub device sign-in was completed by the maintainer. An existing authorized
  account was selected explicitly because multiple local accounts exist. No
  tokens are written to the repository or printed in logs.
- Final manual-only source passed 160/160 local cases across all four Windows
  profiles, with strict UBSan and native/guard controls. Four earlier startup
  failures while capturing a deleted file are retained; the manifest now
  explicitly records deleted paths. See the
  [new preparation report](validation/windows-amd-2026-11-15-08c3fe8-manual.md)
  for exact tested trees/patches, 3,416 total suite executions across snapshots
  and the combined evidence checksum.
- Repository/link checks and whitespace review pass. The checker reads UTF-8
  explicitly on Windows. Historical reports and benchmark assets are unchanged;
  the original serial configuration implementation is restored exactly.
- GitHub workflow `419159800` is `disabled_manually`; its source file is removed.
  The old stuck run is completed/cancelled. No hosted matrices will be started
  by the preparation pushes or PR.
- Merged integration PR: [#1 — ARM64 compatibility and experimental Direct Neon](https://github.com/DigitalExpressions/vcl2-xa64/pull/1),
  head `neon-port`, base `master`. Review links and actual validation remain in
  the PR description. No hosted test run is requested.
- Preparation commits: `972f913` restores GNU i386 CPUID selection; `06e4573`
  adds manual Windows runners and removes hosted tests; `2437bf3` records the
  developer cleanup, review and native evidence. The following documentation
  checkpoint records the PR link; no new matrix is needed for that update.
- On explicit maintainer approval, PR #1 was merged with a merge commit on
  2026-11-15: `732f3e9c430b5f1cf822fcf0dab44ad771646e34`. Its parents are the
  original `master` and reviewed head `abf7a82bbb961a2a46b533158082e4ab0f796db9`;
  its tree equals that head's tree, `9a1464bf795f001241b9c63110a53c1eac387210`.
  This is a merge comparison, not a new native test result. Subsequent completion
  documentation changes preserve the original tested source identifiers.
- The Windows checkout is on `master`. Retained `neon-port` is at `abf7a82`;
  local `windows-native-validation` is at `465436d` and was not published just
  to retain it. Automatic branch deletion is disabled. Merge/API and checkout
  evidence is under ignored `build/master-integration-20261115/`.
- **Next action: update the Mac checkout to `master`** using the
  [native workflow](validation/native-workflow.md). Use `master` as the base for
  future topic branches. Keep both existing branches; the default branch remains
  named `master`. No release or branch retirement is authorized by this merge.
