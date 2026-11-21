# Build reset and native validation plan

This is an on-demand procedure for discarding this checkout's local `build/`
contents and establishing a fresh native validation baseline on the current
host. Saving, reading or reviewing this document does not execute the reset.

## Invocation and authorization

Example request:

> Execute docs/validation/build-reset-and-native-validation-plan.md on this host.

An explicit request to execute this reset plan authorizes discarding the
inventoried local `build/` contents, including historical evidence and downloaded
dependencies, subject to every gate below. If the request only asks for review,
planning or validation without deletion, do not infer authorization to wipe.
Resolve ambiguous deletion scope before proceeding.

At every invocation, ask: **"Is separate performance validation authorized for
this reset: yes or no?"** Record the answer for this session on either host.
Always ask, even if an earlier session included performance. If the invocation
already explicitly answers this question, acknowledge and record that answer
instead of requesting it again. No answer is not authorization for performance.
Read-only consistency checks may proceed while awaiting the answer; do not wipe
or start validation until it is recorded. A negative answer selects correctness
only, including on ARM64.

Before creating or editing any file outside repository-root `work/`, `build/`
or `out/`, ask for explicit authorization for the intended paths and changes.
An authorization already supplied for this session's specific paths is valid;
do not ask again. This applies to existing documentation too. Default to `work/`
for scratch/pre-reset records and `build/` for builds and retained evidence.
Permission to use `out/` is not permission to purge it or change ignore rules;
verify its ignore status before use. Do not create other scratch locations.

Reset execution does not authorize runtime/test/runner changes, tool installation,
machine-wide settings, commits, pushes, merges, tags, releases or branch deletion.
It does not authorize cleaning `work/`, `out/`, another checkout or another host.

## Required reading and host selection

Read the current [agent instructions](../../AGENTS.md),
[fork guide](../../README.md), [contribution rules](../../CONTRIBUTING.md),
[integration status](../integration-plan.md), [validation index](README.md)
and [shared native workflow](native-workflow.md). Read the selected architecture's
runner, publication checklist and current dated results. This plan adds a hard
clean-checkout gate; do not use the general captured-dirty-source exception here.

Identify the physical CPU, operating system, process architecture, Python target
and compiler target. Select scope from the actual executing host:

| Verified host | Reset and validation scope |
| --- | --- |
| Native AMD x64 Windows with native x64 tools | Local build reset, then x86 Windows validation only. |
| Native Apple Silicon macOS with native arm64 tools and Rosetta disabled | Local build reset, then ARM64 validation only; current approved host is M2. |
| Conflicting, unknown or unsupported host evidence | Stop and ask which architecture and host are intended. |

Always ask if x86 versus ARM64 is uncertain. Never infer the architecture from
folder names, prior conversation, a cross-compiler or transferred artifacts.
Intel macOS, Linux, Windows ARM64, another unapproved physical host, Rosetta,
WSL, QEMU and hosted CI are not substitutions for the two native scopes. Ask
for a separately agreed procedure when the detected host falls outside them.
No other host is contacted to execute validation. No cross-host parity claim
follows from this single-host reset.

## Phase 1: consistency sweep and mandatory stop conditions

Perform this sweep at every invocation, even if an earlier sweep passed.
Retain the concrete findings under a fresh ignored `work/` session directory
outside the deletion target. Use fresh session/attempt IDs and record actual
execution dates, including UTC timestamps and the host's local date.

1. Identify the repository root, branch, HEAD/tree, remotes, index and every
   registered worktree. Inspect status without optional Git writes, for example
   `git --no-optional-locks status --porcelain=v1 --untracked-files=all`.
   Confirm the fork and remotes match the current repository instructions.
2. **Stop if any repository file is changed or ready for commit.** Block staged
   or unstaged modifications, additions, deletions, renames, conflicts,
   nonignored untracked files, dirty submodules and incomplete Git operations.
   Check the main checkout and other registered worktrees of this repository;
   never discard or repair someone else's pending work. Legitimately ignored
   build/scratch artifacts alone do not make the source dirty. Do not stash,
   commit, reset, hide changes with ignore/index flags, or create a clean worktree
   to bypass this gate. Ask the maintainer to resolve pending work separately.
   An uncommitted copy of this plan also blocks execution.
