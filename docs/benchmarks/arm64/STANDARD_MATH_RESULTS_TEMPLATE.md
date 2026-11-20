# Standard vector and matrix benchmarks - {{REPORT_DATE}}

> **TEMPLATE — no measured results.** Copy this file to
> `STANDARD_MATH_RESULTS_YYYY-MM-DD.md`, using the benchmark execution date.
> Replace every `{{PLACEHOLDER}}` and complete each `UPDATE` instruction below.
> Remove this setup block and editing comments from the completed report.

<!-- UPDATE: Keep the same report structure: summary, reading guide, eight
result tables, then Source/host/controls, Measurement/correctness, and Evidence.
The default table inventory is ten operations, FP32/FP64, counts 128/4096,
AoS plus SoA widths 128/256/512: 160 three-way rows. If the approved workload
changes, reconcile headings and the complete measured inventory explicitly.
Do not label an unmeasured row as a pass or copy results from an earlier run.
Worked examples and color thresholds in the reading guide are explanations,
not measurements; keep them unless the documented presentation policy changes. -->

**Scalar FP32/FP64 versus <span style="color: #bc4c00;">sse2neon</span> versus <span style="color: #6f42c1;">Direct&nbsp;Neon</span>**:
{{WORKLOAD_SUMMARY}}. The tables describe the native measurements from
**{{REPORT_DATE}}**, at source `{{TESTED_COMMIT_SHORT}}`.

**Run status:** {{PASSED_FAILED_OR_INCOMPLETE_AND_REASON}}.

<!-- UPDATE: Summarize what was measured and why. Report any source or
code-generation comparison only if it was actually performed for this run.
Do not retain a previous run's identical-binary or performance-gain claim.
Keep baseline/control reports and standalone baseline datasets under ignored
build/ during tests; publish the selected implementation's report and results. -->

{{PURPOSE_AND_MAIN_FINDINGS}}

All slower and variable rows are retained. Before/after comparisons and full
suite runs use their own matched controls; do not mix times between runs.

<!-- UPDATE: Replace these code-formatted destinations with working Markdown
links to this run's portable files. Do not point at an older run's data.
If a record was not collected, say so instead of linking to prior evidence. -->

- CSV results: `{{DATA_DIRECTORY}}/results.csv`
- JSON results and raw trials: `{{DATA_DIRECTORY}}/results.json`
- Summary/audit: `{{DATA_DIRECTORY}}/summary.json`
- Metadata and checksums: `{{METADATA_PATH}}`, `{{CHECKSUM_INVENTORY_PATH}}`

In this run, <span style="color: #6f42c1;">Direct&nbsp;Neon</span> wins or is within
the shared 5% band in **{{NEON_WINS_PLUS_WITHIN_BAND}}/{{TOTAL_ROWS}} rows**:
**{{NEON_WIN_ROWS}}** Direct Neon leads, **{{WITHIN_5_PERCENT_ROWS}}** within 5%,
and **{{COMPAT_WIN_ROWS}}** sse2neon leads.

{{CONSISTENCY_AND_SLOWER_ROW_QUALIFICATIONS}}

<!-- UPDATE: Derive every count from the new data and reconcile it with the
complete row inventory. Record omitted/incomplete measurements explicitly.
Do not turn overlapping median ranges into a statistical-significance claim. -->

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

**Executed measurement summary:** {{PROCESS_COUNT}} measured processes /
{{SHAPE_COUNT}} shapes / {{TRIAL_BATCH_COUNT}} trial batches.
**Correctness summary:** {{REFERENCE_INPUT_AND_CANARY_OUTCOME}}.
**Host and build summary:** {{CPU_OS_COMPILER_SDK_ARCHITECTURE_AND_FLAGS}}.

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
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 128 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 4,096 objects — Array of Structures (AoS): SIMD across one object’s components

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 128 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 256 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## 4,096 objects — Structure of Arrays (SoA): SIMD across objects / logical 512 bits

<!-- standard-math-table-notes -->
- **<span style="color: #bc4c00;">sse2neon</span>:** SSE operations translated to Neon SIMD.
- **<span style="color: #6f42c1;">Direct&nbsp;Neon</span>:** native Neon SIMD.
- **(nsec/op):** nanoseconds per complete vector/matrix operation; lower is better.
- **Scalar:** float/double arithmetic without SIMD; auto-vectorization disabled.
<!-- /standard-math-table-notes -->

