# Native Windows AMD final validation — 2026-11-16

## Outcome and tested source

Passed at frozen commit `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`, tree `2a0a5d3250201773cccb06ba0562df728f4e9387`.
The checkout was clean throughout all execution. The full Windows source
fingerprint is `0b1504d4d225f42661763dfc6d17cf5933bfdee52ac38531ee3ef9a5f0d7ab79`; initial/final source manifests
match across all four full suites. No working or added-file patch was required.

Run ID: `simd-parity-20261116`. Full suite: **5,392/5,392 CTest cases**. Separate shared
corpus: **5,376/5,376 vector cases**, 38,864 output lanes. Native Mac execution
and Windows-versus-Mac comparison are pending; old Mac results retain their
original source identifiers. These Windows results do not establish ARM parity.

## Host, tools and dependencies

Native AMD Ryzen 9 7950X, Windows 11 Pro `10.0.26200`, x64 Python 3.11.9.
MSVC 19.50.35723 and native GNU-driver Clang 20.1.8 targeting Windows x86-64;
CMake/CTest 4.2.1 and Ninja 1.12.1. Native process/PE checks passed.
The pristine testbench is pinned to `a7293702385e5169a5f58b83177f2a43cd59139b`;
all three original source hashes and scalar-only adaptations were checked.
The runtime detector reports ISA level 11 with CPU/OS support; the corpus
profiles compile to selected ISA levels 2, 8 and 10.

## Executed matrix

| Compiler | Backend / ISA / logical width | Mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | --- |
| MSVC 19.50.35723 | local SSE2 + pinned SSE4.2, logical 128/256/512 | Release / not requested / MD | 1348 | 1348 | 0 | 0 | 0 | Initial |
| MSVC 19.50.35723 | local SSE2 + pinned SSE4.2, logical 128/256/512 | Debug / not requested / MDd | 1348 | 1348 | 0 | 0 | 0 | Initial |
| clang version 20.1.8 | local SSE2 + pinned SSE4.2, logical 128/256/512 | Release / not requested / msvcrt (dynamic release) | 1348 | 1348 | 0 | 0 | 0 | Initial |
| clang version 20.1.8 | local SSE2 + pinned SSE4.2, logical 128/256/512 | Debug / strict UBSan / libcmt (static release) | 1348 | 1348 | 0 | 0 | 0 | Initial |

Each profile's registered inventory and JUnit case names match exactly, including
all six configuration/fixture checks. Each example produced `2 5 10 17`.
The separate corpus captures cover MSVC/Clang SSE2 Release/Debug (Clang Debug
strict UBSan) and AVX2/AVX-512 Release for both compilers: 672 vector cases per
profile. No full AVX2/AVX-512 CTest matrix or higher-ISA Debug corpus is claimed.

All 28 Windows-only corpus pairings were independently validated and compared:
134488 exact lane matches,
336 bounded numerical differences,
672 declared FMA differences,
528 declared rounding differences,
zero unexpected differences. These are repeated pairings, not unique inputs;
passing declared contracts does not mean all output bits are identical.

## Controls, flags and numerical contracts

- Every suite/corpus profile passed the native runtime consumer and writable
  guard control at the actual 4,096-byte page size, plus all four read/write
  access violations (`0xc0000005`) at protected boundaries.
- Strict UBSan positive controls exited 0; intentional signed overflow exited 1
  with its expected diagnostic and no continuation. Clang Debug uses static
  release CRT, `-O0`, debug information, undefined sanitizer, disabled recovery,
  frame pointers and halt-on-error settings. MSVC Debug is unsanitized.
- MSVC ordinary tests retain `/fp:strict`; only classification stress uses
  `/fp:fast`. Clang retains no contraction, selected rounding options and isolated
  finite-math stress. Raw compile/link commands and executable hashes remain.
- Independent Decimal/Fraction references, fixtures, numerical gates, partial
  counts and canaries were preserved. Per-case/capture deadlines remain 60
  seconds, concurrency three, with serial configuration probes.
- Corpus/policy hashes and selected legacy FMA/rounding behavior are recorded
  in every portable manifest. No tolerance or source fix was introduced during
  this frozen run. Deliberate control faults are separate from test failures.

## Failures, retries and coverage limits

No failed/skipped/timed-out cases or retries occurred at this frozen source.
Preparation oracle failures remain in the separate preparation report and raw
archives; they are not overwritten or pooled into this run's pass count.
Clang UBSan import warnings remain in raw logs. ASan, clang-cl, FP16, unselected
APIs and additional hosts are outside this run. Physical Intel is omitted.
Direct Neon remains experimental. No benchmark timings, hosted CI or emulation
were used. Native M2 execution at this same frozen commit remains outstanding.

## Evidence and handoff

Portable metadata, raw output bits and checksums are in the
[Windows result directory](cross-platform/simd-parity-20261116/windows-amd/README.md).
Every full suite and corpus packet is named and checksummed in its metadata.
The combined ignored archive is `windows-final-simd-20261116T053510Z/windows-final-evidence.zip`
under `build/`, SHA256 `fece4dd2f38f5dbad2da36371ef94cbee871749fec1f8d6b1d4102bc43328d3e`.
It retains all twelve native invocations, raw packets, source proofs,
registration/JUnit records, controls, compiler commands, binary hashes and
28 Windows-only comparison records. Archive contents/CRCs and checksums were
verified. Raw archives transfer separately.

The shared harness was committed and pushed with approval at `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
Publication of these portable result/report files was subsequently approved;
it changes documentation/data only and retains the tested source identifiers.
Both retained branches are unchanged. Use the Mac prompt with the same frozen
commit and run ID;
do not substitute a later report-only HEAD for the actual tested source.
