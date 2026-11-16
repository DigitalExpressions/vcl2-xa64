# MSVC x86 optimization and pristine-source separation, 2026-11-16

The two targeted MSVC workloads improved substantially: approximately **9.2x**
for AVX-512 FP64 classification/feedback and **10x** for AVX-512 FP32 exponential
relative to the pre-change fork. The source layout now keeps original upstream
untouched behind explicit adaptation patches and canonical backend sources.
All 35 shipped runtime files reproduce the optimized frozen bytes exactly;
applications keep direct header-only usage without generation.

These are named workload results, not a whole-library speedup or a clean
higher-ISA correctness claim. The [native report](../validation/windows-amd-2026-11-16-x86-layout.md)
records 5,392 passing baseline case instances and separately failing higher-ISA
selections that also fail on the pre-change runtime, including an existing UBSan
conversion defect. Native M2 remains pending.

## Source and implementation

Candidate: base `44f974ac31e1448e0beb5157d2bd8d669f5c518e`, tree
`77f68cb6590ed95d6a812d7dff3436aff8f73b66`, plus patch
`b954c2894ecac21ad59b3ca3d9ff10401f01fb8ac5552e96eeb4dc300c5855e4`.
The exact source fingerprint is
`1da758d8e17b4f50b60fca687a5dae2f52065b317aa71936f72466ae7375fc45`.
No candidate commit was created: portable metadata says `commit: null` and
records base commit/tree, patch, patched archive and source inventory separately.

The first experiment compares against unmodified fork `44f974a`; the second
against original VCL `667577495efaa36a10cbb5d6cb8c942c23913848` (tree
`36aee120c28c9c64e62903bfc81139003e37cedd`). The pre-change fork's runtime code
matches the older `ade9d9b` benchmark checkpoint; intervening commits are docs.
All source pairs were freshly compiled/measured. Historical timings are not
pooled with this run.

- Native MSVC classification now uses direct integer intrinsics in private
  `backends/x86/x86_classification.h`, reproduced at the original root path.
  It preserves finite-math classification and the wide FPCLASS workaround.
  SSE2 double NaN masks use a bounded integer subtraction/sign mask; SSE4.2 and
  AVX-512 use native comparisons. ARM and other compilers keep their own paths.
- MSVC x86 keeps the existing FP32 exponential template inline. Its polynomial,
  coefficient expressions, operation grouping, explicit FMA and special-value
  policy are unchanged. Assembly confirms the wide array kernel no longer calls
  the outlined `exp_f` helper; coefficient setup is shared across the loop.
- Existing large-value rounding fixes and conversion sentinel policies remain.
  The shared classification test additionally checks mixed lanes and deterministic
  integer-bit references. No oracle, tolerance, fixture or numerical policy was
  weakened.

The maintainer clarified the desired pristine-source architecture during this
work. [The final layout](../backend-layout.md) adds canonical vendor/adaptation/
backend sources and integrity tooling, with unchanged compiled products. Its
[separate provenance](../validation/x86-optimization-2026-11-16/source-layout.json)
does not relabel the pre-layout whole source tree as a later tested commit.

## Measurement protocol and counts

Native Ryzen 9 7950X / Windows 11 Pro 10.0.26200; MSVC 19.50.35723 first,
GNU-driver Clang 20.1.8 second. Each experiment covers SSE2, SSE4.2, AVX2 and
AVX-512F/DQ/BW/VL with logical widths 128/256/512. CPU/OS probes establish native
ISA support. Optimized O2, static release CRT, strict FP/no implicit contraction,
no LTO/sanitizers; same compiler flags per source pair. Measurement threads use
logical CPU 2; Balanced power settings are unchanged. Physical core/SMT activity,
frequency and temperature are not independently controlled.

The unchanged harness has 102 cases per compiler/ISA, four alternating paired
invocations, seven samples per case per invocation and six-millisecond warmup/
calibration targets. Each experiment has **816 comparisons, 45,696 samples,
1,632 passing preflight checks and 6,528 further invocation checks**. Both
experiments therefore retain 1,632 comparisons and 91,392 samples. Normal-input
output hashes match on both source pairs. Four intentional guard faults per
experiment are expected negative controls, not unexpected failures.

Ratios below one mean candidate faster. All-pair screening thresholds remain
1.05/0.95; these descriptive flags are not significance tests. Full per-case
sample/pair ranges and all slower rows remain in the portable tables.

| Comparison | Consistently faster | Consistently slower | Mixed / within band | Unstable both directions |
| --- | ---: | ---: | ---: | ---: |
| Optimized / pre-change fork | 27 | 4 | 784 | 1 |
| Optimized / original upstream | 16 | 75 | 725 | 0 |

The second row is not a claim that all upstream costs were removed. Rounding,
conversion and other existing correctness changes still have costs, and
remaining differences need workload-specific analysis. These measurements do
not establish Intel, Mac, fast-math or application-level performance.

## Targeted outcomes

| MSVC workload | Optimized / pre-change | Four-pair range | Optimized / original upstream |
| --- | ---: | --- | ---: |
| AVX-512 FP64 512-bit classification + feedback, four chains | 0.1085 | 0.1082-0.1090 | 1.0257 |
| AVX-512 FP32 512-bit exponential, 128 elements | 0.0995 | 0.0974-0.0996 | 0.2894 |
| SSE2 FP64 512-bit classification + feedback, four chains | 0.2059 | 0.2052-0.2061 | 1.0947 |

