# Standard vector and matrix benchmarks - 2026-11-19

**Scalar FP32/FP64 versus <span style="color: #bc4c00;">sse2neon</span> versus <span style="color: #6f42c1;">Direct&nbsp;Neon</span>**:
ten operations, FP32/FP64, object and batch layouts, 128/4,096 objects and logical batch widths 128/256/512. The tables describe the native measurements from
**2026-11-19**, at source `5e039b0`.

**Run status:** passed: all scheduled measurements, independent references, input/canary controls and fresh benchmark correctness checks completed.

This is the fresh baseline control; the companion [report](STANDARD_MATH_RESULTS_2026-11-19.md) covers the candidate. Both sources were rebuilt and measured in this same session; no historical timing is reused.

Matched Direct Neon FP32 AoS transpose at 128 objects changed from **1.251 to 1.180 ns/op**, **+6.0% throughput** (5.6% less elapsed time), with disjoint invocation median ranges. At 4,096 objects it changed from **1.323 to 1.316 ns/op**, **+0.5% throughput**, with overlapping ranges.

The tradeoff is FP32 AoS TRS inverse: elapsed time rose **2.2% / 2.7%** at 128/4,096 objects; both median ranges are disjoint, although both changes remain inside the practical 5% band. Across all 160 matched Direct Neon rows, **3** improved by more than 5%, **157** were within 5%, and **0** regressed beyond 5%. Smaller slower rows remain in the data.

The other greater-than-5% gains, FP32 SoA transpose w256/n128 and FP64 AoS transpose n128, have byte-identical selected kernel bodies at different code addresses. Code placement or host variability may contribute; this is an inference, and no isolated primitive-level speedup is established for those rows. All before/after output dumps are bit-identical. See the [complete matched comparison](standard-math-blend4-2026-11-19/before-after.csv) and [full-precision data](standard-math-blend4-2026-11-19/before-after.json).

All slower and variable rows are retained. Before/after comparisons and full
suite runs use their own matched controls; do not mix times between runs.

- CSV results: [CSV results](standard-math-blend4-2026-11-19/baseline/results.csv)
- JSON results and raw trials: [JSON results and raw trials](standard-math-blend4-2026-11-19/baseline/results.json)
- Summary/audit: [Summary/audit](standard-math-blend4-2026-11-19/baseline/summary.json)
- Metadata and checksums: [Metadata](standard-math-blend4-2026-11-19/baseline/metadata.json), [Checksums](standard-math-blend4-2026-11-19/checksums.sha256)

In this run, <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins or is within
the shared 5% band in **160/160 rows**:
**53** Direct Neon leads, **107** within 5%,
and **0** sse2neon leads.

84/160 backend comparisons have disjoint invocation median ranges. No greater-than-5% backend lead has overlapping invocation median ranges. 5 Direct Neon rows and 7 compatibility rows are slower than scalar. These are descriptive ranges, not statistical significance.

<!-- standard-math-reading-guide -->
## How to read the tables

**The competing columns are <span style="color: #bc4c00;">sse2neon</span> and <span style="color: #6f42c1;">Direct&nbsp;Neon</span>.** Their adjacent timing
numbers are bold and colored. The **Winning backend** column names the faster backend,
or says **Within&nbsp;5%** inside the shared 5% throughput band.
The adjacent **Throughput gain (%)** column shows the winner's percentage lead.
**Scalar means no SIMD in this benchmark.** It uses ordinary float/double
arithmetic with the compiler's automatic SIMD vectorization disabled. The
scalar result is the baseline for measuring how much SIMD helps. Both
<span style="color: #bc4c00;">sse2neon</span> and <span style="color: #6f42c1;">Direct&nbsp;Neon</span> use SIMD instructions; they are two implementations
of the same vector/matrix work.

**nsec/op means nanoseconds per complete vector or matrix operation.**

**Lower times win; higher throughput ratios win.** Each timing is measured in
nanoseconds per complete vector or matrix operation, averaged over repeated
array processing. It is not the time for the whole array or one CPU instruction.
Compare implementations within the same row first.

### Memory layout: AoS and SoA

These names describe **how the components of many vectors or matrices are
arranged in memory**. Here is an example with four-component float vectors;
`0`, `1`, etc. identify different vectors.

**Array of Structures (AoS): keep each object's components together.**

```text
Vector 0          Vector 1          Vector 2
[x0, y0, z0, w0]  [x1, y1, z1, w1]  [x2, y2, z2, w2] ...
```

In the object-layout benchmark, a 128-bit Neon register holds one FP32
vector's four components:

```text
SIMD lanes: [x0, y0, z0, w0]
             one vector's components
```

**Structure of Arrays (SoA): keep matching components from different objects together.**

```text
x: [x0, x1, x2, x3, ...]
y: [y0, y1, y2, y3, ...]
z: [z0, z1, z2, z3, ...]
w: [w0, w1, w2, w3, ...]
```

In the batch-layout benchmark, a 128-bit Neon register holds the same FP32
component from four different vectors. Other registers hold their other components:

```text
SIMD lanes: [x0, x1, x2, x3]
             four vectors' x components
```

For matrices, the same principle applies: AoS keeps each matrix's entries
together; SoA groups the same entry from different matrices together.

**The calculation is the same; its storage and SIMD organization differ.**
AoS uses SIMD across one object's components here; SoA applies the same
calculation across independent objects. Both layouts report time per complete
vector or matrix operation, not per component. Compare backends within the
same layout first; neither layout is automatically faster.

### Columns and colors

| Column | Meaning |
| --- | --- |
| Operation | The calculation, such as vector dot product or matrix multiplication. |
| FP | 32 means single-precision `float`; 64 means double-precision `double`. |
| **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | SSE operations translated to Neon through <span style="color: #bc4c00;">sse2neon</span>. Competes directly with the adjacent <span style="color: #6f42c1;">Direct&nbsp;Neon</span> timing. |
| **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | The native Neon backend. Competes directly with the adjacent <span style="color: #bc4c00;">sse2neon</span> timing. |
| **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **<span style="color: #bc4c00;">sse2neon</span> time ÷ <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time**. Above 1 favors <span style="color: #6f42c1;">Direct&nbsp;Neon</span>; below 1 favors <span style="color: #bc4c00;">sse2neon</span>. Its continuous color scale shows which backend leads and by how much. |
| **Winning<br>backend** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span>, <span style="color: #bc4c00;">sse2neon</span>, or Within&nbsp;5% inside the shared band. |
| **Throughput<br>gain&nbsp;(%)** | The named winner's percentage throughput lead. An em dash means the result is within the 5% band. |
| **Scalar<br>(nsec/op)** | Scalar float/double arithmetic with automatic SIMD vectorization disabled. This is a separate reference. |
| Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Non-SIMD scalar reference time ÷ <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time. For example, 2× means <span style="color: #6f42c1;">Direct&nbsp;Neon</span> delivers twice the scalar throughput. |
| Consistent<br>Timing | Consistency of the difference between <span style="color: #bc4c00;">sse2neon</span> and <span style="color: #6f42c1;">Direct&nbsp;Neon</span> across runs: **Yes** means their three-run median timing ranges do not overlap; **No** means they overlap. This does not measure each backend's stability alone, identify the winner, or establish statistical significance. |

