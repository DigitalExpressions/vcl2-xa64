# Native M2 Windows-correctness follow-up — 2026-11-17

## Outcome and tested source

**PASSED — first native attempt.** Actual selected CTest results: **8,036/8,036
executed passed**, 0 failed, 0 skipped and
0 timed out. Registered: 8,036; expected complete
matrix: 8,036. Separate examples passed: 8/8.
Timeouts, when present, are included in failed counts.

- Host: native Apple M2/macOS ARM64, DigitalExpressions/vcl2-xa64.
- Exact tested commit: `ab5bb8754a33724e05a170fa2867475077efd023`.
- Exact tested tree: `48376a53dfa5f5f2139651e3d44f5e1a9ed841b1`.
- Execution checkout: fresh detached worktree, clean before and after execution;
  all tracked file hashes unchanged. No dirty runtime patch was tested on Mac.
- Run ID: `windows-correctness-m2-20261117-ab5bb875`; attempt `01`.
- Native execution UTC interval: `2026-11-17T16:20:48.332474+00:00` to `2026-11-17T17:16:35.061710+00:00`.
- Primary branch/HEAD remained `master` at `e27134bd69d061a78a9e4e0d4b5df260831690b9`.
  Fetching origin obtained the approved commit without moving that branch,
  merging, resetting or overwriting any existing worktree/evidence.

The [approved handoff](mac-arm-correctness-handoff.md) and
[Windows completion plan](../windows-correctness-plan.md) govern this run.
The earlier `a099098` Mac report remains a separate checkpoint.
Documentation and portable results were prepared afterward in another detached
review worktree at the approved commit; the executing worktree stays frozen.

## Windows source comparison

The original [Windows report](windows-amd-2026-11-17-correctness-completion.md)
executed base `e27134bd69d061a78a9e4e0d4b5df260831690b9`,
tree `9b3ea5a74898f34e3bdd94141788a87e8f019d3d`, plus captured changes.
Its original source identifiers remain unchanged:

| Original Windows identifier | Value |
| --- | --- |
| Whole source, 402 paths | `af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528` |
| Runtime, 35 products | `6a108894ee1dacea0199df9f25f6a8b79b5828e03616ea0c6a1c16c53ae66580` |
| Captured source patch SHA256 | `5c5b3b6c06553f9ae72e1765bd7a3fba9d83cd283d0128876de406bf6a00e65e` |
| Original source archive SHA256 | `b54ff5ea91db6d1f5d1094bdae612f723ded0b9352ad21ac90055ecad1e42e7e` |

The Windows portable checksum inventory and both recorded source/runtime
fingerprints were verified before native execution. All **157 listed code
paths** reconciled: **84 raw-identical**, **73 verified newline differences**.
For 72 paths, replacing Mac LF with CRLF reproduced the original Windows raw
SHA256. For `backends/x86/x86_classification.h`, replacing Mac CRLF with LF
reproduced the original tested Windows raw SHA256. Every differing path also
matched its supplied LF-normalized digest. These were in-memory comparisons;
no source or line ending was rewritten. There were **no unexplained code gaps**.

All **28** pristine vendor files, including the upstream README outside the
selected code list, matched original Windows raw hashes and the pinned vendor
inventory exactly. Vendor normalization was neither permitted nor used.

The approved Mac checkout contains **417 tracked paths**, including 15 added
publication documentation/evidence paths. Existing documentation, evidence and
checkout metadata are recorded separately from the 157 code paths. Its full
raw fingerprint is `fb61c5bbe2f16eb94009aab6f906adee17635988c34bd14325aa00e88e135658`; its selected
35-product raw fingerprint is `e41e0283c0d368effcec80b367e71533c3bb17f775920e5a52dc8b2bcd7f743e`.
Different whole-tree/raw-runtime fingerprints are retained, not declared equal.
The complete [source comparison](mac-arm-correctness-2026-11-17-ab5bb87/source-comparison.json) records every
raw hash, verified reconstruction and publication path. This establishes the
specified source relationship, not that Windows executed the later publication
commit or that all numerical outputs are bitwise identical.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| CPU / OS | Apple M2, Mac14,3; macOS 26.7, build 25G229 |
| Native proof | arm64 Python; Rosetta query returned 0; tested executables are arm64 Mach-O |
| Compiler | Apple clang version 17.0.0 (clang-1700.4.4.1); Target: arm64-apple-darwin25.6.0 |
| SDK / tools | macOS SDK 26.1; CMake/CTest 4.4.4; Ninja `1.11.1.git.kitware.jobserver-1`; Python 3.9.6 |
| Testbench | Reused clean `a7293702385e5169a5f58b83177f2a43cd59139b`; revision and all three pinned source hashes verified |
| UBSan control | Positive exit 0; intentional signed overflow exit -6 with expected diagnostic and no continuation |
| Page size / runtime | 16,384 bytes; `arm64=1 neon=1 instrset_detect=0 pointer_bits=64 long_bits=64` |

