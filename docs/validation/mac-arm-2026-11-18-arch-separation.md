# Native M2 architecture separation — 2026-11-18

## Outcome and tested source

**PASSED after retained migration/setup build failures.** Candidate **04** selected CTest results:
**8,036/8,036 executed passed**, 0 failed,
0 skipped and 0 timed out; 8,036
registered. Expected complete matrix: **8,036**, plus eight separate examples;
examples passed: **8/8**. Timeouts are included in failed counts.
This is a new layout result, not a transfer of the earlier `ab5bb87` passes.

- Approved documentation/base commit: `1a5226446f14e1f4d26216ae688b40ccc9b0fa03`.
- Base tree: `e5b8579b6292cce2ea6a67b8cdf8027ec228d7bc`; **not the tested implementation tree**.
- Tested source kind: complete frozen **uncommitted** capture; tested commit/tree
  are null. All **454** source files, including additions, were captured.
- Source fingerprint: `cef817d707baa6033fe70c223538c17daea4152ca85cd16a274e12ea490633a2`.
- Tracked implementation patch SHA256: `e740aaa20b9d4337f035aba689765969ef19498806abff526a94e3c69a553f45`.
  All added/deleted paths are listed in the [capture inventory](arch-separation-m2-2026-11-18/capture.json);
  additions' exact bytes accompany the tracked patch in the full source archive.
- Exact source ZIP SHA256: `28537e29282cd694576fe93cfca2e5fee856f5b11e3305e0641f8eb155648805`.
- Run ID: `arch-separation-m2-20261118T194215Z-candidate04`; final candidate 04.
- Controller/native phase UTC interval: `2026-11-18T20:08:15.047713+00:00` to `2026-11-18T21:11:14.802256+00:00`.
- Implementation worktree/branch: `build/arch-separation-m2/`,
  `arch-separation-m2`, with uncommitted implementation changes. Primary `master`
  remained clean at the approved documentation commit. Fetching origin moved
  no local branch or worktree and overwrote no evidence.

Fingerprint ordering is relative paths sorted by their UTF-8 bytes, hashing
`relative_path + NUL + raw_file_SHA256_hex + newline`. The complete capture,
patch/new files, archive CRC/per-entry hashes and manifest were verified before
and after execution. Builds ran only from `candidate-04/source/`, never a moving
worktree. [Runner/validator hashes](arch-separation-m2-2026-11-18/runner-hashes.json) identify the actual
captured tools separately. The default clean-source guard still rejects a dirty
checkout; an expected rejection control is retained. No source commit or clean
status spoof was used to get past that guard.

The [approved handoff](mac-arch-separation-handoff.md) and
[architecture plan](../arch_separation_plan.md) govern this migration.
Final reports/status/handoff metadata were prepared after execution outside the
frozen capture. Per-path Git whitespace annotations preserve 24 inherited
space-at-EOL lines in eleven derived ARM headers/examples without altering their tested bytes
or checkout filters. They do not change its identity or acquire additional native
coverage. Native Windows has **not** executed this implementation.

## Implementation and source ownership

- `#include "vcl2_xa64.h"` selects original x86 headers or the ARM family at
  preprocessing time. Public VCL types/operators remain direct inline APIs;
  there is no per-operation wrapper or runtime architecture branch.
- All **28** original paths, including README, LICENSE, changelog, runtime
  detector and dispatch examples, are restored at root as exact pinned bytes.
  Vendor/reference bytes and its original inventory remain untouched.
  Fork usage guidance lives in [README-vcl2-xa64.md](../../README-vcl2-xa64.md).
- Root `*_neon.h` files are authoritative ARM sources. Existing Direct Neon
  core names remain distinct from `*_compat_neon.h`; private helpers and common
  conversion/wide-composition/math dependencies have separate ARM names/guards.
  [The file/guard map](../arch-separation-map.json) records all dispositions.
- Compatibility remains the ARM default; Direct Neon remains experimental and
  requires explicit opt-in. Logical maxima 128/256/512 and namespace contracts
  are preserved. ARM64EC, invalid overrides and unsupported targets reject.
- `instrset_detect_neon.cpp` owns ARM runtime queries. CMake selects it for ARM,
  unchanged upstream `instrset_detect.cpp` for x86, and a compile-time selector
  for a universal Apple runtime build. No x86 binary was run on the Mac.
- The old root-generation writer is retired: `--write` always rejects without
  modifying originals. Its manifest has no outputs/patches. Duplicate editable
  ARM sources were retired; historical patches and `backends/x86/` remain
  inactive research assets, outside active include paths. No x86 repair campaign
  was resumed and no native AVX header was blindly renamed into an ARM header.
- Owned examples, tests, capture tools, build logic and benchmark include sites
  use the facade. No benchmark timing was collected. The external pinned
  testbench is unchanged; only its build copies adapt includes/version guards.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| CPU / model | Apple M2; Mac14,3 |
