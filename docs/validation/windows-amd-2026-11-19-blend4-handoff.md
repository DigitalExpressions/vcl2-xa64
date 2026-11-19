# Native AMD Windows handoff: Git four-blend candidate — 19 November 2026

## Scope and publication route

Use Git to transfer this validation candidate: fetch the fork's
`validation/m2-blend4-windows` branch, then pin the **full commit SHA supplied
with its publication notice**. The branch transports a candidate for Windows
review; it is not integration into master or a decision to retain the change.
No ZIP transfer is needed. Do not substitute a later branch tip for the approved
SHA, and do not build an uncommitted live checkout.

The M2 implementation/result identities below remain historical immutable
captures. This Git publication adds reports and validation resources, retains
the later master documentation, and changes checkout rules for exact portable
metadata. Windows must record its actual publication commit/tree and complete
new source fingerprint separately; the Mac did not execute that later commit.
All runtime/test/fixture/build/runner bytes match the M2 candidate exactly.
This handoff is preparation, not an executed Windows result.

Verify the added movement regression and pristine x86 preservation, retaining
the original failed/incomplete gate. ARM `vectori128_perm_neon.h` stays outside
the x86 graph; the only x86-visible change is added test coverage. End with
results and a retain/reject review. No x86 repair, timing, cleanup, merge into
master, PR or later publication is authorized by the Windows execution task.

Read the actual host's applicable `AGENTS.md`, protected root `README.md`,
[fork guide](../../README-vcl2-xa64.md), [contributing rules](../../CONTRIBUTING.md),
[integration plan](../integration-plan.md), [validation index](README.md),
[ownership](../backend-layout.md), [numerical contracts](../numerical-contracts.md),
[Windows procedure](windows-runner.md), [integration acceptance](architecture-separation-integration-2026-11-19.md),
[old matrix](windows-amd-2026-11-19-arch-separation.md),
[old triage](windows-amd-2026-11-19-arch-separation-triage.md),
[new M2 correctness report](mac-arm-2026-11-19-blend4.md),
[candidate benchmark report](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19.md)
and [matched baseline report](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19_BASELINE.md).
Use this new scope; old migration handoffs and their pending status are historical.

## Original M2 identities and publication equivalence

| Input | Identity |
| --- | --- |
| M2-tested candidate | 571-file frozen uncommitted capture; tested commit/tree null |
| M2 candidate fingerprint | `3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36` |
| Retained M2 source ZIP SHA256 | `9883e0da75711f61d5bd5ceb3b7e2c86152b2b2975cebb897a95c179679d213c`; retained on Mac, not needed for Git transfer |
| M2 tracked patch SHA256 | `6ef131ac0f37551994e0b9a8ac4756b1f497bf0ca56c1e5e9ed9a403020e2663` |
| Original M2 base/control | `5e039b01b49abc00548123763be85898932e3ad8`, tree `ea90c1b40fc50936db7c3509aaa11ab51c02d890` |
| Exact clean control | 564 files; fingerprint `fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06` |
| Historical Windows publication | `2d251bc72bcc861edad371499fa732e8eb45a5f7`, tree `8c16f0135a597db27b963dff6c241b533a5b91b4` |
| Historical Windows source | 497 files; fingerprint `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2` |

Fingerprint input: UTF-8-byte-sorted relative paths, concatenate
`path + NUL + raw SHA256 hex + newline`, then SHA256 that UTF-8 input.
Relative to the 564-file control, the M2 capture changes two tracked paths only:
`vectori128_perm_neon.h` and `tests/movement_slice.cpp`. All other 562 base
paths match raw bytes. Seven additions are earlier planning/preflight metadata.
The tracked patch alone is not the full original capture.

The Git publication keeps every one of those 571 paths. Only the explicit
[metadata allowances](windows-blend4-handoff-2026-11-19/git-source-policy.json)
may differ: later master guidance and `.gitattributes` checkout rules. Every
other captured file must match the original raw SHA256. Added paths are a
closed inventory of reports/data and supplemental validation resources; no
addition enters the active library or baseline test/runner graph. The original
[571-file manifest](m2-blend4-2026-11-19/capture.json) is preserved unchanged.

