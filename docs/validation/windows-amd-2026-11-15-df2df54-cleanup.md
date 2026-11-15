# Native Windows AMD: shared evidence helper cleanup

## Outcome and tested source

Passed on 2026-11-15: **160/160 local cases** across four fresh native AMD
Windows profiles, with no failures, skips, timeouts or retries. This validates
the shared evidence helper extraction and local runner paths, not a new full
upstream or higher-ISA matrix.

| Source identifier | Observed value |
| --- | --- |
| Repository / execution branch | `DigitalExpressions/vcl2-xa64`, `master` |
| Base HEAD | `df2df54446198294ed846eea2c965498ff7b1f26` |
| Base Git tree | `5e1923eb47dbf0609566c6a1f7f4650b1bb837b8` |
| Tested checkout | Uncommitted cleanup patch plus added `tools/validation_evidence.py`; unchanged throughout all four profiles |
| Full source manifest fingerprint, 122 entries | `59288593d0b50fe7394c46970c48582d516164393d315bcaeeb729eaa213ab72` |
| Tracked working-patch log SHA256 | `454174ec13727bed3f9864c2f54d00c22028553f6308893fb0c0b487bc7fcd3e` |
| Added helper patch log SHA256 | `1d749aa07601b37c4f8d8a44727ebcd09c7fb02e098edeed34d49dd8883d6fbe` |

The tracked patch changes four Markdown files and the two native runners;
the added helper is captured separately by `git diff --no-index`. Both patch
hashes identify the exact raw logs inside every profile's evidence archive.
The manifest records every source path and SHA256. All initial/final manifests
match, as do fingerprints across the four profiles. HEAD/tree identify the
base, not the complete tested dirty source.

Headers, C++ tests, fixture data, build configuration, benchmarks and historical
reports are unchanged. After execution, only Markdown was added or updated:
this report, the validation index, integration plan, Windows guide and test
README. Retain the original tested identifiers; this report is not itself a
tested commit.

The Mac [existing native report](mac-arm-2026-11-15.md) remains at `465436d`.
No new native Mac run was performed. Structural comparison verifies that Mac
checks and extracted methods retain their original syntax trees; both Mac
plans and all eight Windows local/upstream plans match the previous runners.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| Physical CPU / OS | AMD Ryzen 9 7950X; Windows 11 Pro, `10.0.26200`, 64-bit |
| Native execution | `IsWow64Process2`: native `0x8664`, process `0x0`; native x64 PE executable checks and SHA256 inventories |
| MSVC | `cl` 19.50.35723, x64 |
| GNU-driver Clang | 20.1.8, `x86_64-pc-windows-msvc`; no clang-cl profile |
| Tools | CMake/CTest 4.2.1, Ninja 1.12.1, native x64 Python 3.11.9 |
| ISA consumer | `instrset_detect=11`, required baseline level 2; pointers 64 bits, `long` 32 bits, `arm64=0` |
| Upstream dependency | Not selected; only the local 40-case profiles ran |
| Sanitizer | Native Clang strict UBSan Debug with static release CRT (`MultiThreaded`); positive exit 0, intentional signed-overflow exit 1 with expected diagnostic |

## Executed matrix

| Compiler/driver | Backend / ISA / logical widths | Mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | --- |
| MSVC | x86 / SSE2 baseline / 128, 256, 512 | Release / none / MD | 40 | 40 | 0 | 0 | 0 | Initial |
| MSVC | x86 / SSE2 baseline / 128, 256, 512 | Debug / none / MDd | 40 | 40 | 0 | 0 | 0 | Initial |
| GNU-driver Clang | x86 / SSE2 baseline / 128, 256, 512 | Release / none / dynamic release CRT | 40 | 40 | 0 | 0 | 0 | Initial |
| GNU-driver Clang | x86 / SSE2 baseline / 128, 256, 512 | Debug / strict UBSan / static release CRT | 40 | 40 | 0 | 0 | 0 | Initial |

