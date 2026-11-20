# Repeatable native Windows x86 comparison

Compare the fork's x86 backend against a pinned original VCL revision on the
same native AMD Windows host. This is a performance investigation, separate
from the native correctness and Windows/Mac numerical-parity matrices.

The [first completed report](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-upstream-2026-11-16-ade9d9b.md) records localized
slowdowns and improvements, a separate edge-value comparison, all attempts and
checksummed portable results. It does not establish whole-library performance
equivalence or a single overall slowdown. The report and its
[portable packet](https://github.com/DigitalExpressions/vcl2-xa64/tree/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-upstream-2026-11-16-ade9d9b-final)
are retired from the current checkout and remain available in frozen Git.

The historical first run compared fork
`ade9d9baf559ad480289277dd6ad8e6d1c5b15e3` and upstream
`667577495efaa36a10cbb5d6cb8c942c23913848`. A new investigation needs its own
approved source pair and scope; these instructions do not schedule a run.

## Execution plan and restart point

1. Preserve Git state; record both commits and trees. Export clean Git snapshots
   under ignored `build/`, rather than compiling changing working-tree headers.
   The current inventory records Windows path-sort order; fingerprints hash
   entries in that saved order. Use the full manifests and Git trees when
   comparing inventories rather than assuming another host's ordering.
2. Prepare a standalone common harness, MSVC first and GNU-driver Clang second.
   Use optimized, unsanitized builds, explicit ISA settings, no LTO, no implicit
   floating-point contraction and identical options for both libraries.
3. Probe CPU/OS ISA support before executing SSE2, SSE4.2, AVX2 or AVX-512 builds.
   Cover native and composed 128/256/512-bit vectors, changed rounding/conversion
   paths, basic arithmetic, integer division/lookups and array workloads.
4. Check every benchmark against an independent scalar calculation before
   timing. Keep special-value correctness separate from timing inputs. Save
   optimized object disassembly for both libraries.
5. Pin to one available logical processor, warm up, calibrate samples, alternate
   library execution order across repeated pairs and preserve every sample.
   Record the active power scheme; do not change system settings automatically.
6. Summarize per-case ratios and variation, retaining slower cases. Investigate
   assembly differences and label correctness fixes separately from avoidable
   overhead. A quiet-looking average is not proof that every API is unaffected.
7. Save a dated report and portable tables/checksums; archive full raw evidence
   under `build/`. Review the resulting diff. Leave everything uncommitted.

## Run again

Use native Windows x64 Python 3.9+, an installed Visual Studio x64 C++ toolchain,
and its GNU-driver `clang++` and `llvm-objdump`. Nothing is installed by the runner.
Run from the repository root. Inspect `git status --short --branch`, read local
`AGENTS.md` instructions, and preserve unrelated work first. Fetch upstream if
needed, without resetting branches, and choose explicit full source SHAs. The
runner does not fetch or change branches and requires both commits locally.

```powershell
git fetch --no-tags upstream master
git rev-parse HEAD
git rev-parse upstream/master
& ([scriptblock]::Create((Get-Content -Raw -Encoding UTF8 -LiteralPath tools/run_x86_upstream_comparison.ps1))) `
  -ForkCommit ade9d9baf559ad480289277dd6ad8e6d1c5b15e3 `
  -UpstreamCommit 667577495efaa36a10cbb5d6cb8c942c23913848 `
  -Cpu 2 -Pairs 4 -Trials 7 -SampleMs 6 `
  -PortableOutput docs/benchmarks/x86/x86-upstream-YYYY-MM-DD-UNIQUE-RUN
```

Use a new portable destination on every run; existing output is never
overwritten. `-Output build/UNIQUE-RUN` optionally selects the raw destination.
The wrapper initializes the installed x64 developer environment. The scriptblock
invocation also works on this host without changing its PowerShell execution
policy. These parameters reproduce the final report's sampling protocol; runner
defaults use a shorter three-pair/four-millisecond screen. In an existing
developer shell the Python runner can be called directly;
its `--help` lists the explicit Clang and disassembler paths.

Select an available logical processor deliberately (`-Cpu`; default is the
first allowed processor). Affinity pins the measurement thread, without reserving
the physical core or excluding its SMT sibling. Close competing applications,
avoid sleep and keep power/cooling conditions comparable. The runner records
the active scheme and does not change system power settings. It requires all
four CPU/OS ISA profiles; unsupported hardware stops the run rather than
silently supplying a partial comparison.

## Workloads and controls

The 102 cases cover FP32/FP64 at 128/256/512 bits: add, multiply, divide,
sqrt-plus-feedback and round-plus-feedback, each with one dependent chain and
four independent chains; classification-plus-feedback with four chains;
FP64 truncate-to-int64/back-plus-feedback; array arithmetic `(a+b)*c` at
128/16,384 elements; and sine/exp at 128 elements. Integer division by seven and
256-entry lookup dependency chains cover signed 32-bit vectors at all widths.
Each recurrence executes 4,096 steps. Normal finite timing inputs are seeded
binary fractions; recurrence factors keep values finite and normal.

Every case checks every output lane against an independent scalar calculation
before measurement. Basic operations require exact bits. The separate benchmark
sanity gate allows at most four ULP for sine/exp on these positive ordinary
inputs; it does not replace or modify the library's existing numerical gates.
NaN, infinity, signed-zero and conversion-boundary correctness remain covered
by the separately recorded correctness suites, not this timing corpus.

Inputs/outputs end at inaccessible pages, with four intentional read/write
fault controls checking both sides of a one-page control allocation. These
checks supplement the existing guard-page tests; they do not replace them.
MXCSR starts at `0x1f80` (nearest-even, no FTZ/DAZ). Timings use optimized `/O2`
or `-O2`, MSVC `/fp:strict` or Clang strict FP/no contraction, static release CRT,
and no LTO/sanitizers. SSE4.2 uses explicit `INSTRSET=6` intrinsics on MSVC,
which has no `/arch:SSE4.2`; AVX2 and AVX-512 use explicit target options.

The scalar/check/timer driver is a separate translation unit. Clang disables
automatic loop/SLP vectorization; MSVC uses explicit `loop(no_vector)` controls
on its driver/reference loops and records `/Qvec-report:2` diagnostics. An ignored
compiler option stops the runner. MSVC's reason `1400` confirms a no-vector loop
control was recognized; other scalar loops can be rejected for independent
reasons such as control flow. See [Microsoft's loop pragma reference](https://learn.microsoft.com/en-us/cpp/preprocessor/loop?view=msvc-170).
Kernels have externally visible noinline entry points;
helpers are forced inline. Results cross the translation-unit boundary and are
checksummed outside timing. Each sample repeats the entire kernel, including its
call and final stores. No baseline overhead is subtracted. Consequently these
are workload measurements, not isolated instruction latency specifications.

## Evidence and interpretation

The default warms/calibrates each case to a sample of at least approximately four
milliseconds and records seven samples per invocation. Three paired invocations
alternate upstream/fork, fork/upstream, upstream/fork. More pairs and longer
samples can be selected explicitly; an even pair count balances which library
runs first. All MSVC profiles precede Clang. Ratios compare per-invocation medians,
then take the median of pair ratios;
values above one mean the fork was slower. CSV/JSON retains every case, sample
range and pair-ratio range. The descriptive `slower-consistent` flag requires
all pair ratios above 1.05; `faster-consistent` requires all below 0.95. These
flags are screening criteria, not statistical significance or universal speed
guarantees. Inspect noise and assembly before attributing a difference.

Static instruction counts describe only each named entry point, excluding any
called helper bodies. Equal mnemonic counts do not establish equal machine
code or performance; different counts can reflect inlining decisions. Full
object disassembly, relocations, binaries, sources, command/exit-code logs,
checks, all samples and checksums remain under `build/x86-upstream-<UTC>/`.
Commands have 180-second deadlines; failures stop the run and are not retried
automatically. A raw evidence ZIP and SHA256 receipt accompany a successful run.
Portable metadata excludes absolute machine paths, raw logs and binaries.

If interrupted, inspect `build/x86-upstream-*` before starting another run;
preserve failed attempts and their command logs. Preserve each report and its
checksum inventory. Source commits identify only library snapshots; separately
recorded hashes identify the benchmark/runner snapshot, including uncommitted
files. Do not label later commits as measured without an explicit comparison.

Physical Intel testing is omitted. AMD results do not establish Intel
performance. ARM benchmarks answer a different question and cannot isolate an
x86 regression introduced by this port.

## Compare an uncommitted optimization candidate

Use the same sampling/ISA/FP protocol with a reviewed source patch. The wrapper's
`-CandidatePatch` invokes `tools/run_x86_candidate_comparison.py`, which patches
only ignored Git exports. It rejects binary/link/rename patches and unsafe or
non-source paths; it does not create a commit, alter the index or patch the
primary checkout. The base and applied patch have separate hashes; the candidate
has `commit: null`, an explicit base commit/tree and a canonical path-sorted
source manifest. Its source archive contains the actual patched bytes. The
captured patch and runner/harness hashes make the experiment reproducible even
before publication.

For a newly approved experiment, replace the commit and patch placeholders below
with its reviewed source identities. Keep the patch under ignored `build/`:

```powershell
& ([scriptblock]::Create((Get-Content -Raw -Encoding UTF8 -LiteralPath tools/run_x86_upstream_comparison.ps1))) `
  -ForkCommit APPROVED-BASE-COMMIT `
  -UpstreamCommit APPROVED-REFERENCE-COMMIT `
  -CandidatePatch build/REVIEWED-CANDIDATE.patch `
  -Cpu 2 -Pairs 4 -Trials 7 -SampleMs 6 `
  -Output build/UNIQUE-BEFORE-AFTER `
  -PortableOutput docs/benchmarks/x86/UNIQUE-BEFORE-AFTER
```

The closed 16 November campaign's concrete two-experiment example remains in
[frozen Git](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/benchmarks/x86-upstream-comparison.md#compare-an-uncommitted-optimization-candidate).
Its first reference was the pre-change fork; its second was original VCL.
Legacy result field names call both references `upstream`, so inspect each
recorded source revision/role. Do not pool source pairs or historical samples.

Run correctness before timing, and execute timing processes sequentially without
concurrent builds. Afterward the existing edge follow-up accepts the candidate
manifest and checks the exact retained exports. Keep the Windows native
correctness matrix separate; benchmark checks are not a replacement for it.

The former canonical-patch/generated-root workflow is retired. Preserve the
28 original root files and use the [current ownership rules](../../backend-layout.md).
Any future experiment needs an agreed source/export workflow; reproducing an
older campaign uses its frozen source and tooling, not current-master passes.
Dependency updates require separate approval. The historical post-timing
exporter validation does not transfer to a new source or runner revision.

## Untimed edge-value follow-up

After timing finishes, run the edge probe against the retained source exports
with the exact recorded kernel compile/link flags. Use an x64 Visual Studio
developer PowerShell session (or initialize it as below):

```powershell
$comparisonVs = & 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe' `
  -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$env:PATH = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer;' + $env:PATH
Import-Module (Join-Path $comparisonVs 'Common7/Tools/Microsoft.VisualStudio.DevShell.dll')
Enter-VsDevShell -VsInstallPath $comparisonVs -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64'
python -B tools/check_x86_upstream_edges.py `
  --run build/x86-upstream-REPLACE-WITH-COMPLETED-RUN `
  --portable-output docs/benchmarks/x86/REPLACE-WITH-RESULT-DIRECTORY/edge-controls
```

The probe checks five IEEE classifications across zero, signed zero, infinity,
quiet/signaling NaN, subnormal and ordinary inputs at each width, plus explicit
double-rounding answers including odd integers above `2^52`. It never times
these inputs and never invokes undefined out-of-range scalar conversions.
Every lane rotates through all eight patterns. Both libraries are checked
against independent expectations; an upstream failure remains a failure rather
than becoming the fork's expected answer. The follow-up verifies all exported
source hashes first, records its own source/runner hashes and creates separate
raw evidence and portable checksums. An observed probe exit `1` is retained in
the journal and reports failed lane checks; the orchestration tool still
finishes collecting both sources. Inspect `edge-results.json`, not only the
orchestrator's exit status. The follow-up also builds/runs the frozen fork's
CMake arithmetic example with both compilers. Its compiler/ISA probe flags are
copied from the original run, and the CMake examples are separate consumer checks.
