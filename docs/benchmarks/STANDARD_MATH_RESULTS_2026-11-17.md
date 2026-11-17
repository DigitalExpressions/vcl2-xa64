# Standard vector and matrix benchmarks - 2026-11-17

**Scalar FP32/FP64 versus <span style="color: #bc4c00;">sse2neon</span> versus <span style="color: #6f42c1;">Direct&nbsp;Neon</span>**: ten operations, object
and batch layouts, 128/4096 objects and logical batch widths 128/256/512.
All **160 three-way rows** below use the native M2 measurements from
**17 November 2026**, at source `3abf7af`.

All three Release benchmark binaries match fresh pre-layout builds byte for
byte; the pristine-source layout introduced no executable difference in these
workloads. All slower rows remain. See the
[source, host and code-generation evidence](#source-host-and-controls).
Before/after and full-suite runs use their own controls; do not mix their times.

[CSV](standard-math-2026-11-17/results.csv) · [JSON](standard-math-2026-11-17/results.json) ·
[audit](standard-math-2026-11-17/summary.json)

In this run, <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins or is within the shared 5% band in **159/160 rows**:
**56** Direct Neon leads, **103** within 5%, and **one** sse2neon lead.
The sse2neon lead has overlapping invocation median ranges.

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
baseline time by Neon time: above one means Neon is faster. The scalar batch
baseline is reused across widths. `object_aos` packs four coordinates per
vector/column; `batch_soa` assigns an independent object to each SIMD lane.
Hardware vectors remain 128-bit; wider logical values compose registers.
These are array throughput measurements, including loads/stores and loop/call
overhead, with data creation, references, validation and dumps outside timing.

TRS inverse requires nonzero orthogonal scaled affine columns (rotation/scale/
translation); general projective/sheared inverse is not covered. Scale/add is
`a * 0.625 + b`, without FMA; zero vectors normalize to zero. Scalar float/double
disable loop/SLP auto-vectorization, verified to contain no packed FP arithmetic.

Three invocations per backend/count, rotating order, nine warmed calibrated
trials per shape. Medians are taken across invocation medians. Raw scheduler
outliers are retained; range separation is descriptive, not a significance test.
**18 measured processes / 1,200 shapes / 10,800 trial batches**. All outputs
pass the unchanged independent-reference/canary/input gates before and after
timing; maximum normalized reference error is 6, within the unchanged
64-epsilon gate. Reference
uses coordinate loops, hypot and pivoted Gauss-Jordan; host long double is binary64.

Apple M2/macOS 26.7, Clang 17/SDK 26.1, ARM64 C++20 Release `-O3 -mcpu=apple-m1
-ffp-contract=off`, no fast-math/LTO/sanitizer in timing, nearest/FZ off (FPCR 0).
Frequency/core placement is unpinned. Same guarded memory layout as baseline:
17-element batch-plane padding, data one scalar past allocation base, unaligned
loads/stores permitted. Benchmark formulas/inputs are unchanged; no runtime
source was edited for this run. All 28 pristine VCL2 vendor files retained
their exact bytes and modification timestamps. No commit, push or install occurred.

[Implementation contract](../validation/regression-tests.md) ·
[Original operation/protocol definitions](measurement-notes.md)

## 128 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #0969da;">1.932</span>** | **<span style="color: #0969da;">1.930</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 4.197 | 2.18× | no |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.377</span>** | **<span style="color: #0969da;">3.419</span>** | **<span style="color: #fdfbf9;">0.988×</span>** | Within&nbsp;5% | — | 4.211 | 1.23× | no |
| mat4_multiply | 32 | **<span style="color: #0969da;">2.129</span>** | **<span style="color: #0969da;">2.080</span>** | **<span style="color: #f8f6fc;">1.023×</span>** | Within&nbsp;5% | — | 9.785 | 4.70× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">4.348</span>** | **<span style="color: #0969da;">4.417</span>** | **<span style="color: #fdf9f7;">0.984×</span>** | Within&nbsp;5% | — | 10.231 | 2.32× | yes |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.269</span>** | **<span style="color: #0969da;">1.311</span>** | **<span style="color: #fbf3ee;">0.968×</span>** | Within&nbsp;5% | — | 2.417 | 1.84× | no |
| mat4_transpose | 64 | **<span style="color: #cf222e;">2.503</span>** | **<span style="color: #1a7f37;">2.336</span>** | **<span style="color: #eae4f6;">1.072×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +7.2% | 2.417 | 1.03× | yes |
| mat4_vector | 32 | **<span style="color: #0969da;">0.589</span>** | **<span style="color: #0969da;">0.586</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 2.482 | 4.23× | no |
| mat4_vector | 64 | **<span style="color: #0969da;">1.192</span>** | **<span style="color: #0969da;">1.219</span>** | **<span style="color: #fcf7f4;">0.978×</span>** | Within&nbsp;5% | — | 2.513 | 2.06× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.900</span>** | **<span style="color: #1a7f37;">0.617</span>** | **<span style="color: #7b51c6;">1.459×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +45.9% | 1.121 | 1.82× | yes |
| vec3_cross | 64 | **<span style="color: #cf222e;">0.895</span>** | **<span style="color: #1a7f37;">0.848</span>** | **<span style="color: #efeaf8;">1.055×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.5% | 1.123 | 1.32× | yes |
| vec4_add | 32 | **<span style="color: #0969da;">0.356</span>** | **<span style="color: #0969da;">0.354</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.918 | 2.59× | no |
| vec4_add | 64 | **<span style="color: #0969da;">0.673</span>** | **<span style="color: #0969da;">0.667</span>** | **<span style="color: #fcfcfe;">1.009×</span>** | Within&nbsp;5% | — | 0.919 | 1.38× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.395</span>** | **<span style="color: #0969da;">0.393</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.687 | 1.75× | no |
| vec4_dot | 64 | **<span style="color: #0969da;">0.511</span>** | **<span style="color: #0969da;">0.515</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 0.693 | 1.35× | no |
| vec4_length | 32 | **<span style="color: #0969da;">0.582</span>** | **<span style="color: #0969da;">0.581</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 0.677 | 1.16× | no |
| vec4_length | 64 | **<span style="color: #0969da;">0.578</span>** | **<span style="color: #0969da;">0.574</span>** | **<span style="color: #fdfcfe;">1.007×</span>** | Within&nbsp;5% | — | 0.614 | 1.07× | yes |
| vec4_normalize | 32 | **<span style="color: #0969da;">1.024</span>** | **<span style="color: #0969da;">1.025</span>** | **<span style="color: #fffffe;">0.999×</span>** | Within&nbsp;5% | — | 1.563 | 1.53× | no |
| vec4_normalize | 64 | **<span style="color: #0969da;">1.215</span>** | **<span style="color: #0969da;">1.209</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 1.622 | 1.34× | yes |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.359</span>** | **<span style="color: #0969da;">0.360</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 0.949 | 2.64× | no |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.612</span>** | **<span style="color: #0969da;">0.603</span>** | **<span style="color: #fbfafd;">1.014×</span>** | Within&nbsp;5% | — | 0.948 | 1.57× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">1.638</span>** | **<span style="color: #1a7f37;">1.478</span>** | **<span style="color: #e0d6f1;">1.109×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.9% | 4.654 | 3.15× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">2.738</span>** | **<span style="color: #0969da;">2.738</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 4.734 | 1.73× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.189</span>** | **<span style="color: #1a7f37;">3.314</span>** | **<span style="color: #6f42c1;">2.169×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +116.9% | 17.725 | 5.35× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">6.798</span>** | **<span style="color: #0969da;">6.787</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 17.769 | 2.62× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.601</span>** | **<span style="color: #0969da;">1.566</span>** | **<span style="color: #f9f7fc;">1.022×</span>** | Within&nbsp;5% | — | 3.317 | 2.12× | no |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.598</span>** | **<span style="color: #0969da;">2.598</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 3.318 | 1.28× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">3.085</span>** | **<span style="color: #1a7f37;">0.774</span>** | **<span style="color: #6f42c1;">3.986×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +298.6% | 2.999 | 3.88× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">1.575</span>** | **<span style="color: #0969da;">1.571</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 3.007 | 1.91× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.450</span>** | **<span style="color: #1a7f37;">0.381</span>** | **<span style="color: #cbbbe9;">1.181×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +18.1% | 0.835 | 2.19× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.594</span>** | **<span style="color: #0969da;">0.607</span>** | **<span style="color: #fcf7f4;">0.979×</span>** | Within&nbsp;5% | — | 0.836 | 1.38× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.462</span>** | **<span style="color: #1a7f37;">0.384</span>** | **<span style="color: #c5b3e6;">1.202×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +20.2% | 0.925 | 2.41× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.714</span>** | **<span style="color: #0969da;">0.720</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 0.921 | 1.28× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.407</span>** | **<span style="color: #1a7f37;">0.268</span>** | **<span style="color: #6f42c1;">1.519×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +51.9% | 0.849 | 3.17× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.554</span>** | **<span style="color: #0969da;">0.556</span>** | **<span style="color: #fefdfc;">0.995×</span>** | Within&nbsp;5% | — | 0.865 | 1.55× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.393</span>** | **<span style="color: #1a7f37;">0.237</span>** | **<span style="color: #6f42c1;">1.662×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.2% | 0.684 | 2.89× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.477</span>** | **<span style="color: #0969da;">0.476</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 0.636 | 1.34× | yes |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.640</span>** | **<span style="color: #1a7f37;">0.372</span>** | **<span style="color: #6f42c1;">1.718×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +71.8% | 1.553 | 4.17× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.793</span>** | **<span style="color: #0969da;">0.789</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 1.612 | 2.04× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.501</span>** | **<span style="color: #1a7f37;">0.384</span>** | **<span style="color: #a78cd9;">1.305×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +30.5% | 0.932 | 2.43× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.599</span>** | **<span style="color: #0969da;">0.604</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 0.930 | 1.54× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.189</span>** | **<span style="color: #1a7f37;">1.493</span>** | **<span style="color: #794fc5;">1.466×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.6% | 4.654 | 3.12× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">2.988</span>** | **<span style="color: #0969da;">3.021</span>** | **<span style="color: #fefbf9;">0.989×</span>** | Within&nbsp;5% | — | 4.734 | 1.57× | yes |
| mat4_multiply | 32 | **<span style="color: #cf222e;">4.930</span>** | **<span style="color: #1a7f37;">4.134</span>** | **<span style="color: #c7b6e7;">1.193×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +19.3% | 17.725 | 4.29× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">7.689</span>** | **<span style="color: #0969da;">7.800</span>** | **<span style="color: #fdfaf8;">0.986×</span>** | Within&nbsp;5% | — | 17.769 | 2.28× | yes |
| mat4_transpose | 32 | **<span style="color: #cf222e;">2.213</span>** | **<span style="color: #1a7f37;">1.410</span>** | **<span style="color: #6f42c1;">1.570×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +57.0% | 3.317 | 2.35× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">2.356</span>** | **<span style="color: #0969da;">2.351</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 3.318 | 1.41× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">1.502</span>** | **<span style="color: #1a7f37;">1.092</span>** | **<span style="color: #9371d0;">1.376×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +37.6% | 2.999 | 2.75× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.046</span>** | **<span style="color: #0969da;">2.040</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 3.007 | 1.47× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.357</span>** | **<span style="color: #1a7f37;">0.328</span>** | **<span style="color: #e6def4;">1.087×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.7% | 0.835 | 2.54× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.577</span>** | **<span style="color: #0969da;">0.588</span>** | **<span style="color: #fcf8f5;">0.980×</span>** | Within&nbsp;5% | — | 0.836 | 1.42× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.384</span>** | **<span style="color: #1a7f37;">0.345</span>** | **<span style="color: #ded4f1;">1.114×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +11.4% | 0.925 | 2.68× | yes |
| vec4_add | 64 | **<span style="color: #1a7f37;">0.556</span>** | **<span style="color: #cf222e;">0.596</span>** | **<span style="color: #f6e6db;">0.934×</span>** | <span style="color: #bc4c00;">sse2neon</span> | +7.1% | 0.921 | 1.55× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.288</span>** | **<span style="color: #1a7f37;">0.270</span>** | **<span style="color: #ebe5f7;">1.068×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.8% | 0.849 | 3.15× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.506</span>** | **<span style="color: #0969da;">0.507</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 0.865 | 1.70× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.394</span>** | **<span style="color: #1a7f37;">0.220</span>** | **<span style="color: #6f42c1;">1.790×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +79.0% | 0.684 | 3.11× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.410</span>** | **<span style="color: #0969da;">0.405</span>** | **<span style="color: #fcfafe;">1.012×</span>** | Within&nbsp;5% | — | 0.636 | 1.57× | yes |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.612</span>** | **<span style="color: #1a7f37;">0.403</span>** | **<span style="color: #6f42c1;">1.518×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +51.8% | 1.553 | 3.85× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.790</span>** | **<span style="color: #0969da;">0.784</span>** | **<span style="color: #fdfcfe;">1.008×</span>** | Within&nbsp;5% | — | 1.612 | 2.06× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.396</span>** | **<span style="color: #1a7f37;">0.349</span>** | **<span style="color: #d8ccee;">1.134×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +13.4% | 0.932 | 2.67× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.626</span>** | **<span style="color: #0969da;">0.620</span>** | **<span style="color: #fcfbfe;">1.010×</span>** | Within&nbsp;5% | — | 0.930 | 1.50× | no |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.757</span>** | **<span style="color: #1a7f37;">2.232</span>** | **<span style="color: #bba6e2;">1.235×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +23.5% | 4.654 | 2.09× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">4.741</span>** | **<span style="color: #0969da;">4.725</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 4.734 | 1.00× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">5.322</span>** | **<span style="color: #1a7f37;">4.281</span>** | **<span style="color: #b9a3e1;">1.243×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +24.3% | 17.725 | 4.14× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">10.231</span>** | **<span style="color: #0969da;">10.265</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 17.769 | 1.73× | no |
| mat4_transpose | 32 | **<span style="color: #cf222e;">2.484</span>** | **<span style="color: #1a7f37;">1.695</span>** | **<span style="color: #794fc5;">1.466×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.6% | 3.317 | 1.96× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">4.574</span>** | **<span style="color: #0969da;">4.522</span>** | **<span style="color: #fcfbfe;">1.011×</span>** | Within&nbsp;5% | — | 3.318 | 0.73× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">1.408</span>** | **<span style="color: #1a7f37;">1.223</span>** | **<span style="color: #d4c6ec;">1.151×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.1% | 2.999 | 2.45× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.674</span>** | **<span style="color: #0969da;">2.685</span>** | **<span style="color: #fefefd;">0.996×</span>** | Within&nbsp;5% | — | 3.007 | 1.12× | no |
| vec3_cross | 32 | **<span style="color: #0969da;">0.322</span>** | **<span style="color: #0969da;">0.311</span>** | **<span style="color: #f4f1fa;">1.037×</span>** | Within&nbsp;5% | — | 0.835 | 2.69× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">0.566</span>** | **<span style="color: #0969da;">0.573</span>** | **<span style="color: #fdfbf9;">0.988×</span>** | Within&nbsp;5% | — | 0.836 | 1.46× | no |
| vec4_add | 32 | **<span style="color: #0969da;">0.336</span>** | **<span style="color: #0969da;">0.327</span>** | **<span style="color: #f7f4fc;">1.028×</span>** | Within&nbsp;5% | — | 0.925 | 2.83× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">0.549</span>** | **<span style="color: #0969da;">0.554</span>** | **<span style="color: #fefbfa;">0.990×</span>** | Within&nbsp;5% | — | 0.921 | 1.66× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.260</span>** | **<span style="color: #0969da;">0.253</span>** | **<span style="color: #f7f5fc;">1.027×</span>** | Within&nbsp;5% | — | 0.849 | 3.35× | no |
| vec4_dot | 64 | **<span style="color: #0969da;">0.485</span>** | **<span style="color: #0969da;">0.497</span>** | **<span style="color: #fcf7f3;">0.977×</span>** | Within&nbsp;5% | — | 0.865 | 1.74× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.397</span>** | **<span style="color: #1a7f37;">0.199</span>** | **<span style="color: #6f42c1;">1.998×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +99.8% | 0.684 | 3.45× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.400</span>** | **<span style="color: #0969da;">0.397</span>** | **<span style="color: #fdfcfe;">1.007×</span>** | Within&nbsp;5% | — | 0.636 | 1.60× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.632</span>** | **<span style="color: #1a7f37;">0.448</span>** | **<span style="color: #8964cc;">1.411×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +41.1% | 1.553 | 3.47× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.856</span>** | **<span style="color: #0969da;">0.851</span>** | **<span style="color: #fdfcfe;">1.007×</span>** | Within&nbsp;5% | — | 1.612 | 1.90× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.367</span>** | **<span style="color: #1a7f37;">0.339</span>** | **<span style="color: #e7e0f5;">1.082×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.2% | 0.932 | 2.75× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.637</span>** | **<span style="color: #0969da;">0.660</span>** | **<span style="color: #faf2ed;">0.965×</span>** | Within&nbsp;5% | — | 0.930 | 1.41× | yes |

## 4,096 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #0969da;">1.960</span>** | **<span style="color: #0969da;">1.966</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 4.211 | 2.14× | no |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.399</span>** | **<span style="color: #0969da;">3.425</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 4.494 | 1.31× | yes |
| mat4_multiply | 32 | **<span style="color: #0969da;">2.147</span>** | **<span style="color: #0969da;">2.154</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 11.030 | 5.12× | no |
| mat4_multiply | 64 | **<span style="color: #0969da;">4.391</span>** | **<span style="color: #0969da;">4.400</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 12.295 | 2.79× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">1.333</span>** | **<span style="color: #0969da;">1.296</span>** | **<span style="color: #f7f4fb;">1.029×</span>** | Within&nbsp;5% | — | 2.772 | 2.14× | no |
| mat4_transpose | 64 | **<span style="color: #cf222e;">2.646</span>** | **<span style="color: #1a7f37;">2.344</span>** | **<span style="color: #daceef;">1.129×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +12.9% | 3.384 | 1.44× | yes |
| mat4_vector | 32 | **<span style="color: #0969da;">0.925</span>** | **<span style="color: #0969da;">0.933</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 2.789 | 2.99× | no |
| mat4_vector | 64 | **<span style="color: #0969da;">1.876</span>** | **<span style="color: #0969da;">1.860</span>** | **<span style="color: #fcfcfe;">1.009×</span>** | Within&nbsp;5% | — | 3.239 | 1.74× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.886</span>** | **<span style="color: #1a7f37;">0.613</span>** | **<span style="color: #7f56c8;">1.446×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +44.6% | 1.149 | 1.87× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.010</span>** | **<span style="color: #0969da;">1.000</span>** | **<span style="color: #fcfcfe;">1.009×</span>** | Within&nbsp;5% | — | 1.159 | 1.16× | no |
| vec4_add | 32 | **<span style="color: #0969da;">0.315</span>** | **<span style="color: #0969da;">0.314</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 0.922 | 2.94× | no |
| vec4_add | 64 | **<span style="color: #0969da;">0.980</span>** | **<span style="color: #0969da;">0.981</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 1.127 | 1.15× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.362</span>** | **<span style="color: #0969da;">0.365</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 0.693 | 1.90× | no |
| vec4_dot | 64 | **<span style="color: #0969da;">0.730</span>** | **<span style="color: #0969da;">0.724</span>** | **<span style="color: #fdfcfe;">1.008×</span>** | Within&nbsp;5% | — | 0.758 | 1.05× | no |
| vec4_length | 32 | **<span style="color: #0969da;">0.582</span>** | **<span style="color: #0969da;">0.587</span>** | **<span style="color: #fefcfa;">0.991×</span>** | Within&nbsp;5% | — | 0.676 | 1.15× | no |
| vec4_length | 64 | **<span style="color: #0969da;">0.587</span>** | **<span style="color: #0969da;">0.584</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.624 | 1.07× | no |
| vec4_normalize | 32 | **<span style="color: #0969da;">1.012</span>** | **<span style="color: #0969da;">1.020</span>** | **<span style="color: #fefcfb;">0.992×</span>** | Within&nbsp;5% | — | 1.565 | 1.53× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">1.217</span>** | **<span style="color: #0969da;">1.213</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 1.655 | 1.36× | no |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.317</span>** | **<span style="color: #0969da;">0.319</span>** | **<span style="color: #fefdfc;">0.995×</span>** | Within&nbsp;5% | — | 0.954 | 2.99× | no |
| vec4_scale_add | 64 | **<span style="color: #0969da;">0.988</span>** | **<span style="color: #0969da;">0.985</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 1.131 | 1.15× | no |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">2.493</span>** | **<span style="color: #1a7f37;">2.363</span>** | **<span style="color: #efeaf8;">1.055×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.5% | 4.620 | 1.96× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.755</span>** | **<span style="color: #0969da;">3.778</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 4.799 | 1.27× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.189</span>** | **<span style="color: #1a7f37;">3.884</span>** | **<span style="color: #6f42c1;">1.851×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +85.1% | 18.723 | 4.82× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">7.208</span>** | **<span style="color: #0969da;">7.193</span>** | **<span style="color: #fefeff;">1.002×</span>** | Within&nbsp;5% | — | 19.710 | 2.74× | no |
| mat4_transpose | 32 | **<span style="color: #0969da;">2.610</span>** | **<span style="color: #0969da;">2.506</span>** | **<span style="color: #f3effa;">1.042×</span>** | Within&nbsp;5% | — | 4.581 | 1.83× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">4.071</span>** | **<span style="color: #0969da;">4.086</span>** | **<span style="color: #fefefd;">0.996×</span>** | Within&nbsp;5% | — | 4.637 | 1.13× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">3.192</span>** | **<span style="color: #1a7f37;">1.490</span>** | **<span style="color: #6f42c1;">2.142×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +114.2% | 3.150 | 2.11× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.266</span>** | **<span style="color: #0969da;">2.267</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 3.202 | 1.41× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.703</span>** | **<span style="color: #1a7f37;">0.622</span>** | **<span style="color: #d9cdef;">1.131×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +13.1% | 1.155 | 1.86× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.397</span>** | **<span style="color: #0969da;">1.391</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 1.489 | 1.07× | yes |
| vec4_add | 32 | **<span style="color: #cf222e;">0.978</span>** | **<span style="color: #1a7f37;">0.799</span>** | **<span style="color: #beaae3;">1.224×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +22.4% | 1.376 | 1.72× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">1.701</span>** | **<span style="color: #0969da;">1.685</span>** | **<span style="color: #fcfbfe;">1.010×</span>** | Within&nbsp;5% | — | 1.818 | 1.08× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.416</span>** | **<span style="color: #1a7f37;">0.288</span>** | **<span style="color: #7f57c8;">1.445×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +44.5% | 0.861 | 2.99× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.777</span>** | **<span style="color: #0969da;">0.779</span>** | **<span style="color: #fffefd;">0.997×</span>** | Within&nbsp;5% | — | 1.165 | 1.50× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.395</span>** | **<span style="color: #1a7f37;">0.237</span>** | **<span style="color: #6f42c1;">1.666×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.6% | 0.665 | 2.81× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.481</span>** | **<span style="color: #0969da;">0.478</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.610 | 1.28× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.638</span>** | **<span style="color: #1a7f37;">0.398</span>** | **<span style="color: #6f42c1;">1.602×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +60.2% | 1.548 | 3.89× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.960</span>** | **<span style="color: #0969da;">0.957</span>** | **<span style="color: #fefdff;">1.004×</span>** | Within&nbsp;5% | — | 1.683 | 1.76× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.975</span>** | **<span style="color: #1a7f37;">0.807</span>** | **<span style="color: #c3b0e5;">1.208×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +20.8% | 1.379 | 1.71× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.717</span>** | **<span style="color: #0969da;">1.695</span>** | **<span style="color: #fbfafd;">1.013×</span>** | Within&nbsp;5% | — | 1.828 | 1.08× | no |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">3.366</span>** | **<span style="color: #1a7f37;">2.208</span>** | **<span style="color: #6f42c1;">1.524×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +52.4% | 4.620 | 2.09× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">3.512</span>** | **<span style="color: #0969da;">3.582</span>** | **<span style="color: #fcf8f5;">0.980×</span>** | Within&nbsp;5% | — | 4.799 | 1.34× | yes |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.281</span>** | **<span style="color: #1a7f37;">6.007</span>** | **<span style="color: #c2afe5;">1.212×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +21.2% | 18.723 | 3.12× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">10.220</span>** | **<span style="color: #0969da;">10.189</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 19.710 | 1.93× | no |
| mat4_transpose | 32 | **<span style="color: #cf222e;">3.371</span>** | **<span style="color: #1a7f37;">2.337</span>** | **<span style="color: #7f58c8;">1.443×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +44.3% | 4.581 | 1.96× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">3.721</span>** | **<span style="color: #0969da;">3.738</span>** | **<span style="color: #fefdfc;">0.995×</span>** | Within&nbsp;5% | — | 4.637 | 1.24× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">2.198</span>** | **<span style="color: #1a7f37;">1.585</span>** | **<span style="color: #906dcf;">1.387×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +38.7% | 3.150 | 1.99× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">2.567</span>** | **<span style="color: #0969da;">2.582</span>** | **<span style="color: #fefdfc;">0.994×</span>** | Within&nbsp;5% | — | 3.202 | 1.24× | no |
| vec3_cross | 32 | **<span style="color: #cf222e;">0.642</span>** | **<span style="color: #1a7f37;">0.569</span>** | **<span style="color: #daceef;">1.129×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +12.9% | 1.155 | 2.03× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.367</span>** | **<span style="color: #0969da;">1.367</span>** | **<span style="color: #ffffff;">1.000×</span>** | Within&nbsp;5% | — | 1.489 | 1.09× | no |
| vec4_add | 32 | **<span style="color: #cf222e;">0.889</span>** | **<span style="color: #1a7f37;">0.763</span>** | **<span style="color: #cfc1eb;">1.165×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +16.5% | 1.376 | 1.80× | yes |
| vec4_add | 64 | **<span style="color: #0969da;">1.716</span>** | **<span style="color: #0969da;">1.714</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 1.818 | 1.06× | no |
| vec4_dot | 32 | **<span style="color: #cf222e;">0.312</span>** | **<span style="color: #1a7f37;">0.287</span>** | **<span style="color: #e6def4;">1.087×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.7% | 0.861 | 3.00× | yes |
| vec4_dot | 64 | **<span style="color: #0969da;">0.780</span>** | **<span style="color: #0969da;">0.777</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 1.165 | 1.50× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.394</span>** | **<span style="color: #1a7f37;">0.221</span>** | **<span style="color: #6f42c1;">1.782×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +78.2% | 0.665 | 3.01× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.409</span>** | **<span style="color: #0969da;">0.411</span>** | **<span style="color: #fefdfc;">0.995×</span>** | Within&nbsp;5% | — | 0.610 | 1.49× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.622</span>** | **<span style="color: #1a7f37;">0.414</span>** | **<span style="color: #6f42c1;">1.503×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +50.3% | 1.548 | 3.74× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.917</span>** | **<span style="color: #0969da;">0.916</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 1.683 | 1.84× | no |
| vec4_scale_add | 32 | **<span style="color: #cf222e;">0.890</span>** | **<span style="color: #1a7f37;">0.769</span>** | **<span style="color: #d1c3eb;">1.158×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.8% | 1.379 | 1.79× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.726</span>** | **<span style="color: #0969da;">1.709</span>** | **<span style="color: #fcfbfe;">1.010×</span>** | Within&nbsp;5% | — | 1.828 | 1.07× | no |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **<span style="color: #cf222e;">4.421</span>** | **<span style="color: #1a7f37;">4.152</span>** | **<span style="color: #ece6f7;">1.065×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.5% | 4.620 | 1.11× | yes |
| mat4_inverse_trs | 64 | **<span style="color: #0969da;">6.727</span>** | **<span style="color: #0969da;">6.778</span>** | **<span style="color: #fefcfb;">0.993×</span>** | Within&nbsp;5% | — | 4.799 | 0.71× | no |
| mat4_multiply | 32 | **<span style="color: #cf222e;">7.396</span>** | **<span style="color: #1a7f37;">6.825</span>** | **<span style="color: #e7dff5;">1.084×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.4% | 18.723 | 2.74× | yes |
| mat4_multiply | 64 | **<span style="color: #0969da;">12.478</span>** | **<span style="color: #0969da;">12.372</span>** | **<span style="color: #fcfcfe;">1.009×</span>** | Within&nbsp;5% | — | 19.710 | 1.59× | no |
| mat4_transpose | 32 | **<span style="color: #cf222e;">4.088</span>** | **<span style="color: #1a7f37;">3.089</span>** | **<span style="color: #a285d7;">1.323×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +32.3% | 4.581 | 1.48× | yes |
| mat4_transpose | 64 | **<span style="color: #0969da;">6.521</span>** | **<span style="color: #0969da;">6.512</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 4.637 | 0.71× | no |
| mat4_vector | 32 | **<span style="color: #cf222e;">2.573</span>** | **<span style="color: #1a7f37;">2.004</span>** | **<span style="color: #ad94dc;">1.284×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +28.4% | 3.150 | 1.57× | yes |
| mat4_vector | 64 | **<span style="color: #0969da;">3.598</span>** | **<span style="color: #0969da;">3.594</span>** | **<span style="color: #ffffff;">1.001×</span>** | Within&nbsp;5% | — | 3.202 | 0.89× | no |
| vec3_cross | 32 | **<span style="color: #0969da;">0.544</span>** | **<span style="color: #0969da;">0.526</span>** | **<span style="color: #f5f3fb;">1.033×</span>** | Within&nbsp;5% | — | 1.155 | 2.19× | yes |
| vec3_cross | 64 | **<span style="color: #0969da;">1.329</span>** | **<span style="color: #0969da;">1.324</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 1.489 | 1.12× | no |
| vec4_add | 32 | **<span style="color: #0969da;">0.722</span>** | **<span style="color: #0969da;">0.713</span>** | **<span style="color: #fbfafd;">1.013×</span>** | Within&nbsp;5% | — | 1.376 | 1.93× | no |
| vec4_add | 64 | **<span style="color: #0969da;">1.663</span>** | **<span style="color: #0969da;">1.653</span>** | **<span style="color: #fdfdfe;">1.006×</span>** | Within&nbsp;5% | — | 1.818 | 1.10× | no |
| vec4_dot | 32 | **<span style="color: #0969da;">0.275</span>** | **<span style="color: #0969da;">0.274</span>** | **<span style="color: #fefeff;">1.003×</span>** | Within&nbsp;5% | — | 0.861 | 3.14× | no |
| vec4_dot | 64 | **<span style="color: #0969da;">0.763</span>** | **<span style="color: #0969da;">0.765</span>** | **<span style="color: #fffefe;">0.998×</span>** | Within&nbsp;5% | — | 1.165 | 1.52× | no |
| vec4_length | 32 | **<span style="color: #cf222e;">0.393</span>** | **<span style="color: #1a7f37;">0.201</span>** | **<span style="color: #6f42c1;">1.961×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +96.1% | 0.665 | 3.32× | yes |
| vec4_length | 64 | **<span style="color: #0969da;">0.403</span>** | **<span style="color: #0969da;">0.401</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 0.610 | 1.52× | no |
| vec4_normalize | 32 | **<span style="color: #cf222e;">0.624</span>** | **<span style="color: #1a7f37;">0.453</span>** | **<span style="color: #9270d0;">1.377×</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +37.7% | 1.548 | 3.42× | yes |
| vec4_normalize | 64 | **<span style="color: #0969da;">0.927</span>** | **<span style="color: #0969da;">0.923</span>** | **<span style="color: #fefdfe;">1.005×</span>** | Within&nbsp;5% | — | 1.683 | 1.82× | no |
| vec4_scale_add | 32 | **<span style="color: #0969da;">0.760</span>** | **<span style="color: #0969da;">0.732</span>** | **<span style="color: #f4f1fa;">1.038×</span>** | Within&nbsp;5% | — | 1.379 | 1.88× | yes |
| vec4_scale_add | 64 | **<span style="color: #0969da;">1.668</span>** | **<span style="color: #0969da;">1.689</span>** | **<span style="color: #fdfbf9;">0.988×</span>** | Within&nbsp;5% | — | 1.828 | 1.08× | yes |

## Source, host and controls

- Tested commit: `3abf7af84d5ed8375f00c2a184d8a0e55a7b951e`.
- Tested tree: `19e520999effe6acea50135e5dc8bc6f0ca38a71`.
- Pre-layout comparison: `44f974ac31e1448e0beb5157d2bd8d669f5c518e`.
  All five benchmark source/build files matched exactly. The intervening
  MSVC-only optimization did not alter these ARM benchmark executables.
- Apple M2, native arm64, macOS 26.7 (25G229), Rosetta off. Apple Clang 17.0.0
  (`clang-1700.4.4.1`), SDK 26.1, CMake/CTest 4.4.4, Ninja 1.11.1.
- Run interval: 2026-11-16 23:53:00–23:55:25 UTC, November 17 in Stockholm.
- C++20 Release, `-O3 -mcpu=apple-m1 -ffp-contract=off`. Scalar additionally
  uses `-fno-vectorize -fno-slp-vectorize`; disassembly contains no packed FP
  arithmetic. No fast-math, LTO or sanitizer is used for timing.
- [Correctness checks](standard-math-2026-11-17/correctness.json): all three
  backends passed in pre-layout Release, current Release and current strict
  UBSan Debug (9/9, zero failures/skips/timeouts/retries). Debug uses
  `-fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer`
  and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`.
- Generator read-only checks before/after were identical. All 28 original
  `vendor/vectorclass/` files and the pin manifest retained exact hashes and
  modification timestamps. No vendor, canonical source, generated header or
  benchmark source was changed. The checkout stayed clean throughout execution;
  report/link edits were prepared afterward.

[Kernel comparison](standard-math-2026-11-17/codegen-comparison.json) and
[whole-binary comparison](standard-math-2026-11-17/whole-binary-comparison.json)
retain both builds' hashes. This code-generation check directly addresses the
question of preserving original VCL2 sources behind explicit adaptation.

## Measurement and correctness scope

Ten operations, FP32/FP64, counts 128/4096, object AoS and batch SoA with logical
128/256/512-bit vectors follow the unchanged [protocol](../../benchmarks/README.md).
Hardware Neon registers remain 128-bit. Three rounds rotate the backend order:
scalar/compatibility/Neon, compatibility/Neon/scalar, then Neon/scalar/compatibility.
Each backend/count combination runs three times, sequentially; every shape has
nine warmed, calibrated trials. Total: **18 processes, 1,200 shapes and 10,800
trial batches**, summarized as medians of invocation medians. Scalar batch
controls are reused across logical widths within this run only.

Before and after timing, the harness made **35,481,600 output-field reference
checks** (17,740,800 per pass), retaining the independent coordinate-loop,
`hypot` and pivoted Gauss-Jordan references, input hashes and canaries. Maximum
normalized reference error was **6**, within the unchanged **64-epsilon gate**.
FPCR was zero for every shape (nearest rounding, flush-to-zero off). TRS inverse
remains restricted to nonzero orthogonal scaled affine columns.

All three invocations reproduced identical output bytes within each backend
and shape. Across the 160 row comparisons, first-invocation output comparisons
found 7,913 differing scalar/Neon fields (maximum raw distance 768 ULP in FP32
dot, maximum absolute difference for that row 9.54e-7) and 21,084 differing
compatibility/Neon fields (maximum 4 ULP). Scalar rows are reused across widths
in these totals. These are not universal bitwise-identity results: the existing
reference gate scales error by the operation's magnitude/conditioning, so raw
ULP distance near cancellation is not itself the acceptance test. All backends
passed that same independent gate without changes.

CPU frequency/core placement was unpinned; Caffeinate prevented idle sleep.
Raw batch durations span **6.44–70.81 ms**, with all scheduler outliers retained.
Load averages were 1.59/1.65/2.33 before and 3.66/2.43/2.54 after; macOS reported
no recorded thermal/performance warning. These observations do not establish
an otherwise idle host. Invocation median ranges are disjoint in 75/160 rows;
this is descriptive consistency, not statistical significance. All 56 Direct
Neon leads have disjoint ranges. The sole compatibility lead, FP64 batch
`vec4_add`, width 256/count 128, has overlapping ranges and should be treated
cautiously. Four Direct Neon rows and eight compatibility rows are slower than
the corresponding scalar control; those results are retained too.

## Evidence and next use

All original directories remain under ignored
`build/mac-m2-benchmark-20261117-235141Z/`, including both source builds,
executables, raw trials, all 1,200 output dumps, flags/source/binary inventories,
logs, JUnit, input hashes and code-generation evidence. The checksummed
`benchmark-evidence.zip` contains 1,312 verified records; SHA256:
`d59077e189b525252531dd95bcb770e6265a61940d040eef7ea578fb2bda149c`.
The [archive receipt](standard-math-2026-11-17/archive-receipt.json) identifies
its scope. The later whole-binary comparison is retained separately and included
with the portable data/checksums. No evidence or build worktree was cleaned up.

This run supplies a baseline for future M2 changes. Rebuild and remeasure at a
new recorded source if ARM runtime code changes; compare matched controls from
that new run. No Windows readiness or newly fixed higher-ISA result is inferred.
No commit, push, merge or PR was performed during measurement. The maintainer
subsequently authorized committing/pushing the report, portable data and reporting
workflow on 2026-11-17. The measured source remains `3abf7af`.
