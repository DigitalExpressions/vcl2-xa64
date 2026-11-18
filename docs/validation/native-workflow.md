# Native validation and branch workflow

The [integration plan](../integration-plan.md) records the approved merge of
`neon-port` into the existing default `master` through PR #1. Port review and
developer documentation cleanup are complete. The completed port and audit
branches were subsequently retired; they are not required to run tests.
The default branch remains `master`. Follow the shared [agent instructions](../../AGENTS.md),
including the no-pull-request and fork-protection rules.
See the [developer index](../README.md) for current
instructions and the [Windows runner guide](windows-runner.md) for manual checks.
The maintainer selected manual test execution on the Mac M2 and AMD Windows
hosts only; hosted GitHub Actions test workflows are removed.

For the approved architecture migration, execute the
[Mac separation handoff](mac-arch-separation-handoff.md). It supersedes the
existing layout/clean-source assumptions below for that task only, requires a
complete frozen uncommitted-source capture, and preserves the native controls.
The maintainer subsequently approved topic-branch publication. The
[Windows return handoff](windows-arch-separation-handoff.md) describes validation
from its exact published Git commit; old captured-source IDs remain unchanged.

## Frozen uncommitted architecture candidates

The [current source ownership](../backend-layout.md) uses `vcl2_xa64.h`, protected
originals at root and separate authoritative ARM headers. Source publication is
not required to validate the approved migration. From the implementation root,
choose a fresh ignored capture path and existing pristine `$PINNED_TESTBENCH`:

```sh
python3 -B tools/capture_source.py --output build/UNIQUE-CANDIDATE
python3 -B build/UNIQUE-CANDIDATE/source/tools/run_native_arm_validation.py \
  --checkout build/UNIQUE-CANDIDATE/source \
  --capture-manifest build/UNIQUE-CANDIDATE/capture.json \
  --testbench-dir "$PINNED_TESTBENCH"
```

Keep the Mac awake, verify all source/archive hashes, and retain the complete
capture and any failures. The explicit mode sets tested commit/tree to null and
records the base separately; default clean-checkout behavior remains guarded.
Never edit the capture, weaken gates or use `--expected-commit` to mislabel it.
A source change needs a new capture and fresh relevant/full baseline checks.
See the [actual M2 report](mac-arm-2026-11-18-arm-header-cleanup.md) and
[Windows return handoff](windows-arch-separation-handoff.md) for exact source IDs.

## Available hardware

The maintainer has two physical validation hosts:

| Host | Native target | Preferred toolchain and coverage |
| --- | --- | --- |
| Apple Mac M2, macOS | ARM64 | Apple Clang; sse2neon compatibility and Direct Neon at logical widths 128/256/512; Release and strict UBSan Debug |
| AMD Ryzen 9 7950X, Windows x64 | AMD x86-64 | MSVC first; native Clang for strict UBSan and compiler comparison; baseline and supported selected x86 ISA profiles |

Use these hosts for future cross-platform validation. Physical Intel hardware
is omitted for now; AMD results establish AMD coverage. QEMU/WSL results remain
separately labeled emulation and cannot establish native Mac coverage. Inspect
the actual OS, CPU, compiler, sanitizer runtime and CPU/OS ISA support each time.
Hardware availability does not establish that a particular revision has passed.
Read the [validation status and reports](README.md) when resuming on either
host. Add new results using the [shared report template](report-template.md),
then update the index with the exact tested revision and actual outcome.

## Shared branch and integration

