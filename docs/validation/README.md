# Native validation status and reports

Read this index when continuing work on either native host. A passing report
applies to its recorded source and profiles; a branch name alone does not
identify tested code. Use the [native workflow](native-workflow.md) to run tests
and the [report template](report-template.md) to record a new result.

## Shared checkpoint

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
| [Mac ARM, 2026-11-15](mac-arm-2026-11-15.md) | Native M2, full eight-profile selected matrix at `465436d`; first attempt and replay retained |
| [Windows AMD evidence review, `823ca2a`](windows-amd-2026-11-15-823ca2a.md) | Shared-template retrospective review: final 5,627 native AMD passes, earlier failures/reruns, exact source comparisons and a checksummed evidence packet; no new native execution |
| [Windows AVX-512 and ARM/POSIX follow-up](windows-avx512-2026-11-15.md) | Earlier native AMD/MSVC/Clang results and separately labeled emulated ARM/Linux results |
| [Initial Windows AMD validation](windows-amd-2026-11-15.md) | Earlier native Windows checkpoint, harness changes, initial failures and sanitizer-runtime limits |
| [Migration validation](migration.md) | Dated migration checkpoint; native ARM and separately labeled translated x86 |

Hardware availability and an earlier passing result never establish that a
later source revision has passed. Direct Neon remains experimental.
