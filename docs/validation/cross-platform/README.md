# Native Windows/Mac SIMD comparison

This is an additional bounded output comparison, alongside the existing
[native suites](../native-workflow.md). It does not exhaust every API, input,
exception flag, ISA or compiler. Direct Neon remains experimental. Native
Windows AMD and native Mac M2 results are distinct; physical Intel is omitted.
Use manual execution only, without timing benchmarks, hosted CI or emulation.

## Prepare, review and freeze

The initial run ID is `simd-parity-20261116`, frozen at approved published commit
`bd7807e91f5888bc2ad708ca46c9305d166d6c4b`. The
[final Windows results](simd-parity-20261116/windows-amd/README.md) accompany the
approved result publication. The
[Mac final results](simd-parity-20261116/mac-m2/README.md) record a passing
8,036-case native matrix and 3,996 independent corpus reference passes. The
original comparison completed all 24 Release pairings and rejected eight Debug
pairings because of the frozen loader's UBSan exit-code assumption. The
[authorized validator follow-up](simd-parity-20261116/comparisons/mac-validator-02/README.md)
reran all eight successfully with a separately identified host-specific fix.
All 32 planned pairings now have successful comparisons, with zero unexpected
differences. Original captures, manifests and rejection records are unchanged.
The [published source-byte audit](../source-byte-audit-2026-11-16-bd7807e.md)
at `145c7f5` reconciles all 135 source paths: 43 identical hashes and 92 verified
LF-to-CRLF transformations, with no unexplained differences. The selected run
is complete; its dated attempts preserve their original preparation status.
Use a new run ID for later source checkpoints and a new attempt ID for each
attempt. Do not overwrite failures.

The initial harness publication and frozen comparison are complete. The old
Windows verification prompt is retired. Future captures require a newly agreed
shared frozen source and fresh run/attempt IDs; do not reuse the completed run
as a new execution. Its tested source remains bd7807e, separate from the earlier
fb54e3e documentation checkpoint and current master.

Use clean worktrees at the same frozen commit for both hosts. The capture
runner requires its full SHA via `--expected-commit`. Run everything under
ignored `build/` and verify source stability before copying portable files into
the repository. Later report-only commits retain the original tested HEAD/tree.
If code, corpus or policies change, select a new frozen source for both hosts.

## Corpus and contracts

- [Corpus](../../../tests/cross_platform/corpus.jsonl): canonical UTF-8/LF JSONL,
  stable IDs, fixed seed `1592594996`, hexadecimal IEEE/integer input and output
  words. New-path Git attributes preserve LF for hashes on both platforms.
- [Policies](../../../tests/cross_platform/policies.json): fixed before platform
  comparisons. Unknown policies, missing/duplicate IDs, invalid metadata,
  changed hashes and incomplete width inventories fail closed.
- [Generator](../../../tools/simd_corpus.py): exact integer/Fraction arithmetic,
  nearest-even IEEE rounding without double rounding, and existing independent
  Decimal exp/log/trig fixtures. `--write` is an explicit reviewed regeneration;
  the default command checks the committed corpus. No SIMD result is an oracle.
- [Capture source](../../../tests/cross_platform/capture.cpp): standalone CMake
  project, separate from existing CTest registrations and fixture gates.
- [Comparator](../../../tools/compare_simd_results.py): independently validates
  both complete profiles before an explicitly requested width comparison.

Selected i32/u32/i64/u64 operations include wrapping arithmetic, shifts, masks,
selection and movement. FP32/FP64 cases include raw movement of signed zeros,
subnormals, infinities and NaN payloads; classification; finite arithmetic,
normal/subnormal boundaries, seeded values, min/max/select, all partial counts
with store canaries, four rounding modes for integer conversion, FMA-sensitive
inputs and bounded exp/log/trig fixtures. FP32 reciprocal estimates cover
positive normal inputs away from underflow boundaries. The existing full suite
retains broader exceptional-value and approximation-boundary checks.

