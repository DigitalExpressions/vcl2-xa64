# Standard vector and matrix benchmarks - 2026-11-21

**Scalar FP32/FP64 versus <span style="color: #bc4c00;">sse2neon</span> versus <span style="color: #6f42c1;">Direct&nbsp;Neon</span>**:
ten standard vector/matrix operations, FP32/FP64, 128/4,096 objects, AoS and SoA widths 128/256/512. The tables describe the native measurements from
**2026-11-21**, at source `dc802e466663`.

**Run status:** Completed after one task-local flag-audit setup correction: the full native correctness gate and benchmark checks passed; all planned timing invocations and 160 report rows are present. The earlier setup attempt is retained and collected no timing.

Fresh current-master three-way measurements after the complete native M2 build/test/validation round. 43 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads, 117 within the inclusive 5% band, and 0 compatibility leads. These are workload-specific results, not a before/after source improvement claim. The declined TRS candidate was not built or revived.

**Compiler context:** The [19 November report](STANDARD_MATH_RESULTS_2026-11-19.md) used Apple Clang 17 / SDK 26.1; this run uses Clang 21 / SDK 26.5. For FP32 `mat4_vector`, SoA logical 128 bits at 128 objects, Direct Neon's lead narrowed from **+298.8% to +61.6%**, mainly because sse2neon became faster. A separate matched repeat held SDK 26.1 fixed: sse2neon changed from **3.165 to 1.362 ns/op** (57.0% less elapsed time), while Direct Neon changed from **0.803 to 0.827 ns/op**. Clang 21 kept matrix vectors in SIMD registers through the arithmetic, eliminating the Clang 17 matrix-data stack round trip; SDK 26.5 repeats showed the same pattern. This supports a Clang-toolchain effect for this measurement. The individual compiler pass was not isolated, and other rows need their own analysis. These focused repeat values do not replace the full-suite timings below; [supporting measurements, controls and limits](standard-math-master-2026-11-21/publication-receipt.json) retain their separate identity.

The five Direct Neon rows slower than scalar are all FP64: SoA logical 512-bit TRS inverse at 4,096 objects (+39.640% elapsed), transpose at 128/4,096 objects (+36.590% / +37.864%), and matrix-vector at 4,096 objects (+14.913%); AoS transpose at 128 objects costs +5.028%. Compatibility is also slower than scalar in these rows and in the small FP64 SoA 512-bit inverse. Being within 5% of another SIMD backend does not establish an advantage over scalar.

All slower and variable rows are retained. Before/after comparisons and full
suite runs use their own matched controls; do not mix times between runs.

- CSV results: [results.csv](standard-math-master-2026-11-21/candidate/results.csv)
- JSON results and raw trials: [results.json](standard-math-master-2026-11-21/candidate/results.json)
- Summary/audit: [summary.json](standard-math-master-2026-11-21/candidate/summary.json)
- Metadata and checksums: [metadata.json](standard-math-master-2026-11-21/candidate/metadata.json), [checksums.sha256](standard-math-master-2026-11-21/checksums.sha256)

In this run, <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins or is within
the shared 5% band in **160/160 rows**:
**43** <span style="color: #6f42c1;">Direct&nbsp;Neon</span> leads, **117** within 5%,
and **0** <span style="color: #bc4c00;">sse2neon</span> leads.

