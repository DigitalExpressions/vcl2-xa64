# Developer documentation

Use [the fork guide](../README-vcl2-xa64.md), [backend ownership](backend-layout.md)
and [current integration status](integration-plan.md). Architecture separation
is complete and integrated. Current master keeps the four-blend implementation;
the later TRS-recovery candidate was declined. The Windows gate remains
failed/incomplete. Earlier execution handoffs do not authorize new work.

| Need | Document |
| --- | --- |
| Current validation and coverage gaps | [Validation index](validation/README.md) |
| Latest retained native M2 correctness | [19 November M2 report](validation/mac-arm-2026-11-19-blend4.md) |
| Performance tradeoffs and final decision | [TRS decision](validation/trs-inverse-decision-2026-11-20.md) |
| Why older Mac documentation was removed or kept | [Documentation cleanup](validation/mac-arm-documentation-cleanup-2026-11-20.md) |
| Architecture boundaries and original file dispositions | [Backend layout](backend-layout.md), [file map](arch-separation-map.json) |
| Shared working rules and protected source | [Agent instructions](../AGENTS.md) |
| ARM numerical, memory and measurement lessons | [ARM / Neon lessons](neon-arm-lessons.md) |
| Backend selection and supported API | [Backend reference](backends.md) |
| Floating-point, conversion and memory contracts | [Numerical contracts](numerical-contracts.md) |
| Canonical tests and independent fixtures | [Test guidance](../tests/README.md) |
| New native validation | [Native workflow](validation/native-workflow.md), [Windows runner](validation/windows-runner.md) |
| New result format | [Correctness report template](validation/report-template.md), [performance template](benchmarks/STANDARD_MATH_RESULTS_TEMPLATE.md) |
| Current-master M2 timing reference | [19 November checkpoint](benchmarks/standard-math-master-checkpoint-2026-11-19/README.md), [reading guide](benchmarks/standard_math_reading_guide.md) |
| Selected cross-host comparisons | [Shared corpus and policies](validation/cross-platform/README.md) |
| Earlier x86 findings without resuming fixes | [x86 findings](x86_findings_and_fix_candidates.md) |
| Attribution and dependency boundaries | [Third-party notices](third-party.md), [x86 pin](../audit_x86_history.md), [ARM64 dependency](../audit_arm64_history.md) |

New runtime work needs an agreed scope, frozen source and relevant fresh native
checks. Commits, pushes, merges and branch retirement need task-specific approval.
Keep master as the default branch and preserve upstream attribution and bytes.
