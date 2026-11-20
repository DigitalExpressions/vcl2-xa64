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

## Correctness contracts

The [regression suite](../tests/README.md) checks vector/mask types, boundary-safe
partial memory, permutations, wide composition, namespace/ABI contracts and FP
control modes. Partial-memory and reduction cases cover alignments, exceptional
payloads and signed zeros. Independent scalar/Decimal fixtures verify selected
math functions without using vector results as expected values. Preserve their
seeds, numerical gates, canaries and protected-page controls.

The benchmark adds independent scalar references, canaries and output comparisons
across scalar, sse2neon and Direct Neon. These correctness checks are separate
from timing. The [19 November native report](../docs/validation/mac-arm-2026-11-19-blend4.md)
records a later full matrix at its own frozen source; it does not validate every
later benchmark build. See [numerical contracts](../docs/numerical-contracts.md)
for the supported domains and limits.

## Source identities and retained results

The [19 November current-master checkpoint](../docs/benchmarks/standard-math-master-checkpoint-2026-11-19/README.md)
retains all 160 baseline rows, invocation ranges and source/tool metadata.
Future experiments need a fresh matched baseline and candidate session; do not
construct ratios against historical times or substitute old binary hashes for
fresh build identities. Source hashes identify the actual measured checkpoint;
correctness passes and documentation/layout changes do not refresh timings.
Preserve all slower rows and variability. The
[reading guide](../docs/benchmarks/standard_math_reading_guide.md) defines layouts,
nsec/op, colors, the inclusive 5% band and range separation. That band is not a
numerical gate or significance test. Redacted historical machine paths are
metadata, not runnable instructions for a new audit.

The older [13 November report](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/benchmarks/STANDARD_MATH_RESULTS_2026-11-13.md)
and [17 November report](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md)
remain in frozen Git history with their CSV/JSON and code-generation evidence.
Those measurements belong to older implementations; the 13 November session
predates migration onto VCL 2.02.04. Their local copies were retired on
20 November. They are not current-master baselines. Historical code-generation
findings, including spill removal or byte-identical builds, do not guarantee
performance with another source/compiler or processor.

Some raw Mac sessions were discarded in the
[20 November cleanup](../docs/validation/mac-arm-documentation-cleanup-2026-11-20.md#completed-local-workspace-cleanup);
published metadata does not reconstruct lost trials or executables.

## New reports

Start future dated reports from the [standard report template](../docs/benchmarks/STANDARD_MATH_RESULTS_TEMPLATE.md).
It marks the table values, summaries, source/host details and evidence records
that must be filled from the new run.

Run `python3 -B tools/format_benchmarks.py docs/benchmarks/STANDARD_MATH_RESULTS_YYYY-MM-DD.md`
with the new report's actual date to apply the established color/label policy.
The formatter requires an explicit report path and rejects unfilled template
placeholders. It does not perform a measurement or reconstruct lost raw trials.
Raw evidence from a new run must be retained separately.