`master` is the shared branch to fetch on both hosts after the approved merge
of PR #1 at `732f3e9`. Start short-lived topic branches from `master` for new
work when useful and review changes locally. Do not create pull requests, alter
the fork relationship/remotes or rewrite published history. Commits, pushes,
integrations, releases and further branch retirement need task-specific approval.
Preserve the `upstream` remote for original VCL and never push to that repository.
See the [current restart point](../integration-plan.md#restart-point);
inspect actual local branches and completed removals on each host.

Both machines should test the same commit when comparing results. Inspect
divergence before any integration; preserve unrelated work and use a fresh
worktree when necessary. Record the tested HEAD and tree on each host, rather
than relying on branch names alone. A merge does not establish a new native pass.

On 2026-11-15 the maintainer approved fast-forwarding `neon-port` through the
five `windows-native-validation` commits ending at
`943923a4cce1b3990ee69b37ebdf1c37f0fc67b6`, documenting this workflow, and pushing
the updated `neon-port` to `origin`. Subsequent native Mac confirmation at
`465436d` is recorded in the [Mac ARM report](mac-arm-2026-11-15.md), including
its initial sleep-related timeouts and full-profile replay. The accompanying
workflow/runner commit changes development assets; it does not change the
library or regression cases. That earlier approval excluded `master`. The
maintainer subsequently approved merging PR #1 into `master` while keeping
branches; the merge completed on 2026-11-15. Future pushes and merges require
the maintainer's approval; this approval applies to that completed integration.

From a clean existing Mac checkout:

```sh
git status --short --branch
git fetch origin
git switch master
git pull --ff-only origin master
git rev-parse HEAD
git rev-parse 'HEAD^{tree}'
```

If the checkout contains unrelated changes or has diverged, preserve it and
create a separate worktree instead of resetting or overwriting it:

```sh
git fetch origin
git worktree add --detach build/native-validation origin/master
```

Choose a fresh path and run subsequent commands from that worktree's root.

## Preserve future validation inputs

The maintained runners, shared evidence module, tests, fixture generators,
cross-platform corpus/policies and comparator remain in the repository. Old
branch names are not dependencies. Preserve original raw logs/results, source
and compiler details, command/flag inventories, checksums, output bits and failed
attempts in verified compact archives under ignored `build/`. Save any uncovered
records or generated diagnostic/input sources in a checksummed supplement before
cleanup. Keep tool installations and pristine testbench checkouts available.

Once a run is complete and evidence coverage is verified, the user can authorize
removal of rebuildable objects, test executables, debug symbols, generated build
directories and temporary worktrees. Preserve source commits and original archive
hashes; remove worktrees through Git. New passes use fresh builds rather than
reusing retired outputs. See the [retention follow-up](evidence-retention-2026-11-16.md)
for the approved cleanup of the frozen Mac worktree. Never discard unrelated
work or evidence as part of broad cleanup, and never overwrite an earlier attempt.

The native runners do not install build tools. Inspect available native Python,
compiler, CMake/CTest and Ninja/make first, and put the selected installed tools
on the current process's PATH. Shared corpus capture requires Ninja on both
hosts. Keep machine-specific locations in local ignored notes, not shared
instructions. Reuse a pristine pinned testbench with `--testbench-dir`, or fetch
it explicitly into a new directory using the supplied script.

Before an expensive run, inspect the Mac `--plan` and Windows driver/mode
`--plan` outputs. These are registration checks, not executed test results.
Check the shared corpus and comparator controls using the
[cross-platform instructions](cross-platform/README.md#check-the-harness), which
also create the ignored control-output directory for a fresh checkout.

## Native Mac ARM run

Use native macOS arm64 Terminal and Python, with Xcode Command Line Tools,
Python 3.9+, CMake 3.20+, CTest supporting `--output-junit`, and Ninja or make.
Inspect installed tools first. Run from the repository root after updating:

```sh
sh tools/fetch_testbench.sh build/_deps/testbench
python3 -B tools/run_native_arm_validation.py --plan
python3 -B tools/run_native_arm_validation.py --expected-commit "$(git rev-parse HEAD)"
```

The fetch script creates a separate pinned upstream checkout and refuses to
overwrite an existing path. Reuse a clean checkout at the pinned revision if
it already exists. The runner verifies its Git revision and three source
hashes; `--testbench-dir` selects another existing checkout. It performs no
downloads or installations. For offline restoration, use
`--testbench-bundle /path/to/testbench.bundle` with the previously prepared
bundle; its branch must be named `pinned-testbench`.

| Backend | Logical maximum | Release cases | Strict UBSan Debug cases |
| --- | --- | --- | --- |
| ARM sse2neon compatibility | 512 | 1350 | 1350 |
| Direct Neon | 128 | 486 | 486 |
| Direct Neon | 256 | 890 | 890 |
| Direct Neon | 512 | 1292 | 1292 |

The expected full matrix is 8,036 case instances in eight fresh profiles.
`--local-only` runs 42/40/42/44 cases per mode, 336 total, excluding the
optional upstream matrix. These counts are registration checks, not results.
If registration changes later, review the inventory and update the runner's
checkpoints deliberately; do not omit failing cases to match a count.

The runner records the current clean HEAD/tree, optionally verifies
`--expected-commit`, and requires ancestry through `943923a`. It rejects
Rosetta and non-macOS hosts, verifies arm64 Mach-O executables, probes strict
nonrecovering UBSan, checks a runtime-query consumer, and exercises protected
page read/write controls at the actual host page size. It retains numerical
gates, fixture generators, selected rounding options, no implicit contraction,
classification-only finite-math stress and all original 60-second deadlines.
The documented Apple-host ASan startup hang is not retried. Build/test
concurrency is three; timing benchmarks are outside this validation task.

Every invocation creates an ignored `build/mac-m2-native-<UTC timestamp>/` with
commands and exit codes, host/tool inventory, source and executable hashes,
compile commands, raw configure/build/test logs, JUnit and per-profile results.
Return `return-evidence.zip` and its checksum for review. Keep the full build
directory until archive coverage has been verified and cleanup authorized as
described above. An unavailable compiler/runtime or failed control is a
coverage gap or failure, never a sanitizer pass.

The older offline packet is pinned to `943923a`; its bundled runner remains a
reproduction of that exact checkpoint. Use the in-tree runner above for an
updated `master` checkout. Do not change the packet's checksums or pretend
its older source revision is the current HEAD.

## Native AMD Windows run and evidence

Prioritize MSVC on the AMD host. Follow [the regression instructions](../../tests/README.md)
and the current platform/toolchain steps in the
[Windows runner guide](windows-runner.md), labeling execution as physical AMD.
Run baseline Release/Debug and the pinned upstream selection; probe CPU/OS
support before selecting AVX2/AVX-512. Use native GNU-driver Clang for strict
UBSan after an intentional-UB control proves the installed runtime. MSVC Debug
is useful native coverage and does not provide UBSan. Preserve the documented
floating-point flags, guard pages, partial counts, canaries and deadlines.

Keep raw artifacts under ignored `build/` on both hosts. Record source revision,
compiler, flags, case inventories, failures/skips/timeouts and coverage gaps.
Compare results from the same source commit, with platform/toolchain differences
identified. Publish only portable summaries; leave machine paths, application
data, SDKs and credentials out of public sources. Preserve dated historical
records and add a fresh record for newly executed native tests.

The [initial AMD record](windows-amd-2026-11-15.md) and
[AVX-512/ARM POSIX follow-up](windows-avx512-2026-11-15.md) cover earlier executed
matrices. They remain historical evidence. The
[native Mac report](mac-arm-2026-11-15.md) records the subsequently executed
8,036-case matrix at `465436d`. The [status index](README.md) separates that
result from the earlier Windows checkpoints; a Windows run at the same exact
revision has not been recorded here. Add a fresh dated report for the next
Windows execution rather than rewriting either historical Windows record.
