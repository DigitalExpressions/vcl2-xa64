# Contributing

The [master integration plan](docs/integration-plan.md) tracks manual native runners,
port review, developer documentation cleanup and the integration PR. Read its
resume notes before continuing that work. The default branch stays `master`.

Use [the native validation workflow](docs/validation/native-workflow.md) to keep
the maintainer's Mac M2 ARM64 and AMD Windows x86-64 checkouts on the same
`neon-port` revision during integration preparation. After approved integration,
`master` becomes the shared base and short-lived topic branches feed it through
reviewed pull requests. Record native results separately for each host.
Physical Intel testing is omitted for now. Keep raw evidence under ignored
`build/`.

Use the [developer documentation index](docs/README.md) and
[Windows runner guide](docs/validation/windows-runner.md) for current instructions. Before integration,
review the complete port diff and native evidence from the two physical hosts.
Hosted test workflows are disabled; configuration alone does not prove a pass.
Preserve historical reports and benchmark results during documentation cleanup.
Complete that cleanup before any branch retirement.

Preserve VCL's public names, lane ordering, supported masks/conversions, root
include layout and explicit Direct Neon opt-in. Keep x86 changes limited and
reviewable. Preserve copyright and third-party notices. All source files added
for this port use the complete DigitalExpressions Sweden Apache-2.0 notice;
derived files must retain their original attribution too.

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
