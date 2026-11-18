# Windows return handoff: verify the separated architecture candidate

Updated 18 November 2026 after the ARM header cleanup and native rerun. Use this handoff
when the maintainer requests the native AMD Windows phase. It is an instruction,
not a Windows result or source-publication approval. No benchmark timing or x86
repair campaign is authorized. The maintainer approved topic-branch publication
of the implementation/results; native Windows results need their own approval
before any commit or push. The Mac tested the earlier frozen capture described below.

## Exact candidate and Mac outcome

Read applicable `AGENTS.md`, [fork guide](../../README-vcl2-xa64.md),
[ownership rules](../backend-layout.md), [approved plan](../arch_separation_plan.md),
[Mac report](mac-arm-2026-11-18-arm-header-cleanup.md), [portable capture](arm-header-cleanup-m2-2026-11-18/capture.json),
[attempts](arm-header-cleanup-m2-2026-11-18/attempts.json),
[Windows procedure](windows-runner.md) and
[retained x86 findings](../x86_findings_and_fix_candidates.md).

- Documentation/base commit: `1a5226446f14e1f4d26216ae688b40ccc9b0fa03`.
- Base tree: `e5b8579b6292cce2ea6a67b8cdf8027ec228d7bc`; not the tested source tree.
- Candidate: **cleanup 01**, a complete frozen **uncommitted** 474-file source capture.
- Source fingerprint: `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
- Source ZIP SHA256: `3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`.
- Tracked patch SHA256: `f83b4c2cbb527be2c043c86f65ff7c84c7dc91b892f3ba754236bf3ea9548a3a`; additions/deletions and actual
  new-file bytes accompany it in the capture. A tracked patch alone is incomplete.
- Mac actual result: **8,036/8,036 CTest executions passed**,
  0 failed, 0 skipped, 0 timed out;
  **8/8 examples**. The frozen cleanup matrix passed on its first invocation.
  The report retains the earlier unfrozen preparation mismatch and candidate history.

The fingerprint algorithm sorts relative paths by UTF-8 bytes and hashes
`relative_path + NUL + raw_file_SHA256_hex + newline`. Keep original Mac IDs,
runner hashes and every attempt. No earlier Windows/Mac result establishes a
pass of these new source bytes or of the restored pristine x86 behavior.

This handoff supersedes the candidate 04 instructions. Its original report and
checksums remain historical. The new source ZIP contains earlier documentation
as captured bytes: follow this updated handoff supplied separately, without
editing the frozen source to update its documentation or fingerprint. Obsolete
x86 branches were removed only from ARM-owned headers; protected originals stay
exact. Native Windows still needs its own actual results.

## Preserve the Windows checkout and establish identical inputs

Inspect branch/HEAD/tree, index, changes, worktrees, tools and evidence first.
Fetch `origin` without pruning, resets, stashes, merges, branch deletion or
configuration changes. Preserve old archives and the pristine testbench. Read
this handoff from the exact `arch-separation-m2` publication commit supplied in
the Windows prompt, not a moving branch tip.

### Git publication route

No transfer of ignored Mac files is required. Set `$approvedCommit` to the exact
commit in the prompt. Use a fresh detached worktree under ignored `build/`,
verify HEAD and a clean status, then compare all 474 Mac capture paths against
[the publication source inventory](arch-separation-publication-2026-11-18/source-comparison.json).
Runtime headers, tests, tools, fixtures, dependencies and build files must match
exactly. Listed documentation/metadata differences and added report files are
intentional; they do not mean the Mac ran the publication commit. Stop on any
unlisted or build-affecting difference.

Compare raw worktree hashes AND Git blob bytes. If Windows checkout settings
change an older text file's line endings, preserve its raw hash and prove the
specific LF-to-CRLF transformation from the exact Git blob. Do not silently
normalize source or replace the original Mac inventory. Protected root originals,
vendor files and pin inventory always require exact raw bytes, with no newline
exception. Record Windows source identity separately from the Mac fingerprint.

After this comparison, capture the full published checkout into a fresh ignored
location. This is a new published-source snapshot, not the original Mac snapshot.
From the fresh worktree, with `$captureOutput` set to a new short build path:

```powershell
if ((git rev-parse HEAD).Trim() -ne $approvedCommit) { throw 'Wrong source commit' }
if (git status --porcelain) { throw 'Preserve changes; use a clean detached worktree' }
python -B tools/capture_source.py --output $captureOutput
if ($LASTEXITCODE -ne 0) { throw 'Published source capture failed' }
$sourceRoot = Join-Path $captureOutput 'source'
$captureManifest = Join-Path $captureOutput 'capture.json'
Set-Location -LiteralPath $sourceRoot
python -B tools/capture_source.py --checkout $sourceRoot --verify-manifest $captureManifest
if ($LASTEXITCODE -ne 0) { throw 'Source capture did not verify' }
```

Keep the new complete fingerprint, base commit/tree, raw inventories and source
archive. Build only `$sourceRoot` using this `$captureManifest` for every profile.
No fake Git HEAD, ignored changes or source commit is needed to satisfy a guard.
Read instructions from the reviewed publication, while preserving the snapshot.

### Optional exact Mac ZIP route

The original Mac capture remains available at
`build/arm-header-cleanup-20261118T213003Z/candidate-01/captured-source.zip`,
SHA256 `3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`.
Transfer it and its receipt separately if an exact 474-file Mac snapshot is
wanted. Verify its SHA256 before extracting into a new ignored directory.
Set `$sourceRoot` to its `source` directory and `$captureManifest` to its adjacent
`capture.json`, then run the same verification command above. This route must
retain fingerprint `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
The ZIP's earlier documentation is historical; use this separately supplied
updated handoff. Preserve every original ID and source byte.

