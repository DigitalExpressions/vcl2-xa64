# ARM integration and validation status

PR #1 and the selected `simd-parity-20261116` validation checkpoint are complete.
The Mac evidence and validator correction were published at `99f4a3d`; the
source-byte audit was published at `145c7f5` and is present on `origin/master`.
All 32 planned Windows/Mac pairings have successful comparisons with zero
unexpected differences, and the source-fingerprint discrepancy is reconciled.
Use the [validation index](validation/README.md) for the exact executed profiles,
attempts and coverage limits. The tested source remains frozen at `bd7807e`;
later publication and documentation commits do not establish new native passes.

## Standing decisions

- Keep the default branch named `master`. The user retired the merged
  `neon-port` and source-audit branches; do not recreate them for testing.
  Inspect actual branches on each host. Further branch retirement and release
  publication require separate explicit approval.
- Follow the shared [agent instructions](../AGENTS.md). Do not create pull
  requests or alter the fork relationship, remotes, default branch or published
  history. Never push to original upstream; preserve its remote for reference.
- Use manual native validation on the Mac M2/macOS ARM64 and AMD Ryzen 9
  7950X/Windows x86-64 only. Hosted test workflows are removed/disabled
  (workflow `419159800`); do not start hosted test matrices.
- Physical Intel testing is omitted. No benchmark timings are collected.
  Keep raw evidence under ignored `build/` and transfer it separately.
- Preserve default ARM compatibility, explicit experimental Direct Neon opt-in,
  existing x86 selection, API/ABI boundaries, attribution, numerical gates,
  fixture checks, guard pages and original 60-second deadlines.
- Preserve historical validation reports and benchmark results unchanged.
  Start future topic branches from `master` when useful and review changes locally.
  Future commits, pushes and integrations need task-specific approval.

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

At the integration checkpoint, retained `neon-port` was at `abf7a82` and
`windows-native-validation` was recorded at
`465436de204ebdd59e9b5071c353df372148b3df`; the latter was not published just
to retain it. Inspect actual branch tips on each host before future work.
Merge/API and checkout evidence stays under `build/master-integration-20261115/`.
Detailed preparation chronology remains in Git history and the dated reports.

## Branch retirement and current base

On 2026-11-16, after explicit user approval and ancestry checks, the merged
`audit/simd-parity-source-fingerprints-20261116` branch (tip `145c7f5`) and
`neon-port` (tip `abf7a82`) were removed locally on the Mac and from origin.
Their commits remain in `master`; the validation inputs and reports are intact.
At this sweep, only `master` exists locally on this Mac and on origin.
`upstream/master` is a remote-tracking reference to original VCL, not a retired
fork development branch, and remains available for reference.

`windows-native-validation` is absent from this Mac and origin. Another machine
may still have a local copy; no removal there is claimed or authorized by this
sweep. Check its actual state on Windows before any branch action. The completed
detached worktree under ignored `build/final-mac-bd7807e/` was subsequently
retired with explicit user approval after its records were verified in compact
archives. The [retention follow-up](validation/evidence-retention-2026-11-16.md)
records the archive locations and checksums. Its frozen source remains in Git;
new passes use fresh worktrees/builds. Only the main checkout remains registered.

## Historical evidence and cleanup

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
  `tools/validation_evidence.py`. Native M2 execution was outstanding at that
  checkpoint; the later frozen-source Mac report records the updated runner's
  execution. Evidence stays under `build/master-cleanup-20261115T222559Z/`.
- The superseded Intel handoff was removed; current instructions use the
  [Windows runner guide](validation/windows-runner.md). Historical reports, raw
  evidence and benchmark measurements retain their original checkpoints. Git
  history and the host-local command logs record the earlier helper/documentation
  cleanup and its publication.

## Completed validation work

- [x] Prepare the [shared SIMD comparison harness](validation/cross-platform/README.md)
  and [preparation evidence](validation/cross-platform/preparation-2026-11-16.md).
  Eight candidate Windows profiles passed 5,376 corpus cases after correcting
  a non-default `roundi` oracle assumption; all earlier failures remain in evidence.
- [x] Obtain the approval required by the Windows final-verification prompt
  to commit/push this new harness. Freeze the resulting published commit and
  record its full SHA in the final run manifests and Windows report.
