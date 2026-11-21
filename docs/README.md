# Developer documentation

Use [the fork guide](../README.md), [backend ownership](backend-layout.md)
and [current integration status](integration-plan.md). Current master keeps the
four-blend implementation;
the later TRS-recovery candidate was declined. The Windows gate remains
failed/incomplete. Earlier execution handoffs do not authorize new work.

| Need | Document |
| --- | --- |
| x86 compilation with MSVC/Clang and test-profile availability | [x86 build and test instructions](x86-build-and-test.md) |
| Current validation and coverage gaps | [Validation index](validation/README.md) |
| Latest retained native M2 correctness | [21 November M2 report](validation/arm64/mac-arm-validation-2026-11-21.md) |
| Performance tradeoffs and final decision | [TRS decision](validation/arm64/trs-inverse-decision-2026-11-20.md) |
| Why older Windows x86 documentation was removed or kept | [Windows documentation cleanup](validation/README.md#windows-documentation-cleanup) |
| Architecture boundaries and protected originals | [Backend layout](backend-layout.md) |
| Shared working rules and protected source | [Agent instructions](../AGENTS.md) |
| ARM numerical, memory and measurement lessons | [ARM / Neon lessons](neon-arm-lessons.md) |
| Backend selection and supported API | [Backend reference](backends.md) |
| Floating-point, conversion and memory contracts | [Numerical contracts](numerical-contracts.md) |
| Canonical tests and independent fixtures | [Test guidance](../tests/README.md) |
| New native validation | [Native workflow](validation/native-workflow.md), [Windows runner](validation/x86/windows-runner.md) |
| Benchmark documentation by architecture | [ARM64 and x86 index](benchmarks/README.md) |
| New result format | [Correctness report template](validation/report-template.md), [performance template](benchmarks/arm64/STANDARD_MATH_RESULTS_TEMPLATE.md) |
| Current-master M2 timing reference | [21 November results](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md), [reading guide](benchmarks/arm64/standard_math_reading_guide.md) |
| Future cross-host comparisons and frozen historical checkpoint | [Shared corpus and policies](validation/cross-platform/README.md) |
| Earlier x86 findings without resuming fixes | [x86 findings](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md) |
| Attribution and dependency boundaries | [Third-party notices](third-party.md), [x86 pin](../audit_x86_history.md), [ARM64 dependency](../audit_arm64_history.md) |

New runtime work needs an agreed scope, frozen source and relevant fresh native
checks. Commits, pushes, merges and branch retirement need task-specific approval.
Keep master as the default branch and preserve upstream attribution and bytes.
