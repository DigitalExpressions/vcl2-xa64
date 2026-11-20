# Agent instructions

These instructions apply to this VCL2 XA64 fork on every operating system.
Keep this file generic and versioned. Machine-specific paths, installed-tool
inventories and raw execution evidence belong under ignored `build/`.
An optional `build/local-validation/README.md` can record resources available
on a particular machine; its absence must not prevent a fresh setup.

## Current strategy reset

- On 18 November 2026 the maintainer requested cleanup and retention of ARM/Neon
  lessons before choosing a new implementation strategy. Start with
  `docs/neon-arm-lessons.md` and `docs/integration-plan.md`.
- The root/upstream and `_neon.h` separation is documented in
  `docs/backend-layout.md` and `docs/arch-separation-map.json`. The maintainer approved implementation and
  native validation on Mac M2 followed by native AMD Windows verification.
  That work is complete and integrated; the Mac execution handoff is historical.
  See `docs/validation/architecture-separation-integration-completed-2026-11-19.md`. The
  platform-neutral include name `vcl2_xa64.h` is agreed.
  Preserve `docs/x86_findings_and_fix_candidates.md`
  for future x86/upstream research without resuming the retired campaign.
- The x86 correctness/optimization campaign and old execution handoffs are
  retired. Do not automatically resume them or treat their fixes as requirements
  for new ARM work. Existing source remains unchanged until new scope is agreed.
- The maintainer subsequently approved committing and pushing the completed
  architecture implementation, ARM cleanup and M2 reports on `arch-separation-m2`.
  Native AMD Windows validation and targeted triage are complete with their
  original failed/incomplete correctness gate retained. On 19 November the
  maintainer approved local integration into `master` and evidence-backed cleanup;
  see `docs/validation/architecture-separation-integration-2026-11-19.md` for the
  explicit x86 preservation/equivalence acceptance and remaining limits. The
  maintainer then separately approved pushing `master`; origin was verified at
  `5e039b01b49abc00548123763be85898932e3ad8`. That completed approval does not
  authorize later publication, PRs, branch retirement or runtime changes.
  Preserve reusable resources, source captures and retained archives.
- The maintainer subsequently approved retaining/integrating the four-blend
  candidate, publishing the Windows report and evidence-backed local cleanup.
  Integration/publication and verified local cleanup are complete; see
  `docs/validation/blend4-integration-2026-11-19.md`. The full Windows gate
  remains failed/incomplete. The measured 2.2% / 2.7% TRS-inverse elapsed costs
  are accepted for this integration and remain unresolved.
- The TRS-inverse follow-up and Windows return have been reviewed. Keep the
  current master runtime; the frozen candidate is not selected for integration.
  See `docs/validation/trs-inverse-decision-2026-11-20.md` for the
  tradeoffs and current artifact availability. On 20 November the maintainer
  requested discarding obsolete local Mac experiment material without creating
  replacement archives. Earlier execution handoffs are historical, not pending
  tasks. New runtime work and publication require their own agreed scope.
  Obsolete pre-19 November Mac documentation was subsequently retired; see
  `docs/validation/mac-arm-documentation-cleanup-2026-11-20.md` for the retained
  source/comparison records and reasons. Do not restore old execution plans.

## OS-agnostic

### Repository and fork protection

- Read `README-vcl2-xa64.md`, protected `README.md`, `CONTRIBUTING.md`, `docs/integration-plan.md` and
  `docs/validation/README.md` before making changes or validation claims.
- Do not create pull requests for this fork or against the original VCL project.
  Preserve the fork relationship, default branch `master` and published history.
  Do not change remotes, ownership or repository configuration unless the user
  specifically requests that action. Never push to the original upstream repo.
- `origin` is `https://github.com/DigitalExpressions/vcl2-xa64.git`;
  `upstream` is `https://github.com/vectorclass/version2.git`. The latter is the
  original library, not another development branch for this fork. Read-only
  inspection or fetching does not authorize integrating or publishing changes.
- Commit, push, merge, release, tag and branch-retirement actions require the
  user's task-specific authorization. Historical approvals apply to the
  completed actions, not future publication or repository administration.
- Inspect the actual branch, HEAD, tree, status and worktrees first. Do not
  discard unrelated work, force-push or rewrite published history. Use a fresh
  worktree when a clean validation checkout is required.
- `master` is the shared development base. The user retired `neon-port` and
  `audit/simd-parity-source-fingerprints-20261116` locally on the Mac and from
  origin after their commits were confirmed in master. Do not recreate them
  or require them for tests. `windows-native-validation` has no origin ref in
  the audited state, but older local checkouts may retain a copy. Inspect each
  host and obtain explicit approval before deleting it. Do not assume cross-host state.