3. Confirm `build/` contains no tracked repository paths. Resolve the actual
   checkout root and absolute `build/` target, accounting for a verified checkout
   junction or alias. Every deletion target must remain inside that resolved
   `build/`; reject an unresolved path, external build-root link, child symlink,
   junction, reparse point or nested mount that would escape the deletion scope.
4. Inventory the current build contents, sizes, dependencies, evidence, source
   captures and any nested repositories. Inspect Git worktree registrations
   and administrative directories. If a registered worktree is inside `build/`,
   stop and obtain authorization to remove the named clean worktree through Git;
   never recursively delete it or use force. Unrelated or ambiguous nested
   repositories/material require clarification. Recognized disposable pinned
   dependency checkouts are included in an explicitly authorized full reset,
   but inspect their Git status too: pending changes in a nested dependency
   block deletion even when the parent build directory is ignored.
5. Check for active compilers, tests, benchmarks, package/cleanup processes and
   services using this build tree. Stop while anything is writing to or running
   from it. Do not terminate unrelated processes. Do not reset concurrently with
   another agent or user operation; if exclusive use is uncertain, ask.
6. Inspect installed native tools and sanitizer runtimes. Confirm disk space,
   required filesystem permissions and a workable short output path on Windows.
   Prove the pinned testbench can be restored before deleting its local copy:
   if necessary fetch a fresh copy into a new ignored `work/` preflight directory,
   then verify the exact revision, cleanliness and all three pinned hashes.
   Stop if required tools, storage or dependency availability is missing.
7. Run the repository and protected-byte checks before deletion. Review the
   current runner plans, expected named-case inventories, benchmark protocol
   when selected, and report instructions for internal consistency. Verify
   literal paths and command options against the current tools. Stop on an
   integrity failure, broken required path, unexplained matrix change or
   contradictory instruction; do not weaken controls to proceed.
8. Record a pre-reset receipt: resolved target, inventory, deletion scope,
   source commit/tree and raw source fingerprint, host/tools, dependency pin,
   performance answer, processes/worktrees check and known evidence losses.
   Show the maintainer a concise scope and loss summary. If explicit reset
   authorization already covers that inventory, continue without asking again.
   If shared, unrelated or unexpectedly required material falls outside that
   authorization, stop and obtain a scoped decision first.

The pre-reset receipt must survive deletion: start it in ignored `work/`, then
copy it into the newly created `build/` before validation. Do not create a
replacement archive of the old build. A full reset knowingly discards local
historical payloads; published summaries and hashes cannot recover their bytes.

## Phase 2: reset only the approved local build

Immediately before deletion, repeat the clean-state, HEAD/tree/fingerprint,
worktree, active-process and resolved-path checks. Any change invalidates the
previous sweep; stop and reconcile it before continuing.

Delete only the enumerated approved files inside `build/`, then remove empty
directories without following links. On Windows use a single native filesystem
API or PowerShell end-to-end with literal paths; do not compose deletion across
shells. Do not use a repository-wide clean/reset command. Keep or recreate the
root `build/` directory. If deletion fails or a new file appears, retain the
failure receipt and inspect the situation rather than widening the command.

Verify the old contents are gone and source bytes, HEAD, index and unrelated
files remain unchanged. Do not modify current reports before deletion: that
would violate the clean-state gate. Record obsolete artifact availability in
the surviving receipt, then prepare documentation corrections after collection.

## Phase 3: reconstruct resources from the frozen repository

Use the same selected clean commit and raw source identity throughout. Do not
pull, merge, update dependency pins or resume a retired repair campaign during
the reset. If the source changes, stop; do not silently reassign results.

- Verify `build/` and `work/` are ignored. Use fresh, short, architecture-labelled
  output directories with unique session IDs. Confirm normal builds and probes
  cannot emit files beside sources. Keep native source snapshots byte-exact and
  include both architectures when recording the complete source identity.
- Restore pristine testbench revision
  `a7293702385e5169a5f58b83177f2a43cd59139b` into `build/_deps/testbench`.
  Use the verified fresh preflight checkout or fetch the exact pin through the
  existing helper/native Git instructions. Verify cleanliness and all three
  source hashes again. Never overwrite an existing dependency unexpectedly.
  Required resources must no longer depend on `work/` before collection closes.
