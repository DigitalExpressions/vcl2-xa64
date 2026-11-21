# ARM64 validation

Use the [shared validation index](../README.md) for current status and limits,
and the [native workflow](../native-workflow.md) for newly agreed validation.
Use the [results publication checklist](../native-workflow.md#arm64-results-publication-checklist)
after the correctness and performance runs to copy reports and update summaries.

- [21 November current-master M2 correctness report](mac-arm-validation-2026-11-21.md):
  8,036 cases plus eight arithmetic smoke runs passed with Clang 21 / SDK 26.5;
  [full standard-math results](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
  followed at the same frozen source.
- [19 November M2 four-blend report](mac-arm-2026-11-19-blend4.md) and
  [portable source/results](m2-blend4-2026-11-19/README.md).
- [Four-blend integration decision](blend4-integration-2026-11-19.md) and
  [TRS-inverse follow-up decision](trs-inverse-decision-2026-11-20.md).
- Historical [Mac documentation and artifact cleanup](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md).
- Historical shared checkpoint: [Mac native report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-2026-11-16-bd7807e-final.md)
  and [Debug comparison follow-up](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-2026-11-16-bd7807e-validator-followup.md).

Use the [cross-platform procedure](../cross-platform/README.md) for a new
frozen-source comparison. The pre-separation comparison packet is in frozen Git.
Windows preservation evidence stays under
[x86](../x86/README.md); matched performance reports are under
[ARM64 benchmarks](../../benchmarks/README.md). Earlier passes remain tied to
their recorded sources. Direct Neon remains experimental.
