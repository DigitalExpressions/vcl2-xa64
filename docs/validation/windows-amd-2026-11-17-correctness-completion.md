# Native AMD Windows correctness completion

Date: 2026-11-17. **Selected native Windows matrix passed after fixes.**
Four full baseline profiles pass 5,392/5,392 and all eight higher-ISA local
profiles pass 320/320: **5,712/5,712 selected CTest instances**, no failures,
skips, timeouts or suite UBSan diagnostics. Twenty focused post-fix programs pass.
The [durable plan](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/windows-correctness-plan.md) records the restart point.

## Outcome and tested source

Native AMD Windows x86-64, repository `DigitalExpressions/vcl2-xa64`, primary
branch `master`. The frozen detached validation source is base commit
`e27134bd69d061a78a9e4e0d4b5df260831690b9`, tree
`9b3ea5a74898f34e3bdd94141788a87e8f019d3d`, **plus uncommitted captured changes**.
Its 402-path whole-source fingerprint is
`af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528`.
The 35-product runtime fingerprint is
`6a108894ee1dacea0199df9f25f6a8b79b5828e03616ea0c6a1c16c53ae66580`.
This is not a native pass of the clean base or any later publication commit.
The exact source ZIP, dirty patch including added files and original manifests
are retained under `build/windows-correctness-20261117/`.

Fingerprints use SHA256 of sorted UTF-8
`relative_path + NUL + file_SHA256_hex + newline`. Initial/final inventories
are checked for equality. The baseline worktree was reconstructed from retained
successful iteration06 bytes and verified against all 402 original file hashes.
Two classification-helper files had subsequently been converted from LF to
CRLF in the primary checkout; their retained tested LF bytes were restored only
in this detached snapshot. Neither the original inventories nor vendor bytes
were normalized or replaced.

The Mac report published at `3abf7af` tested `a099098`. It does not cover these
new fixes. A new [M2 handoff](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/mac-arm-correctness-handoff.md) requires an approved
publication checkpoint and explicit source comparison before native execution.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| Hardware / OS | AMD Ryzen 9 7950X; Windows 11 Pro 10.0.26200; native x64 |
| Native proof | `IsWow64Process2`: native `0x8664`, process `0x0`; x64 PE `0x8664` |
| Primary compiler | MSVC 19.50.35723, x64 toolset 14.50.35717 |
| Comparison compiler | GNU-driver Clang 20.1.8, Windows x64; not clang-cl |
| Build tools | Python 3.11.9; CMake/CTest 4.2.1; Ninja 1.12.1 |
| Higher ISA | CPU and OS support SSE2/SSE4.2, AVX2/FMA/F16C and AVX-512F/DQ/BW/VL; XCR0 `0xe7` |
| Original VCL | `667577495efaa36a10cbb5d6cb8c942c23913848`; all 28 vendor files remain pristine |
| Optional testbench | Clean pin `a7293702385e5169a5f58b83177f2a43cd59139b`; source-hash checks retained |
| Sanitizer | Native Clang strict nonrecovering UBSan Debug, static release CRT; MSVC Debug is unsanitized |

Physical Intel is omitted. No WSL, QEMU, Rosetta, hosted workflows or benchmark
timings were used. Native ASan is outside this matrix.

## Implementation and regression coverage

Runtime fixes are in canonical
`adaptation/patches/0004-x86-correctness.patch` and
`backends/x86/x86_classification.h`, layered after the preserved AVX2 conversion
fix in patch 0003. All root products are generated; `vendor/vectorclass/` is
unchanged. The fixes address:

- Bounded native lookup/gather reads, including short tables and the last
  byte/word of a protected-page table. AVX2 dword gathers back up within the
  table and extract the requested byte/word; tables are not padded.
- Compact-mask signed-minimum integer division, native byte shifts at or above
  the full width, and a repeated 16-bit blend mask affecting the second half.
- AVX-512F-only FP64-to-int64 fallback casts, using already guarded half-width
  conversions. Native AVX-512DQ hardware conversion remains selected at level 10.
- Sign-bit-only absolute value, classification under finite-math stress,
  AVX-512 logarithm NaNs/zero/subnormal cases, infinite-base power signed zeros
  and FP64 radian reduction for FMA-enabled x86 trigonometry. Existing gates,
  polynomial coefficients and exceptional-result policies remain in force.
