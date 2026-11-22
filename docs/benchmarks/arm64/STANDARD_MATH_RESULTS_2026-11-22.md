# Standard vector and matrix benchmarks - 2026-11-22

**Scalar FP32/FP64 versus <span style="color: #bc4c00;">sse2neon</span> versus <span style="color: #6f42c1;">Direct&nbsp;Neon</span>**:
ten standard vector/matrix operations, FP32/FP64, 128/4,096 objects, AoS and SoA widths 128/256/512. The tables describe the native measurements from
**2026-11-22**, at source `c120ded4321a`.

**Run status:** Full native correctness and benchmark correctness passed, followed by all 18 timing invocations and 160 report rows. Two SDK environment setup failures are retained in setup-attempts.json..

Fresh baseline after the authorized local build reset. 65 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads, 94 within the inclusive 5% band, and 1 compatibility leads. Each result belongs to this frozen source and installed toolchain.

All slower and variable rows are retained. Before/after comparisons and full
suite runs use their own matched controls; do not mix times between runs.

- CSV results: [results.csv](STANDARD_MATH_RESULTS_2026-11-22/candidate/results.csv)
- JSON results and raw trials: [results.json](STANDARD_MATH_RESULTS_2026-11-22/candidate/results.json)
- Summary/audit: [summary.json](STANDARD_MATH_RESULTS_2026-11-22/candidate/summary.json)
- Metadata and checksums: `metadata.json`, `checksums.sha256`

In this run, <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins or is within
the shared 5% band in **159/160 rows**:
**65** <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads, **94** within 5%,
and **1** <span style="color: #bc4c00;">sse2neon</span> leads.

52/160 rows have disjoint invocation-median ranges. 20 leads outside the 5% band still have overlapping ranges. All 160 rows are retained, including 5 Neon and 5 compatibility row/backend instances slower than scalar; the complete inventory is in summary.json.

**Reporting correction — 22 November 2026:** Four SoA rows at 4,096 objects
exceed 1.05 at full precision but were previously classified within 5% after
rounding to 1.050. They are now <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads: FP64 cross product and vector
length at logical 512 bits, and scale-add at FP32/512 bits and FP64/256 bits.
All four have overlapping invocation ranges. Raw samples, measured times,
source identities and correctness outcomes are unchanged; the
[publication receipt](STANDARD_MATH_RESULTS_2026-11-22/publication-receipt.json)
preserves original hashes and records the derived-field corrections.

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

Each row compares the same operation, precision and input data on the two SIMD
backends. Compare their **nsec/op** values first: a lower time means more
operations can be completed per second. The ratio and winner columns summarize
that comparison using the 5% band defined below.

| Column | Meaning |
| --- | --- |
| Operation | The complete vector or matrix calculation being measured. |
| FP | Component precision: 32 for `float`, 64 for `double`. |
| **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | Time per operation using the SSE-to-Neon compatibility backend. Lower is better. |
| **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | Time per operation using the <span style="color: #6f42c1;">Direct&nbsp;Neon</span> backend. Lower is better. |
| **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span>'s throughput relative to <span style="color: #bc4c00;">sse2neon</span>: **<span style="color: #bc4c00;">sse2neon</span> time ÷ <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time**. Above 1 favors <span style="color: #6f42c1;">Direct&nbsp;Neon</span>; below 1 favors <span style="color: #bc4c00;">sse2neon</span>. |
| **Winning<br>backend** | The backend ahead outside the 5% band, or **Within&nbsp;5%** when the ratio falls inside it. |
| **Throughput<br>gain&nbsp;(%)** | The named winner's percentage throughput lead over the other backend. **—** means the comparison is within the 5% band. |
| **Scalar<br>(nsec/op)** | Reference time using ordinary `float`/`double` code with automatic SIMD vectorization disabled. |
| Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Scalar time ÷ <span style="color: #6f42c1;">Direct&nbsp;Neon</span> time. A value of **2×** means <span style="color: #6f42c1;">Direct&nbsp;Neon</span> delivers twice the scalar throughput. |
| Consistent<br>Timing | **Yes:** the ranges of the three invocation medians for the two SIMD backends do not overlap. **No:** those ranges overlap. This describes the observed timing separation, not statistical significance. |

**Timing markers apply to the <span style="color: #bc4c00;">sse2neon</span> and <span style="color: #6f42c1;">Direct&nbsp;Neon</span> nsec/op columns.**
They identify the lower and higher times when the ratio falls outside the 5%
band. Both timing cells use 🔵 when the comparison falls within that band.

| Timing marker | Meaning |
| --- | --- |
| 🟢 <span style="color: #1a7f37;">**Winning time**</span> | The lower time for the backend classified as the winner |
| 🔴 <span style="color: #cf222e;">**Losing time**</span> | The higher time for the other backend |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | The comparison is inside the practical 5% band; the displayed times can still differ |

**Throughput markers apply to the <span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span> ratio and Winning
backend columns.** Both columns use the same decision:

| Throughput marker | Meaning | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> / <span style="color: #bc4c00;">sse2neon</span> ratio |
| --- | --- | --- |
| 🟣 **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins** | <span style="color: #6f42c1;">Direct&nbsp;Neon</span> has higher throughput beyond the margin | Above 1.05× |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | Neither backend is declared the winner | 0.95–1.05×, inclusive |
| 🟠 **<span style="color: #bc4c00;">sse2neon</span> wins** | <span style="color: #bc4c00;">sse2neon</span> has higher throughput beyond the margin | Below 0.95× |

The **5% band** and **Consistent Timing** answer different questions. A row can
be blue and still say **Yes**: the observed invocation ranges are separated, but
the throughput difference remains inside the practical band. Winner labels and
colors compare the two SIMD backends; use **Scalar / <span style="color: #6f42c1;">Direct&nbsp;Neon</span>** for the scalar
comparison.

