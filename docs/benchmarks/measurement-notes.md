# Recorded measurements and protocol

The [2026-11-13 standard report](STANDARD_MATH_RESULTS_2026-11-13.md) preserves
the 160 historical three-way rows recorded on
Apple M2, macOS 26.7, Apple Clang 17 and SDK 26.1 after the wide FP32 helper
interface/register-allocation work. It compares ten operations, object and batch
layouts, counts 128/4096 and logical batch widths 128/256/512. Programs use
`-O3 -mcpu=apple-m1 -ffp-contract=off` without fast-math, LTO or sanitizers;
the scalar binary also disables automatic vectorization.

Each row derives from three invocation medians with nine timed trials per
operation. The [reading guide](standard_math_reading_guide.md) defines layouts,
nsec/op, winner/color rules, throughput gain and range separation. All slower
rows and the inclusive 5% band remain. Header/helper changes removed full-loop
spills in the recorded wide FP32 kernels; that is a dated code-generation result,
not a guarantee for another compiler or processor.

The historical [CSV](standard_math_results.csv) is copied byte for byte. All
numeric result rows in the historical [JSON](standard_math_results.json) remain
unchanged; machine-local paths in metadata are redacted and marked as recorded
paths. Its source/binary
hashes describe the measured historical checkpoint, before public notice and
path edits. They do not identify newly built public binaries. The
[validation record](standard_math_validation.json) is retained historical evidence,
not a claim that its old local audit can run from this public tree.

Correctness-only validation does not collect timings. Original local raw trials,
comparisons and provenance remain retained separately. To collect new results,
follow [the harness instructions](../../benchmarks/README.md)
and preserve a new run's complete evidence. Never overwrite the dated recorded
numbers with a correctness run or mix controls from separate runs.

These measurements also precede the migration onto official VCL 2.02.04.
[Migration validation](../validation/migration.md) records correctness checks
for that newer source base; it does not establish new performance measurements.

The separate [native M2 baseline, 2026-11-17](STANDARD_MATH_RESULTS_2026-11-17.md)
records fresh measurements at `3abf7af`: all 160 rows, three invocations per
backend/count and nine trials per shape. It also verifies identical Release
benchmark binaries against pre-layout `44f974a`, so the source-layout separation
itself introduced no executable difference for these workloads. Its measurements
and numerical-output comparisons retain their own controls and evidence; the
historical data files and table rows above remain unchanged.