**Throughput percentages describe higher throughput, not reduced elapsed time.**
For the displayed throughput ratio `r`, a <span style="color: #6f42c1;">Direct&nbsp;Neon</span> win shows
`100 × (r − 1)%`; an <span style="color: #bc4c00;">sse2neon</span> win shows `100 × (1/r − 1)%`.
For example, 1.25× gives Winning backend **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>** and Throughput gain **+25.0%**;
0.8× gives Winning backend **<span style="color: #bc4c00;">sse2neon</span>** and Throughput gain **+25.0%**. Percentages use the
displayed ratio, rounded to one decimal place. A Within&nbsp;5% verdict shows
**—** in Throughput gain. The existing ratio column retains its continuous gradient.

**Backend names have fixed identity colors:** <span style="color: #6f42c1;">Direct&nbsp;Neon</span> is purple and
<span style="color: #bc4c00;">sse2neon</span> is orange. <span style="color: #6f42c1;">Direct&nbsp;Neon</span> and Within&nbsp;5% stay together on one line.
These label colors identify the implementation; the timing colors show the result.

**Both competing timing columns are colored relative to each other:**

- <span style="color: #1a7f37;">**Green:**</span> the winning backend's time.
- <span style="color: #cf222e;">**Red:**</span> the losing backend's time.
- <span style="color: #0969da;">**Blue:**</span> both times when approximately equal.

**The throughput ratio uses a continuous gradient:**

- **1.0× is white:** equal throughput.
- **Above 1.0×**, the number fades toward <span style="color: #6f42c1;">Direct&nbsp;Neon</span>'s purple, reaching full
  purple at **1.5×** (<span style="color: #6f42c1;">Direct&nbsp;Neon</span> has a 50% throughput lead).
- **Below 1.0×**, it fades toward <span style="color: #bc4c00;">sse2neon</span>'s orange using the reciprocal lead,
  reaching full orange at **2/3× ≈ 0.667×** (<span style="color: #bc4c00;">sse2neon</span> has a 50% throughput lead).
- Colors saturate beyond those endpoints. Equal winning throughput leads have
  equal fade strength: 1.25× and 0.8× each represent a 25% lead for the winner.

| Throughput ratio | Meaning |
| ---: | --- |
| <span style="color: #bc4c00;">0.667×</span> | <span style="color: #bc4c00;">sse2neon</span> leads by approximately 50% |
| <span style="color: #dea680;">0.800×</span> | <span style="color: #bc4c00;">sse2neon</span> leads by 25% |
| <span style="color: #ffffff;">1.000×</span> | Equal throughput |
| <span style="color: #b7a0e0;">1.250×</span> | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads by 25% |
| <span style="color: #6f42c1;">1.500×</span> | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads by 50% |

This gradient starts changing immediately on either side of 1.0×. The timing
colors and Winning backend column still use the shared 5% band below, so a slight
purple/orange tint can appear alongside a Within&nbsp;5% verdict.

The shared decision uses **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span>**:

| Throughput ratio | Winning backend | <span style="color: #bc4c00;">sse2neon</span> time color | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time color |
| --- | --- | --- | --- |
| Above 1.05× | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Red | Green |
| 0.95–1.05× inclusive | Within&nbsp;5% | Blue | Blue |
| Below 0.95× | <span style="color: #bc4c00;">sse2neon</span> | Green | Red |

Within&nbsp;5% is a practical throughput band, not a claim that the times
are identical. A row with blue timing numbers can have separate median ranges: a small
difference can be consistent while remaining inside that band. Colors compare these two
backends only; they do not indicate which is faster than scalar.

The section headings describe the workload:

- **128 or 4,096 objects** is the array length, not the SIMD width. All timing
  columns still report time **per object**.
- **Array of Structures (AoS)** corresponds to `object_aos` in the data files;
  **Structure of Arrays (SoA)** corresponds to `batch_soa`.
- **Logical 128/256/512 bits** is the batch-vector width. Neon hardware vectors
  remain 128-bit; wider values combine registers. FP32 logical 256 processes
  eight independent objects at a time; FP64 logical 256 processes four.
  Wider logical vectors are not automatically faster. Scalar batch results
  are reused across widths because scalar has no SIMD width.

For a worked example, take **<span style="color: #bc4c00;">sse2neon</span> = 0.318 nsec/op** and
**<span style="color: #6f42c1;">Direct&nbsp;Neon</span> = 0.289 nsec/op**, with a throughput ratio of **1.099×**.
<span style="color: #6f42c1;">Direct&nbsp;Neon</span> delivers approximately **9.9% more throughput**, or about
**9% less elapsed time**, than <span style="color: #bc4c00;">sse2neon</span>. The <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time is green, the
<span style="color: #bc4c00;">sse2neon</span> time is red, Winning backend says **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>**, and Throughput gain shows **+9.9%**. Ratios use
unrounded measurements, so dividing the displayed times can differ slightly.

These results include loads/stores and loop/call overhead. They describe array
throughput on the measured host, not isolated instruction latency or whole-
application speedup. Times are medians of invocation medians; raw outliers are
retained. The separate before/after optimization reports use their own measurements and
controls: do not combine their times with this report's times.
<!-- /standard-math-reading-guide -->

Units are **ns/vector or ns/matrix**, lower is better. Ratios divide the named
baseline time by Neon time: above one means Neon is faster. Scalar batch
controls may be reused across logical widths within the same run. Hardware
Neon vectors remain 128-bit; wider logical values compose registers. Timings
include loads/stores and loop/call overhead, with references, input creation,
validation and dumps outside timing.

TRS inverse covers nonzero orthogonal scaled affine columns; general
projective/sheared inversion is outside this workload. Preserve the benchmark
formulas, input generation, independent references, canaries and numerical
gates when repeating the same protocol.

**Executed measurement summary:** 18 measured processes /
1,200 shapes / 10,800 trial batches.
**Correctness summary:** all checks passed; unchanged 64-epsilon gate, maximum normalized error 6; zero input/canary mismatches.
**Host and build summary:** Apple M2, native arm64, macOS 26.7 (25G229), Apple Clang 17.0.0, SDK 26.1; Release -O3 -mcpu=apple-m1 -ffp-contract=off; scalar auto-vectorization disabled.