## Protect originals and verify the native environment

Never edit, format, normalize, patch or regenerate `vendor/vectorclass/`,
`vendor/vectorclass.json`, or any of the 28 original root paths. Their pin stays
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Record exact raw hashes and pin bytes
before/after every phase. The alias `sse2neon.h` is protected exact dependency
bytes too, but must never enter the x86 include graph.

Use native x64 Windows on the physical AMD host, an installed x64 Visual Studio
developer environment, native x64 Python 3.9+, CMake/CTest, Ninja, MSVC and
GNU-driver Clang. Verify CPU/OS/PE execution and CPU plus OS/XCR0 support before
higher ISA execution. Inspect existing tools; do not install/change machine
settings, PowerShell execution policy or remotes. No WSL, Rosetta/QEMU, ASan or
hosted CI. Read-only checks from the frozen source root:

```powershell
python -B tools/regenerate_headers.py --check
python -B tools/check_repository.py
python -B tools/check_header_generation.py
python -B tools/check_source_capture.py
```

`--write` is retired and must reject without changing files. Integrity self-tests
mutate disposable `build/` copies only. Any unexpected vendor/root/pin mismatch
is a stop condition: retain evidence, do not change expected hashes to accept it.

## Baseline native matrix: MSVC first, then Clang

Set `$testbench` to the existing clean pinned checkout at
`a7293702385e5169a5f58b83177f2a43cd59139b`. Verify revision, cleanliness and all
three hashes recorded by the native runner. Do not overwrite a dependency.
The external sources stay pristine; compile their generated include-adapted
copies under `build/`. Set `$clangCompiler` to the installed native GNU-driver
`clang++.exe` full path after checking its target/version. In the initialized
x64 developer shell, use the explicit
capture mode, never a fake Git HEAD or a source commit to satisfy a guard:

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

Capture each actual exit immediately; an exit zero alone is insufficient.
Collect each independent profile without changing source/gates after a failure.
The expected full registration is 1,348 cases per profile, 5,392 instances total,
plus four separate arithmetic examples. Reconcile names and actual results;
build failures or unavailable cases are not passes. MSVC Debug is unsanitized,
while Clang Debug must have strict nonrecovering UBSan and static release CRT.
Require its positive control and intentional signed-overflow diagnostic with no
continuation; keep actual exits. Keep original 60-second correctness deadlines,
concurrency three, seeds, reference fixtures, FP flags and page/canary controls.

