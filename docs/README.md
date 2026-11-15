# Developer documentation

Start with the root [README](../README.md) for library usage and
[CONTRIBUTING](../CONTRIBUTING.md) for development rules. The default branch
remains `master` and now includes the ARM port merged through PR #1. Use it as
the shared base for the Mac M2 and AMD Windows checkouts. The existing port and
Windows validation branches are retained at the maintainer's request.

| Need | Document |
| --- | --- |
| Integration progress and where to resume | [Integration plan](integration-plan.md) |
| Backend selection, widths and unsupported APIs | [Backend support](backends.md) |
| Floating-point and memory contracts | [Numerical contracts](numerical-contracts.md) |
| Configure and run regressions | [Test instructions](../tests/README.md) |
| Native Mac/AMD workflow and branch synchronization | [Native workflow](validation/native-workflow.md) |
| Actual tested revisions and coverage gaps | [Validation index](validation/README.md) |
| Manual Windows controls and evidence | [Windows runner](validation/windows-runner.md) |
| Format a new native result | [Report template](validation/report-template.md) |
| Port review and remaining integration findings | [Integration review](integration-review.md) |
| Attribution and dependency boundaries | [Third-party notices](third-party.md) |
| Correctness before optional benchmark measurements | [Benchmark protocol](../benchmarks/README.md) |

Current instructions describe the maintained developer workflow. Dated
validation reports and benchmark results describe their recorded source
checkpoints; preserve them, including failed attempts, rather than editing
them to imply validation of a later commit. Validation applies only to its
recorded source and profiles. Raw evidence stays under ignored `build/` and
transfers separately.

`master` is the common development base.
Use short-lived topic branches for features, fixes and upstream VCL updates,
review pull requests, and tag releases for consumers. The original VCL remains
available through the `upstream` remote. Keep the existing branches; release
publication and branch retirement require separate approval.
