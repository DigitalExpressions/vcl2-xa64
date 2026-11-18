# Developer documentation

Start with [ARM64 / Neon lessons](neon-arm-lessons.md) and the
[strategy reset](integration-plan.md). The maintainer requested a new direction
on 18 November 2026. The separation plan records the intended layout and agreed
public include; implementation has not started. This documentation work leaves
all library, backend, vendor, test and tool source unchanged.

The x86 repair/optimization campaign is retired. Its fixes are not requirements
for the next ARM design. Historical reports remain provenance, not active work.

| Need | Document |
| --- | --- |
| What to carry into the next Mac ARM approach | [ARM64 / Neon lessons](neon-arm-lessons.md) |
| Current scope and restart point | [Strategy reset](integration-plan.md) |
| Pristine-root / separate-Neon strategy and agreed public include | [Architecture separation plan](arch_separation_plan.md) |
| Implement and validate the approved separation on Mac M2 | [Mac architecture separation handoff](validation/mac-arch-separation-handoff.md) |
| Retained x86 discoveries, remedies and upstream discussion candidates | [x86 findings and fix candidates](x86_findings_and_fix_candidates.md) |
| Shared working rules and source protection | [Agent instructions](../AGENTS.md) |
| Existing backend selection and API limits | [Backend reference](backends.md) |
| Current pristine source/adaptation layout | [Source layout reference](backend-layout.md) |
| Floating-point, conversion and memory behavior | [Numerical contracts](numerical-contracts.md) |
| Reusable independent tests and fixture generators | [Test guidance](../tests/README.md) |
| Native M2 checkpoints and historical evidence | [Validation index](validation/README.md) |
| Latest native M2 correctness result | [ab5bb87 report](validation/mac-arm-2026-11-17-ab5bb87-correctness.md) |
| Reference native validation procedure | [Native workflow](validation/native-workflow.md) |
| Format a newly executed result | [Report template](validation/report-template.md) |
| M2 vector/matrix performance and interpretation | [17 November results](benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md), [reading guide](benchmarks/standard_math_reading_guide.md) |
| Prior M2 measurements and reporting format | [13 November results](benchmarks/STANDARD_MATH_RESULTS_2026-11-13.md), [template](benchmarks/STANDARD_MATH_RESULTS_TEMPLATE.md) |
| Attribution and dependency boundaries | [Third-party notices](third-party.md) |
| Local cleanup and retained archives | [18 November cleanup](validation/arm-strategy-cleanup-2026-11-18.md) |

Existing implementation references describe the code currently in the checkout;
they do not choose the next architecture. Old execution handoffs/plans are
retired. Choose a new agreed scope and frozen source before implementing or
claiming a new native pass. Keep the default branch `master`, preserve the fork
and upstream attribution, and obtain separate approval before commit/push.