- Native MSVC optimizer workarounds: exact bounded partial reads on level 9+,
  integer classification helpers for 256-bit compact masks, FMA-enabled
  conditional-add signed zero, native-width composition of AVX2 FP32 power,
  and explicit piecewise arithmetic for the existing atan rational argument.
  The unsuccessful tiny-argument atan shortcut was removed.

`tests/boundary_regression.h` adds independent literal lane expectations and
page-end lookups inside existing edge cases. The preserved
`truncate_i64_regression.h` checks exceptional/boundary FP64 bit patterns across
all four rounding modes and vector widths, without implementation-derived
expected values.

Some retained failures were test API assumptions. Compact x86 masks now expand
through `select(mask,-1,0)` rather than broadcasting a scalar mask bit field;
the lane expectations are unchanged. Native `Vec8d::round_to_int32` explicitly
uses nearest+SAE in original VCL, so its oracle now checks that behavior in all
four environments; the separate current-direction `roundi` API is additionally
checked where available. The composed-integer oracle materializes independent
volatile scalar input copies before vector calculations to avoid the reproduced
MSVC optimizer coupling; inputs, arithmetic and acceptance criteria are unchanged.

## Executed matrix

| Driver | ISA / scope | Mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | --- |
| MSVC | Baseline local + pinned SSE4.2 | Release / none / dynamic Release | 1348 | 1348 | 0 | 0 | 0 | First at final source |
| MSVC | Baseline local + pinned SSE4.2 | Debug / none / dynamic Debug | 1348 | 1348 | 0 | 0 | 0 | First at final source |
| Clang | Baseline local + pinned SSE4.2 | Release / none / dynamic Release | 1348 | 1348 | 0 | 0 | 0 | First at final source |
| Clang | Baseline local + pinned SSE4.2 | Debug / strict UBSan / static Release | 1348 | 1348 | 0 | 0 | 0 | First at final source |
| MSVC | AVX2 local | Release / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| MSVC | AVX2 local | Debug / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| Clang | AVX2 local | Release / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| Clang | AVX2 local | Debug / strict UBSan / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| MSVC | AVX-512 local | Release / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| MSVC | AVX-512 local | Debug / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| Clang | AVX-512 local | Release / none / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| Clang | AVX-512 local | Debug / strict UBSan / static Release | 40 | 40 | 0 | 0 | 0 | iteration06 |
| Total | Selected final matrix | | **5712** | **5712** | **0** | **0** | **0** | |

Complete JUnit names/counts match the registered inventories; timeouts are a
subset of failures, both zero. Twelve examples produce `2 5 10 17`. A separate
primary-checkout MSVC Release preflight passes 40/40 at its own recorded source
fingerprint; it is excluded from the final selected total above.

The focused check newly reproduces the pre-0004 AVX2 page-end overread
(`0xc0000005`) and the AVX-512F fallback's undefined cast at
`vectorf512.h:1325` (strict UBSan exit 1, out-of-range diagnostic). These new
reproducers use retained pre-patch headers, not relabeled original run archives.
After the fix, 20 programs pass: both compilers, both modes and ISA selectors
2/6/8/9/10, with strict UBSan for all five Clang Debug processes. Each checks
the boundary literals and 1,232 FP64 conversion lane expectations across all
four rounding modes. Clang level 9 explicitly disables DQ/BW/VL; MSVC selects
the level-9 VCL fallback while `/arch:AVX512` permits broader compiler codegen.
The MSVC check is not proof of execution on F-only physical hardware.

## Controls, flags and numerical contracts

The runners retain native runtime queries and executable hashes; positive
actual-page guard checks and all four intentional read/write boundary faults;
configuration/fixture regeneration checks, canaries, rounding modes and original
numerical thresholds. Windows page size is 4,096 bytes. A negative guard fault
is expected `0xc0000005`, separate from any unexpected suite fault.

