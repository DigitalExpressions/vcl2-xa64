# Native validation and branch workflow

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

## Shared branch and integration

`neon-port` is the shared branch to fetch on both hosts. Topic branches such as
`windows-native-validation` start from `neon-port` and hold focused changes.
Integrate approved work into `neon-port` and push that branch to `origin` so
both machines can test the same commit. Prefer a fast-forward when the branch
relationship permits it. Inspect divergence before any integration; preserve
unrelated work and use a fresh worktree when necessary. Record the tested HEAD
and tree on each host, rather than relying on branch names alone.

On 2026-11-15 the maintainer approved fast-forwarding `neon-port` through the
five `windows-native-validation` commits ending at
`943923a4cce1b3990ee69b37ebdf1c37f0fc67b6`, documenting this workflow, and pushing
the updated `neon-port` to `origin`. Native Mac confirmation of these fixes is
still pending. The accompanying workflow/runner commit changes development
assets; it does not change the library or regression cases. `master` is outside
this integration. Future pushes and merges require the maintainer's approval;
this approval applies to the current integration.

From a clean existing Mac checkout:

```sh
git status --short --branch
git fetch origin
git switch neon-port
git pull --ff-only origin neon-port
git rev-parse HEAD
git rev-parse 'HEAD^{tree}'
```

If the checkout contains unrelated changes or has diverged, preserve it and
create a separate worktree instead of resetting or overwriting it:

```sh
git fetch origin
git worktree add --detach ../vcl2-xa64-native-validation origin/neon-port
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
updated `neon-port` checkout. Do not change the packet's checksums or pretend
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
matrices. They remain historical evidence; this workflow adds no native Mac
result. Native Mac validation is the next execution step on the updated branch.