**Throughput gain is not the same percentage as elapsed-time reduction.** For
the unrounded ratio `r`, the gain is `100 × (r − 1)%` when <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins, or
`100 × (1/r − 1)%` when <span style="color: #bc4c00;">sse2neon</span> wins. Thus **1.25×** gives <span style="color: #6f42c1;">Direct&nbsp;Neon</span> a **25%**
throughput lead, while **0.80×** gives <span style="color: #bc4c00;">sse2neon</span> a **25%** throughput lead.
Classifications, colors and gains use the unrounded measurements; rounding is
only for display. A ratio displayed as **1.050×** can therefore be a <span style="color: #6f42c1;">Direct&nbsp;Neon</span>
win when its full-precision value exceeds 1.05. Reported gains are rounded to
one decimal place. Dividing the displayed timing values can give a slightly
different result.

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

For an illustrative row with **<span style="color: #bc4c00;">sse2neon</span> = 1.000 nsec/op** and
**<span style="color: #6f42c1;">Direct&nbsp;Neon</span> = 0.800 nsec/op**, the timing cells show 🔴 for <span style="color: #bc4c00;">sse2neon</span> and
🟢 for <span style="color: #6f42c1;">Direct&nbsp;Neon</span>. The ratio cell is 🟣 **1.250×**, Winning backend is
🟣 **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>**, and Throughput gain is **+25.0%**. This corresponds to
**20% less elapsed time** for the same work.

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
1200 shapes / 10800 trial batches.
**Correctness summary:** Passed: unchanged independent reference gates, stable input/output hashes and canaries; no timing-output changes.
**Host and build summary:** Apple M2; macOS 26.7 (25G229); Apple Clang 21; native arm64; exact SDK/tool versions and flags are recorded below and in metadata.json.

<!-- UPDATE ALL TABLES: Match rows by object count, AoS/SoA, logical width,
operation and FP precision. Fill every result cell from this run's data.
Timing placeholders are intentionally uncolored: after filling them, apply
the reading guide's green/red/blue timing rule, solid winner ratio colors,
and fixed purple/orange backend label colors. Add 🟢/🔴/🔵 timing markers and
the matching 🟣/🔵/🟠 winner markers to the ratio and Winning backend columns
for GitHub readability.
Recompute both ratios, the
winning backend, throughput gain and consistency; never retain old colors.
Default display: three decimals for ns/op and compatibility/Neon ratio, two
for scalar/Neon, one decimal for percentage gain. Derive ratios from unrounded
measurements; use the displayed comparison ratio for the verdict/gain colors
as documented in the reading guide. Keep full precision and every raw trial
in JSON. Use an explicit unavailable status for unmeasured values, never zero. -->

