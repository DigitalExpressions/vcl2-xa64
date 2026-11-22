# Native M2 ARM64 validation — 22 November 2026

## Outcome and tested source

Passed 8,036 cases and eight arithmetic smokes at clean source `c120ded4321a087339a952ef0671fb89c456ca14`, tree `b02601fc5a04dbdc41e224c1e04d2b987955150a`.

Performance was authorized for this session and followed correctness collection. Complete [source identity](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/source-manifest.json), [named cases](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/native-cases.json) and [reset receipt](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/reset-receipt.json) are retained.

## Host, tools and dependencies

Apple M2; native arm64 Python and Mach-O executables; sysctl.proc_translated=0.

Apple clang version 21.0.0 (clang-2100.1.1.101); Target: arm64-apple-darwin25.6.0. SDK 26.5; cmake version 4.5.1; ctest version 4.5.1; Ninja 1.13.2; native Python 3.9.6.

Pristine testbench a7293702385e5169a5f58b83177f2a43cd59139b was restored and verified at all pinned hashes. [Tool identities](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/installed-tool-identities.json) and [dependency inventory](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/testbench-byte-inventory.json).

## Executed matrix

| Compiler | Backend / width | Mode | Registered | Passed | Failed | Not run | Skipped | Timeout | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Apple Clang 21 | compatibility / 512 | Release | 1350 | 1350 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | compatibility / 512 | Debug / strict UBSan | 1350 | 1350 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 128 | Release | 486 | 486 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 128 | Debug / strict UBSan | 486 | 486 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 256 | Release | 890 | 890 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 256 | Debug / strict UBSan | 890 | 890 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 512 | Release | 1292 | 1292 | 0 | 0 | 0 | 0 | Initial |
| Apple Clang 21 | Direct Neon / 512 | Debug / strict UBSan | 1292 | 1292 | 0 | 0 | 0 | 0 | Initial |

All eight separate arithmetic smokes printed `2 5 10 17`.

## Controls, flags and numerical contracts

Maintenance integrity, repository/link checks, 41 isolated integrity controls, four native standalone consumers and six target-rejection controls passed. Full raw-source verification before/after passed.

-fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer; UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1. Positive exit 0; intentional overflow exit -6, required diagnostic present and no continuation.

Writable guard control passed and all four fault controls stopped at the actual 16384-byte page size. [Exact controls and exits](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22/native-controls.json).

The runner verified native arm64 Mach-O binaries, preserved ordinary no-contraction flags and isolated classification stress. Build/test concurrency was three and registered case deadlines were 60 seconds.

VCL 667577495efaa36a10cbb5d6cb8c942c23913848, tree 36aee120c28c9c64e62903bfc81139003e37cedd: all 28 originals exact (27 root plus audit/vectorclass-README.md). Root sse2neon.h retained SHA-256 932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3; both audit pins preserved.

## Failures, retries and coverage limits

Pre-reset controller setup rejected an inherited default SDKROOT. The first post-reset include attempt then failed to find stdlib.h after that variable was removed. A fresh include attempt used the SDK path verified by xcrun and passed. Numerical tests and timing attempts retain their actual outcomes in the named records.

Native M2 measurements at this frozen source. Windows remains failed/incomplete at its 22 November source with 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen examples. Its historical 19 November gate remains 5,387 passed, 249 failed and 76 not run of 5,712, plus twelve examples.

## Evidence and handoff

The initial reset discarded 40,072 files totaling 4,649,286,799 bytes from this local build tree, including old archives, M2 raw evidence, binaries and Clang study outputs. Historical published hashes and numerical results retain their original identities. Fresh tools use installed external CMake/CTest 4.5.1 and Ninja 1.13.2. The restored testbench is under build/_deps/testbench.

All fresh logs, command/exit journals, named statuses, flags, binaries, JUnit, generated inputs and output dumps remain under build. Portable records are checksummed directly. [Fresh performance report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md).

One approved documentation correction was committed as the tested c120ded source before deletion. Publication requires separate authorization.