63/160 rows have disjoint invocation-median ranges. 0 leads outside the 5% band still have overlapping ranges. All 160 rows are retained, including 5 Neon and 6 compatibility row/backend instances slower than scalar; the complete inventory is in [summary.json](standard-math-master-2026-11-21/candidate/summary.json).

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
the displayed ratio `r`, the gain is `100 × (r − 1)%` when <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins, or
`100 × (1/r − 1)%` when <span style="color: #bc4c00;">sse2neon</span> wins. Thus **1.25×** gives <span style="color: #6f42c1;">Direct&nbsp;Neon</span> a **25%**
throughput lead, while **0.80×** gives <span style="color: #bc4c00;">sse2neon</span> a **25%** throughput lead.
Reported gains use the displayed ratio and are rounded to one decimal place.
Ratios come from unrounded measurements, so dividing the displayed timing
values can give a slightly different result.

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
**Host and build summary:** Apple M2; macOS 26.7 (25G229); Apple Clang 21; native arm64; exact SDK/tool versions and flags are recorded below and in [metadata.json](standard-math-master-2026-11-21/candidate/metadata.json).

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
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">1.885</span>** | 🔵 **<span style="color: #0969da;">1.961</span>** | 🔵 **<span style="color: #0969da;">0.961×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 11.839 | 6.04× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.321</span>** | 🔵 **<span style="color: #0969da;">3.323</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.233 | 1.27× | No |
| mat4_multiply | 32 | 🔵 **<span style="color: #0969da;">2.131</span>** | 🔵 **<span style="color: #0969da;">2.118</span>** | 🔵 **<span style="color: #0969da;">1.006×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 28.628 | 13.52× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">4.264</span>** | 🔵 **<span style="color: #0969da;">4.306</span>** | 🔵 **<span style="color: #0969da;">0.990×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 10.426 | 2.42× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.209</span>** | 🔵 **<span style="color: #0969da;">1.191</span>** | 🔵 **<span style="color: #0969da;">1.015×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.358 | 2.82× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.549</span>** | 🔵 **<span style="color: #0969da;">2.480</span>** | 🔵 **<span style="color: #0969da;">1.028×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.361 | 0.95× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">0.594</span>** | 🔵 **<span style="color: #0969da;">0.594</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.538 | 4.27× | No |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.203</span>** | 🔵 **<span style="color: #0969da;">1.204</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 27.483 | 22.82× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.893</span>** | 🟢 **<span style="color: #1a7f37;">0.624</span>** | 🟣 **<span style="color: #6f42c1;">1.429×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +42.9% | 1.167 | 1.87× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.883</span>** | 🔵 **<span style="color: #0969da;">0.844</span>** | 🔵 **<span style="color: #0969da;">1.046×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 8.040 | 9.52× | Yes |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.348</span>** | 🔵 **<span style="color: #0969da;">0.351</span>** | 🔵 **<span style="color: #0969da;">0.991×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.949 | 2.70× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.655</span>** | 🔵 **<span style="color: #0969da;">0.677</span>** | 🔵 **<span style="color: #0969da;">0.968×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.923 | 1.36× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.400</span>** | 🔵 **<span style="color: #0969da;">0.409</span>** | 🔵 **<span style="color: #0969da;">0.977×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.719 | 1.76× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.519</span>** | 🔵 **<span style="color: #0969da;">0.521</span>** | 🔵 **<span style="color: #0969da;">0.996×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.702 | 1.35× | No |
| vec4_length | 32 | 🔵 **<span style="color: #0969da;">0.577</span>** | 🔵 **<span style="color: #0969da;">0.577</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.714 | 1.24× | No |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.589</span>** | 🔵 **<span style="color: #0969da;">0.576</span>** | 🔵 **<span style="color: #0969da;">1.022×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.633 | 1.10× | No |
| vec4_normalize | 32 | 🔵 **<span style="color: #0969da;">1.007</span>** | 🔵 **<span style="color: #0969da;">1.009</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.622 | 1.61× | No |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">1.191</span>** | 🔵 **<span style="color: #0969da;">1.192</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.632 | 1.37× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.359</span>** | 🔵 **<span style="color: #0969da;">0.367</span>** | 🔵 **<span style="color: #0969da;">0.977×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.985 | 2.69× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.593</span>** | 🔵 **<span style="color: #0969da;">0.594</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.960 | 1.62× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">1.611</span>** | 🟢 **<span style="color: #1a7f37;">1.411</span>** | 🟣 **<span style="color: #6f42c1;">1.141×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +14.1% | 4.634 | 3.28× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">2.787</span>** | 🔵 **<span style="color: #0969da;">2.725</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.756 | 1.74× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">3.843</span>** | 🟢 **<span style="color: #1a7f37;">3.281</span>** | 🟣 **<span style="color: #6f42c1;">1.171×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.1% | 17.548 | 5.35× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">6.764</span>** | 🔵 **<span style="color: #0969da;">6.728</span>** | 🔵 **<span style="color: #0969da;">1.005×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 17.649 | 2.62× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.518</span>** | 🔵 **<span style="color: #0969da;">1.502</span>** | 🔵 **<span style="color: #0969da;">1.011×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.300 | 2.20× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.573</span>** | 🔵 **<span style="color: #0969da;">2.587</span>** | 🔵 **<span style="color: #0969da;">0.995×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.302 | 1.28× | Yes |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">1.288</span>** | 🟢 **<span style="color: #1a7f37;">0.797</span>** | 🟣 **<span style="color: #6f42c1;">1.616×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +61.6% | 3.046 | 3.82× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.598</span>** | 🔵 **<span style="color: #0969da;">1.600</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.048 | 1.91× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.449</span>** | 🟢 **<span style="color: #1a7f37;">0.381</span>** | 🟣 **<span style="color: #6f42c1;">1.178×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.8% | 0.872 | 2.29× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.600</span>** | 🔵 **<span style="color: #0969da;">0.600</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.851 | 1.42× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.470</span>** | 🟢 **<span style="color: #1a7f37;">0.399</span>** | 🟣 **<span style="color: #6f42c1;">1.177×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +17.7% | 0.957 | 2.40× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.732</span>** | 🔵 **<span style="color: #0969da;">0.753</span>** | 🔵 **<span style="color: #0969da;">0.973×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.938 | 1.25× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.412</span>** | 🟢 **<span style="color: #1a7f37;">0.279</span>** | 🟣 **<span style="color: #6f42c1;">1.479×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +47.9% | 0.869 | 3.12× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.558</span>** | 🔵 **<span style="color: #0969da;">0.558</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.56× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.389</span>** | 🟢 **<span style="color: #1a7f37;">0.240</span>** | 🟣 **<span style="color: #6f42c1;">1.620×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +62.0% | 0.713 | 2.97× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.485</span>** | 🔵 **<span style="color: #0969da;">0.478</span>** | 🔵 **<span style="color: #0969da;">1.015×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.630 | 1.32× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.622</span>** | 🟢 **<span style="color: #1a7f37;">0.374</span>** | 🟣 **<span style="color: #6f42c1;">1.663×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +66.3% | 1.541 | 4.12× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.795</span>** | 🔵 **<span style="color: #0969da;">0.796</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.634 | 2.05× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.507</span>** | 🟢 **<span style="color: #1a7f37;">0.401</span>** | 🟣 **<span style="color: #6f42c1;">1.263×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +26.3% | 0.961 | 2.40× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.600</span>** | 🔵 **<span style="color: #0969da;">0.600</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.947 | 1.58× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">1.532</span>** | 🟢 **<span style="color: #1a7f37;">1.456</span>** | 🟣 **<span style="color: #6f42c1;">1.052×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.2% | 4.634 | 3.18× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">2.971</span>** | 🔵 **<span style="color: #0969da;">2.927</span>** | 🔵 **<span style="color: #0969da;">1.015×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.756 | 1.62× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">4.483</span>** | 🟢 **<span style="color: #1a7f37;">4.201</span>** | 🟣 **<span style="color: #6f42c1;">1.067×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.7% | 17.548 | 4.18× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">7.770</span>** | 🔵 **<span style="color: #0969da;">7.795</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 17.649 | 2.26× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.318</span>** | 🔵 **<span style="color: #0969da;">1.341</span>** | 🔵 **<span style="color: #0969da;">0.983×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.300 | 2.46× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.328</span>** | 🔵 **<span style="color: #0969da;">2.333</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.302 | 1.42× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">1.139</span>** | 🔵 **<span style="color: #0969da;">1.113</span>** | 🔵 **<span style="color: #0969da;">1.023×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.046 | 2.74× | No |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.065</span>** | 🔵 **<span style="color: #0969da;">2.066</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.048 | 1.48× | No |
| vec3_cross | 32 | 🔵 **<span style="color: #0969da;">0.356</span>** | 🔵 **<span style="color: #0969da;">0.339</span>** | 🔵 **<span style="color: #0969da;">1.050×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.872 | 2.57× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.583</span>** | 🔵 **<span style="color: #0969da;">0.582</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.851 | 1.46× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.383</span>** | 🟢 **<span style="color: #1a7f37;">0.359</span>** | 🟣 **<span style="color: #6f42c1;">1.067×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.7% | 0.957 | 2.66× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.556</span>** | 🔵 **<span style="color: #0969da;">0.556</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.938 | 1.69× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.288</span>** | 🟢 **<span style="color: #1a7f37;">0.273</span>** | 🟣 **<span style="color: #6f42c1;">1.055×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +5.5% | 0.869 | 3.18× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.509</span>** | 🔵 **<span style="color: #0969da;">0.509</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.71× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.390</span>** | 🟢 **<span style="color: #1a7f37;">0.224</span>** | 🟣 **<span style="color: #6f42c1;">1.745×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +74.5% | 0.713 | 3.19× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.413</span>** | 🔵 **<span style="color: #0969da;">0.413</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.630 | 1.53× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.604</span>** | 🟢 **<span style="color: #1a7f37;">0.396</span>** | 🟣 **<span style="color: #6f42c1;">1.525×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +52.5% | 1.541 | 3.89× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.795</span>** | 🔵 **<span style="color: #0969da;">0.796</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.634 | 2.05× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.394</span>** | 🟢 **<span style="color: #1a7f37;">0.361</span>** | 🟣 **<span style="color: #6f42c1;">1.090×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +9.0% | 0.961 | 2.66× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.622</span>** | 🔵 **<span style="color: #0969da;">0.623</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.947 | 1.52× | No |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">2.732</span>** | 🟢 **<span style="color: #1a7f37;">2.250</span>** | 🟣 **<span style="color: #6f42c1;">1.214×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +21.4% | 4.634 | 2.06× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">4.837</span>** | 🔵 **<span style="color: #0969da;">4.701</span>** | 🔵 **<span style="color: #0969da;">1.029×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.756 | 1.01× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">5.279</span>** | 🟢 **<span style="color: #1a7f37;">4.366</span>** | 🟣 **<span style="color: #6f42c1;">1.209×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +20.9% | 17.548 | 4.02× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">10.405</span>** | 🔵 **<span style="color: #0969da;">10.437</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 17.649 | 1.69× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">2.407</span>** | 🟢 **<span style="color: #1a7f37;">1.617</span>** | 🟣 **<span style="color: #6f42c1;">1.489×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +48.9% | 3.300 | 2.04× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">4.628</span>** | 🔵 **<span style="color: #0969da;">4.511</span>** | 🔵 **<span style="color: #0969da;">1.026×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.302 | 0.73× | Yes |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">1.298</span>** | 🔵 **<span style="color: #0969da;">1.243</span>** | 🔵 **<span style="color: #0969da;">1.044×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.046 | 2.45× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.624</span>** | 🔵 **<span style="color: #0969da;">2.626</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.048 | 1.16× | No |
| vec3_cross | 32 | 🔵 **<span style="color: #0969da;">0.324</span>** | 🔵 **<span style="color: #0969da;">0.313</span>** | 🔵 **<span style="color: #0969da;">1.038×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.872 | 2.79× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">0.572</span>** | 🔵 **<span style="color: #0969da;">0.562</span>** | 🔵 **<span style="color: #0969da;">1.017×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.851 | 1.51× | No |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.337</span>** | 🔵 **<span style="color: #0969da;">0.343</span>** | 🔵 **<span style="color: #0969da;">0.983×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.957 | 2.79× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.560</span>** | 🔵 **<span style="color: #0969da;">0.560</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.938 | 1.68× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.260</span>** | 🔵 **<span style="color: #0969da;">0.256</span>** | 🔵 **<span style="color: #0969da;">1.016×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.869 | 3.39× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.494</span>** | 🔵 **<span style="color: #0969da;">0.497</span>** | 🔵 **<span style="color: #0969da;">0.993×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.871 | 1.75× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.393</span>** | 🟢 **<span style="color: #1a7f37;">0.196</span>** | 🟣 **<span style="color: #6f42c1;">1.999×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +99.9% | 0.713 | 3.63× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.405</span>** | 🔵 **<span style="color: #0969da;">0.398</span>** | 🔵 **<span style="color: #0969da;">1.019×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.630 | 1.58× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.600</span>** | 🟢 **<span style="color: #1a7f37;">0.429</span>** | 🟣 **<span style="color: #6f42c1;">1.400×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +40.0% | 1.541 | 3.59× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">0.815</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.634 | 2.01× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.349</span>** | 🔵 **<span style="color: #0969da;">0.354</span>** | 🔵 **<span style="color: #0969da;">0.987×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.961 | 2.72× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">0.650</span>** | 🔵 **<span style="color: #0969da;">0.663</span>** | 🔵 **<span style="color: #0969da;">0.980×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.947 | 1.43× | No |

