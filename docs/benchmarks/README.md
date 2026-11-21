# Benchmark documentation

- **ARM64 / macOS:** [table reading guide](arm64/standard_math_reading_guide.md),
  [report template](arm64/STANDARD_MATH_RESULTS_TEMPLATE.md),
  [21 November current-master report](arm64/STANDARD_MATH_RESULTS_2026-11-21.md) and
  [selected result bundle](arm64/standard-math-master-2026-11-21/checksums.sha256),
  [19 November optimized report](arm64/STANDARD_MATH_RESULTS_2026-11-19.md),
  [matched result bundle](arm64/standard-math-blend4-2026-11-19/checksums.sha256) and
  [historical 19 November M2 checkpoint](arm64/standard-math-master-checkpoint-2026-11-19/README.md).
- **x86 / Windows:** [upstream comparison guide](x86/x86-upstream-comparison.md).

Benchmark source and correctness contracts remain in the
[benchmark development guide](../../benchmarks/README.md).
For each completed ARM64 validation/performance round, use the
[results publication checklist](../validation/arm64/results-publication-checklist.md)
for reports, supporting files, checksums and README updates.
For completed requested x86 performance tests, use the
[x86 results publication checklist](../validation/x86/results-publication-checklist.md).

The 21 November report records fresh Apple Clang 21 / SDK 26.5 measurements at
commit `dc802e466663d61d9e219bd7c7351aa8ac312204`. Its `candidate/` result files
follow the 19 November selected-source layout; this is a clean master
checkpoint. All 160 rows, raw trials, slower rows, variability and the setup-audit
correction are preserved. The source, flag, correctness, integrity and archive
receipts accompany the report. No separate runtime baseline/candidate timing
comparison was performed in that session.
The [separate native correctness report](../validation/arm64/mac-arm-validation-2026-11-21.md)
records the preceding 8,036-case pass and eight arithmetic smoke runs. A brief
Clang 17 versus 21 note in the benchmark report explains the focused FP32 SoA
matrix-vector comparison without changing any full-suite result.

## Historical records and new results

The 19 November matched report and checkpoint keep their own sources and values.
Earlier standalone baselines and relocation paperwork remain in
[frozen Git](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/benchmarks/README.md),
including original report/checksum identities. A documentation move or later
compiler session does not refresh those timings.

For new tests, keep baseline/control reports and standalone baseline datasets
under ignored `build/`; publish the selected implementation's report and results
using the architecture publication checklist. Keep all slower rows and variability.