| Operation | FP | **<span style="color: #bc4c00;">sse2neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span><br>(nsec/op)** | **<span style="color: #6f42c1;">Direct&nbsp;Neon</span> /<br><span style="color: #bc4c00;">sse2neon</span>** | **Winning<br>backend** | **Throughput<br>gain&nbsp;(%)** | **Scalar<br>(nsec/op)** | Scalar /<br><span style="color: #6f42c1;">Direct&nbsp;Neon</span> | Consistent<br>Timing |
| --- | ---: | ---: | ---: | ---: | --- | ---: | ---: | ---: | --- |
| mat4_inverse_trs | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_inverse_trs | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_multiply | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_transpose | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| mat4_vector | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec3_cross | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_dot | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_length | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_normalize | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 32 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |
| vec4_scale_add | 64 | **{{COMPAT_NS}}** | **{{NEON_NS}}** | **{{COMPAT_OVER_NEON}}×** | {{WINNING_BACKEND}} | {{THROUGHPUT_GAIN_OR_DASH}} | {{SCALAR_NS}} | {{SCALAR_OVER_NEON}}× | {{CONSISTENT_YES_NO}} |

## Source, host and controls

<!-- UPDATE: Replace every field with evidence from this execution. Actual
commit/tree and source hashes identify the tested code; later documentation
commits do not acquire earlier results. Keep machine-specific paths out of
the portable report. Optional comparisons must say not performed when absent. -->

- Tested full commit: `{{TESTED_COMMIT}}`.
- Tested full tree: `{{TESTED_TREE}}`.
- Branch/worktree state before and after: {{SOURCE_STATE_AND_UNRELATED_WORK}}.
- Dirty-source provenance, if applicable: {{BASE_COMMIT_PATCH_SHA256_AND_MANIFEST_OR_NOT_APPLICABLE}}.
- Comparison source, if used: {{COMPARISON_COMMIT_TREE_OR_NOT_PERFORMED}}.
- Benchmark source/build-file comparison: {{COMPARED_FILES_HASHES_AND_FINDINGS_OR_NOT_PERFORMED}}.
- Physical host, architecture and native-execution proof: {{CPU_MODEL_ARCHITECTURE_AND_ROSETTA_STATE}}.
- OS/version/build: {{OS_VERSION_BUILD}}.
- Compiler/version/target: {{COMPILER_VERSION_AND_TARGET}}.
- SDK, CMake/CTest, build tool and Python: {{SDK_AND_TOOL_VERSIONS}}.
- Execution interval: {{START_UTC}} through {{END_UTC}}; report-date timezone: {{TIMEZONE}}.
- Exact Release compile/link flags: `{{RELEASE_FLAGS}}`, `{{LINK_FLAGS}}`.
- Scalar vectorization controls and disassembly findings: {{SCALAR_FLAGS_AND_PACKED_FP_CHECK}}.
- Timing build controls: {{NO_FAST_MATH_LTO_OR_SANITIZER_VERIFICATION}}.
- Correctness-only builds, profiles and sanitizer coverage: {{CORRECTNESS_PROFILES_AND_RESULTS}}.
- Registered/passed/failed/skipped/timed-out counts and attempts: {{CORRECTNESS_COUNTS_AND_RETRIES}}.
- Strict UBSan flags/runtime options and control results, when used: {{UBSAN_FLAGS_CONTROLS_OR_COVERAGE_GAP}}.
- Read-only integrity checks before/after: {{INTEGRITY_RECORDS_AND_OUTCOME}}.
- Protected originals and dependency integrity: {{PINNED_COMMIT_TREE_FILE_COUNT_AND_BEFORE_AFTER_HASH_RESULTS}}.
- Source edits, inventory changes and evidence of stability: {{SOURCE_INTEGRITY_FINDINGS}}.

Optional kernel/whole-binary comparison: {{CODEGEN_AND_BINARY_HASH_RECORD_PATHS_OR_NOT_PERFORMED}}.
{{ACTUAL_COMPARISON_FINDINGS_AND_LIMITS}}

Keep the 28 upstream root files and `audit/vectorclass-2.02.04.json` byte-exact,
along with root `sse2neon.h` and its required `audit/sse2neon-2272251.json` pin.
The vendor catalog is retired. On an unexplained integrity
mismatch, preserve the failed check and stop. Header generation and the old
adaptation/backend pipeline are retired. Approved ARM changes belong in the
authoritative root `*_neon.h` sources, with new source IDs and fresh validation;
follow the [current ownership rules](../../backend-layout.md). An upstream update
requires its own approved scope.

## Measurement and correctness scope

<!-- UPDATE: Describe actual measurements, not the default protocol as if it
had already run. The usual protocol uses three invocations per backend/count,
nine warmed calibrated trials, sequential timing processes, and rotating
backend order. Retain every attempt, outlier, seed and original acceptance gate. -->

