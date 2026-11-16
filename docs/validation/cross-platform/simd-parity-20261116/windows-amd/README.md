# Windows AMD final native results

Run ID: `simd-parity-20261116`. Tested source: `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
Tested tree: `2a0a5d3250201773cccb06ba0562df728f4e9387`. Clean throughout execution.

**5,392/5,392 CTest cases** passed across four profiles, and separately
**5,376/5,376 corpus vector cases** passed across eight profiles.
No failures, skips, timeouts or retries occurred at this frozen source.

## Portable corpus files

| Profile | Metadata | Raw output bits |
| --- | --- | --- |
| msvc-sse2-b1-w512-release | [manifest](win-msvc-sse2-release-01/manifest.json) | [JSONL](win-msvc-sse2-release-01/outputs-msvc-sse2-b1-w512-release.jsonl) |
| msvc-sse2-b1-w512-debug | [manifest](win-msvc-sse2-debug-01/manifest.json) | [JSONL](win-msvc-sse2-debug-01/outputs-msvc-sse2-b1-w512-debug.jsonl) |
| clang-sse2-b1-w512-release | [manifest](win-clang-sse2-release-01/manifest.json) | [JSONL](win-clang-sse2-release-01/outputs-clang-sse2-b1-w512-release.jsonl) |
| clang-sse2-b1-w512-debug-ubsan | [manifest](win-clang-sse2-debug-01/manifest.json) | [JSONL](win-clang-sse2-debug-01/outputs-clang-sse2-b1-w512-debug-ubsan.jsonl) |
| msvc-avx2-b1-w512-release | [manifest](win-msvc-avx2-release-01/manifest.json) | [JSONL](win-msvc-avx2-release-01/outputs-msvc-avx2-b1-w512-release.jsonl) |
| msvc-avx512-b1-w512-release | [manifest](win-msvc-avx512-release-01/manifest.json) | [JSONL](win-msvc-avx512-release-01/outputs-msvc-avx512-b1-w512-release.jsonl) |
| clang-avx2-b1-w512-release | [manifest](win-clang-avx2-release-01/manifest.json) | [JSONL](win-clang-avx2-release-01/outputs-clang-avx2-b1-w512-release.jsonl) |
| clang-avx512-b1-w512-release | [manifest](win-clang-avx512-release-01/manifest.json) | [JSONL](win-clang-avx512-release-01/outputs-clang-avx512-b1-w512-release.jsonl) |

Each attempt includes its own checksums. The root checksum list covers every
portable file, including per-attempt checksum lists. [Matrix metadata](matrix-summary.json)
records the separate full-suite results, controls and raw archive receipts.

## Comparison and Mac handoff

All 28 Windows-only profile pairings have zero unexpected differences.
Exact lane matches: 134488; numerical differences: 336;
declared FMA differences: 672; declared non-default rounding differences: 528.
These totals span repeated pairings and do not assert bitwise identity.

On the Mac, fetch the reviewed results and use a clean worktree at `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
Run the complete native M2 matrix and matching corpus profiles using the
[shared protocol](../../README.md). Use the same corpus/policy hashes and
run ID. Commit additional report-only changes separately from the tested
source. Native Mac execution and Windows-versus-Mac parity remain pending.

Full raw evidence stays under ignored build directories and transfers separately.
Combined archive: `windows-final-evidence.zip`; SHA256:
`fece4dd2f38f5dbad2da36371ef94cbee871749fec1f8d6b1d4102bc43328d3e`.