The [original source comparisons](windows-blend4-handoff-2026-11-19/source-comparison.json)
account for every old Windows path too: 490 of 497 match the clean control,
seven differences are metadata and 67 additions are later metadata. No earlier
Windows runtime/test/runner/build/dependency input changed before this candidate.
Do not compare only an inventory intersection or infer a new native pass from
these byte checks.

## Preserve the Windows checkout and fetch the Git candidate

Before fetching/writing, record actual branch, HEAD/tree, full staged/unstaged
and untracked state, binary diffs, refs, worktrees, remotes and saved local Git
configuration under a new ignored `build/` session. Journal raw arguments,
exits/stdout/stderr. Preserve all work, branches, tools, pinned dependencies,
worktrees and evidence. Leave any legacy sibling checkout alone. Do not
reset/stash/clean/prune, delete branches, change remotes or modify machine policy.

Confirm origin is `https://github.com/DigitalExpressions/vcl2-xa64.git`.
Fetch without pruning. Set `$approvedCommit` to the **literal full 40-character
SHA from the publication notice**, not the branch name or a hash guessed from
this document. If no publication SHA is available, the transport is still
pending; do not test the previous master or original architecture topic instead.

```powershell
git -c gc.auto=0 fetch --no-prune origin
if ($LASTEXITCODE -ne 0) { throw 'Fetch failed' }
git merge-base --is-ancestor $approvedCommit origin/validation/m2-blend4-windows
if ($LASTEXITCODE -ne 0) { throw 'Approved publication is not on the validation branch' }
# Choose a new short ignored session and destinations; reject existing paths.
$publicationCheckout = Join-Path $session 'p'
$baselineCheckout = Join-Path $session 'b'
if ((Test-Path $publicationCheckout) -or (Test-Path $baselineCheckout)) { throw 'Choose fresh worktrees' }
git -c core.autocrlf=false -c core.eol=lf worktree add --detach $publicationCheckout $approvedCommit
if ($LASTEXITCODE -ne 0) { throw 'Publication worktree creation failed' }
git -c core.autocrlf=false -c core.eol=lf worktree add --detach $baselineCheckout 5e039b01b49abc00548123763be85898932e3ad8
if ($LASTEXITCODE -ne 0) { throw 'Control worktree creation failed' }
```

Require exact requested HEADs, empty status and unchanged saved configuration.
Use short roots: some source relative paths are 154 characters, and build paths
add more. Choose fresh shorter permitted workspaces if needed; do not change
machine-wide execution/long-path policy. Never overwrite a checkout/dependency.

Verify all raw publication files against their exact Git blobs and the complete
original M2 manifest using the checked-in read-only verifier:

```powershell
$helperRoot = Join-Path $publicationCheckout 'docs/validation/windows-blend4-handoff-2026-11-19/helpers'
$publicationProof = Join-Path $session 'publication-source-proof.json'
python -B "$helperRoot/verify_git_source.py" --checkout $publicationCheckout `
  --expected-commit $approvedCommit --output $publicationProof
