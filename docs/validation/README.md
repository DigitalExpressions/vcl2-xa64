# Native validation status and reports

Read this index when continuing work on either native host. A passing report
applies to its recorded source and profiles; a branch name alone does not
identify tested code. Use the [native workflow](native-workflow.md) to run tests
and the [report template](report-template.md) to record a new result.

The selected `simd-parity-20261116` checkpoint is complete and published on
`master`: Mac evidence/validator follow-up at `99f4a3d`, and source-byte audit
at `145c7f5`. All 32 planned pairings passed their declared comparison policies,
with zero unexpected differences; the file-fingerprint discrepancy is resolved.
These results retain frozen source `bd7807e`, original failures and the separately
identified validator reanalysis. Publication does not extend native coverage to
later commits or unselected profiles.

## Reusable final verification prompts

1. [Windows final verification prompt](windows-final-verification-prompt.md):
   prepare/reuse the shared SIMD corpus, capture harness and comparator, freeze
   the approved source commit, then run native MSVC and Clang validation.
2. [Mac final verification prompt](mac-final-verification-prompt.md): use the
   same commit, corpus and policies for native M2 validation and compare the
   portable results with Windows.

These are execution instructions, not test results. The shared comparison
harness must be prepared before running both hosts. Portable result files are
prepared for review and commit; full raw evidence stays under ignored `build/`.

## Completed shared checkpoint