- Reconstruct the original VCL comparison resource from protected repository
  files with `tools/restore_upstream_vcl.py` when required by the selected checks
  or benchmark reference. Verify the audit pin; do not download a newer VCL,
  overwrite root originals or recreate retired source/vendor layouts.
- Record executable/tool versions, targets, compiler/SDK and sanitizer runtime
  identities. Initialize a command-scoped native developer environment; reject
  inherited compiler-policy overrides. Do not change saved tool configuration,
  execution policy or global compiler flags.
- Repeat the repository and read-only integrity checks. Use `python` on Windows
  and native arm64 `python3` on macOS. The shared commands are
  `tools/check_repository.py` and `tools/regenerate_headers.py --check`.
  Never invoke the retired generator's write mode. Run isolated integrity
  controls with `tools/check_header_generation.py` as required by the host guide.

## Phase 4A: native Windows x86 correctness

Follow the [x86 compiler guide](../x86-build-and-test.md) and
[Windows runner](x86/windows-runner.md). Inspect the runner plan with the verified
testbench path supplied; omitting it selects only the smaller local suite.
Use native x64 MSVC and GNU-driver Clang, not clang-cl or emulation.

| Compiler | Profile | Mode | Ordinary compiler policy |
| --- | --- | --- | --- |
| MSVC | upstream-optimized | Release | `/O2 /fp:precise /fp:except-`, no RTC |
| Clang | upstream-optimized | Release | `-O2 -fno-trapping-math -fno-math-errno` |
| MSVC | strict-validation | Release | Retained strict FP controls |
| Clang | strict-validation | Release | Retained no-contraction/rounding controls |
| Clang | strict-ubsan | Debug | `-O0`, nonrecovering UBSan, frame pointers, static release CRT `MultiThreaded` |

Run all five pinned baseline profiles: each currently registers 1,348 cases and
one separate example. After proving CPU and OS/XCR0 support, run the documented
40-case local AVX2/FMA and AVX-512 selections for each configuration. These
higher-ISA selections are local coverage, not full pinned-testbench coverage at
those ISAs. With both higher ISAs supported, the complete selection is 7,140
case instances and fifteen examples across fifteen configurations. Unsupported
ISA selections remain explicit gaps. Reconcile actual full named inventories;
these numbers are registration expectations, not predicted passes.

Verify facade examples, native x64 PE identity, runtime detection, x86-only
include graphs, actual-page-size writable and all four fault controls, and
UBSan positive exit zero plus deliberate signed-overflow exit one with the
expected diagnostic and no continuation. MSVC Debug/RTC remains deferred and
rejected. Do not relabel the existing failed/incomplete Windows gate as green.

## Phase 4B: native macOS ARM64 correctness

Follow the [shared native workflow](native-workflow.md) and current
[ARM64 validation index](arm64/README.md). Verify native arm64 Python, Apple
Clang, SDK, CPU/OS, Rosetta disabled, and native arm64 Mach-O outputs. Keep the
Mac awake for collection without changing persistent system policy.

Use `tools/run_native_arm_validation.py --plan` to review the current full
matrix, then execute that runner with `--expected-commit` identifying the frozen
full SHA and `--testbench-dir` identifying the verified dependency. Do not use
`--local-only` or `--capture-manifest` for this clean reset procedure.

| Backend | Logical maximum | Release cases | Strict UBSan Debug cases |
| --- | --- | ---: | ---: |
| sse2neon compatibility | 512 | 1,350 | 1,350 |
| Direct Neon | 128 | 486 | 486 |
| Direct Neon | 256 | 890 | 890 |
| Direct Neon | 512 | 1,292 | 1,292 |

Expected full selection is 8,036 case instances across eight profiles, plus
eight examples. Direct Neon remains experimental and explicitly enabled;
compatibility remains the default. Preserve the full named inventory.

Verify runtime-query consumers, ARM-only include graphs, actual-page-size
writable and four protected-fault controls, partial-access canaries and strict
nonrecovering UBSan. Require the positive control to exit zero and intentional
overflow to emit its diagnostic with no continuation; preserve actual exit one
or SIGABRT (-6) as applicable. Never retry the documented ASan hang.

## Shared correctness and optional performance rules