## 128 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #0969da;">1.925</span>** | **<span style="color: #0969da;">1.929</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 4.180 | 2.17× | no |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.354</span>** | **<span style="color: #0969da;">3.424</span>** | **<span style="color: #fcf7f4;">0.979×</span>** | Within&nbsp;5% | — | 4.236 | 1.24× | yes |
| mat4_multiply | 32 | **<span style="color: #0969da;">2.079</span>** | **<span style="color: #0969da;">2.086</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 9.839 | 4.72× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">4.285</span>** | **<span style="color: #0969da;">4.372</span>** | **<span style="color: #fcf8f5;">0.980×</span>** | Within&nbsp;5% | — | 10.453 | 2.39× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.226</span>** | **<span style="color: #0969da;">1.251</span>** | **<span style="color: #fcf8f5;">0.980×</span>** | Within&nbsp;5% | — | 2.414 | 1.93× | no |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.405</span>** | **<span style="color: #0969da;">2.524</span>** | **<span style="color: #f8ede6;">0.953×</span>** | Within&nbsp;5% | — | 2.408 | 0.95× | no |
| mat4_vector | 32 | **<span style="color: #0969da;">0.582</span>** | **<span style="color: #0969da;">0.589</span>** | **<span style="color: #fefbf9;">0.989×</span>** | Within&nbsp;5% | — | 2.484 | 4.22× | no |
| mat4_vector | 64 | **<span style="color: #0969da;">1.176</span>** | **<span style="color: #0969da;">1.204</span>** | **<span style="color: #fcf6f2;">0.976×</span>** | Within&nbsp;5% | — | 2.492 | 2.07× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.897</span>** | **<span style="color: #1a7f37;">0.614</span>** | **<span style="color: #7a50c6;">1.462×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.2% | 1.126 | 1.83× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.879</span>** | **<span style="color: #0969da;">0.850</span>** | **<span style="color: #f5f2fb;">1.034×</span>** | Within&nbsp;5% | — | 1.135 | 1.33× | yes |
| vec4_add | 32 | **<span style="color: #0969da;">0.354</span>** | **<span style="color: #0969da;">0.353</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 0.913 | 2.59× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.659</span>** | **<span style="color: #0969da;">0.678</span>** | **<span style="color: #fbf5f1;">0.973×</span>** | Within&nbsp;5% | — | 0.923 | 1.36× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.392</span>** | **<span style="color: #0969da;">0.392</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 0.688 | 1.76× | no |
| vec4_dot | 64 | **<span style="color: #0969da;">0.509</span>** | **<span style="color: #0969da;">0.521</span>** | **<span style="color: #fcf7f3;">0.977×</span>** | Within&nbsp;5% | — | 0.689 | 1.32× | no |
| vec4_length | 32 | **<span style="color: #0969da;">0.580</span>** | **<span style="color: #0969da;">0.580</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 0.678 | 1.17× | no |
| vec4_length | 64 | **<span style="color: #0969da;">0.574</span>** | **<span style="color: #0969da;">0.576</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 0.613 | 1.06× | no |
| vec4_normalize | 32 | **<span style="color: #0969da;">1.024</span>** | **<span style="color: #0969da;">1.027</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 1.565 | 1.52× | no |
| vec4_normalize | 64 | **<span style="color: #0969da;">1.207</span>** | **<span style="color: #0969da;">1.214</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 1.624 | 1.34× | yes |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.366</span>** | **<span style="color: #0969da;">0.358</span>** | **<span style="color: #f8f6fc;">1.023×</span>** | Within&nbsp;5% | — | 0.949 | 2.65× | no |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.600</span>** | **<span style="color: #0969da;">0.601</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 0.958 | 1.59× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">1.629</span>** | **<span style="color: #1a7f37;">1.476</span>** | **<span style="color: #e1d8f2;">1.104×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.4% | 4.618 | 3.13× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">2.704</span>** | **<span style="color: #0969da;">2.741</span>** | **<span style="color: #fdfaf8;">0.986×</span>** | Within&nbsp;5% | — | 4.733 | 1.73× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.138</span>** | **<span style="color: #1a7f37;">3.317</span>** | **<span style="color: #6f42c1;">2.152×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +115.2% | 17.722 | 5.34× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">6.776</span>** | **<span style="color: #0969da;">6.786</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 17.803 | 2.62× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.591</span>** | **<span style="color: #0969da;">1.561</span>** | **<span style="color: #faf8fd;">1.019×</span>** | Within&nbsp;5% | — | 3.310 | 2.12× | no |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.575</span>** | **<span style="color: #0969da;">2.578</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 3.310 | 1.28× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">3.068</span>** | **<span style="color: #1a7f37;">0.775</span>** | **<span style="color: #6f42c1;">3.960×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +296.0% | 2.989 | 3.86× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">1.564</span>** | **<span style="color: #0969da;">1.571</span>** | **<span style="color: #fefefd;">0.996×</span>** | Within&nbsp;5% | — | 2.987 | 1.90× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.443</span>** | **<span style="color: #1a7f37;">0.378</span>** | **<span style="color: #cdbeea;">1.173×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.3% | 0.838 | 2.22× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.588</span>** | **<span style="color: #0969da;">0.594</span>** | **<span style="color: #fefbfa;">0.990×</span>** | Within&nbsp;5% | — | 0.838 | 1.41× | yes |
| vec4_add | 32 | **<span style="color: #cf222e;">0.459</span>** | **<span style="color: #1a7f37;">0.380</span>** | **<span style="color: #c4b1e5;">1.206×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +20.6% | 0.917 | 2.41× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.717</span>** | **<span style="color: #0969da;">0.718</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 0.920 | 1.28× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.403</span>** | **<span style="color: #1a7f37;">0.267</span>** | **<span style="color: #6f42c1;">1.510×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +51.0% | 0.848 | 3.17× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.550</span>** | **<span style="color: #0969da;">0.548</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 0.863 | 1.57× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.394</span>** | **<span style="color: #1a7f37;">0.237</span>** | **<span style="color: #6f42c1;">1.664×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.4% | 0.683 | 2.89× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.475</span>** | **<span style="color: #0969da;">0.479</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 0.634 | 1.32× | yes |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.634</span>** | **<span style="color: #1a7f37;">0.372</span>** | **<span style="color: #6f42c1;">1.703×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +70.3% | 1.556 | 4.18× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.786</span>** | **<span style="color: #0969da;">0.791</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 1.613 | 2.04× | yes |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.498</span>** | **<span style="color: #1a7f37;">0.383</span>** | **<span style="color: #a98eda;">1.298×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +29.8% | 0.927 | 2.42× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.592</span>** | **<span style="color: #0969da;">0.597</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 0.938 | 1.57× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.176</span>** | **<span style="color: #1a7f37;">1.494</span>** | **<span style="color: #7b52c6;">1.457×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +45.7% | 4.618 | 3.09× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">2.988</span>** | **<span style="color: #0969da;">3.007</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 4.733 | 1.57× | yes |
| mat4_multiply | 32 | **<span style="color: #cf222e;">4.861</span>** | **<span style="color: #1a7f37;">4.122</span>** | **<span style="color: #cbbbe9;">1.179×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.9% | 17.722 | 4.30× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">7.664</span>** | **<span style="color: #0969da;">7.774</span>** | **<span style="color: #fdfaf8;">0.986×</span>** | Within&nbsp;5% | — | 17.803 | 2.29× | yes |
| mat4_transpose | 32 | **<span style="color: #cf222e;">2.203</span>** | **<span style="color: #1a7f37;">1.416</span>** | **<span style="color: #6f42c1;">1.556×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +55.6% | 3.310 | 2.34× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.353</span>** | **<span style="color: #0969da;">2.337</span>** | **<span style="color: #fdfcfe;">1.007×</span>** | Within&nbsp;5% | — | 3.310 | 1.42× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">1.502</span>** | **<span style="color: #1a7f37;">1.092</span>** | **<span style="color: #9371d0;">1.375×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +37.5% | 2.989 | 2.74× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.033</span>** | **<span style="color: #0969da;">2.041</span>** | **<span style="color: #fefefd;">0.996×</span>** | Within&nbsp;5% | — | 2.987 | 1.46× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.349</span>** | **<span style="color: #1a7f37;">0.331</span>** | **<span style="color: #efe9f8;">1.057×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.7% | 0.838 | 2.54× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.575</span>** | **<span style="color: #0969da;">0.586</span>** | **<span style="color: #fdf9f6;">0.983×</span>** | Within&nbsp;5% | — | 0.838 | 1.43× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.380</span>** | **<span style="color: #1a7f37;">0.345</span>** | **<span style="color: #e1d8f2;">1.104×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.4% | 0.917 | 2.66× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.555</span>** | **<span style="color: #0969da;">0.562</span>** | **<span style="color: #fdfaf8;">0.987×</span>** | Within&nbsp;5% | — | 0.920 | 1.64× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.285</span>** | **<span style="color: #1a7f37;">0.268</span>** | **<span style="color: #ede8f7;">1.061×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.1% | 0.848 | 3.16× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.502</span>** | **<span style="color: #0969da;">0.499</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.863 | 1.73× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.394</span>** | **<span style="color: #1a7f37;">0.220</span>** | **<span style="color: #6f42c1;">1.791×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +79.1% | 0.683 | 3.11× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.406</span>** | **<span style="color: #0969da;">0.407</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 0.634 | 1.56× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.610</span>** | **<span style="color: #1a7f37;">0.403</span>** | **<span style="color: #6f42c1;">1.514×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +51.4% | 1.556 | 3.86× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.781</span>** | **<span style="color: #0969da;">0.784</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 1.613 | 2.06× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.387</span>** | **<span style="color: #1a7f37;">0.347</span>** | **<span style="color: #ddd3f0;">1.117×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +11.7% | 0.927 | 2.67× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.616</span>** | **<span style="color: #0969da;">0.614</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 0.938 | 1.53× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.752</span>** | **<span style="color: #1a7f37;">2.232</span>** | **<span style="color: #bca7e2;">1.233×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +23.3% | 4.618 | 2.07× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">4.777</span>** | **<span style="color: #0969da;">4.725</span>** | **<span style="color: #fcfbfe;">1.011×</span>** | Within&nbsp;5% | — | 4.733 | 1.00× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">5.245</span>** | **<span style="color: #1a7f37;">4.268</span>** | **<span style="color: #bda8e3;">1.229×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +22.9% | 17.722 | 4.15× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">10.430</span>** | **<span style="color: #0969da;">10.220</span>** | **<span style="color: #f9f7fc;">1.021×</span>** | Within&nbsp;5% | — | 17.803 | 1.74× | no |
| mat4_transpose | 32 | **<span style="color: #cf222e;">2.464</span>** | **<span style="color: #1a7f37;">1.691</span>** | **<span style="color: #7b52c6;">1.457×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +45.7% | 3.310 | 1.96× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">4.527</span>** | **<span style="color: #0969da;">4.518</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 3.310 | 0.73× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">1.398</span>** | **<span style="color: #1a7f37;">1.221</span>** | **<span style="color: #d5c8ed;">1.145×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +14.5% | 2.989 | 2.45× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.683</span>** | **<span style="color: #0969da;">2.724</span>** | **<span style="color: #fdfaf7;">0.985×</span>** | Within&nbsp;5% | — | 2.987 | 1.10× | no |
| vec3_cross | 32 | **<span style="color: #0969da;">0.323</span>** | **<span style="color: #0969da;">0.310</span>** | **<span style="color: #f3f0fa;">1.041×</span>** | Within&nbsp;5% | — | 0.838 | 2.71× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.565</span>** | **<span style="color: #0969da;">0.569</span>** | **<span style="color: #fefcfb;">0.993×</span>** | Within&nbsp;5% | — | 0.838 | 1.47× | yes |
| vec4_add | 32 | **<span style="color: #0969da;">0.336</span>** | **<span style="color: #0969da;">0.325</span>** | **<span style="color: #f6f3fb;">1.031×</span>** | Within&nbsp;5% | — | 0.917 | 2.82× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.562</span>** | **<span style="color: #0969da;">0.551</span>** | **<span style="color: #f9f7fc;">1.022×</span>** | Within&nbsp;5% | — | 0.920 | 1.67× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.255</span>** | **<span style="color: #0969da;">0.252</span>** | **<span style="color: #fcfafe;">1.012×</span>** | Within&nbsp;5% | — | 0.848 | 3.37× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.484</span>** | **<span style="color: #0969da;">0.491</span>** | **<span style="color: #fdfaf8;">0.987×</span>** | Within&nbsp;5% | — | 0.863 | 1.76× | yes |
| vec4_length | 32 | **<span style="color: #cf222e;">0.395</span>** | **<span style="color: #1a7f37;">0.198</span>** | **<span style="color: #6f42c1;">1.996×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +99.6% | 0.683 | 3.45× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.398</span>** | **<span style="color: #0969da;">0.398</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 0.634 | 1.59× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.627</span>** | **<span style="color: #1a7f37;">0.450</span>** | **<span style="color: #8d69ce;">1.396×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +39.6% | 1.556 | 3.46× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.851</span>** | **<span style="color: #0969da;">0.853</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 1.613 | 1.89× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.359</span>** | **<span style="color: #1a7f37;">0.339</span>** | **<span style="color: #eee8f8;">1.060×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.0% | 0.927 | 2.74× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.638</span>** | **<span style="color: #0969da;">0.652</span>** | **<span style="color: #fcf7f4;">0.978×</span>** | Within&nbsp;5% | — | 0.938 | 1.44× | yes |