## 128 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">1.986</span>** | 🔵 **<span style="color: #0969da;">2.071</span>** | 🔵 **<span style="color: #0969da;">0.959×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.373 | 2.11× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.507</span>** | 🔵 **<span style="color: #0969da;">3.497</span>** | 🔵 **<span style="color: #0969da;">1.003×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.427 | 1.27× | No |
| mat4_multiply | 32 | 🔵 **<span style="color: #0969da;">2.227</span>** | 🔵 **<span style="color: #0969da;">2.167</span>** | 🔵 **<span style="color: #0969da;">1.028×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 10.256 | 4.73× | No |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">4.430</span>** | 🔵 **<span style="color: #0969da;">4.430</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 10.711 | 2.42× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.263</span>** | 🔵 **<span style="color: #0969da;">1.286</span>** | 🔵 **<span style="color: #0969da;">0.981×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.485 | 1.93× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.682</span>** | 🔵 **<span style="color: #0969da;">2.575</span>** | 🔵 **<span style="color: #0969da;">1.041×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.485 | 0.96× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">0.608</span>** | 🔵 **<span style="color: #0969da;">0.608</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.600 | 4.27× | No |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.232</span>** | 🔵 **<span style="color: #0969da;">1.234</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.596 | 2.10× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.939</span>** | 🟢 **<span style="color: #1a7f37;">0.640</span>** | 🟣 **<span style="color: #6f42c1;">1.466×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.6% | 1.169 | 1.83× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.904</span>** | 🔵 **<span style="color: #0969da;">0.866</span>** | 🔵 **<span style="color: #0969da;">1.044×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.167 | 1.35× | Yes |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.366</span>** | 🔵 **<span style="color: #0969da;">0.365</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.949 | 2.60× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.680</span>** | 🔵 **<span style="color: #0969da;">0.692</span>** | 🔵 **<span style="color: #0969da;">0.983×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.947 | 1.37× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.410</span>** | 🔵 **<span style="color: #0969da;">0.410</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.718 | 1.75× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.533</span>** | 🔵 **<span style="color: #0969da;">0.532</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.718 | 1.35× | No |
| vec4_length | 32 | 🔵 **<span style="color: #0969da;">0.607</span>** | 🔵 **<span style="color: #0969da;">0.608</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.713 | 1.17× | No |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.603</span>** | 🔵 **<span style="color: #0969da;">0.603</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.642 | 1.07× | No |
| vec4_normalize | 32 | 🔵 **<span style="color: #0969da;">1.061</span>** | 🔵 **<span style="color: #0969da;">1.061</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.624 | 1.53× | No |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">1.253</span>** | 🔵 **<span style="color: #0969da;">1.255</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.672 | 1.33× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.366</span>** | 🔵 **<span style="color: #0969da;">0.367</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.985 | 2.68× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.607</span>** | 🔵 **<span style="color: #0969da;">0.607</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.984 | 1.62× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">1.697</span>** | 🟢 **<span style="color: #1a7f37;">1.543</span>** | 🟣 **<span style="color: #6f42c1;">1.099×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +9.9% | 4.882 | 3.16× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">2.854</span>** | 🔵 **<span style="color: #0969da;">2.869</span>** | 🔵 **<span style="color: #0969da;">0.995×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 5.009 | 1.75× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">4.045</span>** | 🟢 **<span style="color: #1a7f37;">3.459</span>** | 🟣 **<span style="color: #6f42c1;">1.169×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +16.9% | 17.982 | 5.20× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">7.041</span>** | 🔵 **<span style="color: #0969da;">7.055</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 18.067 | 2.56× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.597</span>** | 🔵 **<span style="color: #0969da;">1.638</span>** | 🔵 **<span style="color: #0969da;">0.975×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.473 | 2.12× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.711</span>** | 🔵 **<span style="color: #0969da;">2.718</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.475 | 1.28× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">1.356</span>** | 🟢 **<span style="color: #1a7f37;">0.817</span>** | 🟣 **<span style="color: #6f42c1;">1.660×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.0% | 3.127 | 3.83× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.639</span>** | 🔵 **<span style="color: #0969da;">1.640</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.123 | 1.90× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.460</span>** | 🟢 **<span style="color: #1a7f37;">0.397</span>** | 🟣 **<span style="color: #6f42c1;">1.157×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.7% | 0.872 | 2.19× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.616</span>** | 🔵 **<span style="color: #0969da;">0.613</span>** | 🔵 **<span style="color: #0969da;">1.004×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.42× | Yes |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.479</span>** | 🟢 **<span style="color: #1a7f37;">0.399</span>** | 🟣 **<span style="color: #6f42c1;">1.200×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +20.0% | 0.957 | 2.40× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.768</span>** | 🔵 **<span style="color: #0969da;">0.768</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.961 | 1.25× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.422</span>** | 🟢 **<span style="color: #1a7f37;">0.279</span>** | 🟣 **<span style="color: #6f42c1;">1.514×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +51.4% | 0.867 | 3.11× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.573</span>** | 🔵 **<span style="color: #0969da;">0.574</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.893 | 1.56× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.410</span>** | 🟢 **<span style="color: #1a7f37;">0.247</span>** | 🟣 **<span style="color: #6f42c1;">1.663×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.3% | 0.713 | 2.89× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.496</span>** | 🔵 **<span style="color: #0969da;">0.497</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.645 | 1.30× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.653</span>** | 🟢 **<span style="color: #1a7f37;">0.384</span>** | 🟣 **<span style="color: #6f42c1;">1.701×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +70.1% | 1.620 | 4.22× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.675 | 2.06× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.518</span>** | 🟢 **<span style="color: #1a7f37;">0.402</span>** | 🟣 **<span style="color: #6f42c1;">1.290×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +29.0% | 0.961 | 2.39× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.616</span>** | 🔵 **<span style="color: #0969da;">0.616</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.966 | 1.57× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">1.596</span>** | 🔵 **<span style="color: #0969da;">1.531</span>** | 🔵 **<span style="color: #0969da;">1.043×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.882 | 3.19× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.048</span>** | 🔵 **<span style="color: #0969da;">3.082</span>** | 🔵 **<span style="color: #0969da;">0.989×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 5.009 | 1.63× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">4.599</span>** | 🟢 **<span style="color: #1a7f37;">4.308</span>** | 🟣 **<span style="color: #6f42c1;">1.067×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.7% | 17.982 | 4.17× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">7.956</span>** | 🔵 **<span style="color: #0969da;">7.993</span>** | 🔵 **<span style="color: #0969da;">0.995×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 18.067 | 2.26× | No |
| mat4_transpose | 32 | 🟢 **<span style="color: #1a7f37;">1.345</span>** | 🔴 **<span style="color: #cf222e;">1.480</span>** | 🟠 **<span style="color: #bc4c00;">0.909×</span>** | 🟠 <span style="color: #bc4c00;">sse2neon</span> | +10.0% | 3.473 | 2.35× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.443</span>** | 🔵 **<span style="color: #0969da;">2.438</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.475 | 1.43× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">1.168</span>** | 🔵 **<span style="color: #0969da;">1.135</span>** | 🔵 **<span style="color: #0969da;">1.029×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.127 | 2.75× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.115</span>** | 🔵 **<span style="color: #0969da;">2.115</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.123 | 1.48× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.365</span>** | 🟢 **<span style="color: #1a7f37;">0.340</span>** | 🟣 **<span style="color: #6f42c1;">1.074×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +7.4% | 0.872 | 2.56× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.597</span>** | 🔵 **<span style="color: #0969da;">0.597</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.46× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.391</span>** | 🟢 **<span style="color: #1a7f37;">0.360</span>** | 🟣 **<span style="color: #6f42c1;">1.087×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.7% | 0.957 | 2.66× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.571</span>** | 🔵 **<span style="color: #0969da;">0.570</span>** | 🔵 **<span style="color: #0969da;">1.003×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.961 | 1.69× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.295</span>** | 🟢 **<span style="color: #1a7f37;">0.280</span>** | 🟣 **<span style="color: #6f42c1;">1.055×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.5% | 0.867 | 3.10× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.522</span>** | 🔵 **<span style="color: #0969da;">0.522</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.893 | 1.71× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.410</span>** | 🟢 **<span style="color: #1a7f37;">0.229</span>** | 🟣 **<span style="color: #6f42c1;">1.794×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +79.4% | 0.713 | 3.11× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.423</span>** | 🔵 **<span style="color: #0969da;">0.423</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.645 | 1.53× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.635</span>** | 🟢 **<span style="color: #1a7f37;">0.418</span>** | 🟣 **<span style="color: #6f42c1;">1.520×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +52.0% | 1.620 | 3.88× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.675 | 2.06× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.403</span>** | 🟢 **<span style="color: #1a7f37;">0.362</span>** | 🟣 **<span style="color: #6f42c1;">1.113×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +11.3% | 0.961 | 2.66× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.638</span>** | 🔵 **<span style="color: #0969da;">0.638</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.966 | 1.51× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">2.802</span>** | 🟢 **<span style="color: #1a7f37;">2.377</span>** | 🟣 **<span style="color: #6f42c1;">1.179×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.9% | 4.882 | 2.05× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">4.930</span>** | 🔵 **<span style="color: #0969da;">4.932</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 5.009 | 1.02× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">5.549</span>** | 🟢 **<span style="color: #1a7f37;">4.469</span>** | 🟣 **<span style="color: #6f42c1;">1.242×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +24.2% | 17.982 | 4.02× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">10.671</span>** | 🔵 **<span style="color: #0969da;">10.699</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 18.067 | 1.69× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">2.530</span>** | 🟢 **<span style="color: #1a7f37;">1.773</span>** | 🟣 **<span style="color: #6f42c1;">1.427×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +42.7% | 3.473 | 1.96× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">4.752</span>** | 🔵 **<span style="color: #0969da;">4.741</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.475 | 0.73× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">1.330</span>** | 🔵 **<span style="color: #0969da;">1.272</span>** | 🔵 **<span style="color: #0969da;">1.045×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.127 | 2.46× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.676</span>** | 🔵 **<span style="color: #0969da;">2.670</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.123 | 1.17× | No |
| vec3_cross | 32 | 🔵 **<span style="color: #0969da;">0.332</span>** | 🔵 **<span style="color: #0969da;">0.321</span>** | 🔵 **<span style="color: #0969da;">1.034×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.872 | 2.72× | No |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.586</span>** | 🔵 **<span style="color: #0969da;">0.590</span>** | 🔵 **<span style="color: #0969da;">0.993×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.48× | No |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.343</span>** | 🔵 **<span style="color: #0969da;">0.343</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.957 | 2.79× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.574</span>** | 🔵 **<span style="color: #0969da;">0.573</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.961 | 1.68× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.267</span>** | 🔵 **<span style="color: #0969da;">0.263</span>** | 🔵 **<span style="color: #0969da;">1.015×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.867 | 3.30× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.505</span>** | 🔵 **<span style="color: #0969da;">0.510</span>** | 🔵 **<span style="color: #0969da;">0.991×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.893 | 1.75× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.413</span>** | 🟢 **<span style="color: #1a7f37;">0.206</span>** | 🟣 **<span style="color: #6f42c1;">2.001×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +100.1% | 0.713 | 3.45× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.415</span>** | 🔵 **<span style="color: #0969da;">0.415</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.645 | 1.56× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.632</span>** | 🟢 **<span style="color: #1a7f37;">0.451</span>** | 🟣 **<span style="color: #6f42c1;">1.401×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +40.1% | 1.620 | 3.59× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.858</span>** | 🔵 **<span style="color: #0969da;">0.858</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.675 | 1.95× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.358</span>** | 🔵 **<span style="color: #0969da;">0.354</span>** | 🔵 **<span style="color: #0969da;">1.010×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.961 | 2.71× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.665</span>** | 🔵 **<span style="color: #0969da;">0.679</span>** | 🔵 **<span style="color: #0969da;">0.979×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.966 | 1.42× | No |

