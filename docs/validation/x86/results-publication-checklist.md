# x86 results publication checklist

Use the [shared native validation workflow](../native-workflow.md) for source,
execution and retention rules, the [Windows runner guide](windows-runner.md)
for native AMD Windows checks, and the [x86 validation index](README.md) for
recorded results and limits.

Use this checklist to prepare completed x86 results for repository documentation.
It does not execute tests, establish a later source pass or authorize cleanup,
commits or pushes. Failed and incomplete runs retain their actual status.

For future requests, refer to this document with:

> Prepare the completed native Windows x86 validation results and any requested
> performance results for documentation using the x86 results publication
> checklist in docs/validation/x86/results-publication-checklist.md.

## Performance choice at the start of each validation round

Whenever the maintainer requests a new x86 validation round, ask whether to
include separate performance tests. Record the answer in the run plan and
dated report. If that request already explicitly includes or excludes
performance tests, use it as the answer for that round. Earlier rounds' choices
do not select performance tests for a later request.

While the answer is pending, proceed with authorized correctness preparation
and execution independently; do not start timing. If performance is declined,
state that no performance tests were requested. If the answer remains pending,
record that performance was not executed; silence is not approval. A
correctness-only round is valid within its selected scope. Documentation-only
publication does not require asking again about a completed round or running
new tests. Historical reports keep their recorded scope.

When performance is selected, run it separately after correctness collection,
using optimized unsanitized builds at the same frozen fork source. Use the
[native x86 comparison protocol](../../benchmarks/x86/x86-upstream-comparison.md)
and agree the source/reference pair and workload in the run plan. Verify the
current harness and exporter against that source layout; historical campaign
commands and results are not a new run plan. Keep the pinned original VCL
reference unless a different reference is explicitly selected; no dependency
update or runtime repair is implied.

Check benchmark outputs against independent references before timing. Keep
full-suite failures visible even when independent benchmark checks pass;
do not time a workload whose required correctness check failed. Record missing
or failed performance coverage explicitly. Use MSVC and GNU-driver Clang with
matched per-compiler options, verified CPU/OS ISA support, warm-up, calibration,
alternating paired order and sequential timing processes without concurrent
builds or correctness runs. Record affinity, power scheme, raw samples,
variability and all slower rows. The comparison harness's strict-FP Release
flags are separate from the ordinary optimized correctness profile; preserve
and report the actual benchmark flags rather than silently substituting them.
No sanitizer timings or historical-time ratios replace a fresh matched run.

## Copy destinations

