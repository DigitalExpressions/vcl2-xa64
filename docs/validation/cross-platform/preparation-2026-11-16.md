# Shared SIMD comparison preparation — 2026-11-16

## Outcome and tested source

**Preparation passed after correcting an oracle assumption; final validation
has not started.** The final candidate passed 672/672 vector cases in eight
native AMD Windows profiles: 5,376 cases and 38,864 output lanes. These are
additional corpus cases, not the existing pinned CTest matrix.

The [Windows prompt](../windows-final-verification-prompt.md) requires approval
before committing/pushing the shared harness, then one frozen commit for both
hosts. No frozen validation commit exists yet. Run ID: `simd-parity-20261116`.

| Tested candidate identifier | Value |
| --- | --- |
| Base HEAD / branch | `fb54e3e179dca347ae0ff02fc27aff4b40ce26fa` / `master` |
| Base tree | `200d85fdf91348857bfbecfe90ffc1e3b8d64127` |
| State | Ten added files; 134-file inventory; unchanged during each capture and identical across all final eight profiles |
| Working fingerprint | `ad1593eb1edf9c4a249bd446a9a01da11c29acbd768958541340458721d037f7` |
| Tracked patch SHA256 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` (empty: added files captured separately) |
| Corpus SHA256 | `0e18a9aecb2b15b31b1ff0aeaa61e1d8f91867df675900114c6ecbf3e385946c` |
| Policies SHA256 / version | `610be76973950ba9eea1c3779ccdb4397e4c82fe4bf2e73f3226039edf152fd3` / `simd-policies-2` |

Each packet retains the source inventory, working patch and added-file patches.
Base HEAD/tree alone do not identify this dirty candidate. Subsequent review
changes are documentation-only: this report, navigation, numerical-contract
explanations and integration-plan notes. Existing reports retain their source IDs.

## Host, tools and dependencies

Native AMD Ryzen 9 7950X, Windows 11 Pro `10.0.26200`, x64 Python 3.11.9.
`IsWow64Process2`: native `0x8664`, process `0x0`; native PE checks and executable
hashes retained. MSVC 19.50.35723; GNU-driver Clang 20.1.8 targeting
`x86_64-pc-windows-msvc`; CMake/CTest 4.2.1; Ninja 1.12.1.

The separate baseline consumer reports runtime ISA 11, required level 2,
64-bit pointers, 32-bit `long`, `arm64=0`. CPU/OS support is checked before
higher-ISA execution. Compiled captures report ISA 2, 8 or 10. No upstream
testbench selection ran during preparation.

## Executed matrix

All profiles cover backend 1, logical widths 128/256/512 and 4,858 output lanes.

| Compiler | ISA | Mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| MSVC | SSE2 | Release / none / MD | 672 | 672 | 0 | 0 | 0 |
| MSVC | SSE2 | Debug / none / MDd | 672 | 672 | 0 | 0 | 0 |
| GNU-driver Clang | SSE2 | Release / none / dynamic release | 672 | 672 | 0 | 0 | 0 |
| GNU-driver Clang | SSE2 | Debug / strict UBSan / static release | 672 | 672 | 0 | 0 | 0 |
| MSVC | AVX2 | Release / none / MD | 672 | 672 | 0 | 0 | 0 |
| MSVC | AVX-512 | Release / none / MD | 672 | 672 | 0 | 0 | 0 |
| GNU-driver Clang | AVX2 | Release / none / dynamic release | 672 | 672 | 0 | 0 | 0 |
| GNU-driver Clang | AVX-512 | Release / none / dynamic release | 672 | 672 | 0 | 0 | 0 |

All 28 Windows-only profile pairs were compared after independent validation:
134,488 exact lane matches, 336 bounded numerical differences, 672 declared
FMA differences, 528 declared rounding differences, zero unexpected differences.
These repeated pairings do not establish bitwise identity or Mac parity.

## Controls, flags and numerical contracts

- All eight writable guard controls passed at the actual 4,096-byte page size;
  each profile's four fault controls returned `0xc0000005`.
- Clang UBSan positive control exited 0; intentional overflow exited 1 with the
  expected diagnostic and no continuation. MSVC Debug is unsanitized.
- MSVC retains `/fp:strict`; Clang retains no contraction and `-frounding-math`.
  Clang Debug uses `-O0`, strict nonrecovering UBSan, frame pointers, static
  release CRT and halt-on-error settings. Raw compile/link commands are retained.
- Baseline detection receives no higher-ISA flags. Captures have a 60-second
  deadline and build concurrency three. Each case checks its rounding mode with
  FTZ/DAZ off; every partial count and store canary remains.
- Exact integer/Fraction references and existing Decimal fixtures are independent
  of SIMD results. Original accuracy gates, fixtures, CTest registrations, test
  source and deadlines are unchanged. The full suite is still separate.
- Nine groups of comparator controls passed: corruption, zero signs, wrong
  rounding variant, excess error, missing/duplicate cases, unknown policies,
  source/input/integrity mismatches, guard failures, width scope and preparation
  rejection. Negative-control files remain under ignored build directories.

## Failures, retries and coverage limits

An initial MSVC Release capture passed 576/576 cases. The expanded policy-1
candidate executed eight profiles: 5,352/5,376 passed; 24 vector cases failed
(88 lanes) across four higher-ISA profiles. No skips/timeouts occurred.

The oracle incorrectly assumed all `roundi` overloads use current direction.
Existing source explicitly uses nearest-even for some paths: FP64 on AVX2/ARM,
and native AVX-512 FP32 width 512. Other selected paths use current direction.
Policy 2 validates the source-derived choice exactly and labels differences
`documented_rounding`. All inputs/modes and original failures are retained.
No library fix or accuracy tolerance relaxation was made. This interpretation
requires review; it does not promise identical non-default rounding results.

After correction, eight fresh captures passed 5,376/5,376. Across all snapshots,
17 captures executed 11,328 cases: 11,304 passed and 24 earlier failed. These
mixed-source totals are retained history, not one final-source pass rate.

Final frozen-source Windows pinned matrix/corpus replay, native M2 matrix and
captures, and cross-platform comparison remain outstanding. Higher-ISA Debug/
sanitizer captures, clang-cl, FP16, ASan and unselected domains are outside this
preparation. No benchmarks, hosted CI, WSL or QEMU were used. Physical Intel is
omitted; Direct Neon remains experimental.

## Evidence and handoff

| Packet | Ignored host-local directory | SHA256 |
| --- | --- | --- |
| `preparation-evidence.zip` | `build/cross-platform-preparation-20261115T233331Z/` | `dd11c114fb53f972a6d52ce54bb3307ab12385e26973f1b6434c973fb160dd11` |

The combined packet retains all 17 captures, original failures/policies,
portable preparation bundles, raw packets, source patches/inventories, 28
comparison files, harness check logs and the audit. Original build directories
remain under `build/simd-capture-<UTC>-<profile>/`. Final raw packets/contents,
portable checksums and combined ZIP CRC were verified. UTC directory dates can
precede this report's Stockholm date.

Nothing was committed, pushed, merged or deleted from existing branches during
preparation. Next: review the [harness and policies](README.md), approve
commit/push, freeze that exact commit, then run final Windows validation and
hand the same commit/run ID to the Mac. Final portable results will use the
documented per-host/per-attempt paths; preparation bundles stay under build.