## 4,096 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">2.040</span>** | 🔵 **<span style="color: #0969da;">2.002</span>** | 🔵 **<span style="color: #0969da;">1.019×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.213 | 2.10× | No |
| mat4_inverse_trs | 64 | 🔴 **<span style="color: #cf222e;">3.547</span>** | 🟢 **<span style="color: #1a7f37;">3.354</span>** | 🟣 **<span style="color: #6f42c1;">1.058×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.8% | 4.560 | 1.36× | No |
| mat4_multiply | 32 | 🔵 **<span style="color: #0969da;">2.244</span>** | 🔵 **<span style="color: #0969da;">2.197</span>** | 🔵 **<span style="color: #0969da;">1.021×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 10.917 | 4.97× | No |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">4.568</span>** | 🔵 **<span style="color: #0969da;">4.520</span>** | 🔵 **<span style="color: #0969da;">1.011×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 12.566 | 2.78× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.382</span>** | 🔵 **<span style="color: #0969da;">1.326</span>** | 🔵 **<span style="color: #0969da;">1.042×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.729 | 2.06× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.608</span>** | 🔵 **<span style="color: #0969da;">2.653</span>** | 🔵 **<span style="color: #0969da;">0.983×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.359 | 1.27× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">0.954</span>** | 🔵 **<span style="color: #0969da;">0.947</span>** | 🔵 **<span style="color: #0969da;">1.007×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.831 | 2.99× | No |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.933</span>** | 🔵 **<span style="color: #0969da;">1.911</span>** | 🔵 **<span style="color: #0969da;">1.012×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.280 | 1.72× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.931</span>** | 🟢 **<span style="color: #1a7f37;">0.623</span>** | 🟣 **<span style="color: #6f42c1;">1.494×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +49.4% | 1.191 | 1.91× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.027</span>** | 🔵 **<span style="color: #0969da;">1.024</span>** | 🔵 **<span style="color: #0969da;">1.003×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.211 | 1.18× | No |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.328</span>** | 🔵 **<span style="color: #0969da;">0.328</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.936 | 2.85× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">1.025</span>** | 🔵 **<span style="color: #0969da;">1.000</span>** | 🔵 **<span style="color: #0969da;">1.024×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.145 | 1.14× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.373</span>** | 🔵 **<span style="color: #0969da;">0.367</span>** | 🔵 **<span style="color: #0969da;">1.017×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.722 | 1.97× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.758</span>** | 🔵 **<span style="color: #0969da;">0.742</span>** | 🔵 **<span style="color: #0969da;">1.021×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.775 | 1.05× | No |
| vec4_length | 32 | 🔵 **<span style="color: #0969da;">0.609</span>** | 🔵 **<span style="color: #0969da;">0.584</span>** | 🔵 **<span style="color: #0969da;">1.044×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.677 | 1.16× | No |
| vec4_length | 64 | 🔴 **<span style="color: #cf222e;">0.610</span>** | 🟢 **<span style="color: #1a7f37;">0.579</span>** | 🟣 **<span style="color: #6f42c1;">1.053×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.3% | 0.644 | 1.11× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">1.059</span>** | 🟢 **<span style="color: #1a7f37;">1.007</span>** | 🟣 **<span style="color: #6f42c1;">1.052×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.2% | 1.554 | 1.54× | No |
| vec4_normalize | 64 | 🔴 **<span style="color: #cf222e;">1.255</span>** | 🟢 **<span style="color: #1a7f37;">1.194</span>** | 🟣 **<span style="color: #6f42c1;">1.051×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.1% | 1.619 | 1.36× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.330</span>** | 🔵 **<span style="color: #0969da;">0.330</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.988 | 2.99× | No |
| vec4_scale_add | 64 | 🔴 **<span style="color: #cf222e;">1.026</span>** | 🟢 **<span style="color: #1a7f37;">0.972</span>** | 🟣 **<span style="color: #6f42c1;">1.055×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.5% | 1.153 | 1.19× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">2.598</span>** | 🟢 **<span style="color: #1a7f37;">2.330</span>** | 🟣 **<span style="color: #6f42c1;">1.115×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +11.5% | 4.766 | 2.05× | Yes |
| mat4_inverse_trs | 64 | 🔴 **<span style="color: #cf222e;">3.927</span>** | 🟢 **<span style="color: #1a7f37;">3.736</span>** | 🟣 **<span style="color: #6f42c1;">1.051×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.1% | 4.857 | 1.30× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">4.449</span>** | 🟢 **<span style="color: #1a7f37;">3.916</span>** | 🟣 **<span style="color: #6f42c1;">1.136×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +13.6% | 18.733 | 4.78× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">7.489</span>** | 🔵 **<span style="color: #0969da;">7.329</span>** | 🔵 **<span style="color: #0969da;">1.022×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.761 | 2.70× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">2.702</span>** | 🟢 **<span style="color: #1a7f37;">2.473</span>** | 🟣 **<span style="color: #6f42c1;">1.093×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +9.3% | 4.558 | 1.84× | No |
| mat4_transpose | 64 | 🔴 **<span style="color: #cf222e;">4.233</span>** | 🟢 **<span style="color: #1a7f37;">4.028</span>** | 🟣 **<span style="color: #6f42c1;">1.051×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.1% | 4.571 | 1.13× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">1.813</span>** | 🟢 **<span style="color: #1a7f37;">1.479</span>** | 🟣 **<span style="color: #6f42c1;">1.226×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +22.6% | 3.182 | 2.15× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.353</span>** | 🔵 **<span style="color: #0969da;">2.299</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.251 | 1.41× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.728</span>** | 🟢 **<span style="color: #1a7f37;">0.611</span>** | 🟣 **<span style="color: #6f42c1;">1.190×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +19.0% | 1.170 | 1.91× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.451</span>** | 🔵 **<span style="color: #0969da;">1.384</span>** | 🔵 **<span style="color: #0969da;">1.049×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.529 | 1.11× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">1.021</span>** | 🟢 **<span style="color: #1a7f37;">0.838</span>** | 🟣 **<span style="color: #6f42c1;">1.218×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +21.8% | 1.423 | 1.70× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">1.765</span>** | 🔵 **<span style="color: #0969da;">1.683</span>** | 🔵 **<span style="color: #0969da;">1.049×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.856 | 1.10× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.434</span>** | 🟢 **<span style="color: #1a7f37;">0.288</span>** | 🟣 **<span style="color: #6f42c1;">1.508×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +50.8% | 0.890 | 3.09× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.806</span>** | 🔵 **<span style="color: #0969da;">0.789</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.189 | 1.51× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.412</span>** | 🟢 **<span style="color: #1a7f37;">0.235</span>** | 🟣 **<span style="color: #6f42c1;">1.758×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +75.8% | 0.677 | 2.89× | Yes |
| vec4_length | 64 | 🔴 **<span style="color: #cf222e;">0.498</span>** | 🟢 **<span style="color: #1a7f37;">0.473</span>** | 🟣 **<span style="color: #6f42c1;">1.051×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.1% | 0.612 | 1.29× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.665</span>** | 🟢 **<span style="color: #1a7f37;">0.391</span>** | 🟣 **<span style="color: #6f42c1;">1.699×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +69.9% | 1.526 | 3.90× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.995</span>** | 🔵 **<span style="color: #0969da;">0.971</span>** | 🔵 **<span style="color: #0969da;">1.025×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.640 | 1.69× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">1.021</span>** | 🟢 **<span style="color: #1a7f37;">0.799</span>** | 🟣 **<span style="color: #6f42c1;">1.277×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +27.7% | 1.431 | 1.79× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.763</span>** | 🔵 **<span style="color: #0969da;">1.679</span>** | 🔵 **<span style="color: #0969da;">1.050×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.814 | 1.08× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">2.314</span>** | 🟢 **<span style="color: #1a7f37;">2.162</span>** | 🟣 **<span style="color: #6f42c1;">1.070×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +7.0% | 4.766 | 2.20× | No |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.677</span>** | 🔵 **<span style="color: #0969da;">3.615</span>** | 🔵 **<span style="color: #0969da;">1.017×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.857 | 1.34× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">6.789</span>** | 🟢 **<span style="color: #1a7f37;">6.127</span>** | 🟣 **<span style="color: #6f42c1;">1.108×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.8% | 18.733 | 3.06× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">10.679</span>** | 🔵 **<span style="color: #0969da;">10.324</span>** | 🔵 **<span style="color: #0969da;">1.034×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.761 | 1.91× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">2.430</span>** | 🟢 **<span style="color: #1a7f37;">2.311</span>** | 🟣 **<span style="color: #6f42c1;">1.051×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.1% | 4.558 | 1.97× | No |
| mat4_transpose | 64 | 🔴 **<span style="color: #cf222e;">3.926</span>** | 🟢 **<span style="color: #1a7f37;">3.696</span>** | 🟣 **<span style="color: #6f42c1;">1.062×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.2% | 4.571 | 1.24× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">1.715</span>** | 🟢 **<span style="color: #1a7f37;">1.611</span>** | 🟣 **<span style="color: #6f42c1;">1.065×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.5% | 3.182 | 1.98× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.621</span>** | 🔵 **<span style="color: #0969da;">2.561</span>** | 🔵 **<span style="color: #0969da;">1.024×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.251 | 1.27× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.672</span>** | 🟢 **<span style="color: #1a7f37;">0.580</span>** | 🟣 **<span style="color: #6f42c1;">1.158×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.8% | 1.170 | 2.02× | Yes |
| vec3_cross | 64 | 🔴 **<span style="color: #cf222e;">1.430</span>** | 🟢 **<span style="color: #1a7f37;">1.358</span>** | 🟣 **<span style="color: #6f42c1;">1.053×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.3% | 1.529 | 1.13× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.931</span>** | 🟢 **<span style="color: #1a7f37;">0.801</span>** | 🟣 **<span style="color: #6f42c1;">1.162×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +16.2% | 1.423 | 1.78× | Yes |
| vec4_add | 64 | 🔴 **<span style="color: #cf222e;">1.783</span>** | 🟢 **<span style="color: #1a7f37;">1.695</span>** | 🟣 **<span style="color: #6f42c1;">1.052×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.2% | 1.856 | 1.09× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.326</span>** | 🟢 **<span style="color: #1a7f37;">0.290</span>** | 🟣 **<span style="color: #6f42c1;">1.125×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +12.5% | 0.890 | 3.07× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.810</span>** | 🔵 **<span style="color: #0969da;">0.792</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.189 | 1.50× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.411</span>** | 🟢 **<span style="color: #1a7f37;">0.225</span>** | 🟣 **<span style="color: #6f42c1;">1.831×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +83.1% | 0.677 | 3.01× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.427</span>** | 🔵 **<span style="color: #0969da;">0.417</span>** | 🔵 **<span style="color: #0969da;">1.024×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.612 | 1.47× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.651</span>** | 🟢 **<span style="color: #1a7f37;">0.408</span>** | 🟣 **<span style="color: #6f42c1;">1.597×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +59.7% | 1.526 | 3.74× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.957</span>** | 🔵 **<span style="color: #0969da;">0.957</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.640 | 1.71× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.930</span>** | 🟢 **<span style="color: #1a7f37;">0.762</span>** | 🟣 **<span style="color: #6f42c1;">1.220×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +22.0% | 1.431 | 1.88× | Yes |
| vec4_scale_add | 64 | 🔴 **<span style="color: #cf222e;">1.783</span>** | 🟢 **<span style="color: #1a7f37;">1.698</span>** | 🟣 **<span style="color: #6f42c1;">1.050×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.0% | 1.814 | 1.07× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">4.531</span>** | 🟢 **<span style="color: #1a7f37;">4.194</span>** | 🟣 **<span style="color: #6f42c1;">1.080×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +8.0% | 4.766 | 1.14× | No |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">7.049</span>** | 🔵 **<span style="color: #0969da;">6.803</span>** | 🔵 **<span style="color: #0969da;">1.036×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.857 | 0.71× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">7.642</span>** | 🟢 **<span style="color: #1a7f37;">6.765</span>** | 🟣 **<span style="color: #6f42c1;">1.130×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +13.0% | 18.733 | 2.77× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">12.925</span>** | 🔵 **<span style="color: #0969da;">12.630</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.761 | 1.56× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">4.222</span>** | 🟢 **<span style="color: #1a7f37;">3.053</span>** | 🟣 **<span style="color: #6f42c1;">1.383×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +38.3% | 4.558 | 1.49× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">6.776</span>** | 🔵 **<span style="color: #0969da;">6.456</span>** | 🔵 **<span style="color: #0969da;">1.050×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.571 | 0.71× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">2.649</span>** | 🟢 **<span style="color: #1a7f37;">2.028</span>** | 🟣 **<span style="color: #6f42c1;">1.306×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +30.6% | 3.182 | 1.57× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">3.829</span>** | 🔵 **<span style="color: #0969da;">3.757</span>** | 🔵 **<span style="color: #0969da;">1.019×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.251 | 0.87× | No |
| vec3_cross | 32 | 🔵 **<span style="color: #0969da;">0.550</span>** | 🔵 **<span style="color: #0969da;">0.535</span>** | 🔵 **<span style="color: #0969da;">1.029×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.170 | 2.19× | No |
| vec3_cross | 64 | 🔴 **<span style="color: #cf222e;">1.376</span>** | 🟢 **<span style="color: #1a7f37;">1.310</span>** | 🟣 **<span style="color: #6f42c1;">1.050×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.0% | 1.529 | 1.17× | No |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.745</span>** | 🔵 **<span style="color: #0969da;">0.746</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.423 | 1.91× | No |
| vec4_add | 64 | 🔴 **<span style="color: #cf222e;">1.733</span>** | 🟢 **<span style="color: #1a7f37;">1.645</span>** | 🟣 **<span style="color: #6f42c1;">1.054×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.4% | 1.856 | 1.13× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.288</span>** | 🔵 **<span style="color: #0969da;">0.277</span>** | 🔵 **<span style="color: #0969da;">1.041×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.890 | 3.21× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.799</span>** | 🔵 **<span style="color: #0969da;">0.761</span>** | 🔵 **<span style="color: #0969da;">1.049×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.189 | 1.56× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.416</span>** | 🟢 **<span style="color: #1a7f37;">0.207</span>** | 🟣 **<span style="color: #6f42c1;">2.008×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +100.8% | 0.677 | 3.27× | Yes |
| vec4_length | 64 | 🔴 **<span style="color: #cf222e;">0.417</span>** | 🟢 **<span style="color: #1a7f37;">0.397</span>** | 🟣 **<span style="color: #6f42c1;">1.050×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.0% | 0.612 | 1.54× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.643</span>** | 🟢 **<span style="color: #1a7f37;">0.432</span>** | 🟣 **<span style="color: #6f42c1;">1.487×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +48.7% | 1.526 | 3.53× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.954</span>** | 🔵 **<span style="color: #0969da;">0.917</span>** | 🔵 **<span style="color: #0969da;">1.040×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.640 | 1.79× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.761</span>** | 🟢 **<span style="color: #1a7f37;">0.724</span>** | 🟣 **<span style="color: #6f42c1;">1.050×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.0% | 1.431 | 1.98× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.735</span>** | 🔵 **<span style="color: #0969da;">1.680</span>** | 🔵 **<span style="color: #0969da;">1.033×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.814 | 1.08× | No |

