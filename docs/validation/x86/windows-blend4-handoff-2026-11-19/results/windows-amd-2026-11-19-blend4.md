# Native AMD Windows four-blend validation — 19 November 2026

## Separate assessments

**Full correctness gate: failed/incomplete.** The candidate registered **5,712**
instances; **5,636 executed: 5,387 passed and 249 failed**, with **76 not run**,
zero skipped and **eight timeouts included in failures**. All **12 separate
examples passed** with `2 5 10 17`. No failed or unavailable case was removed,
relabeled or replaced by a supplemental pass.

**Bounded x86 preservation / added-regression assessment: passed within the
selected checks.** All **96 supplemental cases passed** against candidate and
unchanged control, facade/direct-original routes, default/isolated namespaces,
both compilers/modes and SSE2/AVX2/AVX-512 profiles. All 96 selected-code
comparisons matched instruction bytes and normalized relocation target names,
covering **1,728 complete noinline blend-function pairs**. Whole-object identity
is not asserted. Pristine x86 inputs, protected files, controls and compile flags
were preserved. No unexplained candidate-specific regression was observed.

## Exact source and publication provenance

Approved Git publication: `0c69b9992d0fac09f71482a0db403276cdd8f1a1`, tree
`fd580a67ad4707116dbc7bf7b230050d635f8704`, transported on
`origin/validation/m2-blend4-windows`. Both publication and control were checked
out into fresh detached worktrees with command-scoped LF settings. All raw bytes
matched their exact Git blobs; no protected file was normalized or repaired.

| Input | Complete source identity |
| --- | --- |
| Executed Windows publication capture | 625 files; `735caf6d2ad31792c7fc215f2240187b55e768e272be7d0218df8a6c22ceeafb` |
| Current control, Git base `5e039b01b49abc00548123763be85898932e3ad8`, tree `ea90c1b40fc50936db7c3509aaa11ab51c02d890` | 564 files; `fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06` |
| Original M2-tested candidate, tested commit/tree null | 571 files; `3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36` |
| Historical Windows publication `2d251bc72bcc861edad371499fa732e8eb45a5f7` | 497 files; `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2` |

Every one of the 571 M2 paths was accounted for: five explicitly allowed
metadata changes and 54 closed-inventory additions explain the publication.
All other M2 bytes, including runtime/test/fixture/build/runner inputs, match.
Against the current control, only `vectori128_perm_neon.h` and
`tests/movement_slice.cpp` change implementation/test source; other shared
differences are metadata. The x86-visible source change is added test coverage.
The ARM header stays outside the x86 graph. Full inventories, source ZIP hashes
and comparisons are in [sources.json](sources.json).

All execution used immutable gitless captures and their explicit manifests,
with empty tracked patches and no added/deleted input paths. The runner records
tested commit/tree null under its generic captured-source schema; exact clean
Git bases and per-path hashes identify the executed bytes. Neither a later
publication nor this report acquires the earlier Mac native pass.

## Native host, tools and controls

Physical AMD Ryzen 9 7950X; Windows 11 Pro 10.0.26200; native x64 Python 3.11.
`IsWow64Process2` reports native `0x8664` / process 0; executed binary records
are x64 PE. Installed MSVC **19.50.35723** (toolset 14.50.35717), native GNU-driver
Clang **20.1.8**, target `x86_64-pc-windows-msvc`, CMake/CTest **4.2.1** and Ninja
**1.12.1** were reused. All ten comparable recorded tool/runtime hashes match
history; `dumpbin` has a newly recorded hash rather than a historical match.
The pristine testbench remained clean at
`a7293702385e5169a5f58b83177f2a43cd59139b`; all three pinned raw CPP hashes
matched before and after. No tool/dependency was installed or updated.

Independent CPUID and OS/XCR0 probes before higher execution confirmed AVX2/FMA
and AVX-512F/DQ/BW/VL; `XCR0=0xe7`. The original runtime detector reported level
11 and no ARM target. All **24 fresh control profiles** passed their conditions:
actual **4,096-byte pages**, writable exit zero and all four boundary read/write
exits `0xc0000005` (raw process return 3221225477). Six Clang Debug control
profiles passed positive zero and deliberate signed-overflow exit **1**, required
diagnostic and no continuation. MSVC Debug remains unsanitized.

Clang Debug used static release CRT `MultiThreaded`, disabled sanitizer recovery,
frame pointers and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. The known
LNK4217 import warnings remain in raw logs. Ordinary MSVC uses `/fp:strict`, with
`/fp:fast` only for classification stress. Clang retains no contraction, selected
rounding flags and classification-only finite-math stress. All **5,808** complete
compile records match historical non-path arguments, including ISA, optimization,
CRT, sanitizer, namespace, testcase and seed settings. Every registered case
kept its **60-second deadline**, and build/test concurrency stayed **three**.
All six configuration/reference fixtures passed in each profile: **72 instances**.

## Actual complete matrix