Total selected and total executed: **160**, with no replays. Timeout counts
would be included in failures. Each profile also ran the example, yielding
`2 5 10 17`, and the independent runtime and guard controls.

## Controls, flags and numerical contracts

- All four writable-page controls passed at an actual page size of 4,096 bytes.
  Each profile's four read/write faults at the inaccessible boundaries returned
  `0xc0000005`. Partial counts, boundaries and canaries remain unchanged.
- Native binary inspection, binary hashes, registration inventories and JUnit
  results were retained. All six configuration/fixture regeneration checks ran.
- MSVC ordinary tests retain `/fp:strict`; classification alone uses `/fp:fast`.
  Clang retains `-ffp-contract=off`, selected rounding options and isolated
  classification-only finite-math stress. Broad fast-math/native-CPU options
  remain rejected. Numerical gates and fixtures are unchanged.
- Clang Debug uses `-O0`, debug information, `-fsanitize=undefined`,
  `-fno-sanitize-recover=undefined`, `-fno-omit-frame-pointer` and
  `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Its deliberate overflow
  stops before the continuation marker. MSVC Debug is not UBSan coverage.
- Original per-case deadlines remain 60 seconds, with build/test concurrency
  three and original serial configuration probes. No benchmarks ran.
- Focused helper controls verified combined stdout/stderr logs, required and
  optional command failures, timeout exit 124 and journal marking, Git output,
  archive contents, repeated packaging and SHA256. The intentional helper
  timeout is separate from the native matrix; no suite timeout occurred.

## Failures, retries and coverage limits

No failed cases, skips, timeouts or retries occurred in this native selection.
The deliberate helper failures and guard/UBSan faults are successful controls.
The usual Clang UBSan LNK4217 import warnings remain in raw build logs.

This run does not cover the pinned upstream selection, AVX2/AVX-512/FP16
matrices, clang-cl, ASan, native ARM, emulation or physical Intel. Physical
Intel testing remains omitted. The documented Mac ASan startup hang was not
retried. Direct Neon remains experimental. The new Mac runner/helper combination
still needs native M2 execution when that host is available.

## Evidence and handoff

| Evidence packet | Host-local ignored directory | SHA256 | Contents |
| --- | --- | --- | --- |
| `master-cleanup-evidence.zip` | `build/master-cleanup-20261115T222559Z/` | `dfdc7b32b24df74fa0c20028aad861f2ac72e08ada1515d36320b425a1a618f1` | Four native packets, shell logs, helper controls, ten plan comparisons, structural check and evidence audit |

Individual native packets are retained in their original ignored directories:

| Directory under `build/` | `return-evidence.zip` SHA256 |
| --- | --- |
| `windows-native-msvc-release-20261115T222720085398Z/` | `2f89a23b6dd09c2cfcc405c2720faddea8faf621a091e68aa654759996aca158` |
| `windows-native-msvc-debug-20261115T222755473129Z/` | `f40fee1cc6e1430718da96acf0406e97aad3fff3a61bb1941a6cbba8dc98f71f` |
| `windows-native-clang-release-20261115T222823833027Z/` | `42716a0f56770d1f30ce960b575bdb540c583f0180c8eabcdbb41d17d795c6c5` |
| `windows-native-clang-debug-20261115T222901829398Z/` | `c57b698210c9d2ec25ac5ecb73360dd8dd82078ab6b3030b4cd8cf4e86f23fb7` |

Raw packets contain command/exit journals, host/tool inventories, source
manifests and patches, compile commands, registration/JUnit results, all
control logs and executable hashes. ZIP CRCs, archived file contents and
checksums were verified. Transfer packets separately from the public report.

During validation, no commit, push, merge, branch deletion or hosted test run
was performed. Both retained branches are unchanged. The maintainer subsequently
approved committing and pushing the cleanup; the tested snapshot identifiers
above remain unchanged. After publication, update the Mac checkout through the
[native workflow](native-workflow.md)
and run the in-tree Mac runner with its shared helper, recording the actual
new HEAD/tree rather than reusing an older passing label.