## Source, host and controls

- Tested full commit: `c120ded4321a087339a952ef0671fb89c456ca14`.
- Tested full tree: `b02601fc5a04dbdc41e224c1e04d2b987955150a`.
- Branch/worktree state before and after: Clean primary master checkout at the exact recorded commit and tree, with every tracked raw hash verified before and after collection.
- Dirty-source provenance, if applicable: Not applicable: clean committed source; the complete raw-byte manifest is source-manifest.json.
- Comparison source, if used: No separate source candidate/control comparison; scalar and both ARM backends use the same frozen source.
- Benchmark source/build-file comparison: All tracked source hashes verified before/after against the frozen source, including benchmark source/build files listed in metadata.json; no benchmark source edits.
- Physical host, architecture and native-execution proof: Apple M2; native arm64 Python and Mach-O executables; sysctl.proc_translated=0.
- OS/version/build: ProductName:		macOS; ProductVersion:		26.7; BuildVersion:		25G229.
- Compiler/version/target: Apple clang version 21.0.0 (clang-2100.1.1.101); Target: arm64-apple-darwin25.6.0.
- SDK, CMake/CTest, build tool and Python: SDK 26.5; cmake version 4.5.1; ctest version 4.5.1; Ninja 1.13.2; native Python 3.9.6.
- Execution interval: 2026-11-22T01:55:11.531794+00:00 through 2026-11-22T02:13:57.965928+00:00; report-date timezone: Europe/Stockholm.
- Exact Release compile/link flags: `-O3 -DNDEBUG -std=gnu++20 -arch arm64 -isysroot <SDK> -ffp-contract=off -mcpu=apple-m1; STANDARD_IMPL=0/2/3; SIMD MAX_VECTOR_SIZE=512 with backend-specific definitions; see metadata.json for all six translation units`, `-O3 -DNDEBUG -arch arm64 -isysroot <SDK> -Wl,-search_paths_first -Wl,-headerpad_max_install_names`.
- Scalar vectorization controls and disassembly findings: -fno-vectorize -fno-slp-vectorize; fresh scalar disassembly checked with no packed FP arithmetic matches; scalar-codegen.json records the result and binary SHA-256. Loads/stores and library callees are separate from this finding.
- Timing build controls: Inspected actual compile/link commands: no broad fast-math, LTO or sanitizer flags in timing builds. No fixed-frequency/core-affinity control was imposed.
- Correctness-only builds, profiles and sanitizer coverage: All eight full native Release/strict UBSan Debug profiles passed; six additional benchmark CTest runs passed in Release/strict UBSan Debug, and all six workset-correctness probes passed.
- Registered/passed/failed/skipped/timed-out counts and attempts: 8,036 native cases and eight arithmetic smokes passed, plus six benchmark CTest cases and six actual-workset correctness probes. The native matrix and timings ran once. One failed standalone include attempt was repeated after correcting its SDK environment..
- Strict UBSan flags/runtime options and control results, when used: -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer; UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1. Positive exit 0; intentional overflow exit -6, required diagnostic present and no continuation.
- Read-only integrity checks before/after: Maintenance integrity, repository/link checks, 41 isolated integrity controls, four native standalone consumers and six target-rejection controls passed. Full raw-source verification before/after passed.
- Protected originals and dependency integrity: VCL 667577495efaa36a10cbb5d6cb8c942c23913848, tree 36aee120c28c9c64e62903bfc81139003e37cedd: all 28 originals exact (27 root plus audit/vectorclass-README.md). Root `sse2neon.h` retained SHA-256 932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3; both audit pins preserved.
- Source edits, inventory changes and evidence of stability: No runtime/test/runner/build-policy/source edits or inventory changes. The auxiliary correctness driver renames the unchanged benchmark entry point and calls its existing check-only functions at 128/4096; it is separately identified probe evidence, not a modified implementation.