if ($LASTEXITCODE -ne 0) { throw 'Git/raw-byte/M2-equivalence verification failed' }
```

The verifier requires a clean pinned Git checkout, compares every raw file with
its blob, rejects any unlisted metadata change/addition and verifies protected
root/vendor pins. A newline-only difference is a failure; retain bytes and stop
rather than normalizing files or editing accepted hashes. On the control, compare
every raw checkout path with its Git blob and require tree/file-count/fingerprint
`ea90c1b...`, 564, `fa9efdd...` above. Record all full identifiers, not abbreviations.

## Capture the exact clean Git inputs for native execution

Create fresh complete captures **from both clean worktrees**, preserving Git
commit/tree, all raw per-path hashes, source ZIPs/receipts and empty tracked
patch/no added/deleted paths. Do not use a live tree after source edits or make
a commit just to bypass a validation guard.

```powershell
$candidateContainer = Join-Path $session 'c'
$baselineContainer = Join-Path $session 'a'
Set-Location -LiteralPath $publicationCheckout
python -B tools/capture_source.py --output $candidateContainer
if ($LASTEXITCODE -ne 0) { throw 'Published source capture failed' }
Set-Location -LiteralPath $baselineCheckout
python -B tools/capture_source.py --output $baselineContainer
if ($LASTEXITCODE -ne 0) { throw 'Control source capture failed' }
$sourceRoot = Join-Path $candidateContainer 'source'
$captureManifest = Join-Path $candidateContainer 'capture.json'
$baselineRoot = Join-Path $baselineContainer 'source'
$baselineManifest = Join-Path $baselineContainer 'capture.json'
python -B "$sourceRoot/tools/capture_source.py" --checkout $sourceRoot --verify-manifest $captureManifest
if ($LASTEXITCODE -ne 0) { throw 'Publication capture integrity failed' }
python -B "$baselineRoot/tools/capture_source.py" --checkout $baselineRoot --verify-manifest $baselineManifest
if ($LASTEXITCODE -ne 0) { throw 'Control capture integrity failed' }
```

The new publication capture must equal every path/hash in `$publicationProof`;
retain its **own** full file count/fingerprint, exact `$approvedCommit` and tree.
It is larger/different in metadata from the original 571-file M2 capture; do not
rename its fingerprint to `3ae638...` or claim the Mac executed the Git commit.
The control must still have exactly the original 564-file `fa9efdd...` identity.
The tool's schema name `frozen-uncommitted-source` is generic: these new captures
represent clean Git inputs with empty patches. It does not imply a dirty tree.

Build only `$sourceRoot` / `$baselineRoot` with their respective explicit capture
manifests. Do not write newer documentation into them or build directly from a
moving checkout. Use Python `-B` for every script/import. The original Mac ZIPs
and earlier ZIP handoff remain retained local evidence; neither is needed here.

## Native prerequisites and controls

Use the physical AMD x64 Windows host and an installed x64 Visual Studio developer
shell. Inspect actual Windows/CPU/compiler versions, native x64 Python 3.9+,
CMake/CTest with JUnit and Ninja; inspect available MSVC and native GNU-driver
`clang++.exe`. Establish native process/PE machine `0x8664`, not WSL/emulation.
Do not install/update tools, use ASan, hosted CI or collect timing samples.

The existing pristine testbench must be at
`a7293702385e5169a5f58b83177f2a43cd59139b`, clean including untracked paths.
Verify all three raw hashes explicitly; a runner plan or HEAD check alone is
not the hash proof:

| File | SHA256 |
| --- | --- |
| testbench1.cpp | `7411fc27af1055744c7e8b8b4fe64b52859b561dab0f48b9b0278413c71aea27` |
| testbench2.cpp | `6a23124de4495368bb1e343569f7c1c8cbf7c29f5ab0c0c729e9c068fb8c2a5b` |
| testbench3.cpp | `51964802a6789e230ce2fc2ed23e6adbc9ef5153815246b4cbc09a735d2ffc49` |

Never overwrite a checkout. External testbench source stays pristine; generated
include-adapted copies belong under build. Set `$testbench` to its existing
verified directory and `$clangCompiler` to the inspected installed x64 executable.
The frozen Windows runner takes its source root from its own file location;
it has no `--checkout`. Invoke the script inside `$sourceRoot`, not a live
checkout's runner with another capture's manifest.

Before/after every phase, verify all 28 root originals, 28 vendor originals,
both pin files and sse2neon alias/vendor header: all 60 paths retain raw hashes,
inventory and nanosecond mtimes. Pin remains
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Preserve original offending bytes and
stop on mismatch. From the frozen source run read-only maintenance:

```powershell
Set-Location -LiteralPath $sourceRoot
python -B tools/regenerate_headers.py --check
python -B tools/check_repository.py
python -B tools/check_header_generation.py
python -B tools/check_source_capture.py
python -B tools/run_windows_validation.py --driver msvc --mode Release `
  --testbench-dir $testbench --capture-manifest $captureManifest --plan
```

