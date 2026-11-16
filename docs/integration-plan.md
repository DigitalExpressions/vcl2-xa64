# ARM integration: completion and next steps

PR #1 is merged. Use this record and the [validation index](validation/README.md)
when resuming work. Exact tested source identifiers remain in the dated reports;
a branch name or merge alone does not establish a native pass.

## Standing decisions

- Keep the default branch named `master`. Retain `neon-port` and
  `windows-native-validation` as explicitly requested; automatic branch deletion
  is disabled. Branch retirement and release publication require separate approval.
- Use manual native validation on the Mac M2/macOS ARM64 and AMD Ryzen 9
  7950X/Windows x86-64 only. Hosted test workflows are removed/disabled
  (workflow `419159800`); do not start hosted test matrices.
- Physical Intel testing is omitted. No benchmark timings are collected.
  Keep raw evidence under ignored `build/` and transfer it separately.
- Preserve default ARM compatibility, explicit experimental Direct Neon opt-in,
  existing x86 selection, API/ABI boundaries, attribution, numerical gates,
  fixture checks, guard pages and original 60-second deadlines.
- Preserve historical validation reports and benchmark results unchanged.
  Start future topic branches from `master`, review PRs and retain the `upstream`
  remote for original VCL. Future pushes and merges need task-specific approval.

## Completed integration

- [x] Add repeatable manual native runners with source/patch manifests,
  runtime/binary checks, guard controls, strict UBSan controls and checksummed
  evidence. Restore original serial configuration probes after discarding the
  hosted/parallel prototype.
- [x] Review the complete port, correct the GNU/Clang i386 CPUID regression,
  and record findings and limits in the [integration review](integration-review.md).
- [x] Complete developer documentation cleanup and current build/test guidance.
- [x] Merge [PR #1](https://github.com/DigitalExpressions/vcl2-xa64/pull/1)
  on explicit maintainer approval, preserve both branches and sync Windows `master`.

Preparation started at `08c3fe899ab9306c11eb58f101aece392c5d7374`.
Commits `972f913`, `06e4573`, `2437bf3` and `abf7a82` contain the CPUID fix,
manual runner/workflow changes, review/documentation and PR checkpoint.

The merge on 2026-11-15 is `732f3e9c430b5f1cf822fcf0dab44ad771646e34`.
Its parents are original `master` `667577495efaa36a10cbb5d6cb8c942c23913848`
and reviewed head `abf7a82bbb961a2a46b533158082e4ab0f796db9`. Its tree equals
the reviewed head's tree, `9a1464bf795f001241b9c63110a53c1eac387210`.
This comparison is not a new test result. Completion documentation was pushed
at `df2df54446198294ed846eea2c965498ff7b1f26`.

Retained `neon-port` is at `abf7a82`; local `windows-native-validation` is at
`465436de204ebdd59e9b5071c353df372148b3df` and was not published just to retain it.
Merge/API and checkout evidence stays under `build/master-integration-20261115/`.
Detailed preparation chronology remains in Git history and the dated reports.

## Evidence and follow-up cleanup

- [Native Mac report](validation/mac-arm-2026-11-15.md): 8,036 selected cases
  at `465436d`, passed after a sleep-related full-profile replay.
- [Historical AMD review](validation/windows-amd-2026-11-15-823ca2a.md): final
  5,627 cases at `823ca2a` plus identified documentation changes, with earlier
  failures and reruns retained. This is a different whole tree from the Mac run.
- [Manual runner preparation](validation/windows-amd-2026-11-15-08c3fe8-manual.md):
  final 160/160 local cases, separately identified 2,776/2,776 pinned cases,
  discarded prototype failure and startup failures. Exact snapshots and the
  combined packet checksum remain in that report; raw evidence stays under
  `build/master-integration-20261115/` and its original native run directories.
- [Post-merge helper cleanup](validation/windows-amd-2026-11-15-df2df54-cleanup.md):
  160/160 fresh local AMD cases, strict UBSan/guard controls, ten unchanged plans
  and exact dirty-source identifiers based on `df2df54`. Both runners now import
  `tools/validation_evidence.py`. Native M2 execution of this combination remains
  outstanding. Evidence stays under `build/master-cleanup-20261115T222559Z/`.
- The superseded Intel handoff was removed; current instructions use the
  [Windows runner guide](validation/windows-runner.md). The empty, untracked
  local `.github/workflows` and `.github` directories remain because automatic
  approval review rejected their removal as "blocked by policy" without a
  further reason. They do not affect the Git tree.
  This follow-up changes documentation only. Historical reports, raw evidence,
  benchmarks, source and retained branches are preserved. The maintainer has
  approved committing and pushing the reviewed helper/documentation cleanup;
  Git history and the host-local command logs record the publication checkpoint.

## Next action

- [x] Prepare the [shared SIMD comparison harness](validation/cross-platform/README.md)
  and [preparation evidence](validation/cross-platform/preparation-2026-11-16.md).
  Eight candidate Windows profiles passed 5,376 corpus cases after correcting
  a non-default `roundi` oracle assumption; all earlier failures remain in evidence.
- [x] Obtain the approval required by the Windows final-verification prompt
  to commit/push this new harness. Freeze the resulting published commit and
  record its full SHA in the final run manifests and Windows report.
- [ ] Freeze the approved published commit, execute the final pinned Windows
  MSVC/Clang matrix and corpus profiles, then prepare portable Windows results.
  Reserved run ID: `simd-parity-20261116`. Raw preparation/audit evidence is under
  `build/cross-platform-preparation-20261115T233331Z/`; original native attempts
  remain under their `build/simd-capture-<UTC>-<profile>/` directories.
- [x] Review the cleanup and obtain explicit maintainer approval to commit/push.
- [ ] Update the Mac checkout to `master` using the
  [native workflow](validation/native-workflow.md), preserving unrelated work.
  Use the whole checkout so the shared helper is present.
- [ ] Run the in-tree Mac runner on the actual new source and record its HEAD/tree,
  controls, counts and evidence checksum in a new dated report. Do not relabel
  the older Mac result as validation of a later revision.