Optional kernel/whole-binary comparison: scalar-codegen.json and benchmark-correctness.json; raw scalar disassembly retained locally.
No cross-source or cross-session executable-equivalence comparison was performed. Scalar disassembly and all new executable hashes belong only to this run

Keep the 28 upstream originals (27 root files plus audit/vectorclass-README.md) and `audit/vectorclass-2.02.04.json` byte-exact,
along with root `sse2neon.h` and its required `audit/sse2neon-2272251.json` pin.
The vendor catalog is retired. On an unexplained integrity
mismatch, preserve the failed check and stop. Header generation and the old
adaptation/backend pipeline are retired. Approved ARM changes belong in the
authoritative root `*_neon.h` sources, with new source IDs and fresh validation;
follow the [current ownership rules](../../backend-layout.md). An upstream update
requires its own approved scope.

## Measurement and correctness scope

- Operations, precision, object counts, layouts and logical widths: Ten operations, FP32/FP64, counts 128 and 4096; AoS and SoA widths 128/256/512, complete 160-row inventory.
- Invocation count, order and scheduling: Three rounds; backend order scalar/compat/neon, compat/neon/scalar, neon/scalar/compat; both counts in each round. Exactly 18 sequential invocations, schedule.json; no overlapping builds/correctness work during timing.
- Calibration, warm-up and trials per shape: Unchanged harness: minimum of three calibration samples, adaptive repeat count, warm-up with optional repeat recalibration, then nine trial batches per shape.
- Aggregation: Each batch divided by repeats and object count; median of nine samples per invocation, then median of three invocation medians. Range separation uses min/max of those three medians. The same scalar layout/precision/count control is reused across corresponding SoA widths within this session.
- Actual totals: 18 processes, 1200 shapes,
  10800 trial batches and 160 comparison rows.
