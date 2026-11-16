# Windows AMD x86 upstream comparison, 2026-11-16

The fork changes x86 code generation and has repeatable slowdowns in selected
workloads. The largest measured differences concern classification; rounding
and integer conversion also have costs. Some costs accompany correctness fixes:
the untimed probe finds upstream SSE2 rounding failures that the fork avoids.
This is not a claim that every slower case has a proven cause, or that the
entire fork has any single overall slowdown.

## Frozen sources

| Source | Commit | Tree |
| --- | --- | --- |
| Fork | `ade9d9baf559ad480289277dd6ad8e6d1c5b15e3` | `85c2b81ce71bde49b5c697775531956cba27ff88` |
| Original VCL | `667577495efaa36a10cbb5d6cb8c942c23913848` | `36aee120c28c9c64e62903bfc81139003e37cedd` |

The checkout started clean on `master`, matching `origin/master`. No applicable
`AGENTS.md` was found. `git fetch --no-tags upstream master` preserved local
branches and confirmed the original VCL revision remains upstream `master`.
Both libraries were exported from Git into fresh ignored directories; new
uncommitted harness/documentation files were not included in either library
snapshot. The harness/runner bytes have separate SHA256 inventories.

Recorded Git-export whole-file fingerprints (path, NUL, SHA256, newline in
the saved inventory's Windows path-sort order): fork
`d4e62f8c2c08e219e9534c26f0122ffbcfbc65c579c771240dfb0b95f8eb78ea`;
upstream `b9fc80c216272c8e06431f9da7523c3ce03f1064a01f34cd9f7ff9216c46869a`.
These cover different whole repositories and are not the earlier SIMD-parity
source inventories. Git-export bytes also avoid checkout line-ending ambiguity.

## Host and protocol

Native AMD Ryzen 9 7950X, 16 cores/32 logical processors, Windows 11 Pro
10.0.26200. Native x64 Python 3.11 and x64 PE binaries were checked; no QEMU,
WSL or translated execution was used. MSVC 19.50.35723 ran first, followed by
GNU-driver Clang 20.1.8 targeting `x86_64-pc-windows-msvc`. Both used the static
release CRT, optimized O2, strict floating point, no implicit contraction,
no LTO and no sanitizers. MSVC Debug and UBSan results from other reports are
not performance measurements from this run.

CPU/OS probes reported SSE2, SSE4.2, AVX2/FMA/F16C and AVX-512F/DQ/BW/VL;
XCR0 was `0xe7`. Every profile enabled logical maximum 512 and the x86 backend.
Measurement threads were pinned to logical CPU 2, without exclusive physical
core/SMT isolation. The active power scheme was Balanced and was not changed.
Background activity, clock frequency and temperature were not independently
controlled or sampled; variation must remain visible.

See the [repeatable protocol](x86-upstream-comparison.md) for workload definitions,
units, correctness gates, affinity, compiler flags, guard controls and rerun
commands. The final run uses four alternating pairs, seven trials per case per
invocation and six-millisecond calibration targets. The 102 cases cover
128/256/512-bit FP32/FP64 arithmetic, rounding, classification, FP64 integer
conversion, integer division/lookup and small/cache-resident array workloads.
These synthetic workloads do not represent a complete application or API.

## Attempts and evidence

1. Preparation at `build/x86-upstream-20261116T200238380359Z/`: all eight MSVC
   builds passed their 102 normal-input checks. The first Clang dynamic-CRT
   link failed with mixed CRT libraries/unresolved imports. No timing started.
   Both toolchains were changed to the static release CRT for subsequent runs.
   Its retained `preparation-evidence.zip` SHA256 is
   `6c29c504d93bb8eda8d58b8dd2787701c32938125db6289c0755d93073a56125`.
2. Preliminary run at `build/x86-upstream-20261116T200347903568Z/`: 816 comparisons,
   1,632 preflight checks, 34,272 timing samples, zero output-hash disagreements.
   MSVC warned that `/Qvec-` was ignored. Retained for transparency, superseded
   by explicit scalar-loop controls and a complete new run. It is not pooled
   with final samples. Its raw `evidence.zip` SHA256 is
   `0207b626b9b364b383d8319413531639d8b345e6504f89d5c95ed4dfdcb3e001`.
3. Final run at `build/x86-upstream-20261116T200841757885Z/`: uses recognized
   MSVC `loop(no_vector)` controls, vectorizer diagnostics and rejection of
   ignored compiler options. The timed kernel source is unchanged from the
   preliminary run. All 16 binaries passed their preflight checks, with no
   compiler warnings, build failures, skips, timeouts or automatic retries.

## Final results

There are **816 case/profile comparisons**, 102 for each compiler/ISA profile.
All **1,632 preflight case instances passed**. Each of 64 timing invocations
also rechecked all 102 cases (6,528 further checks); all passed. All corresponding
upstream/fork normal-input output hashes match. The run retains **45,696 timing
samples**, 28 per source for each comparison, and four expected guard-control
access violations. The guard faults are negative controls, not unexpected test
failures. No timings are pooled with the preliminary attempt.

A separate offline audit independently recalculated all 816 ratios/statuses
from invocation logs, reconciled all 8,160 preflight/invocation checks, verified
identical source-pair compiler flags and one input hash, checked all 16 PE
binaries and unchanged source/harness bytes, and verified 2,434 checksum entries
inside the four raw archives. Its [portable verification record](x86-upstream-2026-11-16-ade9d9b-final/verification.json)
and private script remain separate from measurement. Raw audit work is under
`build/x86-upstream-review-20261116/`.

| Compiler | ISA | All pairs >5% slower | All pairs >5% faster | Mixed / within band | Unstable both directions |
| --- | --- | ---: | ---: | ---: | ---: |
| MSVC | SSE2 | 21 | 2 | 78 | 1 |
| MSVC | SSE4.2 | 15 | 2 | 85 | 0 |
| MSVC | AVX2 | 4 | 1 | 97 | 0 |
| MSVC | AVX-512 | 4 | 0 | 98 | 0 |
| Clang | SSE2 | 20 | 1 | 81 | 0 |
| Clang | SSE4.2 | 10 | 0 | 86 | 6 |
| Clang | AVX2 | 3 | 0 | 96 | 3 |
| Clang | AVX-512 | 0 | 0 | 102 | 0 |
| Total | | **77** | **6** | **723** | **10** |

These are descriptive screening flags based on *all four paired medians*,
not formal significance tests. Mixed/within-band does not establish performance
equivalence. Ten cases had pair ratios spanning both below 0.95 and above 1.05;
they are explicitly unstable. Other cases can also have wide ranges, including
Clang SSE4.2 truncation, and should not be summarized by a median alone.

Selected observations, with full results including every faster/slower case in
[CSV](x86-upstream-2026-11-16-ade9d9b-final/results.csv) and
[JSON](x86-upstream-2026-11-16-ade9d9b-final/results.json):

| Compiler / ISA | Workload | Median fork/upstream | Pair-ratio range |
| --- | --- | ---: | --- |
| MSVC / AVX-512 | FP64 512-bit classification + feedback, four chains | 9.419 | 9.395–9.512 |
| MSVC / SSE4.2 | FP64 512-bit classification + feedback, four chains | 5.371 | 5.365–5.393 |
| MSVC / AVX-512 | FP32 512-bit exponential, 128 elements | 2.910 | 2.892–2.946 |
| MSVC / SSE2 | FP64 128-bit rounding + feedback, one chain | 1.183 | 1.183–1.185 |
| Clang / SSE2 | FP64 512-bit rounding + feedback, one chain | 1.296 | 1.285–1.309 |
| Clang / SSE2 | FP64 256-bit truncation/back + feedback, four chains | 1.318 | 1.318–1.324 |
| MSVC / AVX2 | Integer 512-bit lookup dependency chain | 0.903 | 0.903–0.910 |

Each ratio applies to the named synthetic workload, not to all uses of that
function or to a complete application. Classification includes select/add
feedback; rounding includes an addition; truncation includes conversion back
to double. Timings include the function call and final stores. There is no
unsupported claim that the whole library is 9.4 times slower.

## Assembly and correctness interpretation

- **SSE2 double rounding:** the MSVC fork loop has additional magnitude masking,
  `pcmpgtd` and `pshufd` work before the add/subtract rounding sequence. The named
  entry point contains 31 static instructions versus upstream's 17, including
  prologue/epilogue. The independent edge probe shows exactly why removing this
  check is unacceptable: each upstream SSE2 compiler profile fails 42 lane checks
  involving already-integral large doubles; both fork profiles pass. For example,
  `4503599627370497` must remain unchanged. The observed rounding cost accompanies
  a demonstrated correctness improvement.
- **MSVC classification:** the fork uses integer-encoding classification in
  places where upstream uses FP comparisons/FPCLASS. The 512-bit header documents
  a workaround for MSVC narrowing FPCLASS memory broadcasts. In the FP64 AVX-512
  kernel, upstream uses `vfpclasspd`; the fork emits integer shifts/comparisons
  and additional stack traffic. This gives a concrete code-generation difference
  associated with the large timing gap. This probe shape passes classification
  on both sources; it does not reproduce or invalidate the historical compiler
  defect. The workaround must retain its original regression coverage when
  optimized.
- **Integer conversion:** fork `truncatei` fallback range checks replace undefined
  out-of-range scalar casts with a defined signed-minimum sentinel. Clang SSE2
  FP64 256-bit throughput has 216 named-entry instructions versus 105 upstream,
  and a tightly repeated approximately 32% workload cost. Normal timing inputs
  are in range. Undefined upstream out-of-range casts are not used as an oracle.
- **Wide exponential:** the approximately 2.91 ratio is repeatable in this MSVC
  AVX-512 workload, but its exact cause is not isolated. The named wrappers both
  have 31 instructions and call helpers; wrapper counts cannot explain the
  difference. Review called helpers and transitive classification/conversion
  changes before attributing this result to a particular source edit.
- **Unchanged-looking entries:** equal instruction/mnemonic counts are not proof
  of identical machine code, constants, helper bodies, placement or timing.
  Full disassembly and relocations for every object are retained. No instruction
  count is presented as a cycle estimate.

The separate [edge results](x86-upstream-2026-11-16-ade9d9b-final/edge-controls/edge-results.json)
contain **28,672 lane checks** across 16 binaries: 14,336/14,336 fork passes;
14,252/14,336 upstream passes, with all 84 failures confined to SSE2 rounding
(42 MSVC and 42 Clang). Five IEEE classification operations pass all probe
patterns on both sources. Both MSVC and Clang also configure, build and run the
frozen fork's CMake arithmetic example, producing `2 5 10 17`. These observations
are untimed and excluded from the normal-input benchmark pass totals.

## Reproduction, evidence and remaining work

Use the [repeatable instructions](x86-upstream-comparison.md), selecting
`-Pairs 4 -Trials 7 -SampleMs 6 -Cpu 2` and the two full source SHAs above.
Run the separate edge follow-up against the resulting raw directory. The
[portable metadata](x86-upstream-2026-11-16-ade9d9b-final/metadata.json),
[summary](x86-upstream-2026-11-16-ade9d9b-final/summary.json) and
[checksums](x86-upstream-2026-11-16-ade9d9b-final/checksums.sha256) identify the
exact harness, binary hashes, source trees, profiles and exported result bytes.

| Evidence | Host-local ignored directory | Archive SHA256 |
| --- | --- | --- |
| Final benchmark, `evidence.zip` | `build/x86-upstream-20261116T200841757885Z/` | `0046685fccb4db81b710f73af9a3398b25be84bb5e6c0c209e605ec441b2e097` |
| Untimed follow-up, `edge-evidence.zip` | `build/x86-upstream-edges-20261116T201744673430Z/` | `0229e2745e6717ea03411e9c0cd293368790eccb29bda757c783e63fe5146753` |
| Independent audit, `review-evidence.zip` | `build/x86-upstream-review-20261116/` | `798d139721b742d6e10d35b5483801d87d2c06a245b69c91b6493cd3be764ad5` |

Raw archives retain source exports/manifests, exact commands/exits, compiler
diagnostics, binaries/objects, disassembly, preflight checks and every sample.
Preliminary data and the failed preparation are retained separately under
`build/`; portable final tables contain no absolute machine paths, binaries or
raw command logs. Nothing in historical validation or ARM benchmark results is
overwritten.

The next optimization investigation should prioritize MSVC classification and
wide exponential helper code, then conversion/rounding costs at baseline ISAs.
Preserve edge-case behavior and original guard/sanitizer/numerical controls.
Use isolated candidate changes and repeat these same comparisons before
claiming an improvement. No library optimization is performed in this task.

Coverage gaps remain: physical Intel, AMD-vs-M2 performance, other compilers and
flags, fast-math, Debug/sanitized performance, FP16, the full VCL API, DRAM-sized
working sets and complete application workloads. The host is not an isolated
benchmark system and no cycle-counter/frequency/thermal attribution was made.

The existing library, numerical policies, fixtures, correctness tests and
guard-page tests are unchanged. The diff between frozen parity source
`bd7807e` and this fork commit contains no headers, C++ sources, root CMake,
tests or fixtures; older native validation retains its original source IDs
and is not relabeled as execution at this commit. This comparison adds its
own normal-input checks and separate untimed edge observations.

No commits, pushes, merges or branch changes were performed.
