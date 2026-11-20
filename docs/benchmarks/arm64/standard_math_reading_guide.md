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

| Column | Meaning |
| --- | --- |
| Operation | The calculation, such as vector dot product or matrix multiplication. |
| FP | 32 means single-precision `float`; 64 means double-precision `double`. |
| **sse2neon<br>(nsec/op)** | SSE operations translated to Neon through sse2neon. Competes directly with the adjacent Direct Neon timing. |
| **Direct Neon<br>(nsec/op)** | The native Neon backend. Competes directly with the adjacent sse2neon timing. |
| **Direct Neon /<br>sse2neon** | **sse2neon time ÷ Direct Neon time**. Above 1 favors Direct Neon; below 1 favors sse2neon. Its marker and solid color identify the winner outside the shared 5% band. |
| **Winning<br>backend** | 🟣 Direct Neon, 🟠 sse2neon, or 🔵 Within&nbsp;5% inside the shared band. |
| **Throughput<br>gain&nbsp;(%)** | The named winner's percentage throughput lead. An em dash means the result is within the 5% band. |
| **Scalar<br>(nsec/op)** | Scalar float/double arithmetic with automatic SIMD vectorization disabled. This is a separate reference. |
| Scalar /<br>Direct Neon | Non-SIMD scalar reference time ÷ Direct Neon time. For example, 2× means Direct Neon delivers twice the scalar throughput. |
| Consistent<br>Timing | Consistency of the difference between sse2neon and Direct Neon across runs: **Yes** means their three-run median timing ranges do not overlap; **No** means they overlap. This does not measure each backend's stability alone, identify the winner, or establish statistical significance. |

**Throughput percentages describe higher throughput, not reduced elapsed time.**
For the displayed throughput ratio `r`, a Direct Neon win shows
`100 × (r − 1)%`; an sse2neon win shows `100 × (1/r − 1)%`.
For example, 1.25× gives Winning backend **Direct Neon** and Throughput gain **+25.0%**;
0.8× gives Winning backend **sse2neon** and Throughput gain **+25.0%**. Percentages use the
displayed ratio, rounded to one decimal place. A Within&nbsp;5% verdict shows
**—** in Throughput gain. Ratio colors use the same 5% band as the winner markers.

**Backend names have fixed identity colors:** Direct Neon is purple and
sse2neon is orange. Direct Neon and Within&nbsp;5% stay together on one line.
These label colors identify the implementation; the timing colors show the result.

**Winner markers remain visible on GitHub**, where inline text colors are removed.
The ratio and Winning backend columns use the same marker and inclusive 5% band:

| Winner marker | Meaning | Direct Neon / sse2neon ratio |
| --- | --- | --- |
| 🟣 **Direct Neon** | Direct Neon wins beyond the 5% margin | Above 1.05× |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | Approximately equal throughput | 0.95–1.05×, inclusive |
| 🟠 **sse2neon** | sse2neon wins beyond the 5% margin | Below 0.95× |

The timing and ratio colors remain available in supporting
Markdown viewers. Markers and colors describe measured throughput, not
statistical significance.

**Both competing timing columns are colored relative to each other:**

| Timing marker | Meaning |
| --- | --- |
| 🟢 <span style="color: #1a7f37;">**Winning time**</span> | The winning backend's time, beyond the 5% margin |
| 🔴 <span style="color: #cf222e;">**Losing time**</span> | The losing backend's time, beyond the 5% margin |
| 🔵 <span style="color: #0969da;">**Within&nbsp;5%**</span> | Both times when approximately equal |

Each timing cell keeps its marker when viewed on GitHub.

**Throughput ratios use solid winner colors:** purple for Direct Neon above
1.05×, orange for sse2neon below 0.95×, and blue within 0.95–1.05× inclusive.

| Throughput ratio | Meaning |
| ---: | --- |
| 🟠 <span style="color: #bc4c00;">0.667×</span> | 🟠 sse2neon leads by approximately 50% |
| 🟠 <span style="color: #bc4c00;">0.800×</span> | 🟠 sse2neon leads by 25% |
| 🔵 <span style="color: #0969da;">1.000×</span> | 🔵 Equal throughput |
| 🟣 <span style="color: #6f42c1;">1.250×</span> | 🟣 Direct Neon leads by 25% |
| 🟣 <span style="color: #6f42c1;">1.500×</span> | 🟣 Direct Neon leads by 50% |

Markers, ratio colors and the Winning backend column all use the shared 5%
band. The timing columns use green/red to identify the winning/losing times.

The shared decision uses **Direct Neon / sse2neon**:

| Throughput ratio | Winning backend | sse2neon timing label | Direct Neon timing label |
| --- | --- | --- | --- |
| Above 1.05× | 🟣 Direct Neon | 🔴 Losing time | 🟢 Winning time |
| 0.95–1.05× inclusive | 🔵 Within&nbsp;5% | 🔵 Within&nbsp;5% | 🔵 Within&nbsp;5% |
| Below 0.95× | 🟠 sse2neon | 🟢 Winning time | 🔴 Losing time |

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

For a worked example, take **sse2neon = 0.318 nsec/op** and
**Direct Neon = 0.289 nsec/op**, with a throughput ratio of **1.099×**.
Direct Neon delivers approximately **9.9% more throughput**, or about
**9% less elapsed time**, than sse2neon. The Direct Neon time is green, the
sse2neon time is red, Winning backend says **Direct Neon**, and Throughput gain shows **+9.9%**. Ratios use
unrounded measurements, so dividing the displayed times can differ slightly.

These results include loads/stores and loop/call overhead. They describe array
throughput on the measured host, not isolated instruction latency or whole-
application speedup. Times are medians of invocation medians; raw outliers are
retained. The separate before/after optimization reports use their own measurements and
controls: do not combine their times with this report's times.