Exact cases compare all bits. Numerical cases keep their existing independent
reference gates. Pairwise limits are the sum of both independently applicable
error bounds, not a reused single-reference tolerance. Trig keeps the existing
reference-spacing floor and legacy non-FMA FP32 tangent gate. Each differing
lane is retained in the comparison output.

FMA-sensitive cases use separately rounded exact-rational references: one
rounding for fused backends/ISA paths, two for unfused paths. Only this declared
contract difference is classified as `documented_fma`; it is not called an exact
match. NaN payloads are preserved by raw movement/bitwise cases; no universal
NaN propagation or exception-flag equivalence is claimed.

Non-default rounding exposes another existing difference: some `roundi` paths
use current direction, others fixed nearest-even. Policy version 2 records the
source-derived selection and validates it exactly, retaining every input/mode.
Comparisons label differing lanes `documented_rounding`, not exact matches.
This corrects the preparation oracle's original universal-current assumption;
the original failed attempts remain in evidence. No library code or original
test gate was changed, and no numerical accuracy tolerance was increased.

Output words have the input element's bit width. `lt` stores integer 0/1,
`classify` stores flags 1=NaN, 2=infinity, 4=zero, 8=subnormal, 16=normal, and
`roundi` stores signed integer bits. Other rows store the result element bits.

## Check the harness

Use `python3` on macOS and substitute `python` on native Windows. Create the
ignored output directory first; comparator controls retain their temporary
inputs there, including on a fresh checkout.

```sh
python3 -c "from pathlib import Path; Path('build').mkdir(exist_ok=True)"
python3 -B tools/simd_corpus.py
python3 -B tools/check_simd_comparison.py
python3 -B tools/check_repository.py
```

Comparator controls cover positive references, corruption, duplicate/missing
cases, unknown policies, signed-zero changes, excess numerical error, source and
input-hash mismatches, guard controls, width scope and rejection of preparation
results as final evidence. Negative-control files remain under ignored `build/`.

## Windows execution after the approved freeze

Use an x64 Visual Studio developer shell and the installed native x64 GNU-driver
Clang path. First run the agreed optimized Release, strict Release and strict
Clang UBSan profiles with the [Windows runner](../windows-runner.md); retain their
registered inventories, fixtures, numerical gates, guard controls, strict UBSan
proof and 60-second deadlines. These are separate from the additional capture
cases below.

For each new capture, substitute the newly agreed commit and unique run/attempt IDs:

```powershell
python -B tools/run_simd_capture.py --run-id <new-run-id> --attempt-id win-msvc-release-01 --driver msvc --mode Release --isa sse2 --backend 1 --expected-commit <full-frozen-SHA>
```

Repeat for GNU-driver Clang Release and Clang Debug with `--sanitize`.
MSVC Debug/RTC captures are deferred until noninteractive reporting is validated;
the standalone capture runner has no validated popup handler. Report the
omitted profile and resulting comparison coverage explicitly. The completed
historical eight-profile Windows capture and 32 pairings retain their original
scope; a new reduced inventory does not repeat that checkpoint.
Use `--compiler` for the actual native Clang executable. Additionally run
`--isa avx2` and `--isa avx512` Release captures for each compiler when supported.
A separately compiled baseline runtime detector checks CPU and OS support
before any higher-ISA capture binary executes. Unsupported profiles fail with
an explicit gap; they are not silently skipped or counted as passing.

The separate corpus runner retains strict FP flags; it does not inherit the
full-suite runner's optimized profile. It uses no implicit contraction, native
PE checks, writable/four inaccessible-page controls at the actual page size, and explicit
UBSan positive/negative controls when requested. Clang UBSan uses static release
CRT even in Debug. MSVC Debug is not UBSan. Every capture has a 60-second limit;
build concurrency is three. No original suite deadline is changed.

`--preparation` permits dirty-source development checks. They record patches,
added files and source fingerprints, are labeled preparation, and are rejected
by the final comparator unless explicitly enabled for development inspection.
They do not satisfy the final frozen-source run requirement.

## Mac execution at the same frozen commit