## 4,096 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">1.950</span>** | 🔵 **<span style="color: #0969da;">2.004</span>** | 🔵 **<span style="color: #0969da;">0.973×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.296 | 2.14× | No |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.365</span>** | 🔵 **<span style="color: #0969da;">3.350</span>** | 🔵 **<span style="color: #0969da;">1.005×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.558 | 1.36× | Yes |
| mat4_multiply | 32 | 🔵 **<span style="color: #0969da;">2.192</span>** | 🔵 **<span style="color: #0969da;">2.192</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 11.212 | 5.11× | No |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">4.448</span>** | 🔵 **<span style="color: #0969da;">4.460</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 12.506 | 2.80× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">1.314</span>** | 🔵 **<span style="color: #0969da;">1.367</span>** | 🔵 **<span style="color: #0969da;">0.961×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.728 | 2.00× | No |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">2.656</span>** | 🔵 **<span style="color: #0969da;">2.608</span>** | 🔵 **<span style="color: #0969da;">1.018×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.357 | 1.29× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">0.945</span>** | 🔵 **<span style="color: #0969da;">0.945</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 2.827 | 2.99× | No |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">1.907</span>** | 🔵 **<span style="color: #0969da;">1.896</span>** | 🔵 **<span style="color: #0969da;">1.006×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.276 | 1.73× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.884</span>** | 🟢 **<span style="color: #1a7f37;">0.620</span>** | 🟣 **<span style="color: #6f42c1;">1.425×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +42.5% | 1.166 | 1.88× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.024</span>** | 🔵 **<span style="color: #0969da;">1.022</span>** | 🔵 **<span style="color: #0969da;">1.003×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.181 | 1.16× | Yes |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.321</span>** | 🔵 **<span style="color: #0969da;">0.323</span>** | 🔵 **<span style="color: #0969da;">0.992×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.934 | 2.89× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">0.998</span>** | 🔵 **<span style="color: #0969da;">0.996</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.142 | 1.15× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.372</span>** | 🔵 **<span style="color: #0969da;">0.371</span>** | 🔵 **<span style="color: #0969da;">1.004×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.709 | 1.91× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.740</span>** | 🔵 **<span style="color: #0969da;">0.740</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.771 | 1.04× | No |
| vec4_length | 32 | 🔵 **<span style="color: #0969da;">0.580</span>** | 🔵 **<span style="color: #0969da;">0.577</span>** | 🔵 **<span style="color: #0969da;">1.006×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.677 | 1.17× | No |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.579</span>** | 🔵 **<span style="color: #0969da;">0.580</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.628 | 1.08× | No |
| vec4_normalize | 32 | 🔵 **<span style="color: #0969da;">1.005</span>** | 🔵 **<span style="color: #0969da;">1.008</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.552 | 1.54× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">1.193</span>** | 🔵 **<span style="color: #0969da;">1.190</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.620 | 1.36× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.324</span>** | 🔵 **<span style="color: #0969da;">0.324</span>** | 🔵 **<span style="color: #0969da;">0.997×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.965 | 2.97× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.002</span>** | 🔵 **<span style="color: #0969da;">1.003</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.148 | 1.14× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔴 **<span style="color: #cf222e;">2.466</span>** | 🟢 **<span style="color: #1a7f37;">2.327</span>** | 🟣 **<span style="color: #6f42c1;">1.060×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +6.0% | 4.769 | 2.05× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.761</span>** | 🔵 **<span style="color: #0969da;">3.830</span>** | 🔵 **<span style="color: #0969da;">0.982×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.843 | 1.26× | No |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">4.338</span>** | 🟢 **<span style="color: #1a7f37;">3.921</span>** | 🟣 **<span style="color: #6f42c1;">1.106×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.6% | 18.695 | 4.77× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">7.151</span>** | 🔵 **<span style="color: #0969da;">7.296</span>** | 🔵 **<span style="color: #0969da;">0.980×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.847 | 2.72× | No |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">2.567</span>** | 🔵 **<span style="color: #0969da;">2.468</span>** | 🔵 **<span style="color: #0969da;">1.040×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.556 | 1.85× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">4.025</span>** | 🔵 **<span style="color: #0969da;">4.023</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.666 | 1.16× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">1.767</span>** | 🟢 **<span style="color: #1a7f37;">1.480</span>** | 🟣 **<span style="color: #6f42c1;">1.194×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +19.4% | 3.178 | 2.15× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.294</span>** | 🔵 **<span style="color: #0969da;">2.295</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.242 | 1.41× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.696</span>** | 🟢 **<span style="color: #1a7f37;">0.628</span>** | 🟣 **<span style="color: #6f42c1;">1.109×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +10.9% | 1.166 | 1.86× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.380</span>** | 🔵 **<span style="color: #0969da;">1.379</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.518 | 1.10× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.970</span>** | 🟢 **<span style="color: #1a7f37;">0.813</span>** | 🟣 **<span style="color: #6f42c1;">1.194×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +19.4% | 1.396 | 1.72× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">1.676</span>** | 🔵 **<span style="color: #0969da;">1.677</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.856 | 1.11× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.425</span>** | 🟢 **<span style="color: #1a7f37;">0.290</span>** | 🟣 **<span style="color: #6f42c1;">1.467×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +46.7% | 0.868 | 3.00× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.788</span>** | 🔵 **<span style="color: #0969da;">0.788</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.190 | 1.51× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.392</span>** | 🟢 **<span style="color: #1a7f37;">0.242</span>** | 🟣 **<span style="color: #6f42c1;">1.620×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +62.0% | 0.682 | 2.82× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.473</span>** | 🔵 **<span style="color: #0969da;">0.485</span>** | 🔵 **<span style="color: #0969da;">0.977×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.612 | 1.26× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.631</span>** | 🟢 **<span style="color: #1a7f37;">0.401</span>** | 🟣 **<span style="color: #6f42c1;">1.573×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +57.3% | 1.525 | 3.80× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.970</span>** | 🔵 **<span style="color: #0969da;">0.972</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.636 | 1.68× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.977</span>** | 🟢 **<span style="color: #1a7f37;">0.806</span>** | 🟣 **<span style="color: #6f42c1;">1.212×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +21.2% | 1.398 | 1.73× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.677</span>** | 🔵 **<span style="color: #0969da;">1.676</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.859 | 1.11× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">2.230</span>** | 🔵 **<span style="color: #0969da;">2.207</span>** | 🔵 **<span style="color: #0969da;">1.010×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.769 | 2.16× | No |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">3.605</span>** | 🔵 **<span style="color: #0969da;">3.627</span>** | 🔵 **<span style="color: #0969da;">0.994×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.843 | 1.34× | Yes |
| mat4_multiply | 32 | 🔴 **<span style="color: #cf222e;">6.607</span>** | 🟢 **<span style="color: #1a7f37;">6.128</span>** | 🟣 **<span style="color: #6f42c1;">1.078×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +7.8% | 18.695 | 3.05× | Yes |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">10.420</span>** | 🔵 **<span style="color: #0969da;">10.321</span>** | 🔵 **<span style="color: #0969da;">1.010×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.847 | 1.92× | Yes |
| mat4_transpose | 32 | 🔵 **<span style="color: #0969da;">2.359</span>** | 🔵 **<span style="color: #0969da;">2.307</span>** | 🔵 **<span style="color: #0969da;">1.022×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.556 | 1.98× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">3.670</span>** | 🔵 **<span style="color: #0969da;">3.748</span>** | 🔵 **<span style="color: #0969da;">0.979×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.666 | 1.24× | No |
| mat4_vector | 32 | 🔵 **<span style="color: #0969da;">1.674</span>** | 🔵 **<span style="color: #0969da;">1.617</span>** | 🔵 **<span style="color: #0969da;">1.035×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.178 | 1.97× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">2.556</span>** | 🔵 **<span style="color: #0969da;">2.557</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.242 | 1.27× | No |
| vec3_cross | 32 | 🔴 **<span style="color: #cf222e;">0.638</span>** | 🟢 **<span style="color: #1a7f37;">0.569</span>** | 🟣 **<span style="color: #6f42c1;">1.122×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +12.2% | 1.166 | 2.05× | Yes |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.356</span>** | 🔵 **<span style="color: #0969da;">1.355</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.518 | 1.12× | No |
| vec4_add | 32 | 🔴 **<span style="color: #cf222e;">0.883</span>** | 🟢 **<span style="color: #1a7f37;">0.768</span>** | 🟣 **<span style="color: #6f42c1;">1.150×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.0% | 1.396 | 1.82× | Yes |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">1.695</span>** | 🔵 **<span style="color: #0969da;">1.695</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.856 | 1.09× | No |
| vec4_dot | 32 | 🔴 **<span style="color: #cf222e;">0.318</span>** | 🟢 **<span style="color: #1a7f37;">0.290</span>** | 🟣 **<span style="color: #6f42c1;">1.095×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +9.5% | 0.868 | 2.99× | Yes |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.791</span>** | 🔵 **<span style="color: #0969da;">0.791</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.190 | 1.50× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.391</span>** | 🟢 **<span style="color: #1a7f37;">0.222</span>** | 🟣 **<span style="color: #6f42c1;">1.759×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +75.9% | 0.682 | 3.07× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.416</span>** | 🔵 **<span style="color: #0969da;">0.416</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.612 | 1.47× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.620</span>** | 🟢 **<span style="color: #1a7f37;">0.407</span>** | 🟣 **<span style="color: #6f42c1;">1.523×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +52.3% | 1.525 | 3.75× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.933</span>** | 🔵 **<span style="color: #0969da;">0.933</span>** | 🔵 **<span style="color: #0969da;">1.001×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.636 | 1.75× | No |
| vec4_scale_add | 32 | 🔴 **<span style="color: #cf222e;">0.885</span>** | 🟢 **<span style="color: #1a7f37;">0.766</span>** | 🟣 **<span style="color: #6f42c1;">1.155×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +15.5% | 1.398 | 1.82× | Yes |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.694</span>** | 🔵 **<span style="color: #0969da;">1.697</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.859 | 1.10× | No |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | 🔵 **<span style="color: #0969da;">4.332</span>** | 🔵 **<span style="color: #0969da;">4.185</span>** | 🔵 **<span style="color: #0969da;">1.035×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.769 | 1.14× | Yes |
| mat4_inverse_trs | 64 | 🔵 **<span style="color: #0969da;">6.785</span>** | 🔵 **<span style="color: #0969da;">6.763</span>** | 🔵 **<span style="color: #0969da;">1.003×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.843 | 0.72× | No |
| mat4_multiply | 32 | 🔵 **<span style="color: #0969da;">7.281</span>** | 🔵 **<span style="color: #0969da;">6.973</span>** | 🔵 **<span style="color: #0969da;">1.044×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 18.695 | 2.68× | No |
| mat4_multiply | 64 | 🔵 **<span style="color: #0969da;">12.442</span>** | 🔵 **<span style="color: #0969da;">12.623</span>** | 🔵 **<span style="color: #0969da;">0.986×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 19.847 | 1.57× | No |
| mat4_transpose | 32 | 🔴 **<span style="color: #cf222e;">4.038</span>** | 🟢 **<span style="color: #1a7f37;">3.062</span>** | 🟣 **<span style="color: #6f42c1;">1.318×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +31.8% | 4.556 | 1.49× | Yes |
| mat4_transpose | 64 | 🔵 **<span style="color: #0969da;">6.435</span>** | 🔵 **<span style="color: #0969da;">6.432</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 4.666 | 0.73× | No |
| mat4_vector | 32 | 🔴 **<span style="color: #cf222e;">2.571</span>** | 🟢 **<span style="color: #1a7f37;">2.029</span>** | 🟣 **<span style="color: #6f42c1;">1.267×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +26.7% | 3.178 | 1.57× | Yes |
| mat4_vector | 64 | 🔵 **<span style="color: #0969da;">3.742</span>** | 🔵 **<span style="color: #0969da;">3.726</span>** | 🔵 **<span style="color: #0969da;">1.004×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 3.242 | 0.87× | Yes |
| vec3_cross | 32 | 🔵 **<span style="color: #0969da;">0.522</span>** | 🔵 **<span style="color: #0969da;">0.521</span>** | 🔵 **<span style="color: #0969da;">1.002×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.166 | 2.24× | No |
| vec3_cross | 64 | 🔵 **<span style="color: #0969da;">1.321</span>** | 🔵 **<span style="color: #0969da;">1.310</span>** | 🔵 **<span style="color: #0969da;">1.008×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.518 | 1.16× | No |
| vec4_add | 32 | 🔵 **<span style="color: #0969da;">0.709</span>** | 🔵 **<span style="color: #0969da;">0.714</span>** | 🔵 **<span style="color: #0969da;">0.992×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.396 | 1.95× | No |
| vec4_add | 64 | 🔵 **<span style="color: #0969da;">1.645</span>** | 🔵 **<span style="color: #0969da;">1.646</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.856 | 1.13× | No |
| vec4_dot | 32 | 🔵 **<span style="color: #0969da;">0.275</span>** | 🔵 **<span style="color: #0969da;">0.276</span>** | 🔵 **<span style="color: #0969da;">0.996×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.868 | 3.14× | No |
| vec4_dot | 64 | 🔵 **<span style="color: #0969da;">0.780</span>** | 🔵 **<span style="color: #0969da;">0.781</span>** | 🔵 **<span style="color: #0969da;">0.999×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.190 | 1.52× | No |
| vec4_length | 32 | 🔴 **<span style="color: #cf222e;">0.395</span>** | 🟢 **<span style="color: #1a7f37;">0.213</span>** | 🟣 **<span style="color: #6f42c1;">1.855×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +85.5% | 0.682 | 3.20× | Yes |
| vec4_length | 64 | 🔵 **<span style="color: #0969da;">0.406</span>** | 🔵 **<span style="color: #0969da;">0.407</span>** | 🔵 **<span style="color: #0969da;">1.000×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 0.612 | 1.51× | No |
| vec4_normalize | 32 | 🔴 **<span style="color: #cf222e;">0.610</span>** | 🟢 **<span style="color: #1a7f37;">0.432</span>** | 🟣 **<span style="color: #6f42c1;">1.413×</span>** | 🟣 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> | +41.3% | 1.525 | 3.53× | Yes |
| vec4_normalize | 64 | 🔵 **<span style="color: #0969da;">0.907</span>** | 🔵 **<span style="color: #0969da;">0.917</span>** | 🔵 **<span style="color: #0969da;">0.989×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.636 | 1.78× | No |
| vec4_scale_add | 32 | 🔵 **<span style="color: #0969da;">0.725</span>** | 🔵 **<span style="color: #0969da;">0.726</span>** | 🔵 **<span style="color: #0969da;">0.998×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.398 | 1.93× | No |
| vec4_scale_add | 64 | 🔵 **<span style="color: #0969da;">1.650</span>** | 🔵 **<span style="color: #0969da;">1.676</span>** | 🔵 **<span style="color: #0969da;">0.984×</span>** | 🔵 <span style="color: #0969da;">Within&nbsp;5%</span> | — | 1.859 | 1.11× | Yes |

