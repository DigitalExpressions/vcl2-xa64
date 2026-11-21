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

All 28 original files (27 at root plus the upstream README under `audit/`)
and their audit pin remain exact bytes, as does root
`sse2neon.h`, verified directly by required `audit/sse2neon-2272251.json`.
The vendor catalog is retired. Never regenerate or normalize these headers. Before and after
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
[Windows runner](x86/windows-runner.md). The integrated Windows runner defaults to
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

## ARM64 results publication checklist

Use this checklist after a native ARM64 validation round and its separately
executed Release performance measurements. It covers copying completed results
into the repository documentation. Execution still follows the agreed run plan;
copying results does not run tests, establish a later source pass or authorize
cleanup, commits or pushes. Failed or incomplete runs retain their actual status.

For future requests, refer to this section with:

> Prepare the completed ARM64 validation and performance results for documentation
> using the ARM64 results publication checklist in docs/validation/native-workflow.md.

### Copy destinations

Use the actual report date and a descriptive source/session label. The
[21 November correctness report](arm64/mac-arm-validation-2026-11-21.md),
[standard-math report](../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
and [supporting bundle](../benchmarks/arm64/standard-math-master-2026-11-21/checksums.sha256)
are an example of the layout, not a source of timings for another run.

| Destination | Copy or prepare from the completed run |
| --- | --- |
| `docs/validation/arm64/mac-arm-validation-YYYY-MM-DD.md` | Correctness report using the [native report template](report-template.md): exact source, host/tools, complete profile totals, smoke runs, controls, failures/retries and limits. |
| `docs/benchmarks/arm64/STANDARD_MATH_RESULTS_YYYY-MM-DD.md` | Selected implementation's performance report using the [standard-math template](../benchmarks/arm64/STANDARD_MATH_RESULTS_TEMPLATE.md), with all measured rows, slower rows, variability and numerical/output checks. |
| `docs/benchmarks/arm64/standard-math-LABEL-YYYY-MM-DD/candidate/` | Byte-exact `results.csv`, `results.json`, `summary.json` and `metadata.json` from the selected implementation's measured session. The inherited `candidate/` name also applies to a clean-master checkpoint; explain that identity in `sources.json`. |
| The same supporting bundle | Source manifest/identity, actual compile/link flags, compiler/SDK/tool identities, benchmark executable hashes, native and benchmark correctness summaries, sanitizer/guard controls, integrity, protocol, attempts and archive receipts. Reuse records between the two reports through relative links. |
| The same supporting bundle, when measured | Output comparisons, code-generation records and matched before/after comparisons, with their own source and session identities. Include a separate compiler investigation's findings, measurements, limitations and retained evidence identity when cited. |
| The same supporting bundle | A publication receipt describing copied data hashes, original and adapted report identities, link/presentation changes, intentionally local evidence and inapplicable comparison files; `checksums.sha256` covering the final reports and bundle payloads. |

Supporting filenames can follow the existing bundle: `sources.json`,
`source-manifest.json`, `flags.json`, `binaries.json`, `correctness.json`,
`native-validation-summary.json`, `ubsan-control.json`, `protected-integrity.json`,
`protocol.json`, `attempts.json`, `setup-attempts.json`, `archive-receipt.json`,
`retained-archive-receipts.json` and `publication-receipt.json`. Preserve the
actual contents and explain any renamed source records. Copy the records that
exist and apply; identify missing evidence as a gap. Do not fabricate a runtime
baseline/candidate comparison for a single-source checkpoint.

### Publication steps

1. Inspect local changes and read applicable `AGENTS.md`. Record the tested
   commit/tree or complete capture identity, run/attempt IDs and original report
   hashes. Preserve the frozen source, dependencies, protected originals and all
   prior evidence. Prepare copies under ignored `work/`. Obtain approval for new
   permanent paths unless the current task already explicitly authorizes them.
2. Verify the retained source and evidence inventories and archive receipts:
   archive SHA-256, entry counts, CRC where applicable and per-entry hashes.
   Keep complete native named-case statuses, executable hashes, control exits,
   JUnit and diagnostics available in the checksummed local archive. Check
   supplements for records outside that archive; a receipt alone is not a copy
   of its payload. Retain every failed setup, run, timeout and unavailable control.
3. Copy the approved reports and selected data to the destinations above.
   Rewrite evidence links to relative published paths, sharing the bundle between
   correctness and performance reports. Keep raw result files byte-exact and
   verify every measured table row against its source. Do not change fixtures,
   seeds, flags, tolerances, numerical contracts, output comparisons, timing
   samples or slower rows while preparing documentation. Keep focused repeats
   separate from the full-suite results; compiler/SDK changes do not establish
   a library source improvement. Preserve the Windows failed/incomplete gate
   and its recorded counts separately from this native M2 result.
4. Update `docs/validation/README.md`, `docs/validation/arm64/README.md`,
   `docs/benchmarks/README.md` and `benchmarks/README.md` with the new report links.
   Update root `README.md`'s latest results/correctness links and brief performance
   overview from this same session. Derive ratios and range endpoints from
   unrounded data across every advertised width/count, then round for display.
   Refresh winner counts, cross-product percentages, colors and variability/cost
   commentary together. Use the existing inclusive 0.95–1.05 ratio band; range
   endpoints inside that band must not be colored as backend wins. Identify the
   new date, compiler and SDK. Earlier dated reports and datasets keep their own
   values and source identities.
5. Set or verify `.gitattributes` coverage so checksummed files retain their raw
   bytes across hosts, including intentional CSV CRLF. Generate the bundle's
   checksum inventory from the final adapted reports and supporting files,
   excluding the checksum file itself. Keep original run/archive checksums and
   original report hashes separately; an adapted documentation report can have
   a different hash without changing measurements. Verify every listed checksum,
   all relative links and all new untracked files as well as tracked edits.
6. Run the documentation checks below. Review the diff and copied-file inventory
   for missing records, placeholders, broken inline-code paths, machine-specific
   paths, credentials or compiled payloads. Confirm protected bytes and unrelated
   work are unchanged. Report the final document locations, checks performed,
   intentionally local evidence and remaining gaps. Commit/push and cleanup stay
   separate actions requiring task-specific authorization.

```sh
python3 -B tools/check_repository.py
python3 -B tools/regenerate_headers.py --check
git diff --check
```

`git diff --check` does not inspect untracked copies; check their whitespace too.
Formatting must preserve measured values and literal paths. Documentation-only
publication needs these checks, not a repeat of the native matrix or timings.

### Evidence that stays local

Keep full raw logs, commands/exits, named-case and binary inventories, JUnit,
source archives/captures, generated inputs, output dumps, assembly, binaries,
objects, tools, testbench checkouts and worktrees under ignored `build/`.
Baseline/control reports and standalone baseline datasets also stay there;
publish applicable matched comparison records with the selected result.
Avoid duplicating large native inventories in the benchmark bundle when both
reports can refer to the same verified archive receipt. A useful focused study
can be summarized in the report and existing publication receipt, retaining its
measurements, failures, scope and archive identity.

Ignored evidence and archives do **not** travel with Git. Public documentation
links must resolve within the repository or to an explicitly retained external
source; local archive locations belong in receipts as availability information.
A hash identifies an artifact but cannot recover it. Any later transfer or
cleanup must account for that availability explicitly; preparing these reports
does not authorize either action.

## Evidence retention and cleanup

During active work retain complete evidence. Later cleanup needs explicit scope;
normally verify compact coverage before removing outputs. A maintainer request
to discard obsolete campaign artifacts may instead remove them without new
archives, as recorded in the [20 November cleanup](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md#completed-local-workspace-cleanup).
State what is no longer available. Keep tools, dependencies, canonical tests,
useful findings and unrelated work. Remove worktrees through Git. Future native
passes use fresh builds and establish their own results.