- Leave any legacy sibling `vcl2-arm64` checkout and its retained `port-tmp`
  workspace alone. Its earlier approved restoration is complete. Further edits,
  deletion, migration or cleanup require separate explicit instructions.
  Read-only Git inspection there must use `git --no-optional-locks` or
  `GIT_OPTIONAL_LOCKS=0`; never write build/log output there.
- Do not implicitly modify external consumers, application tests, SDKs, scenes
  or license integrations. Application-specific tests stay outside this library.

### Library and validation contracts

- The separated layout is integrated into local `master`; verified local cleanup
  is complete. See `docs/validation/architecture-separation-integration-completed-2026-11-19.md`.
  The original Windows correctness gate stays failed/incomplete.
  Read `README-vcl2-xa64.md`, `docs/backend-layout.md` and the dated validation
  report for its actual result/source identity. `README.md` is protected upstream.
- The maintainer approved replacing the duplicate VCL vendor snapshot with
  `audit_x86_history.md`, `audit_arm64_history.md` and retained audit manifests.
  `vendor/vectorclass/` is retired; its pin moved byte-exact to
  `audit/vectorclass-2.02.04.json`. All 28 original root paths remain protected
  exact bytes. Never edit, normalize, format or regenerate them for ARM work or
  to make tests pass. Read-only checks verify SHA-256, Git blobs and the upstream
  tree directly; `tools/restore_upstream_vcl.py` reconstructs a comparison copy
  under ignored build without overwriting existing files. Keep sse2neon root,
  vendor header and its existing pin exact; its upstream representation audit
  records differences without normalizing. Verify before/after validation.
  An upstream dependency update requires separate explicit scope. Historical
  captures retain their old vendor layout and must not be rewritten.
- Authoritative ARM implementations are root `*_neon.h` files and
  `instrset_detect_neon.cpp`. `vcl_config.h` is neutral selection policy;
  `vcl2_xa64.h` selects the complete supported vector/math family at compile time.
  Compatibility remains default; Direct Neon requires explicit experimental opt-in.
  Native FP16, SVML and ARM64EC are outside this common surface.
- ARM header cleanup removed inactive x86 implementations and old MSVC x86
  workarounds. Preserve that boundary: read-only integrity checks also reject
  native x86 helpers, ISA macros and wide intrinsics in inactive ARM branches.
  Keep required SSE-shaped sse2neon APIs, ARM correctness fixes and generic
  compiler portability. Use the validation index and 19 November M2 report for
  the later frozen source. Earlier cleanup/candidate results remain historical;
  they do not transfer to a later publication.
- The old adapted-root generator is retired. `tools/regenerate_headers.py --check`
  is read-only; `--write` always rejects. Historical adaptation patches and
  `backends/x86/` are inactive research assets and must not enter either active
  include graph. Keep one editable ARM implementation; do not recreate copies in
  `backends/arm64/` or `adaptation/include/`.
- Preserve public VCL type/API names, ABI/namespace/width settings, attribution,
  licenses, independent fixtures, seeds, numerical gates, FP/rounding flags,
  classification-only finite-math stress, strict UBSan and protected-page/canary
  controls. Keep 60-second correctness deadlines and concurrency three.
- Use native M2 and native AMD Windows validation, no hosted CI, WSL/QEMU/Rosetta,
  ASan retries or timings during correctness work. Physical Intel remains omitted.
  Do not implicitly change external consumers or application/SDK integrations.
- Preserve tests, CMake, independent reference generators, shared corpus/policies,
  comparator and runners. Reuse pristine testbench
  `a7293702385e5169a5f58b83177f2a43cd59139b`; verify revision, cleanliness and
  all pinned hashes. Never overwrite an existing dependency checkout.
- Clean-source validation remains the default. The explicit captured-source
  mode is only for a complete frozen uncommitted implementation: capture all
  tracked and nonignored added files, exact raw hashes, added/deleted inventory,
  base commit/tree and patch. Build only the immutable capture; verify every
  source path and its fingerprint before/after. Never label the clean base as
  the tested implementation or make a source commit just to satisfy a guard.
- During active validation retain every failed attempt, raw command/exit journal, logs, inventories,
  source archive, patch, compiler/flag/binary hashes, generated probe/reference
  inputs, JUnit, checksums and dated reports under ignored `build/`. Portable
  reports contain relative paths and no credentials or binaries. Preserve
  historical identifiers/checksums; publication does not inherit native passes.
  A later explicit maintainer request may discard closed-campaign artifacts
  without replacement archives. Record that loss of availability and preserve
  useful findings; do not claim discarded binaries or raw records remain usable.
- New runtime/test/runner changes require fresh relevant checks and the full
  baseline matrix at a new frozen source. Documentation-only changes need
  repository/link/whitespace checks. Do not weaken gates after observing failures.
