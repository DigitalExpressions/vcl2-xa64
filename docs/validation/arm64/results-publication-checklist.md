# ARM64 results publication checklist

Use the [shared native validation workflow](../native-workflow.md) for execution
and the [ARM64 validation index](README.md) for recorded results and limits.

Use this checklist after a native ARM64 validation round and its separately
executed Release performance measurements. It covers copying completed results
into the repository documentation. Execution still follows the agreed run plan;
copying results does not run tests, establish a later source pass or authorize
cleanup, commits or pushes. Failed or incomplete runs retain their actual status.

For future requests, refer to this document with:

> Prepare the completed ARM64 validation and performance results for documentation
> using the ARM64 results publication checklist in
> docs/validation/arm64/results-publication-checklist.md.

## Copy destinations

Use purpose and the actual ISO report date in published filenames, with a
matching primary evidence-folder stem. Add a descriptive suffix only for
multiple collections with the same purpose and date. Keep procedures and
session/attempt IDs inside reports or metadata, and retain raw build names.
Follow the [report naming rules](../report-template.md#published-report-names).
The existing
[21 November correctness report](mac-arm-validation-2026-11-21.md),
[standard-math report](../../benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
and [supporting bundle](../../benchmarks/arm64/standard-math-master-2026-11-21/checksums.sha256)
are an example of the layout, not a source of timings for another run.

| Destination | Copy or prepare from the completed run |
| --- | --- |
| `docs/validation/arm64/mac-arm-validation-YYYY-MM-DD.md` | Correctness report using the [native report template](../report-template.md): exact source, host/tools, complete profile totals, smoke runs, controls, failures/retries and limits. |
| `docs/benchmarks/arm64/STANDARD_MATH_RESULTS_YYYY-MM-DD.md` | Selected implementation's performance report using the [standard-math template](../../benchmarks/arm64/STANDARD_MATH_RESULTS_TEMPLATE.md), with all measured rows, slower rows, variability and numerical/output checks. |
| `docs/benchmarks/arm64/STANDARD_MATH_RESULTS_YYYY-MM-DD/candidate/` | Byte-exact `results.csv`, `results.json`, `summary.json` and `metadata.json` from the selected implementation's measured session. The inherited `candidate/` name also applies to a clean-master checkpoint; explain that identity in `sources.json`. |
| The same supporting bundle | Source manifest/identity, actual compile/link flags, compiler/SDK/tool identities, benchmark executable hashes, native and benchmark correctness summaries, sanitizer/guard controls, integrity, protocol, attempts and archive receipts. Reuse records between the two reports through relative links. |
| The same supporting bundle, when measured | Output comparisons, code-generation records and matched before/after comparisons, with their own source and session identities. Include a separate compiler investigation's findings, measurements, limitations and retained evidence identity when cited. |
| The same supporting bundle | A publication receipt describing copied data hashes, original and adapted report identities, link/presentation changes, intentionally local evidence and inapplicable comparison files; `checksums.sha256` covering the final reports and bundle payloads. |

Supporting filenames can follow the existing bundle: `sources.json`,
`source-manifest.json`, `flags.json`, `binaries.json`, `correctness.json`,
`native-validation-summary.json`, `ubsan-control.json`, `protected-integrity.json`,
`protocol.json`, `attempts.json`, `setup-attempts.json`, `archive-receipt.json`,
`retained-archive-receipts.json` and `publication-receipt.json`. Preserve the
actual contents and explain any renamed source records. Copy the records that
exist and apply; identify missing evidence as a gap. Do not fabricate a runtime
baseline/candidate comparison for a single-source checkpoint.

## Root README performance-table standard

The maintainer selected this format on 22 November 2026 for the root
[`README.md`](../../../README.md#direct-neon-vs-sse2neon) performance overview.
Preserve it when refreshing results unless the maintainer requests a change.
Also preserve the preceding x86 implementation section and its source links
according to the [root README x86 source-identity standard](../../backend-layout.md#root-readme-x86-source-identity).

- Keep **two separate tables**, batch processing (SoA) followed by per-object
  processing (AoS). Both tables use FP32 and the same eight operation rows in
  this order: 4D dot product, 3D cross product, vector length, vector
  normalization, 4×4 matrix multiplication, 4×4 matrix–vector multiplication,
  4×4 matrix transpose and TRS matrix inverse.
- The batch table has an operation column and a throughput-ratio range column.
  Each range spans all logical widths 128/256/512 and both counts 128/4,096
  from the same full benchmark session. State that scope above the table.
- The per-object table has operation, FP precision (`32`), ratio at 128 objects
  and ratio at 4,096 objects columns. Keep any selected FP64 figures in a
  separate, explicitly labeled note below it so the table rows remain matched.
- Every ratio is **Direct Neon throughput divided by sse2neon throughput**,
  calculated as compatibility elapsed time divided by Direct Neon elapsed
  time. These ratios describe the backend comparison within each layout;
  actual AoS-versus-SoA speed comparisons require the full report's ns/op data.
- Use two decimal places for batch range endpoints and three for per-object
  ratios. Calculate endpoints and classifications from unrounded data. Keep
  the shared legend, bold ratios, colored spans and markers: purple `#6f42c1`
  / 🟣 above 1.05, blue `#0969da` / 🔵 within inclusive 0.95–1.05, and orange
  `#bc4c00` / 🟠 below 0.95. Classify each range endpoint separately.
- Retain brief context for material mixed outcomes, identifying precision,
  layout, width and object count when explaining a result such as transpose.
  Colors express the 5% comparison band; invocation-range overlap and
  statistical significance remain separate considerations.
- Refresh both tables, context, date/toolchain, winner totals and links from
  the same selected full-suite session. Verify identical row labels/order and
  every displayed value against its published data. Focused repeats and
  compiler experiments retain their separate identities and do not replace
  individual full-suite cells.

## Publication steps

1. Inspect local changes and read applicable `AGENTS.md`. Record the tested
   commit/tree or complete capture identity, run/attempt IDs and original report
   hashes. Preserve the frozen source, dependencies, protected originals and all
   prior evidence. Prepare copies under ignored `work/`. Obtain approval for new
   permanent paths unless the current task already explicitly authorizes them.
2. Verify the retained source and evidence inventories and archive receipts:
   archive SHA-256, entry counts, CRC where applicable and per-entry hashes.
   Keep complete native named-case statuses, executable hashes, control exits,
   JUnit and diagnostics available in the checksummed local archive. Check
   supplements for records outside that archive; a receipt alone is not a copy
   of its payload. Retain every failed setup, run, timeout and unavailable control.
3. Copy the approved reports and selected data to the destinations above.
   Rewrite evidence links to relative published paths, sharing the bundle between
   correctness and performance reports. Keep raw result files byte-exact and
   verify every measured table row against its source. Do not change fixtures,
   seeds, flags, tolerances, numerical contracts, output comparisons, timing
   samples or slower rows while preparing documentation. Keep focused repeats
   separate from the full-suite results; compiler/SDK changes do not establish
   a library source improvement. Preserve the Windows failed/incomplete gate
   and its recorded counts separately from this native M2 result.
4. Update `docs/validation/README.md`, `docs/validation/arm64/README.md`,
   `docs/benchmarks/README.md` and `benchmarks/README.md` with the new report links.
   Update root `README.md`'s latest results/correctness links and brief performance
   overview from this same session. Derive ratios and range endpoints from
   unrounded data across every advertised width/count, then round for display.
   Refresh winner counts, cross-product percentages, colors and variability/cost
   commentary together. Follow the [root README performance-table standard](#root-readme-performance-table-standard).
   Use the existing inclusive 0.95–1.05 ratio band; range
   endpoints inside that band must not be colored as backend wins. Identify the
   new date, compiler and SDK. Earlier dated reports and datasets keep their own
   values and source identities.
5. Set or verify `.gitattributes` coverage so checksummed files retain their raw
   bytes across hosts, including intentional CSV CRLF. Generate the bundle's
   checksum inventory from the final adapted reports and supporting files,
   excluding the checksum file itself. Keep original run/archive checksums and
   original report hashes separately; an adapted documentation report can have
   a different hash without changing measurements. Verify every listed checksum,
   all relative links and all new untracked files as well as tracked edits.
6. Run the documentation checks below. Review the diff and copied-file inventory
   for missing records, placeholders, broken inline-code paths, machine-specific
   paths, credentials or compiled payloads. Confirm protected bytes and unrelated
   work are unchanged. Report the final document locations, checks performed,
   intentionally local evidence and remaining gaps. Commit/push and cleanup stay
   separate actions requiring task-specific authorization.

```sh
python3 -B tools/check_repository.py
python3 -B tools/regenerate_headers.py --check
git diff --check
```

`git diff --check` does not inspect untracked copies; check their whitespace too.
Formatting must preserve measured values and literal paths. Documentation-only
publication needs these checks, not a repeat of the native matrix or timings.

## Evidence that stays local

Keep full raw logs, commands/exits, named-case and binary inventories, JUnit,
source archives/captures, generated inputs, output dumps, assembly, binaries,
objects, tools, testbench checkouts and worktrees under ignored `build/`.
Baseline/control reports and standalone baseline datasets also stay there;
publish applicable matched comparison records with the selected result.
Avoid duplicating large native inventories in the benchmark bundle when both
reports can refer to the same verified archive receipt. A useful focused study
can be summarized in the report and existing publication receipt, retaining its
measurements, failures, scope and archive identity.

Ignored evidence and archives do **not** travel with Git. Public documentation
links must resolve within the repository or to an explicitly retained external
source; local archive locations belong in receipts as availability information.
A hash identifies an artifact but cannot recover it. Any later transfer or
cleanup must account for that availability explicitly; preparing these reports
does not authorize either action.
