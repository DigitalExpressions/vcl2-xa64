# Native M2 ARM64 validation — 21 November 2026

## Outcome and tested source

Passed: 8,036 registered cases and eight separate arithmetic smoke runs. No failed, skipped or timed-out cases; no retries.

- Tested commit: `dc802e466663d61d9e219bd7c7351aa8ac312204`.
- Tested tree: `7eaae08ad9b0a813ace5c74aff9be6ff2fc7630b`.
- Fresh detached clean-source worktree; exact raw tracked bytes verified before/after. Primary master checkout preserved.
- Full pinned matrix at this source; no prior pass transferred. Cross-host source comparison was not performed.
- Complete [source manifest](../../benchmarks/arm64/standard-math-master-2026-11-21/source-manifest.json), named case statuses in the separately checksummed native runner archive identified by the [archive receipts](../../benchmarks/arm64/standard-math-master-2026-11-21/retained-archive-receipts.json) and [summary](../../benchmarks/arm64/standard-math-master-2026-11-21/native-validation-summary.json).

## Host, tools and dependencies

- Apple M2; native arm64 Python and Mach-O executables; sysctl.proc_translated=0.
- ProductName:		macOS; ProductVersion:		26.7; BuildVersion:		25G229.
- Apple clang version 21.0.0 (clang-2100.1.1.101); Target: arm64-apple-darwin25.6.0.
- SDK 26.5; cmake version 4.4.4; ctest version 4.4.4; Ninja 1.11.1.git.kitware.jobserver-1; native Python 3.9.6.
- Pristine testbench a7293702385e5169a5f58b83177f2a43cd59139b, clean before/after; all three pinned hashes verified.
- Actual protected-memory page size: 16384 bytes.

## Executed matrix

| Profile | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| compat512-release | 1350 | 1350 | 0 | 0 | 0 | Initial |
| compat512-debug | 1350 | 1350 | 0 | 0 | 0 | Initial |
| neon128-release | 486 | 486 | 0 | 0 | 0 | Initial |
| neon128-debug | 486 | 486 | 0 | 0 | 0 | Initial |
| neon256-release | 890 | 890 | 0 | 0 | 0 | Initial |
| neon256-debug | 890 | 890 | 0 | 0 | 0 | Initial |
| neon512-release | 1292 | 1292 | 0 | 0 | 0 | Initial |
| neon512-debug | 1292 | 1292 | 0 | 0 | 0 | Initial |

Each profile separately executed the arithmetic program with output `2 5 10 17`; all eight passed. ARM dispatch tests remain registered.

## Controls, flags and numerical contracts

- Maintenance integrity, repository/link checks, 41 isolated integrity controls, four native standalone consumers and six target-rejection controls passed. Full raw-source verification before/after passed.
- -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer; UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1. Positive exit 0; intentional overflow exit -6, required diagnostic present and no continuation.
- Guard positive exit 0; all four read/write boundary controls faulted as required. Exact exits are in [native controls](../../benchmarks/arm64/standard-math-master-2026-11-21/ubsan-control.json).
- Runtime-query consumer passed: arm64=1, neon=1, instrset_detect=0, pointer_bits=64, long_bits=64.
- All selected binaries verified native arm64 Mach-O and hashed. Fixture/configuration cases passed; no independent fixtures, seeds, gates, canaries or FP controls changed.
- Actual compile commands audited by the maintained runner: no implicit contraction; strict nonrecovering UBSan in Debug; isolated classification-only finite-math stress.
- Builds and tests used concurrency three; all registered case deadlines remained 60 seconds.
- VCL 667577495efaa36a10cbb5d6cb8c942c23913848, tree 36aee120c28c9c64e62903bfc81139003e37cedd: all 28 originals exact (27 root plus audit/vectorclass-README.md). Root sse2neon.h retained SHA-256 cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff; both audit pins preserved.

## Failures, retries and coverage limits

No unexpected failures, skips, timeouts or retries in this run. Expected negative integrity, compiler-target, UBSan and protected-memory controls are retained with their actual diagnostics/exits.

This is native M2 coverage at the exact source above. Direct Neon remains experimental. ASan, other hosts/toolchains, physical Intel, native FP16/SVML/ARM64EC, unselected APIs, external consumers and new cross-host parity are outside this result.

The existing full Windows gate remains failed/incomplete: 5,387 passed, 249 failed, 76 not run of 5,712, plus twelve passing examples. This run does not change that gate.

## Artifact availability after cleanup

On 21 November, after publication, the completed detached native worktree was
retired through Git after all **26,984 files**, including generated inputs,
diagnostics, objects and binaries, were verified in
`cleanup-2026-11-21/retired-native-worktree.zip`. The original source/build paths
now describe archived files. The [availability receipts](../../benchmarks/arm64/standard-math-master-2026-11-21/retained-archive-receipts.json)
record the archive SHA-256, entry count, per-file inventory and cleanup receipt.

The exact `source.tar` and original `return-evidence.zip` remain at the run root.
Performance attempts, their benchmark binaries/output dumps and the separate
Clang study remain unpacked. Tools, dependencies and raw evidence were preserved.
The 46 task scratch files were archived before clearing `work/`; that scratch
folder remains reusable. These availability changes establish no new native pass.

## Evidence and handoff

- Portable data archive: `portable-results-20261121T030229947697Z.zip`; SHA-256 `dd6c2a60734830f4f154d59d809917d636b218dc4027e06f9cd06af3ff378dde`; [verified receipt](../../benchmarks/arm64/standard-math-master-2026-11-21/archive-receipt.json).
- Complete raw command/exit journals, registered lists, JUnit, compile/link commands, build/test logs, generated fixtures/adapters, probes, source tar and binary hashes stay under ignored `build/` in this run.
- The runner archive is separately retained with its checksum; the portable data packet excludes executables, output dumps and raw machine paths. At execution closeout the detached worktree, tools/dependencies and raw evidence were retained; the later archival cleanup is documented above.
- Performance followed this passing correctness gate and used fresh Release builds at the same source: [standard-math report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md). Benchmark setup 01 stopped on a task-local flag-audit scope error after three passing Release CTest cases and no timing. Setup 02 used fresh builds; six CTest cases and six actual-workset probes passed before the 18 timing processes. Both attempts are retained in [setup records](../../benchmarks/arm64/standard-math-master-2026-11-21/setup-attempts.json).
- No runtime/test/runner edits, commits, pushes, merges, PRs or branch retirement. This later documentation copy preserves the tested source identity; raw reports and evidence remain under ignored build/.

## Native M2 collection and local artifact availability — 22 November 2026

On 22 November 2026 the authorized local Mac build reset discarded 40,072 files totaling 4,649,286,799 bytes, including historical M2 archives, source captures, raw logs, binaries, benchmark output dumps and the Clang study outputs. Earlier identifiers, hashes and numerical results describe their original checkpoints. Their old local payloads are no longer available. The fresh collection used clean commit `c120ded4321a087339a952ef0671fb89c456ca14`, with 8,036 cases and eight smokes passed, followed by 18 timing invocations and all 160 report rows.

[Fresh correctness report](mac-arm-validation-2026-11-22.md) · [Fresh performance report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