## Source, host and controls

Source and evidence inventories: [sources](standard-math-master-2026-11-21/sources.json), [flags](standard-math-master-2026-11-21/flags.json), [benchmark binaries](standard-math-master-2026-11-21/binaries.json), [native correctness report](../../validation/arm64/mac-arm-validation-2026-11-21.md), [native validation totals](standard-math-master-2026-11-21/native-validation-summary.json), [publication receipt](standard-math-master-2026-11-21/publication-receipt.json).

- Tested full commit: `dc802e466663d61d9e219bd7c7351aa8ac312204`.
- Tested full tree: `7eaae08ad9b0a813ace5c74aff9be6ff2fc7630b`.
- Branch/worktree state before and after: Fresh detached worktree at the exact commit/tree; clean before and after. The primary master checkout and unrelated work were preserved.
- Dirty-source provenance, if applicable: Not applicable: clean committed source; the complete raw-byte manifest is [source-manifest.json](standard-math-master-2026-11-21/source-manifest.json).
- Comparison source, if used: No separate source candidate/control comparison; scalar and both ARM backends use the same frozen source.
- Benchmark source/build-file comparison: All tracked source hashes verified before/after against the frozen source, including benchmark source/build files listed in [metadata.json](standard-math-master-2026-11-21/candidate/metadata.json); no benchmark source edits.
- Physical host, architecture and native-execution proof: Apple M2; native arm64 Python and Mach-O executables; sysctl.proc_translated=0.
- OS/version/build: ProductName:		macOS; ProductVersion:		26.7; BuildVersion:		25G229.
- Compiler/version/target: Apple clang version 21.0.0 (clang-2100.1.1.101); Target: arm64-apple-darwin25.6.0.
- SDK, CMake/CTest, build tool and Python: SDK 26.5; cmake version 4.4.4; ctest version 4.4.4; Ninja 1.11.1.git.kitware.jobserver-1; native Python 3.9.6.
- Execution interval: 2026-11-21T02:59:08.606723+00:00 through 2026-11-21T03:01:22.594361+00:00; report-date timezone: Europe/Stockholm.
- Exact Release compile/link flags: `-O3 -DNDEBUG -std=gnu++20 -arch arm64 -isysroot <SDK> -ffp-contract=off -mcpu=apple-m1; STANDARD_IMPL=0/2/3; SIMD MAX_VECTOR_SIZE=512 with backend-specific definitions; see [metadata.json](standard-math-master-2026-11-21/candidate/metadata.json) for all six translation units`, `-O3 -DNDEBUG -arch arm64 -isysroot <SDK> -Wl,-search_paths_first -Wl,-headerpad_max_install_names`.
- Scalar vectorization controls and disassembly findings: -fno-vectorize -fno-slp-vectorize; fresh scalar disassembly checked with no packed FP arithmetic matches; [scalar-codegen.json](standard-math-master-2026-11-21/scalar-codegen.json) records the result and binary SHA-256. Loads/stores and library callees are separate from this finding.
- Timing build controls: Inspected actual compile/link commands: no broad fast-math, LTO or sanitizer flags in timing builds. No fixed-frequency/core-affinity control was imposed.
- Correctness-only builds, profiles and sanitizer coverage: All eight full native Release/strict UBSan Debug profiles passed; six additional benchmark CTest runs passed in Release/strict UBSan Debug, and all six workset-correctness probes passed.
- Registered/passed/failed/skipped/timed-out counts and attempts: 8,036 registered/passed native cases; zero failed/skipped/timed-out; eight separate smoke passes and no native retries. Final benchmark session: six CTest passes plus six workset probes. The earlier setup had three Release CTest passes, then stopped at the overly broad task-local flag audit before timing; fresh benchmark builds were used for the correction.
- Strict UBSan flags/runtime options and control results, when used: -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer; UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1. Positive exit 0; intentional overflow exit -6, required diagnostic present and no continuation.
- Read-only integrity checks before/after: Maintenance integrity, repository/link checks, 41 isolated integrity controls, four native standalone consumers and six target-rejection controls passed. Full raw-source verification before/after passed.
- Protected originals and dependency integrity: VCL 667577495efaa36a10cbb5d6cb8c942c23913848, tree 36aee120c28c9c64e62903bfc81139003e37cedd: all 28 originals exact (27 root plus audit/vectorclass-README.md). Root <span style="color: #bc4c00;">sse2neon</span>.h retained SHA-256 cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff; both audit pins preserved.
- Source edits, inventory changes and evidence of stability: No tracked runtime/test/maintained-runner/build-policy edits or source inventory changes occurred during execution. The auxiliary correctness driver renames the unchanged benchmark entry point and calls its existing check-only functions at 128/4096; it is separately identified probe evidence, not a modified implementation.