## 4,096 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #0969da;">1.955</span>** | **<span style="color: #0969da;">1.952</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 4.207 | 2.15× | no |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.375</span>** | **<span style="color: #0969da;">3.426</span>** | **<span style="color: #fdfaf7;">0.985×</span>** | Within&nbsp;5% | — | 4.447 | 1.30× | yes |
| mat4_multiply | 32 | **<span style="color: #0969da;">2.146</span>** | **<span style="color: #0969da;">2.202</span>** | **<span style="color: #fcf6f2;">0.975×</span>** | Within&nbsp;5% | — | 11.045 | 5.02× | no |
| mat4_multiply | 64 | **<span style="color: #0969da;">4.365</span>** | **<span style="color: #0969da;">4.373</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 12.259 | 2.80× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.315</span>** | **<span style="color: #0969da;">1.323</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 2.741 | 2.07× | no |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.630</span>** | **<span style="color: #0969da;">2.632</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 3.373 | 1.28× | no |
| mat4_vector | 32 | **<span style="color: #0969da;">0.929</span>** | **<span style="color: #0969da;">0.932</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 2.771 | 2.97× | no |
| mat4_vector | 64 | **<span style="color: #0969da;">1.872</span>** | **<span style="color: #0969da;">1.866</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 3.200 | 1.71× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.887</span>** | **<span style="color: #1a7f37;">0.611</span>** | **<span style="color: #7d54c7;">1.452×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +45.2% | 1.138 | 1.86× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.997</span>** | **<span style="color: #0969da;">1.000</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 1.180 | 1.18× | no |
| vec4_add | 32 | **<span style="color: #0969da;">0.316</span>** | **<span style="color: #0969da;">0.315</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 0.911 | 2.89× | no |
| vec4_add | 64 | **<span style="color: #0969da;">0.979</span>** | **<span style="color: #0969da;">0.979</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 1.127 | 1.15× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.360</span>** | **<span style="color: #0969da;">0.363</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 0.688 | 1.89× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.728</span>** | **<span style="color: #0969da;">0.726</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 0.756 | 1.04× | no |
| vec4_length | 32 | **<span style="color: #0969da;">0.582</span>** | **<span style="color: #0969da;">0.581</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 0.679 | 1.17× | no |
| vec4_length | 64 | **<span style="color: #0969da;">0.582</span>** | **<span style="color: #0969da;">0.583</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 0.624 | 1.07× | no |
| vec4_normalize | 32 | **<span style="color: #0969da;">1.011</span>** | **<span style="color: #0969da;">1.009</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 1.569 | 1.55× | no |
| vec4_normalize | 64 | **<span style="color: #0969da;">1.206</span>** | **<span style="color: #0969da;">1.205</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 1.639 | 1.36× | no |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.316</span>** | **<span style="color: #0969da;">0.321</span>** | **<span style="color: #fdf9f6;">0.983×</span>** | Within&nbsp;5% | — | 0.941 | 2.93× | no |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.978</span>** | **<span style="color: #0969da;">0.978</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 1.135 | 1.16× | no |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.475</span>** | **<span style="color: #1a7f37;">2.342</span>** | **<span style="color: #efe9f8;">1.057×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.7% | 4.621 | 1.97× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.754</span>** | **<span style="color: #0969da;">3.766</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 4.748 | 1.26× | yes |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.168</span>** | **<span style="color: #1a7f37;">3.933</span>** | **<span style="color: #6f42c1;">1.823×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +82.3% | 18.578 | 4.72× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">7.145</span>** | **<span style="color: #0969da;">7.147</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 19.825 | 2.77× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">2.579</span>** | **<span style="color: #0969da;">2.499</span>** | **<span style="color: #f6f3fb;">1.032×</span>** | Within&nbsp;5% | — | 4.573 | 1.83× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">4.046</span>** | **<span style="color: #0969da;">4.040</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 4.583 | 1.13× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">3.188</span>** | **<span style="color: #1a7f37;">1.488</span>** | **<span style="color: #6f42c1;">2.142×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +114.2% | 3.111 | 2.09× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.269</span>** | **<span style="color: #0969da;">2.279</span>** | **<span style="color: #fefdfc;">0.995×</span>** | Within&nbsp;5% | — | 3.171 | 1.39× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.702</span>** | **<span style="color: #1a7f37;">0.614</span>** | **<span style="color: #d6c9ed;">1.144×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +14.4% | 1.146 | 1.87× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.384</span>** | **<span style="color: #0969da;">1.385</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 1.490 | 1.08× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.977</span>** | **<span style="color: #1a7f37;">0.800</span>** | **<span style="color: #bfabe4;">1.221×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +22.1% | 1.362 | 1.70× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">1.685</span>** | **<span style="color: #0969da;">1.691</span>** | **<span style="color: #fefefd;">0.996×</span>** | Within&nbsp;5% | — | 1.816 | 1.07× | yes |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.416</span>** | **<span style="color: #1a7f37;">0.285</span>** | **<span style="color: #7a51c6;">1.461×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.1% | 0.852 | 2.99× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.774</span>** | **<span style="color: #0969da;">0.772</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 1.174 | 1.52× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.393</span>** | **<span style="color: #1a7f37;">0.235</span>** | **<span style="color: #6f42c1;">1.674×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +67.4% | 0.667 | 2.84× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.475</span>** | **<span style="color: #0969da;">0.476</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 0.602 | 1.27× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.632</span>** | **<span style="color: #1a7f37;">0.393</span>** | **<span style="color: #6f42c1;">1.607×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +60.7% | 1.540 | 3.92× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.955</span>** | **<span style="color: #0969da;">0.950</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 1.666 | 1.75× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.974</span>** | **<span style="color: #1a7f37;">0.805</span>** | **<span style="color: #c3b0e5;">1.210×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +21.0% | 1.364 | 1.69× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.689</span>** | **<span style="color: #0969da;">1.692</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 1.818 | 1.07× | no |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">3.324</span>** | **<span style="color: #1a7f37;">2.188</span>** | **<span style="color: #6f42c1;">1.520×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +52.0% | 4.621 | 2.11× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.510</span>** | **<span style="color: #0969da;">3.554</span>** | **<span style="color: #fdfbf9;">0.988×</span>** | Within&nbsp;5% | — | 4.748 | 1.34× | yes |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.278</span>** | **<span style="color: #1a7f37;">6.087</span>** | **<span style="color: #c7b5e7;">1.196×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +19.6% | 18.578 | 3.05× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">10.434</span>** | **<span style="color: #0969da;">10.142</span>** | **<span style="color: #f7f4fb;">1.029×</span>** | Within&nbsp;5% | — | 19.825 | 1.95× | yes |
| mat4_transpose | 32 | **<span style="color: #cf222e;">3.332</span>** | **<span style="color: #1a7f37;">2.350</span>** | **<span style="color: #8761cb;">1.418×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +41.8% | 4.573 | 1.95× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">3.734</span>** | **<span style="color: #0969da;">3.728</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 4.583 | 1.23× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">2.202</span>** | **<span style="color: #1a7f37;">1.579</span>** | **<span style="color: #8d6ace;">1.395×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +39.5% | 3.111 | 1.97× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.538</span>** | **<span style="color: #0969da;">2.529</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 3.171 | 1.25× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.641</span>** | **<span style="color: #1a7f37;">0.562</span>** | **<span style="color: #d7caee;">1.139×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +13.9% | 1.146 | 2.04× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.366</span>** | **<span style="color: #0969da;">1.361</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 1.490 | 1.09× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.891</span>** | **<span style="color: #1a7f37;">0.766</span>** | **<span style="color: #d0c1eb;">1.164×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +16.4% | 1.362 | 1.78× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">1.707</span>** | **<span style="color: #0969da;">1.708</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 1.816 | 1.06× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.313</span>** | **<span style="color: #1a7f37;">0.287</span>** | **<span style="color: #e4dcf3;">1.093×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +9.3% | 0.852 | 2.97× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.777</span>** | **<span style="color: #0969da;">0.788</span>** | **<span style="color: #fdfaf8;">0.986×</span>** | Within&nbsp;5% | — | 1.174 | 1.49× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.394</span>** | **<span style="color: #1a7f37;">0.218</span>** | **<span style="color: #6f42c1;">1.811×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +81.1% | 0.667 | 3.07× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.409</span>** | **<span style="color: #0969da;">0.408</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 0.602 | 1.48× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.626</span>** | **<span style="color: #1a7f37;">0.408</span>** | **<span style="color: #6f42c1;">1.532×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +53.2% | 1.540 | 3.77× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.917</span>** | **<span style="color: #0969da;">0.926</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 1.666 | 1.80× | yes |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.887</span>** | **<span style="color: #1a7f37;">0.764</span>** | **<span style="color: #d0c2eb;">1.162×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +16.2% | 1.364 | 1.79× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.706</span>** | **<span style="color: #0969da;">1.700</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 1.818 | 1.07× | yes |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">4.443</span>** | **<span style="color: #1a7f37;">4.156</span>** | **<span style="color: #ebe5f6;">1.069×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.9% | 4.621 | 1.11× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">6.702</span>** | **<span style="color: #0969da;">6.787</span>** | **<span style="color: #fdfaf8;">0.987×</span>** | Within&nbsp;5% | — | 4.748 | 0.70× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.324</span>** | **<span style="color: #1a7f37;">6.791</span>** | **<span style="color: #e9e2f5;">1.078×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +7.8% | 18.578 | 2.74× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">12.368</span>** | **<span style="color: #0969da;">12.393</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 19.825 | 1.60× | no |
| mat4_transpose | 32 | **<span style="color: #cf222e;">4.056</span>** | **<span style="color: #1a7f37;">3.154</span>** | **<span style="color: #ad93dc;">1.286×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +28.6% | 4.573 | 1.45× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">6.458</span>** | **<span style="color: #0969da;">6.459</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 4.583 | 0.71× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">2.582</span>** | **<span style="color: #1a7f37;">1.992</span>** | **<span style="color: #aa8fda;">1.296×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +29.6% | 3.111 | 1.56× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">3.649</span>** | **<span style="color: #0969da;">3.597</span>** | **<span style="color: #fbf9fd;">1.015×</span>** | Within&nbsp;5% | — | 3.171 | 0.88× | yes |
| vec3_cross | 32 | **<span style="color: #0969da;">0.540</span>** | **<span style="color: #0969da;">0.524</span>** | **<span style="color: #f6f4fb;">1.030×</span>** | Within&nbsp;5% | — | 1.146 | 2.19× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.328</span>** | **<span style="color: #0969da;">1.324</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 1.490 | 1.13× | no |
| vec4_add | 32 | **<span style="color: #0969da;">0.722</span>** | **<span style="color: #0969da;">0.714</span>** | **<span style="color: #fcfbfe;">1.011×</span>** | Within&nbsp;5% | — | 1.362 | 1.91× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">1.655</span>** | **<span style="color: #0969da;">1.658</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 1.816 | 1.10× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.275</span>** | **<span style="color: #0969da;">0.271</span>** | **<span style="color: #faf9fd;">1.017×</span>** | Within&nbsp;5% | — | 0.852 | 3.14× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.764</span>** | **<span style="color: #0969da;">0.763</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 1.174 | 1.54× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.395</span>** | **<span style="color: #1a7f37;">0.199</span>** | **<span style="color: #6f42c1;">1.981×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +98.1% | 0.667 | 3.35× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.400</span>** | **<span style="color: #0969da;">0.398</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.602 | 1.51× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.625</span>** | **<span style="color: #1a7f37;">0.449</span>** | **<span style="color: #8e6ace;">1.393×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +39.3% | 1.540 | 3.43× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.925</span>** | **<span style="color: #0969da;">0.921</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 1.666 | 1.81× | no |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.759</span>** | **<span style="color: #0969da;">0.729</span>** | **<span style="color: #f3f0fa;">1.041×</span>** | Within&nbsp;5% | — | 1.364 | 1.87× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.656</span>** | **<span style="color: #0969da;">1.684</span>** | **<span style="color: #fdf9f6;">0.983×</span>** | Within&nbsp;5% | — | 1.818 | 1.08× | yes |

