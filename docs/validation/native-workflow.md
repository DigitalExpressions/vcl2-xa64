# Native validation workflow

Start with the [validation index](README.md), [source ownership](../backend-layout.md)
and [shared agent instructions](../../AGENTS.md). Architecture separation is
complete. Its migration plan and old execution prompts are not current tasks.
Current master retains the accepted four-blend code; the TRS-recovery candidate
was declined. Choose a newly agreed frozen source before implementation or runs.

Use native Apple M2/macOS ARM64 and native AMD Windows x64. Physical Intel and
other hosts/toolchains are omitted unless separately tested. Hosted CI, WSL,
QEMU/Rosetta and ASan retries are outside the documented workflow. Timing is
separate from correctness. Preserve numerical gates, fixtures, seeds, FP flags,
classification-only stress, strict UBSan, guard pages and canaries.

## Source and reusable tools

Inspect actual branch, HEAD/tree, status, refs and worktrees. Confirm origin is
DigitalExpressions/vcl2-xa64 and fetch without pruning when starting a new task.
Do not overwrite local work, change remotes, merge a candidate or recreate retired
branches. Use a fresh detached worktree when validation needs a clean checkout.
Commits, pushes, integrations and branch retirement require task-specific approval.

Inspect installed native Python 3.9+, compiler, CMake 3.20+, CTest with JUnit
and Ninja/make before installing anything. Keep machine paths in ignored local
notes. Reuse a pristine testbench at
`a7293702385e5169a5f58b83177f2a43cd59139b` with all three pinned hashes.
The runners install no tools and download no dependencies. Only if unavailable,
fetch the testbench into a new path; never overwrite an existing checkout:

```sh
sh tools/fetch_testbench.sh build/_deps/testbench
```

All 28 original root files and the audit pin remain exact bytes, as do the
sse2neon alias/header/pin. Never regenerate or normalize them. Before and after
validation use the documented read-only maintenance checks:

```sh
python3 -B tools/regenerate_headers.py --check
python3 -B tools/check_repository.py
```

Use `tools/check_header_generation.py` for isolated integrity controls when
required by the new runtime task. ARM include traces must exclude original x86
headers. New runtime/test/runner changes require fresh relevant checks and the
full matrix at the new frozen source. Documentation-only edits need repository,
link and whitespace checks; they do not require native matrix/timing execution.

## Clean-source and captured-source modes

From a clean checkout, inspect the plan before native execution. Reuse the
existing testbench via a resolved `$PINNED_TESTBENCH` path:

```sh
python3 -B tools/run_native_arm_validation.py --plan
python3 -B tools/run_native_arm_validation.py \
  --expected-commit "$(git rev-parse HEAD)" --testbench-dir "$PINNED_TESTBENCH"
```

The default guard requires a clean checkout and records actual HEAD/tree.
Keep that guard. A complete uncommitted candidate may instead use explicit
immutable capture mode; a source commit is not required just to satisfy a guard:

```sh
python3 -B tools/capture_source.py --output build/UNIQUE-CANDIDATE
python3 -B build/UNIQUE-CANDIDATE/source/tools/run_native_arm_validation.py \
  --checkout build/UNIQUE-CANDIDATE/source \
  --capture-manifest build/UNIQUE-CANDIDATE/capture.json \
  --testbench-dir "$PINNED_TESTBENCH"
```

Capture all tracked and nonignored added paths, exact raw hashes, additions,
deletions, base commit/tree and patch. Verify the complete immutable capture
before and after. Build only that source, never edit it. Captured-source results
have tested commit/tree null, with the base identified separately. A source edit
needs a new capture and fresh relevant/full validation. Do not relabel an old
pass as a later publication's native result.

## Native Mac matrix and controls

| Backend | Logical maximum | Release cases | Strict UBSan Debug cases |
| --- | --- | --- | --- |
| sse2neon compatibility | 512 | 1350 | 1350 |
| Direct Neon | 128 | 486 | 486 |
| Direct Neon | 256 | 890 | 890 |
| Direct Neon | 512 | 1292 | 1292 |

Expected inventory: 8,036 case instances across eight fresh profiles, plus eight
examples. These are registration counts, not new results. Preserve every named
case, 60-second deadlines and concurrency three. Review deliberate registration
changes; never omit a failing case to match a count. Direct Neon stays an
explicit experimental opt-in; compatibility stays default.

Verify native ARM64 Python and Mach-O execution, CPU/OS and Rosetta off. Keep
the Mac awake for the run. Prove strict nonrecovering UBSan with a passing positive
control and intentional overflow: require its diagnostic, no continuation and
actual exit (Apple may exit 1 or abort with -6). Verify writable and all four
inaccessible-page controls at the actual page size, plus runtime-query consumers,
partial accesses and canaries. Keep no implicit contraction and selected rounding
flags; classification-only finite-math stress stays separate.

Every run uses a fresh ignored build directory and retains commands/exits,
host/tools, source and binary hashes, compile/link flags, logs, JUnit, source
captures, generated inputs and all failures. Missing controls/runtimes and
unexpected exits are failures or gaps. Do not retry the documented ASan hang.

## Native AMD Windows and shared corpus

Use an x64 Visual Studio developer shell and native x64 Python/CMake/CTest/Ninja.
Use both MSVC and native GNU-driver Clang. Follow the
[x86 compiler policy and availability guide](../x86-build-and-test.md) and
[Windows runner](windows-runner.md). The integrated Windows runner defaults to
`upstream-optimized` Release. Select `strict-validation` Release explicitly to
preserve `/fp:strict` or Clang no-contraction/rounding comparisons. Direct CMake
keeps the strict default; choose `VCL_TEST_PROFILE` explicitly for optimized tests.
Clang `strict-ubsan` stays separate with the documented static release CRT.
MSVC Debug/RTC is deferred and rejected until noninteractive reporting is validated.
Probe CPU and OS before higher-ISA execution. Prove native PE, guard and sanitizer
controls; retain numerical gates, fixtures, 60-second deadlines and concurrency three.

The current full Windows gate is failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples. Bounded preservation and
matching RTC diagnostics do not convert that gate into a pass. Record new and
inherited failures separately; do not resume x86 repairs.

For a newly agreed cross-host comparison, use the
[shared corpus and policies](cross-platform/README.md) at the same approved
frozen source. Create ignored build first and check its harness. Separate native
matrix passes are not cross-host parity. Use fresh run/attempt IDs, preserve
full inventories and compare all selected records under their declared policies.

## Reports and cleanup

Use the [report template](report-template.md); publish portable relative paths,
source/tool identities, actual attempts, failures and coverage gaps without
credentials, machine paths or binaries. Existing dated passes keep their own
source identities. Benchmarks require a separate matched Release protocol with
all slower rows, output checks and variability; correctness runs collect no timing.

During active work retain complete evidence. Later cleanup needs explicit scope;
normally verify compact coverage before removing outputs. A maintainer request
to discard obsolete campaign artifacts may instead remove them without new
archives, as recorded in the [20 November cleanup](mac-arm-documentation-cleanup-2026-11-20.md#completed-local-workspace-cleanup).
State what is no longer available. Keep tools, dependencies, canonical tests,
useful findings and unrelated work. Remove worktrees through Git. Future native
passes use fresh builds and establish their own results.