The largest classification gap shrank from the earlier roughly 9.4 ratio to
roughly 1.026 versus original upstream in this fresh comparison. Wide FP32
exponential is roughly 3.46x faster than original upstream in this workload.
Classification includes select/add feedback and exponential includes array
loads/stores; these are not isolated instruction-latency measurements.

All four Clang baseline/candidate kernel object files are identical after
excluding their COFF build timestamp (bytes 4-7); all 102
instruction listings per ISA match. Only the four timestamp bytes are ignored,
not code, constants or relocations. This supports the source-level compiler
isolation; it does not replace native ARM validation.

## Slower stream cases and layout diagnostic

The before/after comparison retains four slower MSVC stream cases:

| ISA / case | Candidate / pre-change | Pair range |
| --- | ---: | --- |
| SSE2 / FP32 128-bit, 128 elements | 1.3780 | 1.3477-1.4289 |
| SSE2 / FP32 512-bit, 128 elements | 1.0678 | 1.0661-1.1016 |
| SSE4.2 / FP32 512-bit, 128 elements | 1.0662 | 1.0631-1.0667 |
| AVX-512 / FP32 128-bit, 128 elements | 1.4248 | 1.4196-2.4634 |

The [independent verification](x86-optimization-2026-11-16/verification.json)
records their machine-byte equality and placement. A short follow-up relinks the same
MSVC SSE2 object files with unused code padding, leaving all kernel instructions
unchanged. The 59-byte FP32 128-bit stream function moves from RVA `0x8660`
(modulo 64 = 32) to `0x8680` (modulo 64 = 0); its median changes from about
0.421 to 0.301 ns/vector chunk. This demonstrates layout sensitivity for that
workload, rather than a changed arithmetic implementation. It does not explain
every slower row or establish one microarchitectural cause.

[Diagnostic data](x86-optimization-2026-11-16/alignment-diagnostic.json) uses one
invocation, three samples and two-millisecond targets per placement, with all
102 reference checks retained. It is kept separate from official comparison
samples. The first padding attempt did not move the function and was
uninformative; both attempts remain in raw evidence. No padding change was
introduced into the production library or original benchmark results.

## Correctness, attempts and evidence

Both screening steps are retained: direct classification alone improves that
workload but not wide exponential; the second adds inlining and improves both.
Screens use three pairs/three samples/two-millisecond targets and are not
pooled with final data. The native/higher-ISA failures and initial collector
stops are documented in the native report, not hidden as timing retries.

The final untimed edge follow-up retains 28,672 lane checks: candidate passes
all 14,336; original upstream fails 84 SSE2 rounding checks (42 per compiler).
Five IEEE classification probes pass both sources. Keeping the rounding fix
therefore preserves demonstrated correctness. Separately, four root-headers-only
consumer builds/runs (MSVC/Clang, SSE2/AVX-512, isolated namespace) pass with no
vendor/backend source directories or generation step in the application package.

An independent offline audit recalculates all 1,632 comparison ratios/statuses,
verifies all 91,392 samples and 16,320 normal-input checks, checks frozen bytes,
source-pair metadata, original compact packets and unchanged initial Git index.
A restored historical screen test was checked against its original archive;
the first audit rejection and correction are retained. Original benchmark
reports/manifests and their checksum inventories are unchanged.

A separately identified [source-export tooling follow-up](../validation/x86-optimization-2026-11-16/candidate-tooling-followup.json)
adds canonical backend/adaptation patch support and rejects stale generated
candidate products. Ten source-path controls, current-layout verification,
stale-product rejection and legacy-patch fingerprint reproduction pass. This
post-timing orchestration change does not alter the compiled C++ harness or
library, rerun timings, or replace the original measured runner hashes.

| Evidence | Ignored directory | SHA256 |
| --- | --- | --- |
| Before/after `evidence.zip` | `build/x86-optimization-20261116/04-before-after/` | `b5003c416ed5f09faf64ab069a5504948c8982553e3a1744dd30b7f4a1c07cfc` |
| Original-upstream `evidence.zip` | `build/x86-optimization-20261116/05-upstream/` | `a908ab86fba9ac4dff47f1b482486b7bc92437e5cdcb53cd5cedd5277c7d22d3` |
| Untimed `edge-evidence.zip` | `build/x86-upstream-edges-20261116T214624798808Z/` | `2a524a864208576219a36a0e94cded6a8ffe7227646869c32496e3a19579ff4a` |
| Source/probe/attempt supplement | `build/x86-optimization-20261116/compact-supplement.zip` | `10c4c5bd728594c0df46766f406df6f2516d403364c75ab598c01bf3371bb109` |

The supplement's 2,741 entries were checksummed and verified. It includes new
source inputs, generated probes/adaptors, failure records, canonical sources,
integrity controls and original compact packets. Rebuildable binaries remain
host-local with recorded hashes; no cleanup, commit, push, merge or PR occurred.

Use the [repeatable comparison instructions](x86-upstream-comparison.md) and
the [source-layout workflow](../backend-layout.md). All portable data is under
[this result directory](x86-optimization-2026-11-16/README.md); raw paths,
binaries and command logs remain under ignored `build/`. Native M2 validation
and higher-ISA correctness/oracle triage remain explicit follow-ups before a
broad portability/performance readiness claim.