## Source, host and controls

- Tested full commit: `5e039b01b49abc00548123763be85898932e3ad8`.
- Tested full tree: `ea90c1b40fc50936db7c3509aaa11ab51c02d890`.
- Branch/worktree state before and after: fresh builds used existing gitless immutable captures; primary master stays clean at a04eb834c0e6707256009d007c77f4ccaf66beca, and the retained detached implementation worktree keeps its two source edits and prior reports; all refs, remotes, configuration and unrelated files preserved.
- Dirty-source provenance, if applicable: exact clean Git-base capture, 564 files, fingerprint `fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06`, base `5e039b01b49abc00548123763be85898932e3ad8`, base tree `ea90c1b40fc50936db7c3509aaa11ab51c02d890`, patch SHA256 `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`; empty tracked patch, no added/deleted paths; original [source archive receipt](standard-math-blend4-2026-11-19/baseline-source-archive-receipt.json).
- Comparison source, if used: matched candidate frozen fingerprint 3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36; tested commit/tree null.
- Benchmark source/build-file comparison: all six root/benchmark build and source files are raw-byte identical between captures; [per-file hashes](standard-math-blend4-2026-11-19/benchmark-source-comparison.json).
- Physical host, architecture and native-execution proof: Apple M2 / Mac14,3, arm64 Python/compiler and arm64 Mach-O binaries; Rosetta query 0; tool arm64 slices verified; actual page size 16,384 bytes.
- OS/version/build: macOS 26.7 (25G229).
- Compiler/version/target: Apple clang version 17.0.0 (clang-1700.4.4.1); Target: arm64-apple-darwin25.6.0.
- SDK, CMake/CTest, build tool and Python: SDK 26.1; CMake/CTest 4.4.4; retained Ninja 1.11.1.git.kitware.jobserver-1; native Python 3.9.6.
- Execution interval: 2026-11-19T10:52:08.142787+00:00 through 2026-11-19T10:56:28.584323+00:00; report-date timezone: Europe/Stockholm; both sources interleaved during one 36-process session.
- Exact Release compile/link flags: `-O3 -DNDEBUG -std=gnu++20 -arch arm64 -isysroot <SDK> -ffp-contract=off -mcpu=apple-m1`, `-O3 -DNDEBUG -arch arm64 -isysroot <SDK> -Wl,-search_paths_first -Wl,-headerpad_max_install_names`.
- Scalar vectorization controls and disassembly findings: scalar additionally uses -fno-vectorize -fno-slp-vectorize; both scalar kernel disassemblies contain no packed FP arithmetic. Backend definitions, include/output paths and all exact normalized compile/link commands are in [flags](standard-math-blend4-2026-11-19/flags.json); `<SDK>` identifies installed SDK 26.1.
- Timing build controls: actual timing compile/link commands verified: no fast-math, finite-math-only, LTO or sanitizer flags; separately compiled kernels and the unchanged memory fence retain timed work.
- Correctness-only builds, profiles and sanitizer coverage: this source's scalar/compatibility/Direct benchmark executables passed fresh Release and strict UBSan Debug before and after timing; [all four build profiles](standard-math-blend4-2026-11-19/correctness.json). The earlier candidate full M2 result (8,036 cases/eight examples) retains its [separate report](../../validation/arm64/mac-arm-2026-11-19-blend4.md); it is not a new full-matrix execution here.
- Registered/passed/failed/skipped/timed-out counts and attempts: 3 registered benchmark cases per build profile; 6/6 before plus 6/6 after timing for this source, 24/24 bracketed executions across both sources; all 4 separate examples passed; zero failures/skips/timeouts; one scheduled attempt, no replay.
- Strict UBSan flags/runtime options and control results, when used: Debug -g -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer; UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1. Fresh positive exit 0; deliberate signed overflow exit -6 with the required diagnostic and no continuation; [control record](standard-math-blend4-2026-11-19/ubsan-control.json). ASan was not run.
- Generator checks before/after: read-only --check passed before/after for each source; generation remains retired, zero generated products.
- Pristine vendor integrity: pin 667577495efaa36a10cbb5d6cb8c942c23913848, tree 36aee120c28c9c64e62903bfc81139003e37cedd; all 28 root and 28 vendor originals exact. All 60 protected/dependency raw hashes and nanosecond mtimes unchanged in both captures, primary and editable sources; [integrity record](standard-math-blend4-2026-11-19/protected-integrity.json).
- Source edits, generated-product changes and evidence of stability: complete 564-file baseline and 571-file candidate manifests verified before/after; no runtime/test/benchmark/runner/build/source edit occurred during measurement, no dependency/tool update; later report files are outside both frozen captures.

