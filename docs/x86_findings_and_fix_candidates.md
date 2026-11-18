# Retained x86 findings and fix candidates

Prepared 18 November 2026 for future x86 work and possible technical discussion
with Agner Fog. This is a preservation/review document, not an active repair
task or a message sent to upstream. The
[architecture separation plan](arch_separation_plan.md) restores original x86
files at root and keeps Neon adaptations separate. No fix below authorizes
editing those protected originals.

## Scope and strength of evidence

The inspected original is Agner Fog's VCL at commit
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`, retained byte-exact in
`vendor/vectorclass/`. Findings concern that pin and the specifically identified
fork snapshots. They do not establish the status of upstream's latest revision.

Execution was native AMD Ryzen 9 7950X / Windows 11 Pro 10.0.26200, MSVC
19.50.35723 and GNU-driver Clang 20.1.8. MSVC Debug is unsanitized; Clang Debug
uses strict nonrecovering UBSan. AVX-512F/DQ/BW/VL was available. Physical Intel
testing is omitted. No WSL/QEMU result is used as native proof.

Distinguish three kinds of finding:

- **Pinned-original evidence:** original source contains the identified expression;
  some findings also have direct pristine-upstream execution. State which.
- **Fork/checkpoint evidence:** a failure and repair were observed on the captured
  adapted source. Extract and reproduce against pristine upstream before calling
  it an upstream defect or a compiler bug.
- **Test/API assumptions:** an expectation or oracle construction needed correction.
  These are not automatically library defects.

## Original-source findings suitable for isolated upstream reproducers

### 1. SSE2 FP64 rounding changes already integral large values

In original `vectorf128.h`, the SSE2 `round(Vec2d)` fallback adds and subtracts
a signed `2^52` unconditionally. The retained edge probe includes exact integers
`4503599627370497`, `-4503599627370497` and `9007199254740991`; rounding these
must leave them unchanged under the tested nearest-even environment.

Direct comparisons against pristine upstream reproduce **84 failed SSE2 rounding
lanes, 42 per compiler**, in the final-source edge run. Both sources pass all
five classification probes. The fork passes all 14,336 edge lanes; upstream
passes 14,252. These failures are numerical, separate from timing variability.

Existing remedy: patch 0001 restricts the magic-number adjustment to finite
magnitudes below `2^52`, using a magnitude-bit mask. Preserve behavior for
signed zero, infinities and NaNs according to the intended upstream contract.
Suggested upstream change: a small SSE2-only guard with an independent boundary
regression, rather than importing the combined portability patch.

References: [edge probe](../benchmarks/x86_upstream/edge_probe.cpp),
[initial comparison](benchmarks/x86-upstream-2026-11-16-ade9d9b.md),
[optimization follow-up](benchmarks/x86-optimization-2026-11-16.md), and
[patch 0001](../adaptation/patches/0001-portability-and-correctness.patch).

### 2. Unchecked FP64-to-int64 conversion fallbacks

Original `vectorf256.h:2066`, `truncatei(Vec4d)`, casts stored doubles directly
to `int64_t` when the AVX-512DQ/VL hardware branch is unavailable. NaNs,
infinities and finite values outside `[-2^63, 2^63)` reach unchecked casts.
Original `vectorf512.h` has the analogous `truncatei(Vec8d)` fallback without DQ.
The 128-bit fallback also contains unchecked casts; patch 0001 guards that base.

The retained original/pre-fix AVX2 header has Git blob
`b6d8e4592624d3047e4c793a585cefc7f6b6e96b`. Native Clang UBSan reports the
AVX2 conversion at line 2066; the focused regression reproduces exit 1 with
positive `2^63`. A later pre-0004 level-9 probe also reproduces the analogous
AVX-512F fallback diagnostic at adapted `vectorf512.h:1325`. These focused
before-fix probes use retained pre-fix headers; do not relabel them as complete
pristine-upstream suite runs.

Existing remedy: guarded 128-bit conversion in patch 0001; patch 0003 composes
two guarded halves for AVX2; patch 0004 composes guarded halves for level 9.
The fork defines invalid results as `INT64_MIN`, while keeping valid truncation
toward zero. Native DQ conversion stays selected at level 10.

Suggested upstream change: first agree the invalid-input result and FP exception
contract. Guard the base fallback before reusing it at wider widths; applying
patch 0003 alone to pristine upstream is insufficient. Avoid treating a rounded
`double(INT64_MAX)` as a safe inclusive upper bound. Benchmark defined-input and
exceptional-input paths separately after correctness.

The [literal conversion regression](../tests/truncate_i64_regression.h) contains
22 IEEE-754 patterns, rotated through every lane under all four rounding modes;
it never uses an invalid scalar cast as its oracle. Cases include both zeros,
subnormals, fractions, both int64 boundaries, finite extremes, infinities and
signed quiet/signaling NaNs. The final 20 focused programs pass across both
compilers, both modes and selectors 2/6/8/9/10, including strict Clang UBSan.

References: [conversion triage](validation/windows-amd-2026-11-17-isa-triage.md),
[completion](validation/windows-amd-2026-11-17-correctness-completion.md),
[patch 0003](../adaptation/patches/0003-defined-avx2-i64-conversion.patch), and
[patch 0004](../adaptation/patches/0004-x86-correctness.patch).

### 3. Compact masks in signed-minimum constant division

The original `vectori128.h` signed-minimum-divisor special case constructs an
integer vector directly from a comparison mask and then ANDs with one. At
AVX-512 the mask can be a compact scalar bit field rather than broad lanes.
The native probe `[INT32_MIN, 0, INT32_MAX, -1] / const_int(INT32_MIN)` produced
`[1,1,1,1]`, expected `[1,0,0,0]`. The source expression is present in pristine
upstream; the executed probe was part of the fork triage.

Existing remedy in patch 0004: explicitly select per-lane one or zero with
`select(mask, Vec4i(1), Vec4i(0))`; apply the corresponding treatment to the
16-bit special case. Suggested upstream submission: isolate this expression,
verify compact and broad mask configurations, and keep a mixed-lane regression.
Do not confuse this library use of a mask with the separate faulty mask oracle
described below.

### 4. Table lookups can read beyond the logical table

Source inspection and native page-end failures identified these concrete paths:

- A 128-bit 16-bit-element lookup uses 32-bit AVX2 gathers; reading the last
  element can fetch two bytes beyond the table.
- Native 256-bit byte lookup loads 16 bytes even for `n=1`.
- Native 512-bit int32 lookup loads 16 lanes even for `n=1`.

The final focused before/after check reproduces the pre-0004 AVX2 page-end fault
(`0xc0000005`). The original expressions are identifiable in pinned headers;
the retained failure/fix matrix concerns adapted pre-fix headers, not a full
pristine-upstream boundary matrix. Check the documented upstream table-padding
contract before proposing any broader API claim.

Existing remedy in patch 0004: bounded partial loads; for byte/word gathers,
back up the read inside the table and extract the requested element. The fork
keeps exact-sized unpadded tables at inaccessible page boundaries. Suggested
upstream direction: bounded implementations or an explicit padding precondition,
with the chosen contract demonstrated by guard-page tests. Adding padding to
our existing tests would conceal the tested condition.

The [boundary regression](../tests/boundary_regression.h) preserves independent
lane literals and exact page-end allocations. Broader partial-read compiler
workarounds are separate from these source-level access widths.

## Other repairs retained for investigation, not blanket upstream claims

These were observed and repaired on the captured fork. Patch 0004 and the
completion report preserve the implementation; an isolated pristine-upstream
reproducer and contract review remain necessary before proposing each upstream.

| Area | Recorded behavior | Existing approach / future investigation |
| --- | --- | --- |
| Native integer composition | Full-width byte shifts and a repeated 16-bit blend mask failed lane expectations | Handle shifts at/above width explicitly; correct the second-half blend mask; preserve literal lane tests |
| FP absolute value | AVX-512 range-based abs can quiet signaling NaNs or retain their sign relative to the fork's bit-clear contract | Clear only the sign bit if that is the agreed API contract; distinguish bitwise behavior from permitted NaN quieting |
| Log special cases | AVX-512 NaN, zero/subnormal and domain handling differed from the fork's legacy policy | Review FPCLASS coverage and special-result precedence; patch uses mask `0xFF` and orders the negative/subnormal selections deliberately |
| Infinite-base power | `pow(-inf,-3)` lost the expected negative zero in selected paths | Construct the zero sign from original input sign and exponent parity; retain signed-zero regressions |
| FMA-enabled trig | Independent FP32 fixtures exceeded gate 3; observed normalized errors included 6, 25 and 44 | Correct quadrant/range reduction; the fork widens reduction to FP64, with a substantial measured cost; seek a narrower fast path plus accurate exceptional reduction only if it retains the gates |
| Finite-math classification | Clang AVX2 classification stress failed wide mixed-lane/NaN checks | Use integer-bit classification where the intended contract must survive finite-math flags; test that stress contract separately from ordinary FP flags |
| MSVC partial loads, level 9+ | Optimized code could violate exact-page boundaries | Fork uses bounded volatile byte copying via a temporary; isolate generated instructions and compiler version before proposing a workaround |
| MSVC compact classification | Optimized wide classification/feedback differed or compiled poorly | Integer intrinsic helpers in the x86 classification backend; preserve mixed lanes and compare generated code |
| MSVC conditional arithmetic | FMA-enabled inactive conditional-add signed zero, wide AVX2 FP32 power and tiny FP64 atan failed selected optimized cases | Patch uses explicit selection/arithmetic and native-width power composition; the unsuccessful tiny-atan shortcut was removed, not retained as a fix |

Do not identify any of these as a confirmed compiler defect solely because a
source rewrite passed. The retained compiler provenance explicitly excludes an
attempt labeled 19.44 that actually used 19.50; a verified 14.44-path diagnostic
is separate and is not a full matrix pass.

## Test corrections that must not be presented as upstream defects

- A test constructing `Vec4i(mask)` expected broad `[-1,0,...]` lanes. Compact
  masks instead supplied a scalar bit field. The test now expands with
  `select(mask,-1,0)` while preserving its literal lane expectations.
- Original native `Vec8d::round_to_int32` explicitly uses nearest+SAE. Its
  oracle now checks that behavior in all four FP environments; the distinct
  current-direction `roundi` API is checked separately where available.
- The composed-integer scalar oracle needed independently materialized volatile
  inputs to avoid reproduced MSVC optimizer coupling. This is oracle isolation,
  not a changed numerical tolerance or proof of a pristine-upstream defect.

Keep these corrections separate from runtime patches in any future submission.

## Performance lessons and fix costs

The first MSVC optimization isolated direct integer classification and FP32 exp
inlining. Relative to the pre-change fork, two AVX-512 workloads improved about
9.2x and 10x. The polynomial and gates were retained. These are local workload
improvements, not evidence that all x86 overhead was eliminated.

The final frozen comparison used fork
`ab5bb8754a33724e05a170fa2867475077efd023`, tree
`48376a53dfa5f5f2139651e3d44f5e1a9ed841b1`, against the original pin. Across
816 workload comparisons and 45,696 samples: 120 consistently slower, 13 faster,
666 mixed/within band and 17 unstable both directions. All ordinary references
and corresponding output hashes match. All slower rows remain retained.

| Selected workload | Fork/upstream elapsed-time ratio | Supported observation / next candidate |
| --- | ---: | --- |
| MSVC AVX-512 FP32 512-bit sine array | 6.5959 | FP64 widening plus an outlined helper are visible; isolate reduction and inlining costs while preserving accuracy |
| MSVC AVX2 composed FP64 512-bit truncate/back, four chains | 3.4206 | Guarded conversions and helper calls are visible; optimize defined conversion without restoring unchecked casts |
| MSVC SSE2 FP64 512-bit round/back, four chains | 1.4900 | Consider alongside demonstrated upstream large-integer errors, not as equal-contract timing |
| MSVC AVX-512 FP32 512-bit exponential array | 0.2400 | Fork inlining avoids upstream's outlined helper in this workload; possible independent MSVC optimization |

Ratios above one mean slower. The four-pair screening band was 0.95/1.05,
not a statistical significance test. Some byte-identical arithmetic stream
kernels changed speed with function placement in a separate diagnostic.
Instruction count alone, averaging all cases, or removing slower rows cannot
establish equivalent performance. Layout, SMT, frequency and temperature were
not fully controlled. No matched ARM-versus-x86 performance claim follows.

Original optimization details remain in the
[dated report](benchmarks/x86-optimization-2026-11-16.md) and
[patch 0002](../adaptation/patches/0002-msvc-x86-performance.patch).
The final report and all portable tables are archived as described below;
this document does not restore or rewrite that historical report.

## Exact checkpoints and evidence recovery

The final Windows correctness run used base
`e27134bd69d061a78a9e4e0d4b5df260831690b9`, tree
`9b3ea5a74898f34e3bdd94141788a87e8f019d3d`, plus captured uncommitted changes.
Its original 402-path fingerprint is
`af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528`;
35-product fingerprint is
`6a108894ee1dacea0199df9f25f6a8b79b5828e03616ea0c6a1c16c53ae66580`.
Do not replace these with the clean base or later publication identifiers.

At that source, 5,392 baseline plus 320 local higher-ISA CTest instances passed:
5,712 total, zero failures/skips/timeouts, plus 20 focused programs and 12 examples.
All 124 earlier higher-ISA failures were resolved in the selected final matrix.
Six intermediate checkpoints and 197 intermediate CTest failures remain recorded.
Higher-ISA coverage is 40 local cases per profile, not all 1,348 baseline cases
at every ISA. Level 9 has focused fallback coverage, not a full local matrix.

Use the [completion report](validation/windows-amd-2026-11-17-correctness-completion.md)
and [portable evidence](validation/windows-correctness-2026-11-17/README.md)
for source inventories, attempts, flags, controls and receipts. The
[original triage](validation/windows-amd-2026-11-17-isa-triage.md) remains unchanged.
The M2 pass at `ab5bb87` is a separate native result; it is not pristine-x86 proof.

Following the approved cleanup, old raw paths in historical reports are logical
provenance paths, not necessarily existing directories. Current Windows retention
root: `build/retained-evidence/arm-strategy-20261118/`.

- `original-archives.json` maps 212 original archive paths to byte-exact retained
  copies under `original-archives/`. Find a report's packet there before rebuilding.
- `validation-records.zip` retains raw records, commands, diagnostics, manifests,
  samples, probe sources and executable-hash inventories. SHA256:
  `327a227bd142e43d9e6a733b2474d6b610a4a7123564b943e0768338f68e91d8`.
- `worktree-source-snapshots.zip` retains removed worktrees' exact source bytes,
  dirty patches and original heads/trees. SHA256:
  `9493f8bfb34cad864767ddce931a4e0b0b828043f9a5bc34c27fa54c04ee7c4f`.
- `pre-cleanup-documentation.zip` includes the original detailed x86 plans and
  `docs/benchmarks/windows-final-source-2026-11-17-ab5bb87.md`, plus its portable
  dataset `docs/benchmarks/windows-final-correctness-performance-ab5bb875-20261117T180305387696Z/`.
  SHA256: `28ca7403b982a2f816b9fef27769ee35ae8bccc9e6a8cd94bc437df0dea84256`.

Useful original packet hashes, located through `original-archives.json`:

| Packet | SHA256 |
| --- | --- |
| Correctness `correctness-evidence-supplement.zip` | `ea7ce958a81ff8e6245cf656750a6f480d68038e73bd1fdc62f6c5fd2d045e2f` |
| Final performance `evidence.zip`, run suffix `20261117T180305387696Z` | `eb743198e941e1bcea9e9c4b703a2e42f88007f3f04928a270f8e32253273149` |
| Final untimed `edge-evidence.zip`, run suffix `20261117T181416872522Z` | `986b0989035b238fe4a11fd246f628685851373242108dfc41a7d2302c2057d7` |

See the [cleanup report](validation/arm-strategy-cleanup-2026-11-18.md) for the
complete receipt and limitations. Rebuildable executables/worktrees were removed;
the archives are evidence, not an executable backup. Raw Windows archives remain
local and are not transferred by a Git commit. Mac raw evidence stays on the Mac.

## Assets to retain before architecture migration

The current source checkpoint `e41b0776d9e1d8ba51576bf2a17529a013753900` retains
the canonical patches and tooling in Git. Keep these reachable even if a later
separation retires their active use:

- Patches 0001 through 0004 under `adaptation/patches/`; 0001 mixes portability
  and fixes and must be decomposed for a small upstream proposal.
- `backends/x86/x86_classification.h`, the independent conversion/boundary
  regressions, fixture generators, numerical policies and mask/oracle corrections.
- `benchmarks/x86_upstream/`, `tools/check_x86_upstream_edges.py`, the comparison
  runners and [repeatable sampling protocol](benchmarks/x86-upstream-comparison.md).
- Dated reports and published portable evidence with their existing checksums;
  the retained raw/archive/source receipts above and original failed attempts.

Do not blindly apply old patches to restored root headers. Keep future research
in a separately approved experimental source/export, or update protected files
only as part of an explicitly approved new upstream pin after upstream accepts
a change. The original root/vendor copies remain untouched by Neon work.

## Preparing a future upstream discussion

1. Verify whether each issue still exists in the then-current upstream revision;
   retain the old pin for reproduction and identify the new revision separately.
2. Start with one small issue: SSE2 large-value rounding, unchecked conversion,
   or compact-mask division. Build a standalone upstream-only reproducer under
   ignored `build/`, with explicit inputs, independent expected output and flags.
   Extract boundary cases separately and state any padding precondition.
3. Reproduce before changing code, then test the proposed isolated fix. Record
   compiler/CPU/OS, ISA selector, exact source hashes, observed output and contract.
   Preserve strict UBSan controls, actual-page controls and all four rounding
   modes where relevant. Keep correctness deadlines 60 seconds and concurrency
   three. MSVC Debug alone is not sanitizer evidence.
4. Run the relevant native checks and full baseline profiles after runtime
   changes. Label fork-only gates, upstream behavior and test corrections
   separately; do not suppress inherited failures or weaken numerical gates.
5. Only if timing is separately authorized, follow the retained four-pair,
   seven-trial, six-millisecond protocol with matched flags, sequential processes,
   original 180-second performance-command deadlines, untimed edge checks and all
   samples/slower rows retained. Correctness-suite deadlines remain 60 seconds.
6. Prepare a concise technical note containing the minimal reproducer, exact
   upstream revision, evidence, proposed contract and small patch. Ask the
   maintainer to review before contacting Agner Fog or publishing anything.

No native test, benchmark, runtime repair, upstream contact, commit or push was
performed to prepare this document. It preserves prior work and labels candidate
directions; those directions are not newly validated fixes.
