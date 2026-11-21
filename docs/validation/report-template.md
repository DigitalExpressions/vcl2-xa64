# Native validation report template

Copy this template to a new dated report; replace every placeholder with an
observed value. Use the [workflow](native-workflow.md) for execution and update
the [status index](README.md) after reviewing the result. Do not label an
unexecuted profile as passing. Keep historical reports unchanged.

## Outcome and tested source

- Date and report status: `<passed / passed after retry / failed / incomplete>`.
- Host label: `<native Mac ARM64 / native AMD Windows x86-64 / other>`.
- Repository and branch at execution: `<values>`.
- Exact tested HEAD: `<full commit for clean-source mode; null for an uncommitted capture>`.
- Exact tested tree: `<full Git tree for clean-source mode; null for an uncommitted capture>`.
- Working-tree state before/after: `<clean / changes identified>`.
- If changes were tested without a commit: `<base commit/tree, patch SHA256,
  complete source manifest/fingerprint, added/deleted inventory and capture archive>`.
- Source comparison with the other host: `<same exact source / different
  checkpoint and reviewed differences / not compared>`.
- For x86, performance choice for this request: `<included / declined / answer
  pending; record the maintainer's answer and separate performance report or gap>`.

Use the source captured during execution. A later report/documentation commit
is not automatically a tested commit. A dirty checkout needs an exact patch
and source inventory; a HEAD alone cannot identify its tested code.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| Physical CPU, OS/version, process architecture | `<values>` |
| Native execution proof | `<arm64 Mach-O and Rosetta state / x64 PE and native Windows process>` |
| Compiler/driver versions and target | `<values, one entry per compiler>` |
| Build tools and Python | `<values>` |
| Sanitizer runtime and intentional-UB control | `<runtime, positive result, negative diagnostic/exit code, or unavailable>` |
| Pinned upstream revision and source verification | `<commit, clean checkout and hash-check results>` |
| Higher-ISA CPU/OS support, when applicable | `<runtime detector, CPUID/XCR0 evidence and selected ISA>` |

For Windows, distinguish MSVC, GNU-driver Clang and clang-cl. Record the CRT
used for Clang UBSan. MSVC Debug is not UBSan coverage. Physical AMD results
establish AMD coverage; separately identify emulation and compilation-only
checks. Physical Intel is omitted under the current workflow.

## Executed matrix

Every native validation report must include a compiler results table with one
row per selected configuration; profile totals alone are insufficient. Use
the following detailed matrix for ARM and other hosts:

| Compiler/driver | Profile | Backend / ISA / logical width | Build mode / sanitizer / CRT | Registered | Passed | Failed | Not run | Skipped | Timed out | Attempt |
| --- | --- | --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| `<value>` | `<value>` | `<value>` | `<value>` | `<count>` | `<count>` | `<count>` | `<count>` | `<count>` | `<count>` | `<initial / replay>` |

Windows reports must include this standard compiler/profile/ISA table alongside
the profile summary, following the
[21 November report](x86/windows-x86-validation-2026-11-21.md). Identify build
mode, sanitizer and CRT for each profile in the surrounding text; report any
skipped cases and retries explicitly.

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| `<msvc / clang>` | `<profile>` | `<baseline / avx2 / avx512>` | `<count>` | `<count>` | `<count>` | `<count>` | `<count>` |

State whether timeout counts are included in failed counts; avoid double
counting. Reconcile actual JUnit results with the registered inventory. Give
the selected matrix total and total executions including retries separately.
Record example outputs and any additional consumer checks separately.
Identify each selected compiler profile explicitly. Keep Windows optimized
Release, strict Release and strict UBSan totals separate; MSVC RTC remains
deferred under the [current runner policy](x86/windows-runner.md).

## Controls, flags and numerical contracts

- `<Native binary inspection and executable-hash results>`.
- `<UBSan positive/negative controls, exact compile/link options and runtime
  options, or explicit sanitizer coverage gap>`.
- `<Guard-page size, writable control and all four read/write fault controls>`.
- `<Partial counts, exact boundaries and canaries retained>`.
- `<Configuration checks and independent fixture regeneration results>`.
- `<Selected compiler profile and actual FP/rounding flags: strict comparisons
  retain no contraction or MSVC /fp:strict; optimized Windows uses the separate
  precise/except- or no-trapping-math/no-math-errno policy. Classification stress
  keeps its isolated fast/finite-math options>`.
- `<Numerical gates, seeds and 60-second deadlines preserved>`.
- `<Build/test concurrency and commands or evidence journal identifying them>`.

Do not weaken numerical gates, assertions, fixtures, controls or deadlines to
obtain a pass. The documented Mac ASan startup hang must not be retried.
Benchmark timings are outside native correctness validation.

## Failures, retries and coverage limits

For each failed/skipped/timed-out case or unavailable control, record its name,
profile, initial result, investigation, any source change and final outcome.
Keep the first attempt and identify every replay. A fix creates a new source
checkpoint; record that checkpoint's results separately. If there were no
failures or retries, say so explicitly.

List unexecuted APIs, ISA profiles, sanitizer modes and toolchains. Describe
the evidence narrowly enough that the other host can distinguish passed,
failing and unavailable coverage at the tested source.

## Evidence and handoff

| Evidence packet | Host-local ignored directory | SHA256 | Contents / attempt |
| --- | --- | --- | --- |
| `<archive filename>` | `build/<run>/` | `<full SHA256>` | `<scope>` |

Retain raw command/exit-code journals, inventories, source/patch and executable
hashes, compile commands, configure/build/test logs, JUnit, failed attempts
and controls under ignored `build/`. Transfer evidence archives separately;
public report links must not depend on a host's ignored files. Keep machine
paths, credentials, SDKs and application data out of the portable report.

State whether source/branches changed and whether anything was committed,
pushed or merged. Give the next host's concrete comparison or validation
step, and link the relevant existing report. Follow the maintainer's current
authorization for any commit, push, merge or publication.
For x86 report copying, supporting evidence, checksums and index updates, use
the [x86 results publication checklist](x86/results-publication-checklist.md).