- Cleanup, commit, push, merge, release and branch retirement need task-specific
  approval. Unless the maintainer explicitly requests discarding obsolete
  artifacts, before authorized removal verify compact archive coverage and retain
  supplements for uncovered diagnostics/generated inputs. Keep tools/testbench,
  unrelated work and all original failed evidence; remove worktrees through Git.
- For future shared corpus checks, create ignored `build/` first and follow
  `docs/validation/cross-platform/README.md`. Do not claim cross-host parity from
  separate native correctness passes or compare only an inventory intersection.

### Completed checkpoint and preserved evidence

- `simd-parity-20261116` used commit
  `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`, tree
  `2a0a5d3250201773cccb06ba0562df728f4e9387`.
- Native cases passed: Mac 8,036 plus eight examples; Windows 5,392.
  Separate corpus references passed: Mac 3,996 and Windows 5,376.
- All 32 selected cross-host pairings passed the documented policies with zero
  unexpected differences. The original 24 Release results and eight Debug
  reanalysis results use separately identified validator revisions. Four initial
  Debug capture rejections and eight comparison rejections remain preserved.
- The validator correction/Mac evidence were published at `99f4a3d`; the source
  audit at `145c7f5` reconciled all 135 paths: 43 identical hashes and 92 verified
  LF-to-CRLF transformations. There is no remaining source-fingerprint gap.
- Reports, portable data and archive receipts are indexed under
  `docs/validation/`. Direct Neon is still experimental. Other hosts/toolchains,
  unselected APIs, native FP16 and ASan are outside this checkpoint's coverage.

## macOS

- Use native arm64 Python 3.9+, Apple Clang, CMake 3.20+, CTest JUnit and Ninja
  or make. Inspect installed tools first. Verify CPU/OS, target and Rosetta state.
- Run `python3 -B tools/regenerate_headers.py --check` and the repository check;
  exercise isolated integrity controls with `tools/check_header_generation.py`.
  ARM include traces must contain no protected original x86 header.
- Run compatibility maximum 512 and Direct Neon maxima 128/256/512, each Release
  and strict nonrecovering UBSan Debug, preserving the complete registered list.
  Apple UBSan's deliberate overflow may exit 1 or SIGABRT (-6); require its
  diagnostic, no continuation and a passing positive control. Keep actual exits.
- The architecture migration and its native validation are complete. Do not
  resume the historical migration handoff. For new agreed ARM work use a fresh
  capture after source changes; `tools/capture_source.py --help` and
  `tools/run_native_arm_validation.py --help` document explicit capture mode.
  Keep the default clean-checkout guard. Never retry the documented ASan hang.

## Windows

- The maintainer selected Agner-guided optimized compilation on 20 November
  2026: MSVC `/O2 /fp:precise /fp:except-` without RTC, and GNU-driver Clang
  `-O2 -fno-trapping-math -fno-math-errno`. See `docs/x86-build-and-test.md`.
  The profile-aware harness is integrated in the local master working tree.
  The Windows runner defaults to `upstream-optimized`; select `strict-validation`
  Release or `strict-ubsan` Clang Debug separately. Direct CMake keeps its portable
  strict default. MSVC Debug/RTC is deferred and rejected until noninteractive
  reporting is implemented and validated. Preserve numerical gates and protected
  original bytes. Exact source/results are recorded in
  `docs/validation/windows-x86-harness-integration-2026-11-20.md`.

- Use native x64 Windows on the AMD host, an x64 Visual Studio developer shell,
  Python 3.9+, CMake, CTest and Ninja. Prioritize MSVC and also use native x64
  GNU-driver Clang for compiler comparison and strict UBSan. No WSL/emulation.
- Use `python`; start with
  `python -B tools/run_windows_validation.py --driver msvc --mode Release --plan`.
  `tools/run_windows_validation.ps1` can initialize the installed developer
  environment. Do not change machine-wide execution policy or tool configuration.
- Follow `docs/validation/windows-runner.md` for explicitly selected profiles
  and the pinned upstream selection. MSVC Debug is unsanitized, not UBSan proof.
  Probe CPU AND OS ISA support before AVX2/AVX-512 execution. Verify x64 PE files,
  runtime detection and actual-page-size protected-memory controls.
- Strict UBSan uses native GNU-driver Clang Debug with disabled recovery, frame
  pointers and halt-on-error settings. The recorded Clang runtime requires
  static release CRT (`MultiThreaded`) even in Debug; inspect installed runtimes.
  Require positive exit zero and intentional signed-overflow exit `1`, with the
  expected diagnostic and no continuation. Missing runtimes/controls are gaps.
- Retain `/fp:strict` and Clang no-contraction/rounding controls in strict
  comparison profiles. Optimized tests use the compiler policy above. Keep
  fast/finite-math flags confined to the separate classification stress target.
- Preserve original Windows source inventories and line endings. Compare hashes
  using the documented algorithm; do not replace a retained run's manifest with
  a regenerated inventory from a later checkout. Transfer raw archives separately.
