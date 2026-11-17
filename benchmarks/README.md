# Standard vector/matrix benchmarks

For a native Windows comparison of the fork's x86 backend with original VCL,
use the separate [upstream comparison protocol](../docs/benchmarks/x86-upstream-comparison.md).
The ARM three-way benchmark below is a different workload and comparison.

The standalone C++20 harness compares genuine scalar float/double arithmetic,
sse2neon and Direct Neon for identical vector/matrix operations. It is independent
of application or consumer libraries. Scalar auto-vectorization is disabled;
the SIMD programs use the same operation definitions and validation gates.

On ARM64, build explicitly and check correctness first:

```sh
cmake -S . -B build/bench -DCMAKE_BUILD_TYPE=Release -DVCL_BUILD_BENCHMARKS=ON
cmake --build build/bench -j 3
ctest --test-dir build/bench --output-on-failure --timeout 60
```

The three binaries are `build/bench/benchmarks/standard_scalar`,
`standard_compat` and `standard_neon`. `--check` performs numerical correctness
only. To measure, run each separately with `--count 128` or `--count 4096` and
optionally `--dump <new-directory>`; stdout contains JSON records. Record all
three invocations per backend, nine trials per operation and comparable load.
The program calibrates its repeat count. Retain input hashes, raw rows, dump
files, compiler/flags and variability; do not mix controls from different runs.

`-DVCL_SANITIZE=ON` enables strict UBSan for correctness; do not collect timings
from sanitizer builds. The comparison requires ARM64 because both ARM backends
are measured; the numerical source remains generic.

The [2026-11-17 native M2 report](../docs/benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md)
records measurements at `3abf7af` and byte-identical benchmark binaries
before/after the pristine-source layout change. It includes the source, host,
correctness controls and evidence details after the tables.

The [2026-11-13 historical report](../docs/benchmarks/STANDARD_MATH_RESULTS_2026-11-13.md)
preserves the earlier wide FP32 helper measurements, before migration to VCL
2.02.04. Each dated report uses its own controls and data; do not mix their
timings. [Measurement notes](../docs/benchmarks/measurement-notes.md) explain
the protocol, metadata and source checkpoints.

Start future dated reports from the [standard report template](../docs/benchmarks/STANDARD_MATH_RESULTS_TEMPLATE.md).
It marks the table values, summaries, source/host details and evidence records
that must be filled from the new run.

Run `python3 -B tools/format_benchmarks.py docs/benchmarks/STANDARD_MATH_RESULTS_YYYY-MM-DD.md`
with the new report's actual date to apply the established color/label policy.
The formatter requires an explicit report path and rejects unfilled template
placeholders. It does not perform a measurement or reconstruct lost raw trials.
Raw evidence from a new run must be retained separately.