Keep 60-second correctness deadlines, concurrency three, independent references,
fixtures, seeds, numerical tolerances, ABI/namespace/width settings and FP/rounding
controls unchanged. Keep fast/finite-math flags confined to classification-only
stress. Collect complete logs, command/exit journals, generated inputs, named
statuses, JUnit, compiler/flag/binary hashes and every failed setup/run attempt.
Do not mix timings into correctness work. Missing binaries, controls and runtime
support are failures or gaps, never silent omissions.
Keep the first failed runner return packet. Any necessary build-abort
continuation gets its own recorded attempt; do not overwrite failures or replay
completed cases merely to obtain a clean-looking report.

If performance was authorized, run it separately after correctness collection,
at the same frozen source, with optimized unsanitized Release builds. Follow the
current host's protocol:

- x86: [native original-VCL comparison protocol](../benchmarks/x86/x86-upstream-comparison.md)
  and [x86 benchmark sources](../../benchmarks/x86/README.md). Confirm the exact
  reference/source pair and workloads. Preserve the benchmark protocol's actual
  strict FP flags separately from optimized correctness flags.
- ARM64: [benchmark guide](../../benchmarks/README.md),
  [ARM64 benchmark sources](../../benchmarks/arm64/README.md) and
  [standard-math report template](../benchmarks/arm64/STANDARD_MATH_RESULTS_TEMPLATE.md).
  Use matched compatibility/Direct Neon comparisons and the declared widths,
  layouts and working-set counts. No application-specific consumer is implied.

Validate workload outputs independently before timing. Do not time a workload
whose required correctness check failed. Preserve the full suite's failures
even when an independently checked benchmark can run. Use the declared warm-up,
calibration, alternating order, repetitions and host/power/affinity controls;
run timings sequentially without concurrent builds or tests. Retain raw samples,
variability and slower rows. Do not substitute old timings or another host's
results. If performance was declined, explicitly record that it was not run.

## Phase 5: closeout and publication permission

1. Repeat source, repository, protected-header, dependency and consistency
   checks. Reconcile every selected named case, examples and control outcome.
   Record source/host/profile gaps and all failed or aborted attempts. If source
   or tool identities drifted, stop and report the affected coverage honestly.
2. Retain fresh evidence under ignored `build/` and copy the pre-reset receipt
   there. Keep required source/provenance packages produced by the native runner;
   do not create additional compaction/backup archives without a demonstrated
   need. Do not delete fresh failed evidence or perform a second cleanup.
3. Prepare the report and portable evidence under `work/` using the applicable
   [x86 publication checklist](x86/results-publication-checklist.md) or
   [ARM64 publication checklist](arm64/results-publication-checklist.md).
   For correctness-only ARM64 sessions omit performance destinations and state
   the declined choice; this plan does not require ARM timings. Include the
   performance decision, exact source, profile summary, compiler/backend/ISA
   results table, examples, controls and remaining limits. Copy no invented data.
4. Prepare explicit availability corrections for existing documents that still
   say discarded historical payloads are locally retained. Preserve their
   original identifiers, hashes, numerical results and source claims. A fresh
   run does not reconstruct discarded evidence or replace a historical gate.
5. Ask for authorization for each intended permanent report/bundle path and
   existing documentation edit outside `work/`, `build/` or `out/`. Provide
   concrete drafts, destination paths and a concise diff first. After approval,
   apply only those changes and run repository/link/whitespace/checksum checks,
   including untracked copies. Do not publish or commit without separate scope.

Final report: state which host/scope ran, the frozen identity, what was deleted,
what was reconstructed, correctness totals, performance authorization/outcome,
remaining gaps, retained evidence paths and any pending publication approval.
If a gate stopped execution, say where it stopped and whether deletion occurred.

## Additional safety measures included

- Repeat the clean-state and path checks immediately before deletion; an earlier
  pass is not sufficient when another process can change the checkout.
- Confirm dependency recovery and required tool/runtime availability before
  deleting the only local copy. Keep pre-reset records outside the wipe target.
- Require exclusive use of the build tree; reject escaping links/mounts and
  registered worktrees rather than broadening deletion commands.
- Keep the source immutable through collection. Perform approved report edits
  only after the validation identity and results are closed out.
- Leave active/failed evidence intact after the new run. Any future reset is a
  new invocation with a fresh consistency sweep and performance decision.