Optional kernel/whole-binary comparison: [kernel and whole-binary comparison](standard-math-blend4-2026-11-19/codegen-comparison.json), [binary hashes](standard-math-blend4-2026-11-19/binaries.json), [selected function byte slices](standard-math-blend4-2026-11-19/selected-functions.json).
Both scalar and compatibility kernel objects, text and whole Release binaries are byte-identical between sources. Direct Neon text changed from 89,192 to 89,136 bytes. Selected FP32 AoS transpose and inverse bodies changed (104→76 and 184→156 bytes); the SoA FP32 w256 transpose and AoS FP64 transpose bodies are byte-identical but moved addresses. These facts do not isolate code placement or prove instruction-count-based speedup.

Do not edit `vendor/vectorclass/` or its pin/hash inventory to pass validation.
On an unexplained vendor mismatch, preserve the failed check and stop. Runtime
fixes belong in authoritative ARM-owned sources, with new source IDs and
fresh relevant validation.

## Measurement and correctness scope

- Operations, precision, object counts, layouts and logical widths: all ten operations, both precisions, counts 128/4096; AoS plus SoA at logical 128/256/512, complete 160 rows per source; both sources total 320 three-way rows.
- Invocation count, order and scheduling: three invocations per source/backend/count. One timing process at a time; rounds rotate scalar/compat/Neon, compat/Neon/scalar, Neon/scalar/compat. Each backend/count pair runs baseline then candidate in rounds 1/3 and candidate then baseline in round 2; [complete schedule](standard-math-blend4-2026-11-19/protocol.json).
- Calibration, warm-up and trials per shape: unchanged shortest-of-three calibration, doubling to about 1 ms, target about 10 ms with bounded repeats, warm-up and one short-batch adjustment, then nine raw timed trials per shape; every outlier retained.
- Aggregation: median of three invocation medians, each from nine trials normalized by count × repeats; scalar batch controls reused across logical widths only within the same source. Even byte-identical scalar/compat binaries were measured separately for both sources.
- Actual totals: 18 processes, 1,200 shapes,
  10,800 trial batches and 160 for this source; paired comparison has 160 rows / 480 backend comparisons, with scalar-width reuse explicitly retained comparison rows.