Keep disposable integrity-negative inputs/results. Require actual-page-size guard
positive exit zero and four boundary read/write exits `0xc0000005` (retain the
process API's signed/unsigned representation). Runtime must use the original
x86 detector and report expected ISA before selected execution. Clang Debug uses
static release CRT `MultiThreaded`, strict nonrecovering UBSan, frame pointers and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`: positive zero, deliberate
overflow exit 1 with the expected diagnostic and no continuation. MSVC Debug is
unsanitized. Missing runtime/controls are gaps, never sanitizer passes.

## Full candidate baseline, with honest failure collection

Run MSVC first, then Clang, preserving each invocation/packet/exit separately:

```powershell
python -B tools/run_windows_validation.py --driver msvc --mode Release `
  --testbench-dir $testbench --capture-manifest $captureManifest
python -B tools/run_windows_validation.py --driver msvc --mode Debug `
  --testbench-dir $testbench --capture-manifest $captureManifest
python -B tools/run_windows_validation.py --driver clang --mode Release `
  --compiler $clangCompiler --testbench-dir $testbench --capture-manifest $captureManifest
python -B tools/run_windows_validation.py --driver clang --mode Debug --sanitize `
  --compiler $clangCompiler --testbench-dir $testbench --capture-manifest $captureManifest
```

Each profile registers 1,348 cases: **5,392 total**, plus four separate examples.
Preserve all six configuration/reference fixtures and the complete names.
All correctness deadlines remain 60 seconds, concurrency three. Ordinary MSVC
keeps `/fp:strict`, classification alone `/fp:fast`; Clang keeps no contraction,
selected rounding flags and classification-only finite-math stress. Do not relax
flags, thresholds, seeds, canaries, guard controls or invalid-conversion policies.

**Expect inherited build/runtime failures.** The unmodified runner stops at a
build failure and may not write a final CTest inventory/result. Keep that failed
attempt, false summary and original return ZIP unchanged. For a failure that
occurs before any suite execution, use a separately journaled `continuation01`
collection on the same frozen source/configuration/build directory. It may
finish independent Ninja edges and account for every registered case:

```powershell
cmake --build $suiteBuild --parallel 3 --verbose -- -k 0
# Retain the actual nonzero build exit; do not relabel it as successful.
ctest --test-dir $suiteBuild --show-only=json-v1
ctest --test-dir $suiteBuild --output-on-failure --no-tests=error `
  --timeout 60 --parallel 3 --output-junit $uniqueContinuationJUnit
```

`$suiteBuild` is the original runner output's `suite/`; keep continuation command
logs and JUnit at new paths outside original packet files. Never overwrite the
original failed summary/ZIP. Do not rerun completed CTest cases; if a profile
already executed its suite, use its retained results. Record unavailable/missing
executables as **not run**, separately from skips, failures and timeouts. Treat
any timeout as failure at the original deadline. Hash existing executable files
and verify x64 PE; a missing binary is an unavailable case, not a hash pass.
Run and verify the separately built example without hiding suite failures.
Inspect actual compile/FP/CRT flags and all controls even after a failed gate;
manual collection bypasses no acceptance requirement. Do not patch the runner.

## Isolate the exact added regression, keeping full targets untouched

A complete `movement_slice` target may fail to compile on original nonpositive
lookup instantiations (notably Clang), or fault in old lookup tests before reaching
the new call. This does not execute or validate the added regression. Do not
remove/pad old cases or repair original headers to make the full target pass.

The checked-in emitter checks candidate `tests/movement_slice.cpp` raw SHA256
`7ee48af6fa40cd0b52c5a5be979bdaed0632833ab8500128be69ce1162373f25` and copies
its exact `Scalar`, `CHECK`, `do_blend` and 62-line added helper/oracle region.
Only a small isolated `main` calls that region; no oracle/pattern body is edited.
[Extraction hashes](windows-blend4-handoff-2026-11-19/probe-extraction.json) record
the exact emitted sections and helper/project identities. Python emission was
checked on Mac; Windows compilation/execution is still required. The captured
FPCR/FPSR block stays under its ARM/compiler guard: x86 runs the integer-bit
checks, not ARM assembly. `Vec4f`, `Vec4i`, `Vec4ui`, all four tuples, both source
orders, rotating edge bits and fallback zero/don't-care patterns remain intact.

Create a new supplemental probe project outside either immutable source root:

```powershell
$probeSource = Join-Path $session 'probe-source'
if (Test-Path $probeSource) { throw 'Choose a fresh probe source directory' }
New-Item -ItemType Directory -Path $probeSource | Out-Null
Copy-Item -LiteralPath "$helperRoot/probe-project/CMakeLists.txt" `
  -Destination (Join-Path $probeSource 'CMakeLists.txt')
python -B "$helperRoot/generate_movement_probe.py" `
  --candidate-source $sourceRoot --route facade `
  --output (Join-Path $probeSource 'movement-facade.cpp')
if ($LASTEXITCODE -ne 0) { throw 'Exact regression extraction failed' }
python -B "$helperRoot/generate_movement_probe.py" `
  --candidate-source $sourceRoot --route originals `
  --output (Join-Path $probeSource 'movement-originals.cpp')
if ($LASTEXITCODE -ne 0) { throw 'Exact regression extraction failed' }
```

For each compiler/mode, configure a fresh probe build using the same installed
compiler, CRT, ISA and FP settings as its full profile. The project imports the
frozen library's **existing `vcl_test_options`**, preserving actual movement-test
options, and adds four external targets: facade/direct-vendor-original routes,
each default/`VCL_NAMESPACE=isolated`. `VCL_CHECKOUT` selects the frozen headers;
keep the emitted candidate test bodies fixed when building against both sources.
Example shape, with inspected variables and `$sanitize` ON only for Clang Debug:

```powershell
cmake -S $probeSource -B $probeBuild -G Ninja `
  "-DVCL_CHECKOUT=$sourceRoot" "-DCMAKE_CXX_COMPILER=$compiler" `
  "-DCMAKE_BUILD_TYPE=$mode" "-DVCL_SANITIZE=$sanitize" `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "-DPython3_EXECUTABLE=$pythonExecutable"
# Clang Debug also needs -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded.
# Supported higher profiles also pass "-DCMAKE_CXX_FLAGS=$isaFlags" to match their full profile.
cmake --build $probeBuild --parallel 3 --verbose --target `
  probe_movement_facade_default probe_movement_facade_namespace `
  probe_movement_originals_default probe_movement_originals_namespace
ctest --test-dir $probeBuild -R '^probe_movement_' --show-only=json-v1
ctest --test-dir $probeBuild -R '^probe_movement_' --output-on-failure `
  --no-tests=error --timeout 60 --parallel 3 --output-junit $uniqueProbeJUnit
```

The filter selects four **supplemental** cases only. The imported library's
unbuilt legacy targets are not counted as passes here; the full unfiltered
candidate matrix above remains mandatory and retains its failed gate. Require
four selected names, all exits zero, no diagnostic/skip/timeout and x64 PE proof.
Repeat with fresh `$probeBuild` and `VCL_CHECKOUT=$baselineRoot`, using the same
emitted source. Compare facade/original and candidate/control output/status at
matched flags; validate the scalar bit-index oracle, not agreement alone.
Record these extra cases separately from the 5,392/320 registered library cases.
An isolated pass means only the added checks passed; it never changes the full
movement target's failed/not-run status. On discrepancy, retain it and do only
bounded same-input/flags source attribution; no repair or test relaxation.

## Supported higher ISA and x86 path preservation

After a fresh independent CPUID **and OS/XCR0** probe establishes support, collect
candidate AVX2 and AVX-512 local profiles for both compilers, Release/Debug:
40 registered cases per profile, **320 over eight profiles**, plus eight examples.
Use separate fresh CMake builds, no testbench, `VCL_BACKEND=1`, tests/examples ON,
exported compile commands and the same captured source. Use the documented
Windows runner guide/old architecture procedure for these manual configurations;
the baseline runner's ISA-restricting validator is not a higher-ISA pass.

MSVC ISA flags: `/arch:AVX2` or `/arch:AVX512`. GNU-driver Clang: `-mavx2 -mfma`
or `-mavx512f -mavx512vl -mavx512bw -mavx512dq -mfma`. Clang Debug retains strict
UBSan and `MultiThreaded`; ordinary FP and classification-only stress remain
separate. Preserve original build aborts and separately collect independent
edges/inventory as above. Missing CPU/OS support is an explicit gap, not emulated
coverage. No physical Intel or FP16 execution is requested.

Run the same four isolated regression targets against candidate and control at
each supported higher profile too, with matching flags/namespace settings and
fresh positive/runtime/guard/UBSan controls as relevant. At twelve supported
profiles this is 48 supplemental cases per source / 96 total, accounted
separately; any unavailable combination remains explicit. Do not reduce the
unfiltered 5,712-case candidate inventory to these supplemental checks.

For facade consumers capture `/showIncludes` or Clang `-H`; require pristine
original vector/math headers and original detector, with no `_neon.h`, sse2neon,
adaptation or retired backend helper. The direct probe includes unchanged
`vcl_config.h` only for selection/test macros plus explicit vendor originals;
record that neutral shim rather than calling it a completely policy-free probe.
Use matching settings and retain raw objects/disassembly. Compare selected
regression function code or relocation-resolved runtime sections, distinguishing
source/debug/checksum metadata from instructions. Whole-object identity is not
required or implied. The full runner separately validates `vcl::runtime` through
the pristine detector. No source or include policy edit is permitted.

## Keep the historical failed gate and compare complete inventories

The old Windows gate stays **failed/incomplete: 5,387 passed, 249 failed,
76 not run out of 5,712 registered**, plus twelve passing examples. Baseline was
5,231 passed / 133 failed / 28 not run of 5,392; higher ISA was 156 passed /
116 failed / 48 not run of 320. The old one timeout is included in failures.
All 325 nonpassing entries were classified; those original reports, hashes,
source IDs and archives remain unchanged.

Compare every new profile/name against the full old
[inventory/results](windows-arch-separation-2026-11-19/test-results.json) and
[classification](windows-arch-separation-triage-2026-11-19/classification.json).
Use explicit compiler/mode/ISA profile mapping, preserving default/namespace
cases and all fixtures. Do not compare only common passing cases or require
coincident aggregate counts as proof of equivalence. Distinguish inherited
compile/lookup/UB/numerical/Debug-RTC findings, actual new status/diagnostic
changes, unavailable coverage and compiler-version differences.

For any changed outcome needed to assess this candidate, use the supplied exact
control source and matched flags/input for a bounded causal check. If installed
tools differ from the historical run, old results are historical controls only;
run current same-host control checks where attribution depends on that change.
Do not resume the retired x86 repair campaign or replay all old triage by default.
An unexplained candidate-specific regression, failed isolated new check, source
mismatch or missing required control blocks a positive preservation conclusion.
Reproducing old failures is not a new correctness pass, and an unavailable full
movement target does not erase the separate isolated outcome.

Return two assessments: (1) the **actual full correctness gate** with all failed
and unavailable cases retained; (2) the **bounded preservation/new-regression
assessment**, supported by raw-byte/include, controls and paired selected checks.
They may be failed/incomplete and preserved respectively; state evidence limits.
Do not change tests, protected originals, runner behavior or numerical policy.

## Results, evidence and the later retain/commit review

On the actual execution date prepare a new dated Windows report and portable
checksums outside immutable roots. Identify exact tested fingerprint/commit-null
provenance, actual tools, all profile counts, registered/JUnit names, controls,
include/code comparisons, isolated/candidate/control outcomes, new vs inherited
findings and missing coverage. Keep all raw commands/exits, first aborts,
continuations, generated C++/CMake/CPUID/reference inputs, flags, binaries/hashes,
source/mtime snapshots, JUnit and source archives under ignored build. Verify
compact archive CRC/entry hashes and supplement uncovered inputs before any
future cleanup; no cleanup is authorized here. Reverify both complete captured
sources, protected paths, dependencies and testbench after all phases.

The M2 candidate already passed **8,036/8,036 cases plus eight examples** on the
571-file capture. Its matched 36-process benchmark retained all 320 three-way
rows / 160 before-after rows with bit-identical outputs. FP32 AoS transpose
throughput improved **6.0% at 128 objects**, but only **0.5% at 4,096** with
range overlap there. FP32 AoS TRS inverse elapsed time increased **2.2% / 2.7%**
at 128/4,096; both inverse ranges were disjoint. These are this M2 run's results,
not an overall performance win or Windows performance claim. Other apparent
gains included unchanged kernel bytes at different addresses; do not attribute
them automatically to the four blends. No new timings are authorized on Windows.

After Windows evidence is available, present the actual preservation/regression
result and these workload-specific M2 tradeoffs for the maintainer to decide
whether to retain the change. Validation is not an automatic retain decision.
Any source fix/revert is separate scope requiring a new frozen checkpoint and
fresh relevant/full baseline validation. Committing is a later, explicit decision;
this task authorizes no commit, push, merge, PR or publication. Stop at reviewable
Windows results and the retain/reject assessment, preserving all evidence.

## Copyable Windows prompt

```text
Validate the four-blend candidate natively on the AMD x64 Windows host using Git.
Use the full approved publication SHA supplied with this prompt. Fetch origin
without pruning; create fresh clean detached publication/control worktrees with
command-scoped core.autocrlf=false/core.eol=lf. Control is
5e039b01b49abc00548123763be85898932e3ad8. Preserve all local work, refs, worktrees,
tools, dependencies, configuration and evidence. Read actual host AGENTS.md and
docs/validation/windows-amd-2026-11-19-blend4-handoff.md from the approved commit.

Run its checked-in helpers/verify_git_source.py against the exact publication,
checking every Git blob, the original M2 manifest and closed metadata allowances.
Capture both clean complete worktrees with capture_source.py and verify them.
Build only those immutable captures. Record the publication's own commit/tree,
file count and fingerprint; the original 571-file M2 fingerprint
3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36 stays historical.
The 564-file clean control fingerprint remains
fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06.
No ZIP transfer is required. Do not substitute a moving branch tip for the SHA.

Verify protected originals/dependencies, native AMD/Windows x64 tools, pristine
testbench revision and all three pinned hashes. Run the frozen four-profile full
baseline, preserving aborts and separately collecting complete failures/not-run
inventories. Run CPU/OS-gated AVX2/AVX-512 local profiles. Keep 60-second deadlines,
concurrency three, strict FP flags, guard/canary controls and Clang nonrecovering
UBSan with static release CRT. MSVC Debug is unsanitized.

Use the checked-in exact-section regression emitter and supplemental CMake project
for candidate/control, facade/direct originals and default/isolated namespaces.
The full movement target retains inherited lookup compile/runtime failures;
isolated passes supplement the complete matrix and never relabel failed cases.
Verify x86 include/code/runtime preservation and complete per-case changes.

Preserve the historical failed/incomplete gate: 5387 passed, 249 failed, 76 not run
of 5712, plus 12 examples. Report actual full-gate and bounded-preservation outcomes
separately. No repair, gate relaxation, benchmark, ASan/emulation/hosted CI,
consumer edit, cleanup, commit/push/merge/PR or branch retirement. Return dated
results, exact source/control proofs, complete inventories, checked archives and
gaps. Then review the M2 6% small-transpose gain versus 2–3% TRS-inverse elapsed
cost for a maintainer retain/reject decision; integration/publication is separate.
```
