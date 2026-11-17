# Agent instructions

These instructions apply to this VCL2 XA64 fork on every operating system.
Keep this file generic and versioned. Machine-specific paths, installed-tool
inventories and raw execution evidence belong under ignored `build/`.
An optional `build/local-validation/README.md` can record resources available
on a particular machine; its absence must not prevent a fresh setup.

## OS-agnostic

### Repository and fork protection

- Read `README.md`, `CONTRIBUTING.md`, `docs/integration-plan.md` and
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

- Maintain runtime behavior in the canonical `adaptation/` patches/include
  sources and `backends/` implementations. Keep `vendor/vectorclass/` byte-exact
  to its pinned upstream inventory; an upstream update requires explicit scope.
  This is mandatory on every host, including Mac M2: never edit, format,
  regenerate, normalize line endings or update this pristine directory to fix
  validation failures. Verify it before and after native validation. On any
  mismatch, retain evidence and stop; do not alter the pin/hash inventory to
  accept changed vendor bytes. Only a separately authorized upstream-update
  task may replace the pinned snapshot.
  Root runtime headers are shipped generated products for direct header-only
  use. Follow `docs/backend-layout.md`; regenerate after canonical changes and
  run `tools/regenerate_headers.py --check` plus the repository check. Do not
  leave hand-edited generated headers or require application users to generate.

- Preserve public headers/API names, lane ordering, ABI settings, notices and
  upstream attribution. ARM defaults to sse2neon compatibility; Direct Neon
  requires its explicit experimental opt-in. Keep benchmark measurements intact.
- Validation is manual on native Mac M2 and AMD Windows hardware. Do not start
  hosted test workflows, WSL/QEMU/Rosetta execution or benchmark timings as part
  of native correctness validation. Physical Intel testing remains omitted.
- Keep independent references, fixtures/generators, seeds, numerical gates,
  rounding/FP flags, protected-page controls, canaries, strict UBSan checks,
  original 60-second deadlines and runner concurrency of three intact.
  Do not relax acceptance criteria after observing differences.
- Preserve `tests/`, `tools/`, CMake files, fixture generators, the shared corpus,
  policies, comparator and native runners. Reuse the pristine optional testbench
  pinned at `a7293702385e5169a5f58b83177f2a43cd59139b`; verify its revision,
  cleanliness and source hashes. Never overwrite an existing dependency checkout.
- Both hosts must capture the same approved frozen source for a comparison run.
  Use a new run ID for a new source checkpoint and a unique attempt ID for each
  invocation. Preserve input/output bits and the complete registered inventory;
  do not compare only the intersection or claim universal bitwise identity.
- Exact tested commit/tree, source fingerprints and validator revision matter.
  Later documentation or publication commits do not acquire earlier native passes.
  Offline reanalysis must identify its validator separately and preserve captures.
- Retain failed attempts, original logs, archives, manifests, checksums and dated
  reports. Do not rewrite historical evidence or weaken its hash checkpoints.
  Put current status in the validation index and integration plan; add a dated
  follow-up for new findings. Original pending notes can remain in dated records.
- All raw artifacts go under ignored `build/` in the active checkout. Keep
  original evidence archives, logs, results, source/compiler/flag inventories,
  checksums, raw output bits and failed attempts. Before cleanup, verify that
  compact archives cover these records and preserve any uncovered diagnostics,
  inputs or generated reference/probe sources in a checksummed supplement.
- After a completed run and explicit user authorization, rebuildable objects,
  executables, debug symbols, generated build directories and temporary worktrees
  may be removed once evidence coverage is verified. Retain recorded binary hashes
  and the reachable source commit; new passes rebuild in fresh directories.
  Preserve testbench/tool installations and unrelated work. Use Git's worktree
  removal command rather than deleting its administrative metadata manually.
- Publish only reviewed portable metadata, summaries and raw-bit outputs, free
  of machine paths, credentials and binaries. Compact evidence is an audit record,
  not a backup of the original compiled executables; never relabel old results.
