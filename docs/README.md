# Developer documentation

Start with the root [README](../README.md) for library usage and
[CONTRIBUTING](../CONTRIBUTING.md) for development rules. The default branch
remains `master` and includes the completed ARM port. Use it as the shared base
for the Mac M2 and AMD Windows checkouts. The shared
[agent instructions](../AGENTS.md) contain OS-agnostic rules and separate macOS
and Windows sections. Completed port/audit branches are retired; tests use a
chosen source commit rather than requiring an old branch name.

| Need | Document |
| --- | --- |
| Shared agent instructions for both hosts | [Agent instructions](../AGENTS.md) |
| Completed integration, branch status and future work | [Integration status](integration-plan.md) |
| Backend selection, widths and unsupported APIs | [Backend support](backends.md) |
| Floating-point and memory contracts | [Numerical contracts](numerical-contracts.md) |
| Configure and run regressions | [Test instructions](../tests/README.md) |
| Native Mac/AMD workflow and branch synchronization | [Native workflow](validation/native-workflow.md) |
| Actual tested revisions and coverage gaps | [Validation index](validation/README.md) |
| Manual Windows controls and evidence | [Windows runner](validation/windows-runner.md) |
| Format a new native result | [Report template](validation/report-template.md) |
| Historical port review and findings | [Integration review](integration-review.md) |
| Attribution and dependency boundaries | [Third-party notices](third-party.md) |
| Correctness before optional benchmark measurements | [Benchmark protocol](../benchmarks/README.md) |

Current instructions describe the maintained developer workflow. Dated
validation reports and benchmark results describe their recorded source
checkpoints; preserve them, including failed attempts, rather than editing
them to imply validation of a later commit. Validation applies only to its
recorded source and profiles. Raw evidence stays under ignored `build/` and
transfers separately.

`master` is the common development base.
Use topic branches when useful for features and fixes, review changes locally,
and obtain task-specific approval before committing or publishing. Do not create
pull requests, change the fork relationship/remotes or rewrite published history.
The original VCL remains available through the preserved `upstream` remote.
Integrating upstream changes, publishing releases/tags and retiring further
branches each require specific authorization.