- [x] Freeze approved published commit `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`
  (tree `2a0a5d3250201773cccb06ba0562df728f4e9387`) and complete the final Windows
  MSVC/Clang run: 5,392/5,392 CTest cases and separately 5,376/5,376 corpus vector
  cases. The [final report](validation/windows-amd-2026-11-16-bd7807e-final.md)
  and [portable outputs](validation/cross-platform/simd-parity-20261116/windows-amd/README.md)
  accompany this approved result publication. All 28 Windows-only pairings
  have zero unexpected differences; this does not establish native Mac parity.
  Run ID: `simd-parity-20261116`. Raw final evidence is under
  `build/windows-final-simd-20261116T053510Z/`. Raw preparation/audit evidence is under
  `build/cross-platform-preparation-20261115T233331Z/`; original native attempts
  remain under their `build/simd-capture-<UTC>-<profile>/` directories.
- [x] Obtain explicit approval to commit/push the portable Windows results so
  the Mac can fetch the same manifests, output bits, hashes and case inventory.
  This result publication changes documentation/data only; the tested source
  remains frozen at `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
- [x] Review the cleanup and obtain explicit maintainer approval to commit/push.
- [x] Update the Mac checkout to `master` using the
  [native workflow](validation/native-workflow.md), preserving unrelated work.
  Fetch the reviewed portable reports, then use a clean worktree at frozen
  `bd7807e91f5888bc2ad708ca46c9305d166d6c4b` for execution; use the whole checkout.
- [x] Run the full native Mac matrix at that frozen commit/run ID: 8,036/8,036
  cases and eight examples passed with native binary, strict UBSan, runtime and
  guard controls. All 135 tracked files matched frozen Git blobs; source stayed clean.
- [x] Capture all eight Mac corpus profiles: 3,996/3,996 independent-reference
  vector passes, 24,672 output lanes. Preserve four Debug invocation failures:
  the frozen loader rejects Apple UBSan's intentional-overflow `SIGABRT` (`-6`)
  because it requires exit code `1`. No capture or matrix was retried or relabeled.
- [x] Complete the cross-host comparison. After the original 24 Release passes,
  the maintainer explicitly authorized fixing the validator and rerunning the
  eight rejected Debug pairings. The host-specific UBSan exit-code correction
  passed ten regression controls; all eight offline reruns passed. All 32
  pairings now have successful comparisons with zero unexpected differences.
  This uses the original captures and a separately hashed validator patch;
  original failed invocations, manifests and rejection records are unchanged.
- [x] Reconcile the whole-file source fingerprint using all four original Windows
  initial/final manifests and verified original archive checksums. The
  [source-byte audit](validation/source-byte-audit-2026-11-16-bd7807e.md) confirms
  135 paths on each host, no missing/extra paths, 43 identical hashes and 92
  LF-to-CRLF transformations reproducing every recorded Windows hash. Original
  fingerprints/manifests, historical reports and checksum inventories are preserved.
  The [portable Windows inventory](validation/cross-platform/simd-parity-20261116/source-manifests/windows-amd-bd7807e/summary.md)
  records per-file differences and original archive provenance. No source-byte
  fingerprint gap remains; no native rerun, numerical-policy change or
  harness/library modification was needed.
- [x] Publish the approved audit at `145c7f5`, based on `99f4a3d`. Both
  `origin/master` contains it; the temporary audit branch was subsequently retired
  as recorded above. Raw audit work stays under ignored
  `build/source-byte-audit-20261116T182036Z/` on the Windows host.

The [Mac final report](validation/mac-arm-2026-11-16-bd7807e-final.md) and
[portable results](validation/cross-platform/simd-parity-20261116/mac-m2/README.md)
record the original partial comparison outcome and raw archive receipt.
The [validator follow-up](validation/mac-arm-2026-11-16-bd7807e-validator-followup.md)
records the authorized offline reanalysis and successful completion. The
maintainer approved committing and pushing the reviewed validator fix,
regression tests, Mac reports and portable evidence together at `99f4a3d`.
The [published source-byte audit](validation/source-byte-audit-2026-11-16-bd7807e.md)
closes the remaining evidence gap. The captured source remains `bd7807e` and
raw archives stay under ignored `build/`. Dated attempts retain their original
pre-publication status and all original hashes; publication is recorded here
and in Git history.

## Continuing development

There is no remaining action for this selected validation checkpoint. Start new
library work from current `master`, using a topic branch when useful. Preserve
unrelated work and raw evidence, and validate changes according to the native workflow.
Future source checkpoints require a new run ID and an approved frozen source shared
by both hosts. Direct Neon remains experimental; physical Intel, ASan on the
documented Mac host, FP16 and unselected hosts/APIs remain outside this checkpoint.