Follow the [current native workflow](../native-workflow.md) at the newly
agreed frozen source. Run the complete
existing native matrix with the pinned testbench first. Then capture Release
and strict UBSan Debug for compatibility backend 2 at maximum width 512 and
Direct Neon backend 3 at maxima 128, 256 and 512:

```sh
python3 -B tools/run_simd_capture.py --run-id <new-run-id> --attempt-id mac-compat-release-01 --driver apple-clang --compiler "$(xcrun --find clang++)" --mode Release --isa neon --backend 2 --max-width 512 --expected-commit <full-frozen-SHA>
```

Use `--backend 3 --max-width 128`, `256` or `512` for Direct Neon. Use
`--mode Debug --sanitize` for each Debug capture. The native Mac runner controls
prove arm64 Mach-O execution, runtime consumers, guard faults and strict UBSan.
The capture runner requires native M2 Python, Apple Clang, CMake/CTest and Ninja;
it rejects Rosetta. Do not retry the documented ASan startup hang.

## Portable results and comparison

Each invocation creates a fresh `build/simd-capture-<UTC>-<profile>/`. Raw journals,
patches, manifests, command lines, executable hashes, controls and results stay
there. `return-evidence.zip` is created before its portable `bundle/`, avoiding
self-referential checksums. The portable manifest records its name and SHA256.
Full native suite archives are retained separately and summarized alongside the
capture bundles in each host's final report.

After all runs and final source checks, copy each complete `bundle/` into:

```text
docs/validation/cross-platform/<run-id>/<windows-amd-or-mac-m2>/<attempt-id>/
```

Each bundle contains `manifest.json`, `summary.md`, `outputs-<profile>.jsonl` and
`checksums.sha256`. Metadata includes source commit/tree and cleanliness,
corpus/policy/input hashes, seed, native host/tools, compiler/ISA/backend/width,
FP flags, rounding/FTZ/DAZ, CRT, sanitizer, controls, page size, exact case/lane
counts, binary hash and raw archive receipt. It contains no credentials,
absolute machine paths or binaries. Full raw compile/link commands stay in the
checksummed raw packet; portable metadata lists relevant FP/ISA/sanitizer flags.

After both hosts finish, compare each selected Windows compiler/ISA/mode with
both corresponding ARM backends and widths. Specify the width scope explicitly:

```sh
python3 -B tools/compare_simd_results.py <windows-bundle> <mac-bundle> --width 128 --width 256 --width 512 --output build/comparison-unique-attempt.json
```

For a Direct Neon maximum-128 profile, request only `--width 128`; for maximum
256 request 128 and 256. Each input profile is still validated in full. Report
the complete pairing inventory and any missing profiles; a narrow successful
comparison is not full cross-platform coverage. The comparator records excluded
widths and classifies exact, numerical, documented FMA/rounding and unexpected differences.
It refuses mismatched source commits/trees, corpus/policy/input hashes and run IDs.

At the frozen `bd7807e` checkpoint, Apple UBSan's intentional-overflow probe terminated
with `SIGABRT` (`-6`) after its expected diagnostic and without continuation.
The native runner accepted this strict termination, but that version of
`load_bundle` required exit code `1` for strict UBSan captures. The four Mac
Debug bundles retain reference passes and the original workflow rejection together.
Do not change their exit codes, loosen the frozen loader or label rejected
comparisons as passes.
The maintainer subsequently authorized fixing the offline validator and rerunning
these eight comparisons against the existing captures. The correction accepts
Mac exit `1` or `SIGABRT` (`-6`) while retaining Windows exit `1`, positive
controls, reference gates and all original data. Its exact code/test patch and
new results are in the [follow-up](../mac-arm-2026-11-16-bd7807e-validator-followup.md),
published with the correction at `99f4a3d`.
This is separately versioned reanalysis, not native execution at a new source.
Future native captures continue to require an approved shared frozen checkpoint.

Commit portable bundles, per-host summaries and reviewed comparison JSON only
with approval. Preserve original tested source identifiers and all attempts.