## Executed matrix

All rows use Apple Clang. Debug is strict nonrecovering UBSan; Release has no
sanitizer. Counts reconcile complete registered inventories with actual JUnit.

| Backend / maximum | Mode | Registered | Executed | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Compatibility / 512 | Release | 1350 | 1350 | 1350 | 0 | 0 | 0 | 01 (passed) |
| Compatibility / 512 | Debug / strict UBSan | 1350 | 1350 | 1350 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 128 | Release | 486 | 486 | 486 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 128 | Debug / strict UBSan | 486 | 486 | 486 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 256 | Release | 890 | 890 | 890 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 256 | Debug / strict UBSan | 890 | 890 | 890 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 512 | Release | 1292 | 1292 | 1292 | 0 | 0 | 0 | 01 (passed) |
| Direct Neon / 512 | Debug / strict UBSan | 1292 | 1292 | 1292 | 0 | 0 | 0 | 01 (passed) |
| **Total** | | **8036** | **8036** | **8036** | **0** | **0** | **0** | |

Total selected executions including retries: **8,036**; this Mac task
made no replay invocation. Each successful separate example printed `2 5 10 17`.
Examples and deliberate negative controls are outside the 8,036-case total.
The [complete inventory](mac-arm-correctness-2026-11-17-ab5bb87/test-inventory.json) retains every case name,
its original deadline and actual result.

## Integrity, controls and numerical contracts

- Canonical `vendor/vectorclass/` remained untouched in primary and executing
  checkouts. All 28 vendor files and the external pin manifest retained bytes,
  raw hashes, sizes and modification timestamps before/after execution.
  Generator checks and Git vendor-diff checks passed before and after the run.
- Original VCL pin remains `667577495efaa36a10cbb5d6cb8c942c23913848`, reconstructed
  vendor tree `36aee120c28c9c64e62903bfc81139003e37cedd`. Identical before/after
  generation records accept 35 products: 25 exact-byte and
  10 explicitly permitted LF-equivalent root products.
  Vendor bytes always require exact equality. No generator write was used.
- All eight generation integrity controls passed. Deliberate corruptions were
  confined to isolated `build/` fixture copies; canonical vendor and its pin
  were never modified to make any check pass.
- Native executable inspection and SHA256 rechecks covered
  7,996 test/example records plus
  3 probe binaries. The linked runtime consumer passed.