- Independent reference methods and unchanged gate: Unchanged scalar coordinate loops, hypot and pivoted Gauss-Jordan inversion; finite output and absolute error/(type epsilon × reference scale) <=64; fixed seed 0x874af31b01753; canaries and input hashes checked.
- Output fields per validation pass: 17,740,800 across all 1,200 shape executions before timing; combined before/after
  timing checks: 35,481,600.
- Maximum normalized reference error: 6 epsilon-scaled units;
  outcome: Passed every shape before and after timing; no independent-reference, input, output-hash or canary failures.
- Input hashes, canaries, memory layout and partial counts: Every operation/precision/count has identical canonical input hashes across the backends, layouts and widths. Offset buffers, SoA padding canaries and generated boundary-count checks were preserved. The main native matrix separately proved inaccessible-page controls at the actual page size.
- FPCR/rounding/flush-to-zero state: Harness saves/restores FP state; FPCR=0 for every recorded shape, round-to-nearest with flush-to-zero disabled, no implicit contraction.
- Output repeatability within each backend/shape: All 400 backend/shape output buffers byte-identical across their three invocations; dump SHA-256 and canonical FNV output-hash checks passed.
- Cross-backend comparison scope: Canonical AoS-ordered first-invocation output buffers for all 160 three-way rows; scalar controls reused across widths are counted per report row, not as independent datasets. Full per-row output-comparisons.json retained; repeated invocations checked for exact within-backend repeatability.
- Scalar/Neon differing fields, maximum absolute error and ULP distance:
  7,913 of 2,365,440 compared fields differ in bits; maximum absolute difference 9.53674316e-07; maximum ordered-bit ULP distance 768.