MSVC Release/Debug ran before Clang Release/strict UBSan Debug. The baseline is
**5,231 passed / 133 failed / 28 not run of 5,392**, plus four examples. Higher
profiles are **156 passed / 116 failed / 48 not run of 320**, plus eight examples.
Zero skipped cases; timeout counts below are included in failures.

| Profile / attempt | Registered | Passed | Failed | Not run | Timeout |
| --- | ---: | ---: | ---: | ---: | ---: |
| continuation01-clang-debug | 1348 | 1284 | 53 | 11 | 0 |
| continuation01-clang-release | 1348 | 1328 | 9 | 11 | 0 |
| continuation01-msvc-debug | 1348 | 1288 | 57 | 3 | 8 |
| continuation01-msvc-release | 1348 | 1331 | 14 | 3 | 0 |
| higher01-clang-debug-avx2 | 40 | 19 | 9 | 12 | 0 |
| higher01-clang-debug-avx512 | 40 | 19 | 11 | 10 | 0 |
| higher01-clang-release-avx2 | 40 | 24 | 4 | 12 | 0 |
| higher01-clang-release-avx512 | 40 | 19 | 11 | 10 | 0 |
| higher01-msvc-debug-avx2 | 40 | 22 | 16 | 2 | 0 |
| higher01-msvc-debug-avx512 | 40 | 16 | 24 | 0 | 0 |
| higher01-msvc-release-avx2 | 40 | 24 | 14 | 2 | 0 |
| higher01-msvc-release-avx512 | 40 | 13 | 27 | 0 | 0 |

All four original baseline runners stopped at inherited build failures before
suite execution. Their false summaries and original return ZIPs remain unchanged.
Separately journaled `continuation01` collections finished independent Ninja
edges with their actual nonzero build exits, then accounted for every registered
name in CTest/JUnit. Completed canonical cases were not replayed. Higher-profile
initial builds, aborts and independent collections are retained separately.
Unavailable executables are not executed/passed cases. The full movement targets
retain their inherited lookup compile/runtime failures; some never reach the
added call. The [complete case results](test-results.json) and
[325 nonpassing classifications](nonpassing-classification.json) preserve all names.
Historical family/source classifications do not declare every difference an
upstream defect or claim an individual new reproduction for all 325 instances.

## Historical comparison and changed failure modes

The historical gate stays **5,387 passed / 249 failed / 76 not run of 5,712**,
plus twelve examples, failed/incomplete; its **one** original timeout remains
unchanged in the old report. Complete per-profile/name inventories match, and
there are **zero pass/fail/not-run status changes**. This run has **eight**
timeouts: seven formerly failing MSVC Debug cases now timed out at 60 seconds.
Matching aggregate counts alone is not the preservation evidence.

The seven are `inverse_pi`, `hyperbolic_Vec2d_303`, `hyperbolic_Vec2d_304`,
`hyperbolic_Vec16f_300`, `hyperbolic_Vec16f_302`, `math_Vec2d_300` and
`math_Vec16f_300`. Fresh matched control builds ran only these selected cases:
all seven failed without timeout. Fourteen separate bounded debugger sessions
stopped candidate/control pairs at the same `_RTC_UninitUse` sites in byte-exact
original asin/asinh/acosh/sinh headers. This supports attribution to inherited
uninitialized-branch runtime checks. Exact wait stacks for each current full-run
timeout were not captured; the evidence does not assert every current timeout
was individually observed in a modal message box. Debugger exit zero means
diagnostic collection, **not a native test pass**. Native control failures and
candidate timeouts remain intact in [changed-timeouts.json](changed-timeouts.json).

There are 74 printed-output differences after source/run-path normalization:
twelve configuration records contain temporary/tool-path rendering and existing
JUnit truncation differences; 62 sanitizer records retain the same first
diagnostic with differing stack/address/path metadata. Raw output is preserved;
no numerical bits or stack addresses were masked. No universal bitwise program
output identity is claimed. Whole-line event parsing identifies **14 RTC
breakpoints and zero access-violation events** in the changed-case diagnostics;
earlier raw substring counters included echoed debugger commands and remain
preserved separately. No native result or run was rewritten.

## Exact added regression and pristine x86 path

The supplied emitter and CMake project were hash-verified. The exact frozen
scalar alias, CHECK macro, do_blend function and **62-line** regression/oracle
region were copied unchanged from test SHA256
`7ee48af6fa40cd0b52c5a5be979bdaed0632833ab8500128be69ce1162373f25`.
All `Vec4f`/`Vec4i`/`Vec4ui` tuples, both source orders, rotating edge bits and
fallback zero/don't-care patterns remain. The x86 executions check integer lane
bits; guarded ARM FPCR/FPSR assembly is not executed or emulated on Windows.

