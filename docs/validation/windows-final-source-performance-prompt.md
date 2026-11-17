# Windows final-source performance comparison

Prepared after the native M2 correctness pass on 2026-11-17. This is the next
execution prompt, not a performance result. The maintainer authorized publishing
the Mac report and this prompt; Windows measurements and their later publication
remain separate actions. Use the following instructions when the maintainer
requests this Windows step.

## Scope and frozen source

Run a fresh native AMD Windows x64 comparison of the final correctness source
against original Agner Fog VCL:

- Fork: `ab5bb8754a33724e05a170fa2867475077efd023`.
- Fork tree: `48376a53dfa5f5f2139651e3d44f5e1a9ed841b1`.
- Upstream: `667577495efaa36a10cbb5d6cb8c942c23913848`.
- Run ID: `windows-final-correctness-performance-ab5bb875-<actual-UTC-timestamp>`.

Do not use moving `HEAD` as the measured library source or reuse earlier
performance rows. The new Mac report passed 8,036 CTest instances and eight
examples at the fork commit above. Original Windows correctness used `e27134b`
plus captured fixes; its source IDs remain unchanged. The Mac comparison
reconciled all 157 code paths: 84 raw matches and 73 proven newline differences.
This does not make the publication commit an executed Windows correctness run.

## Inspect and preserve

Inspect applicable `AGENTS.md`, branch/HEAD/tree, index, local changes, worktrees,
existing tools and prior evidence first. Fetch `origin` without resetting,
stashing, overwriting or removing existing work. Read this prompt from its
publication commit, then read:

- [Comparison protocol and commands](../benchmarks/x86-upstream-comparison.md).
- [Windows correctness plan](../windows-correctness-plan.md).
- [Windows completion report](windows-amd-2026-11-17-correctness-completion.md).
- [Mac correctness report](mac-arm-2026-11-17-ab5bb87-correctness.md).
- [Mac source comparison](mac-arm-correctness-2026-11-17-ab5bb87/source-comparison.json).
- [Validation index](README.md) and [report template](report-template.md).

Use a fresh detached worktree at the exact fork commit. Keep the primary checkout
and earlier worktrees/evidence intact. The maintained runner exports both exact
Git commits into fresh ignored `build/` directories; those exports, not changing
working headers, are the measured sources. Inspect the actual exported manifests
before interpreting results. Record the runner/harness hashes separately.

**Keep `vendor/vectorclass/` and `vendor/vectorclass.json` untouched.** Verify
their exact bytes against the approved pin before and after, in the checkout and
fork export. Never normalize, format, regenerate into vendor, or change its hash
inventory to accept a mismatch. Run the generation and repository checks in
check-only mode. Reconcile exported code hashes with the retained Windows and
Mac inventories, proving any newline transformation in memory. Preserve original
fingerprints; do not regenerate or relabel historical source inventories.

## Native measurement

Use installed native x64 MSVC first, then GNU-driver Clang and llvm-objdump.
Verify native Windows execution, CPU/OS SSE2, SSE4.2, AVX2 and AVX-512 support,
compiler versions, x64 PE binaries and runtime/guard controls. No WSL, emulation,
ASan or hosted CI. Reuse the existing comparison harness without changing library
source, workload, numerical checks, flags, sampling rules or deadlines.

Choose an available logical CPU deliberately and record why it was chosen.
Keep the active power scheme unchanged and record it. Run sequential Release
processes with no concurrent builds/timings. Use four alternating pairs, seven
trials and six-millisecond samples, matching the earlier completed protocol.
From the frozen checkout, set fresh destinations and the selected CPU, then run:

```powershell
$perfStamp = [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssfffffffZ')
$perfRunId = "windows-final-correctness-performance-ab5bb875-$perfStamp"
$perfRaw = "build/$perfRunId"
$perfPortable = "docs/benchmarks/$perfRunId"
# Set $perfCpu to one verified available logical CPU before invoking the runner.
& ([scriptblock]::Create((Get-Content -Raw -Encoding UTF8 -LiteralPath tools/run_x86_upstream_comparison.ps1))) `
  -ForkCommit ab5bb8754a33724e05a170fa2867475077efd023 `
  -UpstreamCommit 667577495efaa36a10cbb5d6cb8c942c23913848 `
  -Cpu $perfCpu -Pairs 4 -Trials 7 -SampleMs 6 `
  -Output $perfRaw -PortableOutput $perfPortable
```

Require the complete documented 102-workload inventory for every library,
compiler and ISA combination. Check every lane against independent scalar
expectations before timing, preserve all four protected-page fault controls,
strict FP/no contraction, MXCSR, scalar-driver controls, no LTO/sanitizers and
the runner's 180-second command deadlines. Preserve every raw sample, all slower
or unstable rows, source exports, compiler flags and optimized disassembly.
Do not pool these measurements with earlier runs or promise overall equivalence.

After timing finishes, initialize the same installed x64 developer environment
as documented and run the existing untimed edge follow-up against these retained
exports:

```powershell
python -B tools/check_x86_upstream_edges.py `
  --run $perfRaw --portable-output "$perfPortable/edge-controls"
```

Inspect actual `edge-results.json`, including per-library failures; the
orchestrator exit alone does not establish a pass. Keep an upstream discrepancy
separate from fork acceptance. If any prerequisite, correctness gate, control or
measurement fails, retain the frozen source and every failed attempt, report the
defect and stop before changing source, gates, harness or starting a retry.

## Report and stop

Prepare a dated report and checksummed portable results for later review. Compare
per-workload ratios/variation with the earlier reports as separate experiments;
explain correctness-related costs and assembly evidence without hiding slower
rows. State complete/incomplete coverage and actual failures. Update the current
plan/index accurately; do not overwrite historical reports or benchmark tables.

Keep original command journals, logs, binaries/hashes, source inventories and
archives under ignored `build/`; verify archive checksums and coverage. Finish
with actual results, source identities/comparison, vendor integrity, evidence
locations, slower/unstable cases and remaining gaps.

Do not commit, push, merge, create PRs, delete branches/worktrees, clean evidence,
modify consumers, change remotes/configuration or repair source in this task.
Request separate authorization after presenting the concrete reviewed results.
