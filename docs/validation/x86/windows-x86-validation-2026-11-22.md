# Native AMD Windows validation after build reset — 2026-11-22

The full selected collection is complete. The correctness gate remains failed/incomplete: **6,796 passed, 235 failed and 109 not run of 7,140 registered cases**, plus 15 passing canonical examples. There were 0 timeouts and 0 skips. Timeouts count within failures; examples are separate.

Execution started on 21 November 2026; closeout was `2026-11-22T00:06:26.074283+01:00` (UTC `2026-11-21T23:06:26.074283+00:00`). Clean master commit `08f2d2903d9bfdfdb22aa187329e99ce0f44d177`, tree `c712c242205e19e233af93b5c565fa601643749f`, stayed unchanged. The 303-file raw working-source fingerprint is `15989d633301114dd66508b631cc9d82e516010833d707e96689d41b4c88d346`. The exact source ZIP SHA-256 is `31ac410a27dc45c6ed86fb1d99fbb26ef857f7b07c75af644ab7082633621003`; CRC and every entry hash were verified before and after collection. Both architectures are included in the source inventory. Source ZIP evidence is retained under ignored `build/xr21-2211/source.zip`.

The maintainer explicitly authorized separate performance for this reset. Its [report](../../benchmarks/x86/x86-upstream-2026-11-22.md) records the independently checked comparison and any unavailable coverage.

## Publication source reconciliation — 22 November 2026

The checkout fast-forwarded to publication base 88c48689f1ec4d6fa665df6a9c6b41a3d064bb0a
at 00:06:55 +01:00, after runtime collection and its final source checks closed
at 00:06:26 +01:00. This caused the documented source-change stop during report
preparation. The maintainer then requested reconciliation for publication.

An exact comparison found seven changed paths and 296 unchanged paths.
AGENTS.md is unchanged. The ARM compatibility header conversion from
Windows-1252/CRLF without a final newline to UTF-8/LF with a final newline
reproduces the new header byte-for-byte at the same upstream commit. Its pin,
two integrity tools and related documentation changed with that representation.
The x86 runtime, all tests, CMake, native runners, profiles, benchmark harness
and edge probes are unchanged. The x86 facade does not include this ARM
compatibility dependency.

The frozen tested export passed its own original integrity checker and audit.
The publication base passed fresh repository and protected-byte checks plus all
41 isolated integrity controls. This resolves the source-change stop for
publishing the completed historical x86 collection. No native case or timing
was rerun or reassigned: all execution results remain at 08f2d29, and this
publication does not establish new-HEAD native or ARM coverage.
The [source reconciliation proof](windows-x86-validation-2026-11-22/source-reconciliation.json)
records every changed path and both source identities.

## Host and selection

Native AMD Ryzen 9 7950X, Windows 11 Pro x64 build 26200, native x64 Python 3.11.9, MSVC 19.50.35723, GNU-driver Clang 20.1.8 targeting x86_64-pc-windows-msvc, CMake/CTest 4.2.1 and Ninja 1.12.1. IsWow64Process2 and x64 PE inspection establish native execution. CPU CPUID and OS/XCR0 (`0xe7`) support SSE4.2, AVX2/FMA/F16C and AVX-512 F/DQ/BW/VL.

Each of five compiler/profile combinations selected the full 1,348-case pinned SSE4.2 baseline and the separate 40-case local AVX2/FMA and 40-case local AVX-512 selections. These higher-ISA selections cover local regressions; they do not provide full pinned-testbench coverage at those ISAs.

| Profile | Registered | Passed | Failed | Not run | Examples passed |
| --- | ---: | ---: | ---: | ---: | ---: |
| upstream-optimized | 2856 | 2736 | 82 | 38 | 6 |
| strict-validation | 2856 | 2738 | 80 | 38 | 6 |
| strict-ubsan | 1428 | 1322 | 73 | 33 | 3 |

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| msvc | upstream-optimized | baseline | 1348 | 1329 | 16 | 3 | 0 |
| msvc | upstream-optimized | avx2 | 40 | 23 | 15 | 2 | 0 |
| msvc | upstream-optimized | avx512 | 40 | 13 | 27 | 0 | 0 |
| clang | upstream-optimized | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | upstream-optimized | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | upstream-optimized | avx512 | 40 | 19 | 11 | 10 | 0 |
| msvc | strict-validation | baseline | 1348 | 1331 | 14 | 3 | 0 |
| msvc | strict-validation | avx2 | 40 | 23 | 15 | 2 | 0 |
| msvc | strict-validation | avx512 | 40 | 13 | 27 | 0 | 0 |
| clang | strict-validation | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-validation | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | strict-validation | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-ubsan | baseline | 1348 | 1284 | 53 | 11 | 0 |
| clang | strict-ubsan | avx2 | 40 | 19 | 9 | 12 | 0 |
| clang | strict-ubsan | avx512 | 40 | 19 | 11 | 10 | 0 |