Twelve compiler/mode/ISA profiles times two sources times four route/namespace
targets give **96/96 supplemental passes**, zero failed, skipped, unavailable
or timed out. Outputs were `0 isolated transpose blend failures`. These validate
the independent scalar bit-index oracle, not agreement alone. They supplement
the full failed matrix and never turn unavailable/failed movement targets green.
The imported unbuilt legacy targets are not counted as supplemental passes.

Raw `/showIncludes` / `-H` traces show facade consumers use `vcl2_xa64.h`, neutral
policy and pristine original vector/math headers. Direct consumers use the
documented `vcl_config.h` shim plus vendor originals; they are not called
completely policy-free probes. All valid consumer graphs exclude `_neon.h`,
sse2neon, adaptation and retired backend helpers. Runtime controls separately
compile the pristine original detector. Raw objects, disassembly, selected
function payloads and relocations remain retained. The 96 code comparisons
match all 18 noinline helpers per pairing, after anonymous-namespace spelling
normalization for relocation names. Source/debug/checksum/diagnostic metadata
and whole objects are not equated by that selected-code assessment.

## Integrity, evidence, gaps and retention review

All **60** protected/dependency paths kept exact raw hashes, inventories and
nanosecond mtimes across primary, publication/control worktrees and both captures:
28 root originals, 28 vendor originals, both pins and sse2neon alias/vendor header.
Original pin remains `667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Complete captures reverified after
execution. Eleven header-integrity controls and eight capture controls passed;
all 43 preexisting portable checksum manifests passed. Retained fixtures, seeds,
canaries, page/UBSan controls and numerical gates were not weakened.

The unmodified configuration fixture removes its own TemporaryDirectory. Its
exact frozen generator, full five-contract results from all twelve LastTest logs
and six offline-reconstructed Windows input variants are retained. Those copies
are labeled reconstructed, not original filesystem artifacts; no fixture was
edited or native test replayed to produce them. JUnit's existing per-case output
truncation remains; complete LastTest and command logs accompany it.

Raw evidence: ignored `build/b4w19-01/`, run ID
`windows-blend4-20261119-b4w19-01`. Compact packet:
`windows-blend4-evidence02.zip`, SHA256 **`6057d1dda2f4b04942295ee9163afb6a555a088e9f55bf467131b0d67b80fd0c`**,
7,835 recorded files, CRC and every recorded SHA256 verified.
It records hashes for 20,671 compiled artifacts,
including 5,795 x64 PE executables.
Original failed runner archives, source ZIPs/manifests, all failed attempts,
commands/exits, flags, generated probe/config/reference inputs, JUnit, raw logs,
objects, binaries, PDBs and checked-in-source identities remain. Compiled files
are omitted only from the compact packet and remain on disk. Final report and
portable checks have their own checksums and a later review supplement.

The [portable bundle](README.md), [archive receipt](archive-receipt.json),
[source proof](sources.json), [control evidence](controls.json),
[per-case history](per-case-comparison.json) and [code audit](selected-code-comparison.json)
identify scope without machine paths or binaries. New report metadata was written
outside immutable captures after execution. Primary `master` remains clean at
`a04eb834c0e6707256009d007c77f4ccaf66beca`; only review documentation changes
exist in the detached publication worktree. Refs and saved configuration remain.
No source fix, benchmark, ASan, emulation, hosted CI, external consumer edit,
cleanup, commit, push or merge occurred.

Windows evidence provides no new x86 preservation reason to reject this
candidate. **Retain/reject remains a maintainer decision**, not an automatic
integration: the [matched M2 candidate report](../../../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md)
records **6.0%** FP32 AoS transpose throughput gain at 128 objects, **0.5%** at
4,096 with overlapping ranges, and **2.2% / 2.7%** TRS-inverse elapsed costs.
Those workload-specific gains/costs do not establish an overall win or any
Windows performance change. The [M2 correctness report](../../../arm64/mac-arm-2026-11-19-blend4.md)
still applies only to its 571-file capture. Other APIs/toolchains, physical Intel,
ASan, FP16/SVML, consumer integration, universal correctness and new cross-host
parity remain outside this evidence. Any source fix/revert or further publication
requires separately selected scope and a new frozen checkpoint as applicable.

## Packaging and portable review attempts

The first compact packet passed ZIP CRC but failed its entry-hash check for
`package.log`, which grew while being archived. That packet, manifest and log
remain retained: SHA256
`a131e8d0e6603b70dc84b776b1ad0b137b47ac4cdf56c48792a53b61bb0727bd`.
All its other recorded entry hashes matched. The second packet excludes its own
active log and passed CRC plus every recorded entry hash. No native case was
replayed. The completed compiled-hash inventory was retained and reused; all
compiled files remain present, with sizes rechecked for the second packet.

Two portable preparation checks rejected truncated Windows path fragments in
configuration JUnit text. Earlier generator versions and partial metadata remain
retained. Corrected redaction affects only portable path fragments; original
logs, numeric values, source bytes and native outcomes are unchanged. A checked
final-review supplement preserves late logs, metadata and document checks; its
receipt is in [final-review.json](final-review.json).
