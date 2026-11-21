# Retained four-pattern blend decision

Master retains the four-pattern Direct Neon specialization. Its matched
[19 November performance report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md)
records approximately **6.0% higher small-transpose throughput**, with accepted
FP32 AoS TRS-inverse elapsed costs of **2.2% / 2.7%** at 128 / 4,096 objects.
The [completed TRS review](trs-inverse-decision-2026-11-20.md) retains this runtime
and declines the later recovery candidate. The follow-up is closed; no runtime
fix or further execution is implied by this acceptance.

## Scope and numerical contract

The owned ARM blend helper specializes four 32-bit lane patterns:
`a0,a1,b0,b1`, `a2,a3,b2,b3`, `a0,a2,b0,b2` and `a1,a3,b1,b3`.
All other valid patterns keep the existing fallback. Public VCL APIs, namespace,
ABI and logical widths are unchanged; compatibility remains default and Direct
Neon requires experimental opt-in. The original x86 implementation is unchanged.

The movement regression checks `Vec4f`, `Vec4i` and `Vec4ui`, both source orders,
the specialized and fallback tuples, and edge payloads through an independent
integer-bit lane oracle. Selected lanes preserve every bit, including signed
zeros and NaN payloads; zeroing lanes are positive zero and don't-care lanes
remain unconstrained. No FP arithmetic, rounding, conversion or FMA is introduced;
the helper preserves FPCR/FPSR. Keep these tests and their fixtures, seeds and
numerical/guard controls when retiring historical documentation.

## Validation and source identity

The [frozen original M2 report](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/arm64/mac-arm-2026-11-19-blend4.md)
records 8,036 cases and eight examples at its complete 571-file uncommitted
capture. Its [frozen portable packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/6458749cd9405025251aae112d962053577cfdc5/docs/validation/arm64/m2-blend4-2026-11-19)
preserves the exact fingerprint, source mappings, named cases and controls.
The [21 November M2 report](mac-arm-validation-2026-11-21.md) supplies the later
full native checkpoint at clean commit `dc802e466663d61d9e219bd7c7351aa8ac312204`.
These are separate tested identities; documentation publication inherits no pass.

The [Windows return](../x86/windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md)
retains **5,387 passed, 249 failed, 76 not run of 5,712, plus twelve passing
examples; failed/incomplete**. Its 96 isolated regression instances and 96
selected code comparisons are bounded preservation evidence, not universal
correctness or cross-host parity. Later Windows collections remain separately
identified in the [validation index](../README.md).

Completed integration, publication and cleanup instructions are preserved in
the [frozen historical record](https://github.com/DigitalExpressions/vcl2-xa64/blob/6458749cd9405025251aae112d962053577cfdc5/docs/validation/arm64/blend4-integration-2026-11-19.md).
Use the current [native workflow](../native-workflow.md) for newly agreed work.