- Operations, precision, object counts, layouts and logical widths: {{MEASURED_WORKLOAD_INVENTORY}}.
- Invocation count, order and scheduling: {{INVOCATIONS_ORDER_AND_SEQUENTIAL_EXECUTION}}.
- Calibration, warm-up and trials per shape: {{CALIBRATION_WARMUP_AND_TRIALS}}.
- Aggregation: {{STATISTIC_AND_REUSE_OF_SCALAR_CONTROLS_WITHIN_THIS_RUN}}.
- Actual totals: {{PROCESS_COUNT}} processes, {{SHAPE_COUNT}} shapes,
  {{TRIAL_BATCH_COUNT}} trial batches and {{REPORT_ROW_COUNT}} comparison rows.
- Independent reference methods and unchanged gate: {{REFERENCE_METHODS_AND_GATE}}.
- Output fields per validation pass: {{FIELDS_PER_PASS}}; combined before/after
  timing checks: {{TOTAL_OUTPUT_FIELD_CHECKS}}.
- Maximum normalized reference error: {{MAX_REFERENCE_ERROR}};
  outcome: {{REFERENCE_PASS_FAIL_AND_FAILURE_DETAILS}}.
- Input hashes, canaries, memory layout and partial counts: {{INPUT_AND_MEMORY_CONTROL_FINDINGS}}.
- FPCR/rounding/flush-to-zero state: {{FP_ENVIRONMENT_AND_VERIFICATION}}.
- Output repeatability within each backend/shape: {{REPEATABILITY_FINDINGS}}.
- Cross-backend comparison scope: {{COMPARED_OUTPUTS_AND_COUNTING_RULES}}.
- Scalar/Neon differing fields, maximum absolute error and ULP distance:
  {{SCALAR_NEON_OUTPUT_DIFFERENCES}}.
- Compatibility/Neon differing fields, maximum absolute error and ULP distance:
  {{COMPAT_NEON_OUTPUT_DIFFERENCES}}.
- Failed checks, retries, missing profiles or unavailable controls:
  {{FAILURES_RETRIES_AND_COVERAGE_GAPS}}.

{{EXPLAIN_NUMERICAL_DIFFERENCES_USING_THE_EXISTING_REFERENCE_GATE}}

Output bit differences and independent-reference accuracy are separate facts.
State which outputs were compared, whether scalar rows were reused across
widths, and whether differences are within the existing contract. A large raw
ULP distance near cancellation must not replace the operation's acceptance
rule, and a passing accuracy gate does not establish universal bitwise identity.

- Core placement/frequency/power settings and sleep prevention: {{HOST_CONTROLS}}.
- Raw trial-batch duration range: {{MIN_BATCH_MS}}–{{MAX_BATCH_MS}} ms.
- Host load and thermal/performance observations: {{LOAD_AND_THERMAL_OBSERVATIONS}}.
- Rows with disjoint invocation median ranges: {{DISJOINT_RANGE_ROWS}}/{{TOTAL_ROWS}}.
- Leads with overlapping ranges or other variability: {{VARIABILITY_QUALIFICATIONS}}.
- Rows slower than scalar, including which backend: {{SLOWER_THAN_SCALAR_COUNTS_AND_ROWS}}.

{{MEASUREMENT_LIMITS_AND_UNTESTED_PLATFORMS_OR_APIS}}

## Evidence and next use

<!-- UPDATE: Point only to evidence from this run. Retain complete raw records
under ignored build/, including unsuccessful attempts and generated inputs.
Replace code-formatted portable destinations with working links after files
exist. If compact packets omit records, identify and checksum the supplement. -->

- Ignored raw evidence directory: `build/{{RUN_ID}}/`.
- Attempt identifiers and failed/replayed attempts: {{ATTEMPT_INVENTORY}}.
- Compact archive filename: `{{ARCHIVE_FILENAME}}`.
- Archive SHA256: `{{ARCHIVE_SHA256}}`.
- Verified entry count, CRC/hash audit and contents: {{ARCHIVE_VERIFICATION_AND_COVERAGE}}.
- Portable archive receipt: `{{ARCHIVE_RECEIPT_PATH}}`.
- Portable metadata, results and checksum inventory: {{PORTABLE_RECORD_PATHS}}.
- Separately retained records/supplements and hashes: {{SUPPLEMENTS_OR_NOT_NEEDED}}.
- Retained build/source/worktree resources and any explicitly authorized cleanup:
  {{RETENTION_AND_CLEANUP_STATUS}}.

Retain source/compiler/flag inventories, binary hashes, complete raw trials,
output dumps, input hashes, configure/build/test logs, JUnit, failed attempts,
reference/probe sources and relevant code-generation records. Exclude machine
paths, credentials and compiled binaries from the portable report/data.

{{BASELINE_USE_NEXT_VALIDATION_STEP_AND_COVERAGE_LIMITS}}

Record this task's actual Git/publication actions: {{COMMIT_PUSH_MERGE_OR_NONE}}.
Follow the maintainer's task-specific authorization for publication or cleanup.
Future source changes require new recorded measurements and matched controls;
preserve completed dated reports and all original numerical data.
