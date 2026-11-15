# Native validation and branch workflow

The [integration plan](../integration-plan.md) records the approved merge of
`neon-port` into the existing default `master` through PR #1. Port review and
developer documentation cleanup are complete; the maintainer requested keeping
both existing branches. The default branch will not be renamed.
See the [developer index](../README.md) for current
instructions and the [Windows runner guide](windows-runner.md) for manual checks.
The maintainer selected manual test execution on the Mac M2 and AMD Windows
hosts only; hosted GitHub Actions test workflows are removed.

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
work and upstream updates, then integrate approved changes through reviewed
PRs. Keep `neon-port` and `windows-native-validation` as requested. Preserve the
`upstream` remote for original VCL; retirement requires separate approval.

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
git worktree add --detach ../vcl2-xa64-native-validation origin/master
```

Choose a fresh path and run subsequent commands from that worktree's root.

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
Return `return-evidence.zip` and its checksum for review; retain the full build
directory on the Mac. An unavailable compiler/runtime or failed control is a
coverage gap or failure, never a sanitizer pass.

The older offline packet is pinned to `943923a`; its bundled runner remains a
reproduction of that exact checkpoint. Use the in-tree runner above for an
updated `master` checkout. Do not change the packet's checksums or pretend
its older source revision is the current HEAD.

## Native AMD Windows run and evidence

Prioritize MSVC on the AMD host. Follow [the regression instructions](../../tests/README.md)
and the reusable platform/toolchain steps in the
[Windows handoff](windows-intel-handoff.md), labeling execution as physical AMD.
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
