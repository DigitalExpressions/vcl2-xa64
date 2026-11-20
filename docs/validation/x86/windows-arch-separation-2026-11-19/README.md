# Native AMD Windows architecture separation — portable results

This bundle records the failed/incomplete native correctness gate at approved
publication `2d251bc72bcc861edad371499fa732e8eb45a5f7`, not a new Mac or shared
corpus pass. See the [dated report](../windows-amd-2026-11-19-arch-separation.md).

- `summary.json`, `profiles.json`: actual counts and profile outcomes.
- `test-results.json`, per-profile inventories: every registered case and outcome.
- `compile-failures.json`: retained compilation errors and failed build edges.
- `capture.json`, `source-comparison.json`, `publication-blobs.json`: both source identities and raw/blob comparison.
- `protected-integrity.json`: exact bytes and mtimes at all phase checkpoints.
- `host.json`, `controls.json`, `flags.json`, `testbench.json`: native environment and unchanged validation controls.
- `facade.json`, `coff-resolved-analysis.json`: include graphs, independent outputs/bits, original raw-label comparisons and separately identified resolved-code analysis.
- `binaries.json`, `validator-hashes.json`: native executable and validator identities.
- `preparation-attempts.json`: retained external orchestration failures.
- `archive-receipt.json`: CRC/SHA256-verified private raw-packet receipt.

Raw logs, generated sources and compiled artifacts stay in ignored `build/`;
no historical evidence was relabeled or deleted. The raw packet precedes this
review-document preparation; this bundle and report are separately checksummed.
