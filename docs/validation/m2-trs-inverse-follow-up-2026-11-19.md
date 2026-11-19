# M2 handoff: investigate and recover TRS-inverse performance — 19 November 2026

## Task and integration decision

The maintainer approved integrating the four-blend candidate and its Windows
report, accepting the recorded performance tradeoff for now. The next M2 task
is to reproduce, investigate and, where supported by measurements, remove the
**2.2% / 2.7% increase in FP32 AoS TRS-inverse elapsed time** at 128 / 4,096
objects. Preserve the useful transpose improvement and every correctness
contract. Integration does not establish that the slowdown has been fixed.

This handoff prepares a separate ARM optimization change. Invoke it on the
physical Mac M2 to perform the bounded investigation, implementation and native
checks below. No benchmark or runtime change is part of the Windows integration.
Commit, push, merge, branch retirement, evidence cleanup and consumer changes
are outside the M2 task unless separately authorized. Do not create PRs.

## Exact starting point and evidence

Use a fresh detached worktree at the integrated implementation/report commit:

- Commit: `ccde9e43674b77975d1f375d1ecff02aa761aef3`.
- Tree: `1f174faec6d791f92936bf9b8524d62af2435819`.
- Its implementation, tests, fixtures, tools and build inputs are unchanged
  from Windows-approved publication `0c69b9992d0fac09f71482a0db403276cdd8f1a1`,
  tree `fd580a67ad4707116dbc7bf7b230050d635f8704`.
- The later integration/handoff publication is documentation only; record its
  full SHA from the return prompt separately. It does not inherit native passes.
- Pre-blend comparison control: `5e039b01b49abc00548123763be85898932e3ad8`,
  tree `ea90c1b40fc50936db7c3509aaa11ab51c02d890`.

Read applicable `AGENTS.md`, protected `README.md`, the
[fork guide](../../README-vcl2-xa64.md), [contributing rules](../../CONTRIBUTING.md),
[integration plan](../integration-plan.md), [validation index](README.md),
[ownership rules](../backend-layout.md), [numerical contracts](../numerical-contracts.md),
[integration decision](blend4-integration-2026-11-19.md),
[M2 correctness report](mac-arm-2026-11-19-blend4.md),
[Windows result](windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md),
[candidate measurements](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19.md) and
[matched control measurements](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19_BASELINE.md).
The old planning-only handoff and initial migration handoffs are historical.

The M2-tested candidate was a 571-file uncommitted frozen capture, fingerprint
`3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36`.
Windows tested the later 625-file publication, fingerprint
`735caf6d2ad31792c7fc215f2240187b55e768e272be7d0218df8a6c22ceeafb`.
Keep both identities and all historical source/benchmark archives unchanged.
The integrated documentation has its own larger inventory; capture it anew.

## Preserve and prepare

Inspect and record branch, HEAD/tree, staged/unstaged/untracked work, refs,
worktrees, remotes and saved Git configuration under a new ignored `build/`
session before fetching. Preserve all existing work and evidence. Confirm
origin is the fork, fetch without pruning and verify the exact starting commit
is reachable from `origin/master`. Create a new detached worktree using
command-scoped `core.autocrlf=false` and `core.eol=lf`; do not change saved Git
configuration, reset, stash, clean or overwrite any dependency/worktree.

Read this handoff from the separately supplied publication SHA before changing
directories; it is later than the pinned starting commit. Verify the starting
HEAD, tree and cleanliness, every raw tracked file against its Git blob, and
protected inventories/hashes/mtimes before source edits. Capture the clean
integrated control with `python3 -B tools/capture_source.py --output ...`.
Use a separate capture/worktree for the exact pre-blend control if needed.
Keep all raw files and machine paths under ignored `build/`.

Verify native arm64 M2 execution, Rosetta state, compiler target, tool versions
and Mach-O architecture. Reuse installed Apple Clang, arm64 Python 3.9+, CMake,
CTest and Ninja. Reuse the pristine testbench at
`a7293702385e5169a5f58b83177f2a43cd59139b`; verify its cleanliness and all three
hashes from the [Windows handoff](windows-amd-2026-11-19-blend4-handoff.md).
Never overwrite existing tools or dependencies. Use Python `-B` throughout.

**Protect all 28 original root files, all 28 vendor originals,
`vendor/vectorclass.json`, the sse2neon alias/vendor header and its pin.**
Keep their inventory, raw bytes and mtimes unchanged before/after every phase.
No normalization, formatting, generation or upstream pin update is allowed.
Agner's x86 path must continue to use those exact original files through
`#include "vcl2_xa64.h"`. ARM headers must remain outside the x86 include graph.
Do not reapply retired x86 fixes or edit consumers.

