# Windows final verification prompt

Use this prompt on the native AMD Windows host. Run Windows preparation first:
the shared harness must be reviewed, committed and available on `origin` before
the Mac can use the same frozen validation source. For later runs, reuse the
shared harness and policies when available, and choose a new run ID.

## Prompt

Prepare and execute final native Windows AMD64 validation for comparison
with native Apple M2 ARM64.

Read applicable AGENTS.md and:

- docs/validation/README.md
- docs/validation/native-workflow.md
- docs/validation/windows-runner.md
- docs/validation/report-template.md
- docs/numerical-contracts.md
- docs/backends.md
- docs/integration-plan.md

Inspect the machine, checkout, branches and working tree first. Preserve
unrelated changes and both retained branches. Fetch origin without resetting
or overwriting local work. Use master as the development base.

### First: prepare the shared comparison harness

Inspect existing tests before adding anything. Reuse the shared comparison
harness if available; otherwise create one portable, deterministic SIMD input
corpus, result-capture harness and offline comparator that both Windows and Mac
will use.

Document the format, commands, coverage, run ID and comparison policies in:
`docs/validation/cross-platform/README.md`

Requirements:

- Stable case IDs, fixed seeds and identical input bit patterns on both hosts.
- Cover selected shared integer, mask, movement, conversion, FP32 and FP64
  operations at logical widths 128/256/512.
- Include boundary values, signed zeros, subnormals, infinities, NaNs,
  rounding-sensitive cases, FMA-related cases and selected numerical functions.
- Record output lanes as hexadecimal integer/IEEE bit patterns, not rounded
  decimal text.
- Use independent references. Never use a SIMD implementation as its own oracle.
- Define per-operation comparison policies before examining platform differences.
- Require exact results where the contract requires them. Preserve existing
  accuracy gates and explicitly document backend-specific behavior.
- Do not assume that a reference-error tolerance is also a valid pairwise
  Windows-versus-ARM tolerance.
- Preserve NaN payloads in captured data; require payload equality only where
  the API contract requires it.
- Reject duplicate IDs, missing required cases, incompatible corpus versions,
  input-hash mismatches and unknown comparison policies.
- Keep these additional checks separate from existing suite registrations
  unless inventory changes are explicitly reviewed and reconciled.

Prepare the shared harness for review. Obtain approval before committing or
pushing it. Once approved and available on origin, freeze its exact commit as
the validation source. Do not label runs on different harness revisions as
comparable final runs.

### Then: execute Windows validation

Use a clean checkout/worktree at that frozen commit.
Prioritize MSVC; also run native GNU-driver Clang.

Run:

- The full existing selected matrix with the pinned upstream testbench:
  MSVC Release/Debug, Clang Release and strict UBSan Debug.
- The shared output corpus in each compiler/mode.
- Separate AVX2 and AVX-512 corpus profiles where CPU AND OS support are
  verified. Identify exactly which ISA paths ran.

Preserve numerical gates, fixture regeneration, FP flags, native binary checks,
runtime consumers, guard-page controls, original deadlines and concurrency.
MSVC Debug is not UBSan. Prove strict UBSan with positive/intentional-UB controls.
Do not collect benchmark timings or use hosted CI, WSL or emulation.
Physical Intel testing is omitted.

### Result files

During execution, write all artifacts under ignored `build/`. Verify source
stability before copying portable results into:
`docs/validation/cross-platform/<run-id>/windows-amd/<attempt-id>/`

Include:

- `manifest.json`
- `summary.md`
- `outputs-<profile>.jsonl`
- `checksums.sha256`

The manifest must identify:

- Exact tested commit/tree and clean working-tree state.
- Harness/schema version, corpus SHA256, policies SHA256, seed and case inventory.
- CPU/OS, compiler versions/targets, backend, ISA and logical widths.
- FP environment, compile/link flags, sanitizer and CRT settings.
- Registered/executed/pass/fail/skip/timeout counts.
- Guard/sanitizer control outcomes separately from ordinary test failures.
- Attempts/retries, raw evidence archive names and SHA256.
- Checksums of portable result files, without self-referential checksums.

Keep portable files free of credentials, absolute machine paths and binaries.
Retain original failures and every attempt; do not overwrite earlier results.
Do not weaken checks to obtain a pass. If source changes are required, establish
a new frozen checkpoint for BOTH hosts.

Update the validation index and integration plan accurately. Prepare portable
results for commit, but do not commit, push, merge or publish without approval.
Finish with the exact commit and run ID that the Mac must use.
