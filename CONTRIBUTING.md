# Contributing

Read the shared [agent instructions](AGENTS.md), [integration status](docs/integration-plan.md)
and [validation index](docs/validation/README.md) before continuing work.
The ARM port and selected shared validation checkpoint are complete. The default
branch stays `master`; later work needs its own relevant validation.

Use [the native validation workflow](docs/validation/native-workflow.md) to keep
the maintainer's Mac M2 ARM64 and AMD Windows x86-64 checkouts on the same
approved source revision. Use topic branches when useful, review changes locally,
and integrate or publish only with the maintainer's task-specific authorization.
Do not create pull requests for this fork or against upstream. Preserve the fork
relationship, remotes and published history. The completed port and audit branches
were retired; they are not prerequisites for testing and should not be recreated.
Record native results separately for each host.
Physical Intel testing is omitted for now. Keep raw evidence under ignored
`build/`.

Use the [developer documentation index](docs/README.md) and
[Windows runner guide](docs/validation/windows-runner.md) for current instructions.
The [integration review](docs/integration-review.md) records the port review and
the native evidence from the two physical hosts.
Hosted test workflows are disabled; configuration alone does not prove a pass.
Preserve historical reports, benchmark results, original evidence archives and
dependency/tool installations during cleanup. After verifying that compact
archives cover the raw records and failures, completed generated builds and
temporary worktrees may be removed with explicit user authorization. Further
branch retirement requires a specific request; inspect actual local branches first.

Preserve VCL's public names, lane ordering, supported masks/conversions, root
include layout and explicit Direct Neon opt-in. Keep x86 changes limited and
reviewable. Preserve copyright and third-party notices. All source files added
for this port use the complete DigitalExpressions Sweden Apache-2.0 notice;
derived files must retain their original attribution too.

Maintain runtime changes in the [canonical adaptation/backend sources](docs/backend-layout.md),
not by hand-editing generated root headers. Keep `vendor/vectorclass/` pristine.
Run `python -B tools/regenerate_headers.py --write` after canonical edits,
then `--check`, generation integrity controls and relevant native validation.
The root products remain checked in for direct header-only use; Python/Git
generation is a maintainer workflow, not a prerequisite for applications.

Run [the regression suite](tests/README.md) for both ARM backends, native widths
128/256/512, Release and strict UBSan where supported. Exercise selected x86
targets separately. Add a regression that demonstrates a changed contract or
bug; use independent expected values, meaningful edge cases and appropriate
memory/FP checks. Do not suppress failures, relax tolerances or substitute the
implementation itself as its own oracle. Reproduce fixtures with their checked-in
generators. Keep tests application independent.

Build examples and benchmark correctness checks before evaluating performance.
Timing runs use sequential Release processes and retain flags, all slower rows
and variability. Source restructuring or copyright-only edits do not establish
new timings. Preserve recorded benchmark values and presentation.

Use fresh build directories. Document compiler, target, backend, width and any
coverage gaps. A platform is validated only after its native checks actually
run successfully at the recorded source. Additional hosts, unselected APIs
and unavailable sanitizer support remain explicit until tested.