- Strict Debug preserved `-fsanitize=undefined`, `-fno-sanitize-recover=undefined`,
  `-fno-omit-frame-pointer`, and
  `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Apple UBSan's actual negative
  exit is retained above, rather than rewritten to another host's convention.
- The writable-page control exited 0. All four intentional
  forbidden reads/writes faulted with actual exits
  -10, -10, -10, -10.
  These expected controls are distinct from unexpected suite failures.
- New boundary/literal-conversion regressions execute inside the existing
  `edges` case on both ARM implementations at supported widths. They check
  unpadded protected-page lookups, signed-minimum division, byte shifts,
  integer composition, and independently specified FP64 integer expectations
  rotated through mixed lanes under all four rounding modes. No cases were
  removed to keep the original registration counts.
- Independent reference-fixture regeneration and configuration checks remain
  in every profile. Seeds, reference inputs, accuracy/NaN/signed-zero gates,
  canaries and rounding controls were unchanged. Ordinary tests retain
  `-ffp-contract=off`, selected `-frounding-math`, and classification-only
  `-ffinite-math-only`; no broad fast-math option was introduced.
- Original **60-second case deadlines** and **concurrency three** were verified.
  Caffeinate prevented idle sleep; deadlines were not extended.

## Failures, attempts and remaining gaps

All eight native profiles and all eight examples passed on the first Mac
attempt. There were no failed, skipped or timed-out cases, source fixes or
replays. Native runner exit was 0; all post-run integrity checks passed.

The original Windows results are separate: 5,392 full-baseline passes,
320 selected higher-ISA local passes and 20 focused programs after recorded
fixes. Windows intermediate failures remain in their original reports/archives.
An ARM pass does not execute native AVX2/AVX-512 branches or enlarge that Windows
40-case-per-profile higher-ISA selection. The source comparison above ties the
recorded code paths without relabeling Windows source IDs.

Direct Neon remains experimental. Other CPUs/toolchains, physical Intel,
unselected APIs, native FP16 and ASan remain outside this matrix. ARM exception
flags/masks remain unemulated. No ASan retry, benchmark, translated/emulated
execution, new shared corpus capture or cross-host numerical parity comparison
was performed. The older `bd7807e` corpus comparison is not extended by this run.

With this selected M2 gate passed, renewed Windows performance comparison at
these final correctness source IDs is the remaining plan gate, under separate
authorization. Earlier timings do not describe these new correctness fixes.

## Evidence and handoff

| Evidence packet | Host-local ignored directory | SHA256 |
| --- | --- | --- |
| `return-evidence.zip` | `build/mac-m2-correctness-ab5bb8754a33/build/mac-m2-native-20261117T162048332095Z/` | `00717d651a307ee417dd8be5218751f856059c73f4150568153bb91909da49b7` |
| `mac-m2-correctness-ab5bb875-evidence.zip` | `build/mac-m2-correctness-ab5bb8754a33-20261117T161708Z/` | `33fad7ca351fdbf15124287c064690be8b0bd59d458b9cfa44d806a09ea9e1eb` |

The combined packet has **1,212 verified entries**. It retains
original native packet/receipt, command/exit journals, source/compiler/flag
inventories, JUnit, full logs, executable hashes, complete tested source,
before/after vendor snapshots, generation-control fixture copies and generated
C++/CMake probe/reference inputs. ZIP CRCs, per-entry digests and original native
packet SHA256 were verified. At validation completion, all original directories and attempts remained.
The separately authorized cleanup below preserves their evidence packets.

[Portable results](mac-arm-correctness-2026-11-17-ab5bb87/README.md) include complete case inventories,
source comparison/manifests, binary hashes, host/flags/controls, vendor records,
and archive receipts. Their [checksum inventory](mac-arm-correctness-2026-11-17-ab5bb87/checksums.sha256) protects
portable bytes for later Windows/Mac review. These are correctness metadata,
not raw-bit corpus captures or a claim of universal bitwise identity.

At validation completion, report/metadata edits were prepared in a separate
detached review worktree at the approved commit. The primary checkout and clean
execution source remained unchanged during validation. No commit, push, merge,
PR, benchmark or cleanup was performed by that validation phase.

## Authorized publication and cleanup follow-up

The maintainer subsequently authorized cleanup, commit and push of these Mac
results and preparation of the [next Windows prompt](windows-final-source-performance-prompt.md).
This publication changes reports/status metadata only; it does not change the
exact source, tree or original Windows IDs validated above. Master advances
by fast-forward only. No runtime, test, fixture, runner or vendor change is
included, and no new native matrix is claimed for the documentation commit.

Before removing rebuildable output, the original native packet was copied
byte-for-byte to the controller evidence directory below. The combined packet
and a new checksummed supplement retain all original nonbinary build records
and generated inputs. Every removed non-build-system executable matched its
recorded binary hash. The temporary execution worktree was removed using Git;
the temporary review worktree is removed after its commit reaches master.
Older worktrees, tools, the pinned testbench, previous evidence and legacy
checkouts are retained. No PR or upstream write is involved.

| Retained cleanup evidence | Directory | SHA256 |
| --- | --- | --- |
| `return-evidence.zip` (relocated original) | `build/mac-m2-correctness-ab5bb8754a33-20261117T161708Z/` | `00717d651a307ee417dd8be5218751f856059c73f4150568153bb91909da49b7` |
| `cleanup-evidence-supplement.zip` | `build/mac-m2-correctness-ab5bb8754a33-20261117T161708Z/` | `84b058c61a325ef76b7f3d171980a4aa2bfd80c5a07e6c261de1a958d1e8b495` |

The supplement covers 8,964 nonbinary files. It permits removal of
16,119 rebuildable binary/object/debug files (1,407,880,022 bytes). The
[cleanup receipt](mac-arm-correctness-2026-11-17-ab5bb87/cleanup-receipt.json) records
coverage and locations; the original receipts retain their historical paths.
Future Windows measurements and publication of their results need their own
authorization. No performance measurements were run on the Mac in this task.