## Reproduce and isolate the cause

1. Review the original [matched comparison](../benchmarks/standard-math-blend4-2026-11-19/before-after.json),
   [protocol](../benchmarks/standard-math-blend4-2026-11-19/protocol.json),
   [flags](../benchmarks/standard-math-blend4-2026-11-19/flags.json), raw trials,
   output bits and selected-code evidence. The prior three-invocation median
   ranges were disjoint for these slower rows; the shared 5% presentation band
   does not erase a measured 2–3% cost.
2. Rebuild matched controls and integrated source from immutable captures in
   the same native M2 session. Run fresh benchmark correctness/reference and
   input/canary checks before timing. Keep the benchmark source, fixtures,
   compiler flags, calibration, warm-up, nine trials, seed, object counts,
   aggregation and rotating/interleaved source/backend schedule unchanged.
   Only one Release timing process may run at a time. Keep timing separate
   from the correctness matrix. Do not reuse historical times as a new control.
3. Inspect the optimized TRS-inverse and transpose callers, selected blend
   instructions, inlining, register pressure/spills and code placement. Preserve
   complete disassembly, symbol addresses and flags. Compare actual instruction
   bytes/relocations, not just source or instruction counts. Earlier gains in
   some other rows had identical kernel bodies at different addresses; do not
   assume every timing difference was caused by the four specializations.
4. If the slowdown reproduces, try a narrow ARM-only improvement, starting in
   `vectori128_perm_neon.h`. Evaluate which of the four branches affects the
   inverse caller and preserve the good transpose paths where practical. Broaden
   the ARM-owned scope only when the causal evidence requires it, and document
   the reason. Keep API/ABI, lane ordering, fallback/zero/don't-care semantics,
   payload bits, FPCR/FPSR behavior and explicit Direct Neon opt-in intact.
   Do not change benchmark work, FP flags or acceptance gates to obtain speed.
5. If the slowdown does not reproduce reliably, report that result and its
   variability; do not manufacture a source fix. Recovery is the objective,
   not a promised outcome. Preserve every failed/slower experiment and use a
   new run ID for each changed source and unique IDs for every invocation.

## Validate any proposed change

Freeze a complete new source capture after edits, including raw hashes,
base commit/tree, patch and all added/deleted/nonignored inputs. Build only the
immutable capture. Keep the default clean-source guard; use explicit capture
mode rather than making a source commit to satisfy it. Record the integrated
control, pre-blend control and proposed source as distinct identities.

Run read-only header/repository checks and relevant movement, namespace/ABI,
fallback and FP-environment tests first. Preserve the existing independent
four-pattern raw-bit regression, fixtures, seeds and guard controls. Then run
the full native M2 correctness matrix from the new frozen source:
compatibility max 512 and Direct Neon maxima 128/256/512, each Release and
strict nonrecovering UBSan Debug. Expect 8,036 registered cases plus eight
examples unless an explicitly added registration changes the inventory.
Keep 60-second case deadlines, concurrency three, `-ffp-contract=off`, rounding
flags, classification-only finite-math stress and all numerical policies.
Require passing positive controls and the real terminating UBSan overflow
diagnostic/exit (1 or -6), without continuation. Do not retry away failures.

Use `python3 -B tools/run_native_arm_validation.py --plan` to inspect the plan.
Execute the frozen capture's runner with `--capture-manifest` and the verified
`--testbench-dir`; do not combine capture mode with `--expected-commit`.
Keep include isolation, actual-page guard/canary checks and arm64 binaries.
Do not run ASan, emulation, hosted CI or Windows timings.

After correctness passes, repeat the matched Release measurement protocol on
the proposed source and integrated control, retaining all 320 three-way rows,
160 matched Direct Neon rows, output bits, slower rows and variability. Assess
both TRS-inverse sizes and the original approximately 6% small-transpose gain;
do not silently trade one regression for another or claim universal speedup.
Record residual costs even below 5%. Require a fresh native AMD Windows
preservation return at the proposed frozen source before a later integration;
M2 success alone does not establish x86 preservation.

## Deliverables

Produce a dated M2 report, checksummed portable results, complete source
comparison, protected-integrity proofs, commands/flags, raw trials/output bits,
generated inputs, failures, code evidence and remaining gaps. Preserve raw
evidence under ignored `build/`. End with a concrete retain/reject assessment
for a separate review. Keep the Windows full gate **failed/incomplete:
5,387 passed, 249 failed, 76 not run of 5,712, plus twelve passing examples**;
the bounded x86 preservation assessment remains separate. No existing native
result is relabeled as a new-source pass.