- Independent reference methods and unchanged gate: unchanged independent coordinate loops, nested hypot and pivoted Gauss-Jordan inversion; error scaled by operation magnitude/conditioning with a 64-epsilon acceptance limit.
- Output fields per validation pass: 17,740,800; combined before/after
  timing checks: 35,481,600 for this source; 70,963,200 across both sources.
- Maximum normalized reference error: 6;
  outcome: all passed with zero reference failures.
- Input hashes, canaries, memory layout and partial counts: deterministic seed 0x874af31b01753 and all input hashes unchanged; identical inputs across matched sources/backends; canaries and SoA padding preserved. Correctness-only --check includes counts 1/3/7/17/129/257 for partial tails; timed counts remain 128/4096.
- FPCR/rounding/flush-to-zero state: FPCR=0 for every timed shape: nearest rounding, FTZ disabled; harness saves/restores caller FP state.
- Output repeatability within each backend/shape: all three invocation dumps are byte-identical within every source/backend/shape; every matched baseline/candidate output is also bit-identical.
- Cross-backend comparison scope: first-invocation canonical AoS output dumps compared scalar/Direct and compatibility/Direct across all 160 report rows. Scalar SoA controls repeat across widths, so difference totals include that reuse. All 400 unique source/backend shapes per version are covered; all 2,400 dumps retained.
- Scalar/Neon differing fields, maximum absolute error and ULP distance:
  7,913 differing fields; maximum absolute difference 9.53674316e-07; maximum raw distance 768 ULP.