Ordinary MSVC cases retain `/fp:strict`; only classification stress uses
`/fp:fast`. Clang retains `-ffp-contract=off`, selected `-frounding-math` and
classification-only `-ffinite-math-only`. Explicit FMA intrinsics do not change
the no-implicit-contraction policy. Strict UBSan retains
`-fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer`
and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`, with a passing positive
and intentional signed-overflow exit 1, expected diagnostic and no continuation.
All CTest deadlines remain 60 seconds and build/test concurrency remains three.
Fourteen final/preflight/focused control records verify 56 expected guard
faults and four strict UBSan positive/negative pairs. All eight generation
integrity controls pass; read-only post-run checks require exact vendor bytes
and exact reproduction of all 35 products in primary and frozen checkouts.

## Failures, attempts and coverage limits

The [earlier triage](windows-amd-2026-11-17-isa-triage.md) remains unchanged:
196 higher-ISA passes and 124 failures at its own captured source. All 124 are
resolved in the final selected higher-ISA matrix. Six intermediate source
checkpoints retain their complete 320-case results and original failures:

| Attempt | Passed | Failed |
| --- | ---: | ---: |
| iteration01 | 216 | 104 |
| iteration02 | 259 | 61 |
| iteration03 | 299 | 21 |
| iteration04 | 312 | 8 |
| iteration05 | 317 | 3 |
| iteration06 | 320 | 0 |

These are distinct sources after fixes, not six passes of the final candidate.
Including all six higher-ISA checkpoints, the full baselines and separate
preflight, this task executed 7,352 CTest instances: 7,155 passes and 197
intermediate failures. Focused processes/intentional negative controls are
counted separately. No final-source baseline profile required a replay.
All failed logs, manifests, canonical patch snapshots and diagnostic sources
remain retained. Earlier source bytes for iterations 01-04 were recovered from
retained pre-patch headers/patch snapshots, unchanged final-snapshot files and
base Git test blobs, accepting test-edit/newline reconstructions only after
every original recorded raw hash matched (401/401/401/402 files). These are
explicitly labeled reconstructed archives; original manifests remain unchanged.
Iteration05 and the final candidate retain their captured source snapshots.
A diagnostic initially labeled toolset 19.44 actually used
19.50 after developer-shell initialization failed; it is explicitly excluded
as 19.44 evidence. The verified retry selected the actual 14.44 compiler path.
Neither standalone compiler diagnostic is a full matrix pass.

Higher-ISA coverage is the full **local 40-case inventory**, not all 1,348
pinned cases at each higher ISA. Native AVX-512F-only level 9 has focused
conversion/boundary coverage above, not a full local matrix.
M2 validation of this candidate, a new cross-host corpus comparison, other
toolchains/hosts, native FP16 and final performance measurements remain separate
gates. No universal bitwise identity or upstream-equivalent speed is claimed.

## Evidence and handoff

Raw root: `build/windows-correctness-20261117/`; baseline evidence is under its
detached `frozen-source/build/windows-native-*` directories; higher-ISA evidence
is under `iteration01/` through `iteration06/`. Original older triage remains
under `build/windows-isa-triage-20261117/`. All evidence is ignored locally.
[Portable metadata](windows-correctness-2026-11-17/README.md) retains the exact
tested inventory, patch/source archive hashes, all twelve profile counts,
focused exits/binary hashes, controls, all six investigation rounds and
source-reconstruction provenance. The
[receipt inventory](windows-correctness-2026-11-17/receipts.json) lists every
verified native packet and source archive; portable bytes have their own
`checksums.sha256`.

| Packet | Relative location under raw root | SHA256 | Scope |
| --- | --- | --- | --- |
| Evidence supplement | `correctness-evidence-supplement.zip` | `ea7ce958a81ff8e6245cf656750a6f480d68038e73bd1fdc62f6c5fd2d045e2f` | 4,873 verified entries: source snapshots, original failures, command/exit journals, compile flags, JUnit, logs, probes/reference sources and retained executable hashes |

The primary comparison checks 157 non-documentation source paths: 155 are
raw-identical and two are verified exact LF-to-CRLF transformations of the
classification helper and its root copy. Later documentation/evidence and
checkout attributes are identified separately. Original tested identifiers
are retained; the final primary checkout is not relabeled as an executed whole
tree. Vendor files always require exact raw hashes. No raw logs, binaries,
absolute machine paths or credentials are included in portable metadata.

The existing primary index and unrelated work are preserved. Origin was
fetched without resetting, overwriting branches or merging. No commit, push,
merge, PR or evidence cleanup was performed during validation. The maintainer
subsequently approved cleanup, commit and push of the reviewed source, portable
evidence and M2 handoff on 2026-11-17. Publication adds documentation and records
the two verified newline transformations; it does not replace the original
tested source identifiers. Raw evidence and validation worktrees are retained.
Only after both hosts validate the same source should the separate Windows
comparison against Agner Fog's original upstream be repeated.