| OS | ProductName:		macOS; ProductVersion:		26.7; BuildVersion:		25G229 |
| Native proof | arm64 Python and tested arm64 Mach-O executables; Rosetta query `0` |
| Compiler / target | Apple clang version 17.0.0 (clang-1700.4.4.1); Target: arm64-apple-darwin25.6.0 |
| SDK / Python | 26.1; 3.9.6 (default, Oct 17 2025, 17:15:53)  |
| Build tools | cmake version 4.4.4; ctest version 4.4.4; Ninja `1.11.1.git.kitware.jobserver-1` |
| Testbench | clean `a7293702385e5169a5f58b83177f2a43cd59139b`; all three pinned CPP hashes verified |
| Page size | 16,384 bytes, discovered on this host |
| Runtime consumer | `arm64=1 neon=1 instrset_detect=0 pointer_bits=64 long_bits=64` |

Existing installations were reused without installing tools or overwriting the
testbench. Compiler include traces, full compile flags and executable hashes
are retained. Caffeinate prevented idle sleep without extending case deadlines.

## Executed matrix

All rows use native Apple Clang. Debug uses strict nonrecovering UBSan;
Release has no sanitizer. Complete actual JUnit names reconcile with each
registered inventory, including the retained constructor and hyperbolic cases.

| Backend / maximum | Mode | Registered | Executed | Passed | Failed | Skipped | Timed out | Status |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Compatibility / 512 | Release | 1350 | 1350 | 1350 | 0 | 0 | 0 | passed |
| Compatibility / 512 | Debug / strict UBSan | 1350 | 1350 | 1350 | 0 | 0 | 0 | passed |
| Direct Neon / 128 | Release | 486 | 486 | 486 | 0 | 0 | 0 | passed |
| Direct Neon / 128 | Debug / strict UBSan | 486 | 486 | 486 | 0 | 0 | 0 | passed |
| Direct Neon / 256 | Release | 890 | 890 | 890 | 0 | 0 | 0 | passed |
| Direct Neon / 256 | Debug / strict UBSan | 890 | 890 | 890 | 0 | 0 | 0 | passed |
| Direct Neon / 512 | Release | 1292 | 1292 | 1292 | 0 | 0 | 0 | passed |
| Direct Neon / 512 | Debug / strict UBSan | 1292 | 1292 | 1292 | 0 | 0 | 0 | passed |
| **Total** | | **8036** | **8036** | **8036** | **0** | **0** | **0** | |

Total full-matrix CTest executions including prior frozen attempts:
**8,036**. Candidates 01–03 reached no CTest invocation; their failed
builds/setup diagnostics are retained separately. Unfrozen focused development
checks are not added to the final matrix count. Each passing separate example
printed `2 5 10 17`. See the [complete case inventory](arch-separation-m2-2026-11-18/test-inventory.json).

## Controls, flags and integrity

- All 28 protected root files and all 28 vendor files match the original pin
  exactly; no newline equivalence is accepted. The pin remains
  `667577495efaa36a10cbb5d6cb8c942c23913848`, reconstructed vendor tree
  `36aee120c28c9c64e62903bfc81139003e37cedd`. Pin bytes are unchanged.
  Before/after snapshots in primary, implementation and frozen capture retain
  exact vendor/pin bytes and mtimes; restored root originals remain exact in
  the implementation and capture. Primary's pre-migration source was not edited.
- Eight isolated integrity controls passed, including vendor corruption, extra
  vendor files, changed root bytes, a newline-only root change, ARM include
  leakage, active retired outputs and rejecting the old write mode. Deliberate
  mutations affect only disposable copies under ignored `build/`.
- Eight source-capture controls passed: complete fixture acceptance, changed
  bytes/newlines, missing/extra files, wrong fingerprint, duplicate paths and
  traversal rejection. The default dirty-checkout rejection remains active.
- Include traces prove ARM reaches its family rather than protected x86 headers.
  Four namespace-isolated facade consumers compile/run from an include-only
  installation with **no original x86 files present**. Direct Neon traces contain
  no compatibility header or sse2neon. Six architecture/override rejection
  controls passed. Native Windows x86 traces/output/code comparison remain pending.
- Native executable SHA256/architecture rechecks covered **7,999**
  test/example/probe records. No captured source changed during validation.
- Strict Debug retains `-fsanitize=undefined`,
  `-fno-sanitize-recover=undefined`, `-fno-omit-frame-pointer` and
  `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`.
  UBSan positive exit: **0**; intentional signed-overflow
  exit: **-6**, with expected diagnostic and no continuation.
- Writable-page control exit: **0**. Four intentional
  boundary read/write faults exited **-10, -10, -10, -10**.
  Expected negative controls are separate from unexpected suite failures.
- Numerical gates, seeds, reference generators/fixtures, exceptional inputs,
  partial counts, canaries and protected-memory helper remain unchanged.
  C++ regression bodies differ only by include routing. The original scalar
  oracle adaptation function is unchanged; generated version checks preserve
  constructor case 650 rather than removing its assertions.
- Ordinary FP retains `-ffp-contract=off` and selected `-frounding-math`.
  `-ffinite-math-only` remains restricted to the classification stress target;
  no broad fast-math option was introduced. Actual compile commands are archived.