- Compatibility/Neon differing fields, maximum absolute error and ULP distance:
  21,084 differing fields; maximum absolute difference 4.76837158e-07; maximum raw distance 4 ULP.
- Failed checks, retries, missing profiles or unavailable controls:
  zero failed checks, missing profiles, retries or discarded timed processes. Fresh sanitizer positive/negative control passed its original conditions. Core/frequency pinning and ASan coverage are absent and remain explicit.

Cross-backend bit differences reflect unchanged reduction/FMA/grouping behavior; all outputs pass the same independent operation-scaled gate. The maximum scalar/Direct raw ULP distance occurs near cancellation and is not substituted for that gate. The specialization itself changed no before/after output bits.

Output bit differences and independent-reference accuracy are separate facts.
State which outputs were compared, whether scalar rows were reused across
widths, and whether differences are within the existing contract. A large raw
ULP distance near cancellation must not replace the operation's acceptance
rule, and a passing accuracy gate does not establish universal bitwise identity.

- Core placement/frequency/power settings and sleep prevention: core placement, CPU frequency and power settings were not pinned or modified; caffeinate -i prevented idle sleep; all compilation and pre-timing correctness checks completed before the sequential timing interval; post-timing checks followed it.
- Raw trial-batch duration range: 3.137333–34.195000 ms.
- Host load and thermal/performance observations: whole-session load averages before 2.94/1.98/2.12 and after 3.39/2.87/2.49; pmset reported no recorded thermal/performance warning and no recorded CPU power status before/after. This does not establish an otherwise idle host or fixed frequency.
- Rows with disjoint invocation median ranges: 84/160.
- Leads with overlapping ranges or other variability: No greater-than-5% backend lead has overlapping invocation median ranges; three invocation ranges describe consistency, not statistical significance. The matched FP64 AoS transpose gain has overlapping ranges; the 4,096-object FP32 target gain also overlaps.
- Rows slower than scalar, including which backend: compat: 7 rows — `mat4_inverse_trs` FP64 batch_soa w512/n128, `mat4_inverse_trs` FP64 batch_soa w512/n4096, `mat4_transpose` FP64 batch_soa w512/n128, `mat4_transpose` FP64 batch_soa w512/n4096, `mat4_vector` FP32 batch_soa w128/n128, `mat4_vector` FP32 batch_soa w128/n4096, `mat4_vector` FP64 batch_soa w512/n4096; neon: 5 rows — `mat4_inverse_trs` FP64 batch_soa w512/n4096, `mat4_transpose` FP64 batch_soa w512/n128, `mat4_transpose` FP64 batch_soa w512/n4096, `mat4_transpose` FP64 object_aos w256/n128, `mat4_vector` FP64 batch_soa w512/n4096.

These are array-throughput measurements on this M2/toolchain, not isolated latency, all-API performance or application speedup. The matched small FP32 transpose improvement has a measurable TRS-inverse tradeoff and no >5% target gain at 4,096 objects. No Windows or cross-host parity result is added; the existing Windows gate stays failed/incomplete. Direct Neon remains experimental; other hosts/toolchains, physical Intel, FP16/SVML, ASan and external consumers remain outside scope.

## Evidence and next use

- Ignored raw evidence directory: `build/m2-blend4-benchmark-20261119T105004Z-01/`.
- Attempt identifiers and failed/replayed attempts: one scheduled session, timing invocations 01–36, no failure/replay; deliberate UBSan overflow is a required negative control.
- Compact archive filename: `benchmark-evidence.zip`.
- Archive SHA256: `d95f87bc08fa1b927cef7fea4f4411fc737e2559d6f1ed865fe530e36970aa47`.
- Verified entry count, CRC/hash audit and contents: 2,717 entries passed CRC and every SHA256; includes both complete original source ZIPs/patches/manifests, 2,400 output dumps, all 36 raw trial logs, generated controls, commands, flags, JUnit and analysis. Compiled payloads excluded from the packet remain on disk with hashes. The packet precedes final report formatting.
- Portable archive receipt: [Archive receipt](standard-math-blend4-2026-11-19/archive-receipt.json).
- Portable metadata, results and checksum inventory: [this source results](standard-math-blend4-2026-11-19/baseline/results.json), [session metadata](standard-math-blend4-2026-11-19/session-metadata.json), [matched comparisons](standard-math-blend4-2026-11-19/before-after.json), [checksum inventory](standard-math-blend4-2026-11-19/checksums.sha256).
- Separately retained records/supplements and hashes: later offline symbol/function inspection is in `codegen-review-supplement.zip`, SHA256 `21e0e45414a6f874b56a4a3e6531cd9a62fc11118742cd664cbfd3e95c5ea5eb`; all 3 entries passed CRC/hash checks, with a [portable receipt](standard-math-blend4-2026-11-19/codegen-review-receipt.json). Final formatted reports, formatter source and documentation-check records remain separately retained; no measurement record was omitted from the main packet.
- Retained build/source/worktree resources and any explicitly authorized cleanup:
  all sources, fresh build directories, binaries, raw records, failed/negative controls and existing worktrees/tools remain; no cleanup, branch retirement or deletion.

Retain source/compiler/flag inventories, binary hashes, complete raw trials,
output dumps, input hashes, configure/build/test logs, JUnit, failed attempts,
reference/probe sources and relevant code-generation records. Exclude machine
paths, credentials and compiled binaries from the portable report/data.

This supplies a matched 19 November benchmark checkpoint for review of the four-pattern optimization. The evidence supports a small 128-object FP32 AoS transpose gain, not a broad performance win; review the retained inverse tradeoff before deciding on integration. Any later runtime change needs its own frozen source, native correctness and matched measurements. Historical dated reports and the template remain byte-exact.

Record this task's actual Git/publication actions: none — no commit, push, merge, PR or repository administration; new report/data files remain uncommitted in the retained detached implementation worktree.
Follow the maintainer's task-specific authorization for publication or cleanup.
Future source changes require new recorded measurements and matched controls;
preserve completed dated reports and all original numerical data.
