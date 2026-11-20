## How to read the tables

**The competing columns are sse2neon and Direct Neon.** Their adjacent timing
numbers are bold and colored. The **Winning backend** column names the faster backend,
or says **Within&nbsp;5%** inside the shared 5% throughput band.
The adjacent **Throughput gain (%)** column shows the winner's percentage lead.
**Scalar means no SIMD in this benchmark.** It uses ordinary float/double
arithmetic with the compiler's automatic SIMD vectorization disabled. The
scalar result is the baseline for measuring how much SIMD helps. Both
sse2neon and Direct Neon use SIMD instructions; they are two implementations
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
| **sse2neon<br>(nsec/op)** | Time per operation using the SSE-to-Neon compatibility backend. Lower is better. |
| **Direct Neon<br>(nsec/op)** | Time per operation using the Direct Neon backend. Lower is better. |
| **Direct Neon /<br>sse2neon** | Direct Neon's throughput relative to sse2neon: **sse2neon time ÷ Direct Neon time**. Above 1 favors Direct Neon; below 1 favors sse2neon. |
| **Winning<br>backend** | The backend ahead outside the 5% band, or **Within&nbsp;5%** when the ratio falls inside it. |
| **Throughput<br>gain&nbsp;(%)** | The named winner's percentage throughput lead over the other backend. **—** means the comparison is within the 5% band. |
| **Scalar<br>(nsec/op)** | Reference time using ordinary `float`/`double` code with automatic SIMD vectorization disabled. |
| Scalar /<br>Direct Neon | Scalar time ÷ Direct Neon time. A value of **2×** means Direct Neon delivers twice the scalar throughput. |
| Consistent<br>Timing | **Yes:** the ranges of the three invocation medians for the two SIMD backends do not overlap. **No:** those ranges overlap. This describes the observed timing separation, not statistical significance. |

**Timing markers apply to the sse2neon and Direct Neon nsec/op columns.**
They identify the lower and higher times when the ratio falls outside the 5%
band. Both timing cells use 🔵 when the comparison falls within that band.

| Timing marker | Meaning |
| --- | --- |
| 🟢 <span style="color: #1a7f37;">**Winning time**</span> | The lower time for the backend classified as the winner |
| 🔴 <span style="color: #cf222e;">**Losing time**</span> | The higher time for the other backend |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | The comparison is inside the practical 5% band; the displayed times can still differ |

**Throughput markers apply to the Direct Neon / sse2neon ratio and Winning
backend columns.** Both columns use the same decision:

| Throughput marker | Meaning | Direct Neon / sse2neon ratio |
| --- | --- | --- |
| 🟣 **Direct Neon wins** | Direct Neon has higher throughput beyond the margin | Above 1.05× |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | Neither backend is declared the winner | 0.95–1.05×, inclusive |
| 🟠 **sse2neon wins** | sse2neon has higher throughput beyond the margin | Below 0.95× |

The **5% band** and **Consistent Timing** answer different questions. A row can
be blue and still say **Yes**: the observed invocation ranges are separated, but
the throughput difference remains inside the practical band. Winner labels and
colors compare the two SIMD backends; use **Scalar / Direct Neon** for the scalar
comparison.

**Throughput gain is not the same percentage as elapsed-time reduction.** For
the displayed ratio `r`, the gain is `100 × (r − 1)%` when Direct Neon wins, or
`100 × (1/r − 1)%` when sse2neon wins. Thus **1.25×** gives Direct Neon a **25%**
throughput lead, while **0.80×** gives sse2neon a **25%** throughput lead.
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

For an illustrative row with **sse2neon = 1.000 nsec/op** and
**Direct Neon = 0.800 nsec/op**, the timing cells show 🔴 for sse2neon and
🟢 for Direct Neon. The ratio cell is 🟣 **1.250×**, Winning backend is
🟣 **Direct Neon**, and Throughput gain is **+25.0%**. This corresponds to
**20% less elapsed time** for the same work.

These results include loads/stores and loop/call overhead. They describe array
throughput on the measured host, not isolated instruction latency or whole-
application speedup. Times are medians of invocation medians; raw outliers are
retained. The separate before/after optimization reports use their own measurements and
controls: do not combine their times with this report's times.
