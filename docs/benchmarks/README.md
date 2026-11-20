# Benchmark documentation

- **ARM64 / macOS:** [table reading guide](arm64/standard_math_reading_guide.md),
  [report template](arm64/STANDARD_MATH_RESULTS_TEMPLATE.md),
  [19 November optimized report](arm64/STANDARD_MATH_RESULTS_2026-11-19.md),
  [matched result bundle](arm64/standard-math-blend4-2026-11-19/checksums.sha256) and
  [current-master M2 checkpoint](arm64/standard-math-master-checkpoint-2026-11-19/README.md).
- **x86 / Windows:** [upstream comparison guide](x86/x86-upstream-comparison.md).

Benchmark source and correctness contracts remain in the
[benchmark development guide](../../benchmarks/README.md).

## Relocation - 20 November 2026

Architecture-specific documents and their result bundles moved into `arm64/`
and `x86/`. Relative links, current command examples, formatter lookup and
checkout attributes follow these locations. Measured data, source identities
and frozen Git URLs retain their original content. This move establishes no
new native validation or performance result.

Link edits changed four checksummed Markdown documents; only their affected
checksum entries were refreshed. Original documents and inventories remain in
[frozen Git before the move](https://github.com/DigitalExpressions/vcl2-xa64/tree/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs).
The following original SHA-256 values preserve their earlier identities; paths
identify the post-relocation locations relative to the repository root, including
any subsequently retired file.

| Post-relocation path | Original SHA-256 before relocation |
| --- | --- |
| `docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md` | `a82cc922bf0212cefab241531eebc83c1cde7bd89aee9aaf68a4be8fbdd0c90c` |
| `docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19_BASELINE.md` | `56d46f16eae182e1cd9e37d7365cb3f6d1428af42a5613902b55002145d31dc7` |
| `docs/benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md` | `72bfa6f989c90713075d2611ca245ac0275b4c8b39317a67583f5245fb2b449a` |
| `docs/validation/x86/windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md` | `8615ed76d21886158110b9fbd97c9f58d8b59cc4800e13ed658e8bbca1a5c6a9` |
| `docs/benchmarks/arm64/standard-math-blend4-2026-11-19/checksums.sha256` | `42cb7eb92d81ed7355a671ec23f71b150f47252764bdbb528d2d07a3566a832a` |
| `docs/benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/checksums.sha256` | `84621a895e3ee10382be5f529d3792ce5fa1883623d80cd764227e2f39fa4b67` |
| `docs/validation/x86/windows-blend4-handoff-2026-11-19/results/checksums.sha256` | `0e9f9700c5771997d5b11092c6d6ae09e7d85986d034648271a92e8c1f8b50e0` |

## Baseline retirement - 20 November 2026

The maintainer retired the pre-optimization baseline report and its standalone
data/source receipt from the current checkout. Their
[frozen report](https://github.com/DigitalExpressions/vcl2-xa64/blob/369bcb7cbe384cbd1d53762bce3c071ce2171714/docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19_BASELINE.md)
and [complete comparison packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/369bcb7cbe384cbd1d53762bce3c071ce2171714/docs/benchmarks/arm64/standard-math-blend4-2026-11-19)
remain in Git history. The optimized report, its results, shared comparison
evidence and current-master checkpoint remain local. Original source identities
and historical hashes are unchanged.

For new tests, keep baseline/control reports and standalone baseline datasets
under ignored `build/`; publish the selected implementation's report and results.
