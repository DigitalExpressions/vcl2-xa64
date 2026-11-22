# ARM64 validation

## Native M2 collection and local artifact availability — 22 November 2026

On 22 November 2026 the authorized local Mac build reset discarded 40,072 files totaling 4,649,286,799 bytes, including historical M2 archives, source captures, raw logs, binaries, benchmark output dumps and the Clang study outputs. Earlier identifiers, hashes and numerical results describe their original checkpoints. Their old local payloads are no longer available. The fresh collection used clean commit `c120ded4321a087339a952ef0671fb89c456ca14`, with 8,036 cases and eight smokes passed, followed by 18 timing invocations and all 160 report rows.

[Fresh correctness report](mac-arm-validation-2026-11-22.md) · [Fresh performance report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)


Use the [shared validation index](../README.md) for current status and limits,
and the [native workflow](../native-workflow.md) for newly agreed validation.
Use the [results publication checklist](results-publication-checklist.md)
after the correctness and performance runs to copy reports and update summaries.

- [22 November current-master M2 correctness report](mac-arm-validation-2026-11-22.md):
  8,036 cases plus eight arithmetic smoke runs passed with Clang 21 / SDK 26.5;
  [full standard-math results](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-22.md)
  followed separately at the same frozen source `c120ded`.
- [Earlier 21 November M2 correctness report](mac-arm-validation-2026-11-21.md):
  8,036 cases plus eight arithmetic smoke runs passed with Clang 21 / SDK 26.5;
  [full standard-math results](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
  followed at the same frozen source.
- [Retained blend contract and decision](blend4-integration-2026-11-19.md) and
  [TRS-inverse follow-up decision](trs-inverse-decision-2026-11-20.md).

Use the [cross-platform procedure](../cross-platform/README.md) for a new
frozen-source comparison. The pre-separation comparison packet is in frozen Git.
Windows preservation evidence stays under
[x86](../x86/README.md); matched performance reports are under
[ARM64 benchmarks](../../benchmarks/README.md). Earlier passes remain tied to
their recorded sources. Direct Neon remains experimental.