The [shared comparison harness](cross-platform/README.md) was approved and
published at frozen commit `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
Its [preparation report](cross-platform/preparation-2026-11-16.md) preserves
the earlier oracle correction and failures. The
[final Windows report](windows-amd-2026-11-16-bd7807e-final.md) records
5,392/5,392 pinned CTest cases and separately 5,376/5,376 corpus vector cases,
with verified controls and no failures/skips/timeouts/retries at that source.
[Portable Windows output files](cross-platform/simd-parity-20261116/windows-amd/README.md)
accompany the approved result publication. The
[final Mac report](mac-arm-2026-11-16-bd7807e-final.md) records 8,036/8,036 native
CTest passes and eight examples at that same frozen commit/run ID. Separately,
3,996/3,996 corpus vector cases passed independent references. All 24 Release
cross-host pairings completed with zero unexpected differences. The frozen
loader initially rejected eight Debug pairings because it requires UBSan exit
`1`, while Apple UBSan aborts with `SIGABRT` (`-6`). A maintainer-authorized
[validator follow-up](mac-arm-2026-11-16-bd7807e-validator-followup.md) corrected
that host-specific check and reran all eight successfully. All **32 planned
pairings** now have successful comparisons, with zero unexpected differences;
the eight reruns use a separately identified validator patch on unchanged captures.
[Original Mac bundles and rejection records](cross-platform/simd-parity-20261116/mac-m2/README.md)
remain unchanged; [new Debug comparison results](cross-platform/simd-parity-20261116/comparisons/mac-validator-02/README.md)
are retained separately. The
[source-byte audit](source-byte-audit-2026-11-16-bd7807e.md) resolves the whole-file
fingerprint difference using all four original Windows initial/final inventories:
135 paths on each host, no missing/extra paths, 43 identical hashes and 92 exact
LF-to-CRLF reconstructions. Both original fingerprints and all existing evidence
remain unchanged. The [portable Windows inventory/provenance](cross-platform/simd-parity-20261116/source-manifests/windows-amd-bd7807e/summary.md)
and original Mac inventory support file-by-file review; no native rerun or
numerical-policy change was needed.

## Historical shared checkpoint

Source commit: `465436de204ebdd59e9b5071c353df372148b3df`

Source tree: `ff966a90ebfd9a2bc07c76c8418684df3a04ffa8`

| Native host | Evidence for this checkpoint | Report |
| --- | --- | --- |
| Mac M2, macOS ARM64, Apple Clang 17 | Passed after an interrupted profile was rebuilt and replayed: 8,036 selected case instances across eight profiles, plus eight examples | [Mac ARM report, 2026-11-15](mac-arm-2026-11-15.md) |
| AMD Ryzen 9 7950X, Windows x86-64 | No run at this exact commit/tree. Retained evidence verifies 5,627/5,627 final passes at the earlier fixed source; compiled library/tests/fixtures/build configuration match this checkpoint, which also adds a Mac runner | [Windows evidence review](windows-amd-2026-11-15-823ca2a.md), [historical follow-up](windows-avx512-2026-11-15.md) |

The [Windows evidence review](windows-amd-2026-11-15-823ca2a.md) follows the shared
template, verifies the retained raw evidence and preserves the original source
identifiers. The final Windows verification used `823ca2a` plus identified
documentation changes; the later clean `943923a` has the same source fingerprint.
The final 5,627 passes followed fixes and reruns, not a clean first attempt.
They do not establish an executed Windows pass at `465436d` or `306a4a4`.
The 336 ARM/Linux cases in the historical follow-up used QEMU; the Mac report
above supplies separate native ARM evidence. Physical Intel testing is omitted.

Documentation commit `306a4a4c98d7493551cbe99548695372e9795b05` changes only
`README.md` and four validation Markdown files relative to the shared checkpoint.
That comparison requires no matrix rerun solely for documentation. The earlier
`943923a` to `465436d` comparison also adds `tools/run_native_arm_validation.py`,
so it is not entirely documentation-only. See the Windows review for the exact
file lists, tested trees, fingerprints, retained failures/retries and new evidence
archive checksum. Raw Mac archives were not available for this Windows review.

## Continuing on Windows or Mac

PR #1 merged into `master` at `732f3e9`; `master` is now the shared development
base. The merged port and audit branches have since been retired as recorded in
the [integration status](../integration-plan.md#branch-retirement-and-current-base).
Old branch names are not requirements for new tests. Follow the shared
[agent instructions](../../AGENTS.md) and preserve any unrelated local branches.
Validation uses manual testing on the maintainer's Mac M2 and AMD Windows machines only.
Hosted test workflows are removed/disabled. The merge and subsequent cleanup
do not establish a native pass at the current HEAD.
The [new manual runner report](windows-amd-2026-11-15-08c3fe8-manual.md) records
final 160/160 local cases plus the separately identified 2,776-case pinned
preparation and discarded prototype attempts. It preserves its uncommitted
snapshot identifiers rather than relabeling an earlier run as a later commit.
See the [integration plan](../integration-plan.md) for the completed integration
and next steps, and the [Windows runner guide](windows-runner.md) for current
Windows commands. The superseded Intel handoff was removed; its original plan
remains in Git history. Physical Intel testing remains omitted.

The [post-merge shared-helper cleanup report](windows-amd-2026-11-15-df2df54-cleanup.md)
records a separate 160/160 local native AMD selection at `df2df54` plus the
identified cleanup patch and added evidence module. Guard-page and Clang strict
UBSan controls passed. Native Mac validation was pending at that historical
checkpoint; the later frozen-source Mac report above records the updated runner's
execution. Earlier results retain their original source identifiers.

1. Inspect the checkout and preserve unrelated work. Record the actual HEAD,
   tree, working-tree state and host before running tests.
2. Read the report for that exact revision, including its failed attempts and
   coverage limits. If the revision differs, identify the changes before using
   earlier evidence. A later documentation commit does not change the tested
   HEAD in an older report; any comparison must say which files differ.
3. Run the appropriate native profiles using the workflow. Windows MSVC Debug
   and Clang strict UBSan Debug are separate coverage entries. Probe CPU/OS ISA
   support before running higher-ISA Windows profiles.
4. Add a dated report from the template, then update this index with its exact
   tested source and actual outcome. Retain raw evidence under ignored `build/`
   on the host and identify its archive and SHA256 in the portable report.

If only documentation differs from a tested checkpoint, record that comparison
and retain the original tested HEAD/tree; a documentation update alone does
not require another native matrix. Changes to headers, runtime code, tests,
fixtures, build configuration or runners require appropriate fresh validation.

Use `mac-arm-YYYY-MM-DD.md` or `windows-amd-YYYY-MM-DD.md`; if several reports
share a date, append the short tested commit to the new filename. Preserve
earlier reports and add a follow-up rather than rewriting historical results.
Record failures, skips, timeouts and unavailable controls explicitly. Retain
both attempts when a retry is needed; do not present a combined pass as a clean
first-attempt pass. Different backends and selected ISA profiles need not have
equal case counts; compare each with its own registered inventory.

Commit portable reports with reviewed documentation changes so both hosts can
read them through the shared repository. Keep raw logs, binaries, local paths,
SDKs and application data out of public sources. Commits, pushes, merges and
publication follow the maintainer's authorization for the current task.

## Historical records

| Record | Scope |
| --- | --- |
| [Mac Debug validator follow-up, `bd7807e` captures](mac-arm-2026-11-16-bd7807e-validator-followup.md) | Authorized offline validator correction; all eight rejected Debug pairings passed; combined 32-pair coverage, zero unexpected differences; original captures and failures unchanged |
| [Final Mac M2, `bd7807e`](mac-arm-2026-11-16-bd7807e-final.md) | 8,036 native CTest passes and eight examples; 3,996 separate corpus reference passes; 24 Release comparisons passed, eight Debug comparisons rejected by the frozen UBSan control assumption |
| [Final Windows AMD, `bd7807e`](windows-amd-2026-11-16-bd7807e-final.md) | Clean frozen-source MSVC/Clang: 5,392 pinned CTest passes; eight separate corpus profiles, 5,376 vector passes; zero unexpected Windows-only differences; later Mac follow-up above |
| [Mac ARM, 2026-11-15](mac-arm-2026-11-15.md) | Native M2, full eight-profile selected matrix at `465436d`; first attempt and replay retained |
| [Windows AMD evidence review, `823ca2a`](windows-amd-2026-11-15-823ca2a.md) | Shared-template retrospective review: final 5,627 native AMD passes, earlier failures/reruns, exact source comparisons and a checksummed evidence packet; no new native execution |
| [Windows manual-runner preparation](windows-amd-2026-11-15-08c3fe8-manual.md) | Final manual-only snapshot 160/160, intermediate pinned 2,776/2,776, retained prototype failure and startup failures; exact dirty-source identifiers and checksummed packet |
| [Windows shared-helper cleanup](windows-amd-2026-11-15-df2df54-cleanup.md) | Post-merge dirty snapshot based on `df2df54`: 160/160 local native AMD cases, strict UBSan and guard controls; shared module extraction and matching runner plans; native Mac rerun remains |
| [Windows AVX-512 and ARM/POSIX follow-up](windows-avx512-2026-11-15.md) | Earlier native AMD/MSVC/Clang results and separately labeled emulated ARM/Linux results |
| [Initial Windows AMD validation](windows-amd-2026-11-15.md) | Earlier native Windows checkpoint, harness changes, initial failures and sanitizer-runtime limits |
| [Migration validation](migration.md) | Dated migration checkpoint; native ARM and separately labeled translated x86 |

Hardware availability and an earlier passing result never establish that a
later source revision has passed. Direct Neon remains experimental.