Optimized Release used MSVC `/O2 /fp:precise /fp:except-` without RTC and Clang `-O2 -fno-trapping-math -fno-math-errno`. Strict Release retained strict FP/no-contraction/rounding controls. Strict UBSan used GNU-driver Clang Debug, nonrecovering instrumentation, frame pointers, halt-on-error and the verified static release CRT `MultiThreaded`. Independent references, fixtures, seeds, numerical gates, 60-second correctness deadlines and concurrency three were preserved. Classification-only finite-math stress stayed separate.

## Attempts, failures and controls

First canonical runner build failures and their return packets remain unchanged. Separate Ninja `-k 0` continuations built independent targets before one complete CTest collection per configuration. Missing executables remain not run. No completed canonical case was replayed. Scratch setup failures in runtime-path discovery, PowerShell array handling and the extracted ISA probe were retained separately and corrected without changing runtime, tests, runners or gates.

All 7,140 named registrations reconciled against the separately identified 20 November collection, with 6 status changes and zero inventory differences. Runtime, test, runner and CMake source paths are unchanged from clean `dc802e466663d61d9e219bd7c7351aa8ac312204`. Matching failures do not establish correctness.

All 28 pinned VCL originals and root sse2neon remained byte-exact. Repository/link checks and the 41 isolated integrity controls passed. The freshly restored pristine testbench remained at `a7293702385e5169a5f58b83177f2a43cd59139b`, with all three pinned source hashes verified. Runtime detection, native PE controls and MSVC/Clang facade include traces passed; x86 traces excluded ARM and retired source layouts. At the actual 4,096-byte page size, writable controls passed and all four protected-page controls exited `0xc0000005`. Strict UBSan positive controls exited zero; deliberate signed overflow exited one with its diagnostic and no continuation. Profile positive/rejection controls passed.

Both MSVC AVX2 profiles changed the same three statuses: fp_helpers and fp_helpers_namespace changed from passed to failed; fp_helpers_signed_zero changed from failed to passed. That is six status changes and two fewer passes overall. No other selected case status changed.

Fresh local higher-ISA builds used explicit ISA/INSTRSET options and the current CMake default dynamic CRT for ordinary Release; strict UBSan used static release CRT. The earlier local higher-ISA collection used static release CRT for ordinary Release too. Per-configuration flags are retained; changed FP-helper outcomes are reported without attributing them to a particular option or inferring a source regression.

## Reset losses and fresh evidence

Explicit execution of the reset plan authorized deletion of the inventoried local build contents: 34,909 files, 2,979,775,880 bytes, plus their directories. This included earlier raw archives, exact source captures, diagnostics, generated inputs, prior cleanup receipts and the old dependency checkout. They were discarded without a replacement old-build archive. Their published identifiers, checksums and results remain historical; hashes cannot recover the discarded payload bytes. Other hosts and unrelated paths were untouched. The pre-reset receipt and full inventory survived under ignored work and were copied to the new build before validation.

A pristine pinned testbench, the original VCL comparison resource and fresh native evidence were reconstructed. Fresh logs, commands/exits, complete JUnit/LastTest outputs, initial runner packets, continuations, compiler flags, source/binary/tool identities and generated inputs remain under ignored `build/xr21-2211/` and its associated `build/windows-native-*` and `build/windows-profile-controls-*` directories. They depend on no work files. [Portable records](windows-x86-validation-2026-11-22/summary.json) retain source, controls, complete named statuses, nonpassing output and availability; raw payloads stay local.

No runtime repairs, gate relaxation, MSVC Debug/RTC, ASan, physical Intel, ARM execution, external consumer changes or cross-host parity claim belong to this run. No commit, push or merge was performed.