- Before comparator self-checks on a fresh checkout, create ignored `build/`:
  `python -c "from pathlib import Path; Path('build').mkdir(exist_ok=True)"`.
  Use `python3` on macOS. Follow `docs/validation/cross-platform/README.md` for
  corpus checks/capture and `docs/validation/report-template.md` for reports.
- Run checks appropriate to the change. Documentation changes need link/repo
  and whitespace checks, not a new native matrix. Runtime, test, fixture or
  runner changes need fresh relevant validation. Do not label plans as results.

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

- Keep `vendor/vectorclass/` untouched throughout M2 work. Run
  `python3 -B tools/regenerate_headers.py --check` before and after tests; this
  always verifies exact vendor bytes, regardless of generated-header line
  endings. Put necessary ARM fixes in canonical `backends/arm64/` or explicit
  adaptation patches, regenerate root products, and obtain a new approved
  frozen source before claiming a fixed run. Never repair tests in the vendor
  tree. Follow `docs/validation/mac-arm-correctness-handoff.md` for the current
  approved correctness handoff and evidence requirements. The earlier
  `docs/validation/mac-arm-x86-optimization-handoff.md` remains historical.

- Use native arm64 Python 3.9+, Apple Clang, CMake 3.20+ and CTest supporting
  `--output-junit`. The full runner can use Ninja or make; shared corpus capture
  requires Ninja. Inspect existing tool installations before installing anything.
  Keep machine-specific tool paths out of this shared file.
- Verify CPU/OS, compiler target and Rosetta state. Use `python3` and
  `xcrun --find clang++`. Do not execute translated x86 binaries as native proof.
- Start with `python3 -B tools/run_native_arm_validation.py --plan`.
  For execution use a clean approved commit with `--expected-commit` and reuse
  the pinned dependency via `--testbench-dir` when it is already available.
  See `docs/validation/native-workflow.md` for the full commands.
- Run compatibility 512 and Direct Neon maxima 128/256/512, each in Release and
  strict nonrecovering UBSan Debug. Inspect arm64 Mach-O binaries, runtime
  consumers, actual page-size guard controls and independent fixtures.
- Apple UBSan's intentional-overflow control may abort with SIGABRT (`-6`) or
  exit `1`; require the expected diagnostic, no continuation, and a passing
  positive control. Never edit recorded exit codes to satisfy a validator.
- Do not retry the documented Mac ASan startup hang. ARM exception flags/masks
  are not emulated; preserve the documented numerical and FP-environment limits.

## Windows

- Use native x64 Windows on the AMD host, an x64 Visual Studio developer shell,
  Python 3.9+, CMake, CTest and Ninja. Prioritize MSVC and also use native x64
  GNU-driver Clang for compiler comparison and strict UBSan. No WSL/emulation.
- Use `python`; start with
  `python -B tools/run_windows_validation.py --driver msvc --mode Release --plan`.
  `tools/run_windows_validation.ps1` can initialize the installed developer
  environment. Do not change machine-wide execution policy or tool configuration.
- Follow `docs/validation/windows-runner.md` for all four Release/Debug profiles
  and the pinned upstream selection. MSVC Debug is unsanitized, not UBSan proof.
  Probe CPU AND OS ISA support before AVX2/AVX-512 execution. Verify x64 PE files,
  runtime detection and actual-page-size protected-memory controls.
- Strict UBSan uses native GNU-driver Clang Debug with disabled recovery, frame
  pointers and halt-on-error settings. The recorded Clang runtime requires
  static release CRT (`MultiThreaded`) even in Debug; inspect installed runtimes.
  Require positive exit zero and intentional signed-overflow exit `1`, with the
  expected diagnostic and no continuation. Missing runtimes/controls are gaps.
- Retain `/fp:strict` for ordinary MSVC tests and `/fp:fast` only for the separate
  classification stress target. Keep Clang's no-contraction/rounding flags and
  classification-only finite-math stress separate from ordinary numerical tests.
- Preserve original Windows source inventories and line endings. Compare hashes
  using the documented algorithm; do not replace a retained run's manifest with
  a regenerated inventory from a later checkout. Transfer raw archives separately.
