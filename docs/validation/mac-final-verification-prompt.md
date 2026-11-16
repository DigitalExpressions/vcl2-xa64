# Mac final verification prompt

Use this prompt on the native Mac M2 after the Windows preparation and run.
Use the same run ID and frozen source commit. The shared harness and Windows
portable results must be available in the checkout before comparison.

## Prompt

Execute final native Apple M2 ARM64 validation and capture results comparable
with the completed Windows AMD64 run.

Read applicable AGENTS.md and:

- docs/validation/README.md
- docs/validation/native-workflow.md
- docs/validation/report-template.md
- docs/numerical-contracts.md
- docs/backends.md
- docs/integration-plan.md
- docs/validation/cross-platform/README.md
- The Windows manifest and summary for the agreed cross-platform run ID.

Inspect the machine and checkout first. Preserve unrelated changes and existing
branches. Fetch origin without resetting or overwriting local work.

Follow the shared AGENTS.md: do not create pull requests, alter the fork
relationship/remotes or rewrite published history. Retired port/audit branches
are not prerequisites; use the agreed frozen commit in a clean worktree.

Use the EXACT validation source commit, harness, corpus and comparison policies
recorded by Windows. Verify their hashes. If they are unavailable or incompatible,
report the gap before running; do not create an independent replacement harness.

Use a clean checkout/worktree at the frozen validation commit. Later report-only
commits do not change the source revision being tested.

Run natively on macOS ARM64 with Apple Clang:

- The full existing selected matrix for sse2neon compatibility and experimental
  Direct Neon, including logical widths 128/256/512 as supported by each profile.
- Release and strict UBSan Debug.
- The shared output corpus for each corresponding backend/width/mode.

Preserve independent references, numerical gates, fixtures, FP flags and
environment, native Mach-O checks, runtime consumers, guard-page controls,
strict UBSan controls, original deadlines and concurrency.
Do not use Rosetta/emulation, hosted CI or benchmark timings.
Do not retry the documented Mac ASan startup hang. Physical Intel testing
remains omitted.

During execution, keep all artifacts under ignored `build/`. Verify source
stability before copying portable results into:
`docs/validation/cross-platform/<run-id>/mac-m2/<attempt-id>/`

Produce the SAME manifest/schema and output format as Windows:

- `manifest.json`
- `summary.md`
- `outputs-<profile>.jsonl`
- `checksums.sha256`

Record the exact tested commit/tree, corpus/policy hashes, compiler/OS/CPU,
backend/width/mode, FP settings, flags, actual case counts, controls, attempts,
coverage gaps and raw evidence archive checksums. Preserve raw output bits.
Keep portable files free of credentials, absolute machine paths and binaries.

Run the shared comparator against the available Windows results. Report:

- Exact matches.
- Numerically acceptable differences under the predefined policies.
- Documented backend-specific differences.
- Unexpected mismatches, missing cases and untested coverage.

Do not silently compare only the intersection of available cases, change
tolerances after observing results, or claim bitwise identity from passing
reference tests. Preserve every failure and retry; do not overwrite earlier
results.

Update the validation index and integration plan with the actual outcome.
Prepare the portable files for commit so both hosts' results can be reviewed
together. Do not commit, push, merge or publish without approval.