## Supported higher ISA and facade isolation

After an independent native CPU/OS probe establishes support, run fresh local
AVX2 and AVX-512 CMake profiles with each compiler in Release/Debug. Do not use
the baseline runner's ISA-restricting flag validator as a higher-ISA pass.
Use the same frozen `$sourceRoot`, `VCL_BACKEND=1`, `VCL_BUILD_TESTS=ON`, examples,
`CMAKE_EXPORT_COMPILE_COMMANDS=ON`, and no testbench for this local selection.
Expected local inventory is 40 cases per profile (320 over eight profiles).
Preserve all inherited failures and registered names; no original file edits.

A configuration shape for each fresh directory is:

```powershell
cmake -S $sourceRoot -B $higherBuild -G Ninja `
  -DCMAKE_CXX_COMPILER=$compiler -DCMAKE_BUILD_TYPE=$mode `
  -DVCL_BACKEND=1 -DVCL_BUILD_TESTS=ON -DVCL_BUILD_EXAMPLES=ON `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DVCL_TESTBENCH_DIR= `
  -DVCL_SANITIZE=$sanitize -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded `
  "-DCMAKE_CXX_FLAGS=$isaFlags"
cmake --build $higherBuild --parallel 3 --verbose
ctest --test-dir $higherBuild --show-only=json-v1
ctest --test-dir $higherBuild --output-on-failure --no-tests=error `
  --timeout 60 --parallel 3 --output-junit results.xml
```

Select MSVC `/arch:AVX2` or `/arch:AVX512`; for GNU-driver Clang select
`-mavx2 -mfma` or `-mavx512f -mavx512vl -mavx512bw -mavx512dq -mfma`.
Use `sanitize=ON` only for Clang Debug; set/record
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Keep ordinary MSVC `/fp:strict`
and classification-only `/fp:fast`; keep Clang no-contraction/rounding settings.
Verify independent runtime/guard/UBSan controls and actual x64 PE artifacts.
A missing CPU/OS capability is an explicit unexecuted gap, never emulated proof.

Compile the same facade-only consumer with both compilers and save include
traces (`/showIncludes` or `-H`). Prove it reaches the protected original runtime
and math headers, with no `_neon.h`, sse2neon, adapted repair or backend helper.
Compare its output and generated function code with a direct-original consumer
including the same original vector/math surface at the same flags/inputs. Use
independent expected values too: both originals agreeing on a known wrong answer
is not correctness. Keep object/disassembly records; do not infer equivalence
from instruction counts or collect timing samples. Test optional runtime queries
through `vcl::runtime`, preserving original x86 detector bytes.

## Interpret failures and return results

Restored original x86 behavior includes known limitations. Preserve every actual
boundary fault, UBSan diagnostic, numerical mismatch, unavailable profile and
compile failure, linked to the retained x86 findings where evidence supports it.
Do not pad tables, change invalid-conversion expectations, relax tolerances,
remove failing tests, silently reduce inventory or reapply retired fixes. No new
x86 repair/optimization task is authorized. Unexpected source integrity changes
stop execution; other failures remain explicit while collecting independent
selected profiles on unchanged frozen bytes.

Verify the complete capture and protected root/vendor/pin inventory after runs.
Preserve raw logs, commands/exits, flags, source inventories, actual output bits,
JUnit, binary hashes, generated input/probe sources and all failed attempts under
ignored `build/`. Verify archive CRCs/hashes and evidence coverage. Prepare a dated
report and portable checksums for review, retaining original Mac source IDs and
actual Windows source identity. Update current status accurately; distinguish
native Windows verification from a new cross-host numerical-parity comparison.

Do not commit, push, merge, create PRs, retire branches, clean evidence/worktrees,
contact upstream, change consumers/configuration or edit protected originals.
Finish with actual counts, source identity/comparison, include/byte integrity,
control results, failures, report/archive locations and remaining gaps. Present
concrete results before requesting any separate publication or source-fix approval.