Use the actual execution/report dates. Published filenames use purpose and ISO
report date, with matching primary evidence-folder stems; see the
[report naming rules](../report-template.md#published-report-names).
The [22 November correctness report](windows-x86-validation-2026-11-22.md) shows
the compiler table and clean-source identity; its
[portable results](windows-x86-validation-2026-11-22/README.md)
show supporting evidence. Their results belong to their own sources.

| Destination | Copy or prepare from the completed run |
| --- | --- |
| `docs/validation/x86/windows-x86-validation-YYYY-MM-DD.md` | Correctness report using the [native report template](../report-template.md): exact source, host/tools, performance choice, complete profile totals, compiler/profile/ISA table, examples, controls, failures, unavailable binaries and limits. |
| `docs/validation/x86/windows-x86-validation-YYYY-MM-DD/` | Applicable portable evidence: source/validator identities, complete named-case statuses, profile summaries, nonpassing diagnostics, compiler policies, include/control/integrity proofs, binary hashes and archive receipts. |
| `docs/benchmarks/x86/x86-upstream-YYYY-MM-DD.md`, when performance was requested | Dated selected-source performance report: exact fork/reference and harness identities, compiler/ISA/flags, correctness checks, protocol, all measured rows, ratios, variability, slower rows and coverage gaps. Explain which source each reference role identifies. |
| `docs/benchmarks/x86/x86-upstream-YYYY-MM-DD/`, when measured | Selected-source CSV/JSON, summary, metadata, applicable matched comparison/edge-check/code-generation summaries and archive receipts. Share evidence through relative links rather than duplicating it. |
| Each published supporting bundle | A publication receipt recording original/adapted report identities, copied hashes, source filenames and any renames, link/redaction changes, intentionally local evidence and gaps; `checksums.sha256` covering final reports and bundle payloads, excluding itself. |

Add a descriptive suffix only for multiple collections with the same purpose
and date; do not overwrite a prior report or bundle. Keep reset details and
opaque session/attempt IDs inside reports or metadata, and retain raw artifact
names under ignored `build/`. Supporting records can follow the existing
Windows names: `summary.json`, `cases.json`, `nonpassing-output.json`,
`source-manifest.json`, `compiler-policy.json`, `canonical-controls.json`,
`include-graphs.json`, `integrity.json`, `host-tools-dependencies.json`,
`validator-identities.json`, `setup-attempts.json`, `raw-evidence-receipt.json`
and `publication-receipt.json`. Copy applicable collected records and identify
missing evidence as a gap. Do not fabricate a baseline/candidate comparison,
code-equivalence proof, performance result or pristine-upstream full-matrix pass.

## Publication steps

1. Inspect branch, HEAD/tree, status, staged changes and worktrees; read applicable
   `AGENTS.md`. Preserve unrelated and staged work. Record the actual tested
   clean commit/tree or complete uncommitted capture, raw source fingerprint,
   run/attempt IDs, performance choice and original report hashes. Prepare
   copies under ignored `work/`; obtain approval for new permanent paths unless
   the current task already explicitly authorizes them. Stage benchmark portable
   exports under ignored `build/` before copying approved documentation paths.
2. Verify source/evidence inventories, source/archive SHA-256, entry counts,
   CRC where applicable and per-entry hashes. Preserve the original failed
   setup/build/run attempts, first runner return packets and separately logged
   build continuations. Check supplements for evidence outside the main archive;
   a receipt identifies an artifact but is not its payload. Preserve exact raw
   Windows source hashes and line endings; do not regenerate an old inventory
   from a newer checkout or normalize protected files.
3. Prepare the correctness report with both the profile summary and the standard
   compiler table below, one row per selected compiler/profile/ISA configuration.
   Reconcile complete named inventories and per-case statuses, not intersections.
   State whether the baseline uses the pinned testbench and distinguish local
   higher-ISA selections from full upstream-testbench runs at those ISAs.
   Keep optimized Release, strict Release and Clang strict UBSan separate;
   identify build mode, sanitizer and CRT. Unsupported selections and missing
   binaries remain explicit gaps. Count examples and other consumer checks
   separately, explain timeout inclusion in failures, and report skips/retries.
4. Include actual native Windows/x64 and PE proof, compiler/target/tool versions,
   CPU CPUID and OS/XCR0 ISA checks, runtime detection, emitted compile/link flags,
   actual-page-size writable/four-fault guard controls, strict UBSan positive
   exit zero and deliberate-overflow exit one with diagnostic and no continuation.
   Record the verified static release CRT for Clang UBSan. Include compiler-profile
   positive/rejection controls and pristine testbench revision/three pinned hashes.
   Missing controls are gaps; MSVC Debug is not UBSan proof. RTC remains deferred
   and rejected, and historical `NoInitFailures` diagnostics are not passes.
5. Verify all 28 pinned upstream originals and root sse2neon remain exact and
   x86 include traces select original x86 headers through `vcl2_xa64.h`, excluding
   ARM and retired vendor/adaptation/backend paths. Keep numerical expectations,
   fixtures, seeds, FP/rounding flags, classification-only stress, protected-memory
   controls, 60-second correctness deadlines and concurrency three unchanged.
   Retain failing results; copying reports does not resume the retired x86 repair
   campaign. Bound any comparison claim by its actual source and selected checks.
6. If performance was requested, prepare its report and selected results from
   the separate measured session. Verify every table row against raw data and
   retain all samples, slower rows, variability and failed/untimed checks. Keep
   benchmark correctness, untimed edge follow-ups and full native correctness
   results separate. Do not infer x86 equivalence from equal mnemonic counts,
   inherited failures or ARM results. Preserve the M2 report's distinct source,
   flags and status; cross-host parity requires the shared corpus procedure.
7. Update `docs/validation/README.md` and `docs/validation/x86/README.md` with the
   report and current status. Update the root fork guide's validation summary
   if it states superseded results. When performance was measured, also update
   `docs/benchmarks/README.md`, `benchmarks/README.md` and any root README x86
   performance claims affected by that session. Keep ARM performance summaries
   tied to their own measurements. Preserve earlier dated reports and their
   original failed/incomplete gates; a later documentation commit is not a
   tested implementation or a fresh native pass.
8. Redact machine-specific paths only in portable copies; preserve raw originals
   and record redactions. Rewrite public links to relative published targets,
   never to ignored evidence. Verify `.gitattributes` coverage for checksummed
   copies, including intentional CRLF. Generate final checksums after adaptation,
   retain original report/archive hashes separately, and verify every entry.
   Review tracked, staged and untracked files for placeholders, broken literal
   paths, credentials or compiled payloads. Run the documentation checks below
   and review the complete diff. Report locations, checks, local evidence and
   gaps. Cleanup, commit, push and merge remain separately authorized actions.

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| `<msvc / clang>` | `<profile>` | `<baseline / avx2 / avx512>` | `<count>` | `<count>` | `<count>` | `<count>` | `<count>` |

```powershell
python -B tools/check_repository.py
python -B tools/regenerate_headers.py --check
git diff --check
git diff --cached --check
```

Check untracked copies' whitespace too; ordinary `git diff --check` does not
inspect them. Documentation-only publication needs these checks, not a repeat
of correctness or performance execution. Missing requested measurements remain
a reported gap until separately executed within the authorized run scope.

## Evidence that stays local

Keep full command/exit journals, configure/build/test logs, JUnit and complete
process/LastTest outputs, source archives/captures/patches, generated fixtures
and control/probe inputs, output dumps, complete binary inventories, disassembly,
binaries/objects, dependencies, tools and worktrees under ignored `build/`.
Full process logs remain authoritative where JUnit output was truncated.
Baseline/control reports and standalone baseline datasets stay local; publish
applicable matched comparisons with the selected-source results. Compiled
payloads can remain on disk with exhaustive hashes when the compact archive
omits them; verify raw coverage and supplement missing diagnostics/inputs.

Ignored evidence does not travel with Git. Public links must resolve within
the repository or to explicitly retained external sources. Archive receipts
describe host-local availability using portable relative paths; hashes cannot
recover missing payloads. Transfer raw Windows archives separately when requested.
Any later cleanup needs its own scope and must preserve required evidence under
the [shared retention policy](../native-workflow.md#evidence-retention-and-cleanup).