- Compatibility/Neon differing fields, maximum absolute error and ULP distance:
  21,084 of 2,365,440 compared fields differ in bits; maximum absolute difference 4.76837158e-07; maximum ordered-bit ULP distance 4.
- Failed checks, retries, missing profiles or unavailable controls:
  Pre-reset controller setup rejected an inherited default SDKROOT. The first post-reset include attempt then failed to find stdlib.h after that variable was removed. A fresh include attempt used the SDK path verified by xcrun and passed. Numerical tests and timing attempts retain their actual outcomes in the named records..

Any cross-backend bit differences remain separate from the independent numerical acceptance test. All measured outputs satisfy the original finite-output/64-epsilon-scaled gate, including cancellation-sensitive operations. Ordered-bit ULP distances are descriptive and do not replace that gate; -0 and +0 are adjacent in the ordered-bit convention

Output bit differences and independent-reference accuracy are separate facts.
State which outputs were compared, whether scalar rows were reused across
widths, and whether differences are within the existing contract. A large raw
ULP distance near cancellation must not replace the operation's acceptance
rule, and a passing accuracy gate does not establish universal bitwise identity.

- Core placement/frequency/power settings and sleep prevention: Native Apple M2, caffeinate -i sleep prevention; sequential timing; no affinity, frequency or machine-wide power-policy changes.
- Raw trial-batch duration range: 7.527875–992508.992458 ms.
- Host load and thermal/performance observations: Per-invocation uptime/load, thermal and power-source observations retained in metadata.json; no fixed-frequency measurement or claim about the cause of timing variability.
- Rows with disjoint invocation median ranges: 52/160.
- Leads with overlapping ranges or other variability: 20 leads outside the practical band have overlapping invocation ranges. Three invocations describe this session, not statistical significance or a universal application benefit.
- Rows slower than scalar, including which backend: 5 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> and 5 compatibility row/backend instances; complete named inventory and elapsed costs in summary.json.

Native M2 measurements at this frozen source. Windows remains failed/incomplete at its 22 November source with 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen examples. Its historical 19 November gate remains 5,387 passed, 249 failed and 76 not run of 5,712, plus twelve examples.

## Evidence and next use

- Ignored raw evidence directory: `build/arm64-reset-2026-11-22-20261122T002523Z/`.
- Attempt identifiers and failed/replayed attempts: One complete native matrix and one 18-invocation timing session. Two preceding SDK setup failures are retained with the successful include retry in setup-attempts.json..
- Portable data directory: `STANDARD_MATH_RESULTS_2026-11-22/`.
- Original run data checksum-inventory SHA256: `49f7423237c6cece6fb988b4995b060282c98eeec7ce7f3b064086a704b3b120`.
- Verified entry count, CRC/hash audit and contents: The original run inventory covered 23 unpacked data records; its identity remains in archive-receipt.json. The published [checksums.sha256](STANDARD_MATH_RESULTS_2026-11-22/checksums.sha256) verifies 27 entries: 25 supporting files and the two published reports. No report-checksums.sha256 is shipped in this bundle.
- Portable data receipt: `archive-receipt.json`.
- Portable metadata, results and checksum inventory: [metadata](STANDARD_MATH_RESULTS_2026-11-22/candidate/metadata.json), [raw trials and rows](STANDARD_MATH_RESULTS_2026-11-22/candidate/results.json), [CSV](STANDARD_MATH_RESULTS_2026-11-22/candidate/results.csv), [summary](STANDARD_MATH_RESULTS_2026-11-22/candidate/summary.json), [source manifest](STANDARD_MATH_RESULTS_2026-11-22/source-manifest.json), [checksums](STANDARD_MATH_RESULTS_2026-11-22/checksums.sha256).
- Separately retained records/supplements and hashes: Fresh raw runner logs, generated inputs, binaries, benchmark dumps and an expanded source snapshot remain under build. reset-receipt.json records the discarded historical payloads..
- Retained build/source/worktree resources and any explicitly authorized cleanup:
  The authorized initial reset discarded 40,072 historical local files totaling 4,649,286,799 bytes. All fresh outputs and required tools/dependencies remain available..

Retain source/compiler/flag inventories, binary hashes, complete raw trials,
output dumps, input hashes, configure/build/test logs, JUnit, failed attempts,
reference/probe sources and relevant code-generation records. Exclude machine
paths, credentials and compiled binaries from the portable report/data.

This is a current-master checkpoint for the exact source/tool/host recorded. A future implementation comparison needs fresh matched sessions. Review these results before separately approving permanent documentation or further runtime work

Record this task's actual Git/publication actions: One approved documentation correction was committed as the tested c120ded source before deletion. Publication requires separate authorization..
Follow the maintainer's task-specific authorization for publication or cleanup.
Future source changes require new recorded measurements and matched controls;
preserve completed dated reports and all original numerical data.