Optional kernel/whole-binary comparison: [scalar-codegen.json](standard-math-master-2026-11-21/scalar-codegen.json) and [benchmark-correctness.json](standard-math-master-2026-11-21/correctness.json); raw scalar disassembly retained locally.
No cross-source or cross-session executable-equivalence comparison was performed. Scalar disassembly and all new executable hashes belong only to this run

Keep the 28 upstream originals (27 root files plus audit/vectorclass-README.md) and `audit/vectorclass-2.02.04.json` byte-exact,
along with root `sse2neon.h` and its required `audit/sse2neon-2272251.json` pin.
The vendor catalog is retired. On an unexplained integrity
mismatch, preserve the failed check and stop. Header generation and the old
adaptation/backend pipeline are retired. Approved ARM changes belong in the
authoritative root `*_neon.h` sources, with new source IDs and fresh validation;
follow the [current ownership rules](https://github.com/DigitalExpressions/vcl2-xa64/blob/dc802e466663d61d9e219bd7c7351aa8ac312204/docs/backend-layout.md). An upstream update
requires its own approved scope.

## Measurement and correctness scope

- Operations, precision, object counts, layouts and logical widths: Ten operations, FP32/FP64, counts 128 and 4096; AoS and SoA widths 128/256/512, complete 160-row inventory.
- Invocation count, order and scheduling: Three rounds; backend order scalar/compat/neon, compat/neon/scalar, neon/scalar/compat; both counts in each round. Exactly 18 sequential invocations, [schedule.json](standard-math-master-2026-11-21/schedule.json); no overlapping builds/correctness work during timing.
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
- Cross-backend comparison scope: Canonical AoS-ordered first-invocation output buffers for all 160 three-way rows; scalar controls reused across widths are counted per report row, not as independent datasets. Full per-row [output-comparisons.json](standard-math-master-2026-11-21/output-comparisons.json) retained; repeated invocations checked for exact within-backend repeatability.
- Scalar/Neon differing fields, maximum absolute error and ULP distance:
  7,913 of 2,365,440 compared fields differ in bits; maximum absolute difference 9.53674316e-07; maximum ordered-bit ULP distance 768.
- Compatibility/Neon differing fields, maximum absolute error and ULP distance:
  21,084 of 2,365,440 compared fields differ in bits; maximum absolute difference 4.76837158e-07; maximum ordered-bit ULP distance 4.
- Failed checks, retries, missing profiles or unavailable controls:
  One task-local setup audit failed by applying benchmark FP requirements to the untimed integer feature detector; all benchmark units already met those requirements. The corrected audit covers all six benchmark units, with fresh builds and the first attempt retained. No numerical/reference/canary/sanitizer failures and no timing retries. Fixed --check boundary counts are supplemented by unchanged-harness check-only probes at timing counts. No ASan, Windows, other hosts/toolchains, external consumers, native FP16 or universal cross-host parity claim.

Any cross-backend bit differences remain separate from the independent numerical acceptance test. All measured outputs satisfy the original finite-output/64-epsilon-scaled gate, including cancellation-sensitive operations. Ordered-bit ULP distances are descriptive and do not replace that gate; -0 and +0 are adjacent in the ordered-bit convention

Output bit differences and independent-reference accuracy are separate facts.
State which outputs were compared, whether scalar rows were reused across
widths, and whether differences are within the existing contract. A large raw
ULP distance near cancellation must not replace the operation's acceptance
rule, and a passing accuracy gate does not establish universal bitwise identity.

- Core placement/frequency/power settings and sleep prevention: Native Apple M2, caffeinate -i sleep prevention; sequential timing; no affinity, frequency or machine-wide power-policy changes.
- Raw trial-batch duration range: 7.877334–22.427958 ms.
- Host load and thermal/performance observations: Per-invocation uptime/load, thermal and power-source observations retained in [metadata.json](standard-math-master-2026-11-21/candidate/metadata.json); no fixed-frequency measurement or claim about the cause of timing variability.
- Rows with disjoint invocation median ranges: 63/160.
- Leads with overlapping ranges or other variability: 0 leads outside the practical band have overlapping invocation ranges. Three invocations describe this session, not statistical significance or a universal application benefit.
- Rows slower than scalar, including which backend: 5 <span style="color: #6f42c1;">Direct&nbsp;Neon</span> and 6 compatibility row/backend instances; complete named inventory and elapsed costs in [summary.json](standard-math-master-2026-11-21/candidate/summary.json).

Fresh native M2 microbenchmarks only. Historical 19 November timings are context, not matched controls for these measurements. No source-change speedup claim. The existing Windows full gate remains failed/incomplete: 5,387 passed, 249 failed, 76 not run of 5,712, plus twelve examples

## Evidence and next use

- Ignored raw evidence directory: `build/arm64-roundtrip-2026-11-21-20261120T233452093509Z/`.
- Attempt identifiers and failed/replayed attempts: One full native attempt. Benchmark setup 01 stopped at an overly broad scratch audit after three passing Release CTest cases, before timing; setup 02 used fresh builds and passed every benchmark gate. All 18 timing invocations ran once without replays; [setup-attempts.json](standard-math-master-2026-11-21/setup-attempts.json) and [attempts.json](standard-math-master-2026-11-21/attempts.json).
- Compact archive filename: `portable-results-20261121T030229947697Z.zip`.
- Archive SHA256: `dd6c2a60734830f4f154d59d809917d636b218dc4027e06f9cd06af3ff378dde`.
- Verified entry count, CRC/hash audit and contents: 23 entries; ZIP CRC and every entry SHA-256 verified. Textual data packet excludes reports to avoid checksum cycles; complete raw logs/builds and binary dumps remain separately retained under build.
- Portable archive receipt: [archive-receipt.json](standard-math-master-2026-11-21/archive-receipt.json).
- Portable metadata, results and checksum inventory: [metadata](standard-math-master-2026-11-21/candidate/metadata.json), [raw trials and rows](standard-math-master-2026-11-21/candidate/results.json), [CSV](standard-math-master-2026-11-21/candidate/results.csv), [summary](standard-math-master-2026-11-21/candidate/summary.json), [source manifest](standard-math-master-2026-11-21/source-manifest.json), [checksums](standard-math-master-2026-11-21/checksums.sha256).
- Separately retained records/supplements and hashes: [retained-archive-receipts.json](standard-math-master-2026-11-21/retained-archive-receipts.json) identifies the local native runner archive and exact source tar. Complete native executable hashes and named case statuses remain in the separately checksummed native runner archive identified in [retained-archive-receipts.json](standard-math-master-2026-11-21/retained-archive-receipts.json); benchmark/probe binary hashes are in [binaries.json](standard-math-master-2026-11-21/binaries.json). Generated inputs, control sources, executables/disassembly and binary output dumps remain in retained builds; the portable packet does not claim to contain them.
- Retained build/source/worktree resources and any explicitly authorized cleanup:
  Fresh detached worktree, all native/benchmark builds, tools, dependencies and raw evidence retained. No cleanup authorized or performed.

Retain source/compiler/flag inventories, binary hashes, complete raw trials,
output dumps, input hashes, configure/build/test logs, JUnit, failed attempts,
reference/probe sources and relevant code-generation records. Exclude machine
paths, credentials and compiled binaries from the portable report/data.

This is a current-master checkpoint for the exact source/tool/host recorded. A future implementation comparison needs fresh matched sessions. The maintainer subsequently requested this documentation copy; further runtime work and Git publication require their own authorization

Record this task's actual Git/publication actions: During execution: no commits, pushes, merges, PRs, branch retirement or tracked source edits; one fresh detached validation worktree was created. This later documentation copy does not change the tested source identity.
Follow the maintainer's task-specific authorization for publication or cleanup.
Future source changes require new recorded measurements and matched controls;
preserve completed dated reports and all original numerical data.