- Correctness deadlines remain **60 seconds**, with build/test **concurrency three**.

## Failed attempts and corrections

This is not a pristine first-attempt migration. [All attempts](arch-separation-m2-2026-11-18/attempts.json)
retain their own source fingerprints, patches, archives and diagnoses:

1. Candidate 01 was captured but rejected before native execution: the existing
   root sse2neon alias had LF bytes while its retained dependency had CRLF.
   In-memory hashes proved a newline-only difference. Candidate 02 copies exact
   dependency bytes into the alias and protects it from checkout conversion.
   No vendor file or numerical implementation was changed.
2. Candidate 02 passed preflight/include controls, then compatibility Release
   build failed because hyperbolic targets still compiled pristine external
   `testbench3.cpp` directly. That source included restored original x86 headers
   on ARM. The failed snapshot was kept, the subsequent build was interrupted,
   and post-interruption source/vendor/root checks passed. Candidate 03 uses an
   include-adapted build copy and rejects direct compilation of external pinned
   CPP inputs before a build. Focused reproducing targets passed.
3. Candidate 03 next failed to build constructor case 650: the original testbench
   conditions its body on `VECTORCLASS_H`, now a distinct ARM guard. It was kept
   and stopped before CTest. Candidate 04 accepts the equivalent original-or-ARM
   version guard only in generated copies; focused integer/FP constructor cases
   passed. Every original reference/assertion and registered case remains.
4. Candidate 04 is the final frozen attempt summarized above. No older passes
   were attached to it. No source change was made to its capture during execution.

Earlier unfrozen preparation diagnostics (text decoding and compile-probe include
routing) are retained under `build/` too. The external pristine testbench itself
was never edited; exact source hashes and cleanliness were verified again.

All eight profiles and eight examples pass at candidate 04, with no failed,
skipped or timed-out CTest instances in this final complete execution.

## Evidence and Windows handoff

| Evidence | Host-local ignored location | SHA256 |
| --- | --- | --- |
| Complete candidate-04 source ZIP | `build/arch-separation-m2-20261118T194215Z/candidate-04/captured-source.zip` | `28537e29282cd694576fe93cfca2e5fee856f5b11e3305e0641f8eb155648805` |
| Tracked patch + added-file inventory | `build/arch-separation-m2-20261118T194215Z/candidate-04/implementation.patch` plus `capture.json`/source ZIP | `e740aaa20b9d4337f035aba689765969ef19498806abff526a94e3c69a553f45` |
| Final native packet | `build/arch-separation-m2-20261118T194215Z/candidate-04/source/build/mac-m2-native-20261118T200822553678Z/return-evidence.zip` | `395a7997d950ee0e2d75c1bc2d40c510924a0255389c4ba5807a5fa3b2ee9569` |
| Combined all-attempt evidence | `build/arch-separation-m2-20261118T194215Z/arch-separation-m2-evidence.zip` | `499636b722fd27e92f90856d005d39682c2ed9938d23dded2d6e335ec839cfa9` |

The combined packet verifies **12,607 entries**, retaining all
four complete source archives/patches, failed/final native packets, source and
compiler/flag/binary inventories, command/exit journals, JUnit/logs, generation
fixtures and generated C++/CMake/reference/probe inputs. CRC/per-entry digests
and original packet/source hashes were verified. All original raw directories,
binaries and attempts remain; no evidence cleanup was performed. This Mac task
does not claim to inspect Windows-local raw archives described in historical
reports; their portable records and original source identifiers are preserved.

[Portable metadata](arch-separation-m2-2026-11-18/README.md) has a
[checksum inventory](arch-separation-m2-2026-11-18/checksums.sha256) and contains no absolute machine
paths or binaries. Transfer raw archives separately. The
[Windows return handoff](windows-arch-separation-handoff.md) identifies these same
implementation bytes and concrete native commands. Publication requires separate
review/approval; a later published source must prove its relation to this capture.

## Remaining scope and repository state

Native MSVC/GNU-driver Clang Windows baseline and supported higher-ISA checks,
x86 include isolation and facade/direct-original output/generated-code comparison
remain pending. Original upstream behavior and known inherited boundary/UBSan/
numerical findings must be reported as actual failures, not patched or hidden.
No old x86 repair/optimization campaign is resumed by this handoff.

Direct Neon remains experimental. Other CPUs/toolchains, physical Intel,
unselected APIs, native FP16/SVML, ARM64EC, ASan and application integration are
outside this selected matrix. ARM exception flags/masks remain unemulated.
Universal Apple runtime selection is implemented but no multi-slice build or
universal binary execution was performed. Existing external ARM clients using original unsuffixed headers need
explicit migration to the facade in their own repositories; none were modified.
No benchmark timing, Rosetta/QEMU/WSL run, new shared corpus capture or universal
cross-host bitwise-equivalence claim was made.

Implementation, portable results and current-status edits remain uncommitted on
the topic worktree. Primary `master`, remotes, previous branches/worktrees,
legacy checkout and evidence are preserved. No commit, push, merge, PR, branch
retirement, consumer edit, upstream contact or evidence cleanup was performed.
