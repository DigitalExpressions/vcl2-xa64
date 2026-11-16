# Portable Windows x86 comparison results

Read the [dated report](../x86-upstream-2026-11-16-ade9d9b.md) and
[repeatable protocol](../x86-upstream-comparison.md) before interpreting these
numbers. This is fork `ade9d9baf559ad480289277dd6ad8e6d1c5b15e3` versus original
VCL `667577495efaa36a10cbb5d6cb8c942c23913848`, on the same Windows AMD host.

| File | Meaning |
| --- | --- |
| `metadata.json` | Source commits/trees/fingerprints, tool versions, CPU/OS ISA probe, affinity, options and exact captured harness hashes |
| `results.csv`, `results.json` | All 816 comparisons, 28 samples per source represented by medians/ranges and four pair-ratio ranges; no slower rows omitted |
| `summary.json` | Preflight counts, output-hash disagreements, screening totals and native binary hashes |
| `archive-receipt.json` | Full raw benchmark archive name and SHA256, retained under ignored `build/` |
| `harness-manifest.json` | SHA256 of the exact benchmark/runner files used for timing |
| `verification.json` | Separate offline reconciliation of commands, source bytes, checks, measurements and archive inventories |
| `edge-controls/edge-results.json` | Untimed independent IEEE/rounding probes, failed upstream lane checks and two fork consumer examples; separate source/runner hashes |
| `edge-controls/archive-receipt.json` | Separate raw edge/example evidence archive receipt |
| `checksums.sha256` | Recursive inventory of this directory's other files, including the edge directory's own checksum inventory |

Case names encode element type, logical vector width and workload. Recurrence
times are nanoseconds per vector feedback step, averaged across independent
chains for throughput cases; array times are nanoseconds per vector-sized
chunk. Calls, stores and feedback operations are included. Compare matching
cases, compilers and ISA settings. These units are not per-element times or
isolated instruction latency.

`fork_over_upstream` is the median of the four per-pair median ratios, rather
than the ratio of pooled medians. Values above one mean the fork was slower.
`pair_ratio_min/max` retain between-invocation variation; sample ranges retain
within/between-invocation extrema. Screening labels require every pair ratio
above 1.05 or below 0.95. They are not significance tests. Static instruction
counts omit called helper bodies and do not establish machine-code equivalence.

Raw samples, commands, machine paths, binaries and source archives remain under
ignored `build/`; these portable files contain none of those raw artifacts.
Current source identifiers remain those captured during measurement, regardless
of any later documentation commit. Preserve this directory when recording a
future comparison; create a new run and destination instead of replacing it.
