# Native AMD Windows x86-64 validation — 21 November 2026

**The correctness gate remains failed/incomplete. The full planned collection is complete.**

Tested clean `master` commit `dc802e466663d61d9e219bd7c7351aa8ac312204`, tree `7eaae08ad9b0a813ace5c74aff9be6ff2fc7630b`.
The exact 347-file working-source fingerprint is `7ca7fcc2c36848cb6ebad5032cf8bcf8efa269d894690dff69a72b02cc658776` (SHA-256 over sorted path, NUL, raw file SHA-256 and newline records). Source ZIP CRC and every entry hash were verified; HEAD, clean status and all source hashes matched after execution.

## Native configuration and coverage

AMD Ryzen 9 7950X, Windows 11 Pro x64 build 26200; native x64 Python 3.11.9, MSVC 19.50.35723, GNU-driver Clang 20.1.8, CMake/CTest 4.2.1 and Ninja 1.12.1.

Each of five compiler/profile combinations ran the 1,348-case pinned SSE4.2 baseline, plus the separate 40-case local AVX2/FMA and 40-case local AVX-512 F/DQ/BW/VL/FMA selections. The higher-ISA selections are local regression coverage, not full pinned-testbench reruns at those instruction levels. CPU CPUID and Windows XCR0 support were verified before execution.

| Profile | Registered | Passed | Failed | Not run | Examples passed |
| --- | ---: | ---: | ---: | ---: | ---: |
| upstream-optimized | 2856 | 2737 | 81 | 38 | 6 |
| strict-validation | 2856 | 2739 | 79 | 38 | 6 |
| strict-ubsan | 1428 | 1322 | 73 | 33 | 3 |

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| msvc | upstream-optimized | baseline | 1348 | 1329 | 16 | 3 | 0 |
| msvc | strict-validation | baseline | 1348 | 1331 | 14 | 3 | 0 |
| clang | upstream-optimized | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-validation | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-ubsan | baseline | 1348 | 1284 | 53 | 11 | 0 |
| msvc | upstream-optimized | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | upstream-optimized | avx512 | 40 | 13 | 27 | 0 | 0 |
| msvc | strict-validation | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | strict-validation | avx512 | 40 | 13 | 27 | 0 | 0 |
| clang | upstream-optimized | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | upstream-optimized | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-validation | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | strict-validation | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-ubsan | avx2 | 40 | 19 | 9 | 12 | 0 |
| clang | strict-ubsan | avx512 | 40 | 19 | 11 | 10 | 0 |

Timeouts are included in failed counts. Missing executables remain not run;
examples are counted separately from registered cases.

Total: **6798 passed, 233 failed and 109 not run out of 7,140 registrations**, plus **15 passing canonical examples**. There were 0 timeouts and 0 skipped cases.

Optimized Release uses the maintained MSVC `/O2 /fp:precise /fp:except-` and Clang `-O2 -fno-trapping-math -fno-math-errno` policies. Strict Release remains separate. Clang Debug strict UBSan uses nonrecovering instrumentation, frame pointers, halt-on-error and the verified static release CRT. Existing numerical tolerances, independent fixtures, seeds, 60-second case deadlines and concurrency three were preserved.

## Failures and comparison

Initial suite builds stopped on existing compile errors, including the wide-composition type assertions and Clang constant-expression errors in protected upstream code. Their first return packets are preserved unchanged. Separately logged Ninja `-k 0` continuations collected independent build outcomes before one complete CTest collection per configuration. Missing binaries remain not run. No completed canonical case was replayed.

Comparison with the [20 November integrated harness collection](windows-x86-harness-integration-2026-11-20.md) found **0 per-case status changes across all 7,140 matched registrations**. Sources are separately identified; matching statuses do not make failing tests pass or establish complete correctness.
All 87 of 87 compared root runtime and in-tree C++ test-source paths were byte-identical to that retained source after accounting for the approved example-source moves. CMake, runner and repository-layout differences remain part of the current tested commit.

## Controls and retained evidence

Repository/link checks and all 41 isolated integrity controls passed. All 28 upstream originals and root sse2neon remained exact. The pinned testbench revision `a7293702385e5169a5f58b83177f2a43cd59139b` was pristine and all three pinned hashes matched. Native PE/runtime checks passed, all four protected-page fault controls returned the expected access violation at the actual 4,096-byte page size, and strict UBSan required positive exit zero and deliberate overflow exit one with its diagnostic and no continuation. Compiler-profile positive and rejection controls passed.

Fresh MSVC and Clang facade include traces selected original x86 headers and excluded ARM headers and retired vendor/adaptation/backend paths. The arithmetic example now builds from `tests/vector_arithmetic.cpp` and passed in every canonical configuration.

Full raw command/exit journals, inventories, diagnostics, JUnit, LastTest logs, exact source ZIP, compiler flags, hashes, generated inputs and failed attempts are retained locally under ignored `build/x86v21-01/` and the associated runner/control directories. Compiled payloads remain on disk with exhaustive hashes. The verified raw ZIP contains the retained noncompiled records and a coverage manifest; its CRC and every entry SHA-256 were checked.

Raw archive: `build/x86v21-01-raw.zip`; SHA-256 `43fdc930c57e13534d7a8d9c5ef9f1019cfe69737a42948319c7b5e2249dd4d0`.
Exact source archive: `build/x86v21-01/source.zip`; SHA-256 `c8a2d2fc4f7166a0ee1d61b7d8b78697fca109871dff7a89aaf7d4adb3aaa346`.

This round covers native AMD Windows correctness and build integration only. No runtime repairs, gate relaxation, MSVC Debug/RTC, ASan, performance measurement, physical Intel or ARM validation was performed. Separate native results do not establish cross-host parity.
