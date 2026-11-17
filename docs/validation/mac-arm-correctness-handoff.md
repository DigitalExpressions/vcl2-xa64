# Native M2 handoff after Windows correctness fixes

Prepared 2026-11-17. This is an execution instruction, not a native M2 result.
The maintainer approved cleanup, commit and push of this source and handoff on
2026-11-17. The commit containing this handoff version is the approved M2 source.
This approval does not authorize publishing later Mac results or further fixes.
The older Mac result published at `3abf7af` tested `a099098`; it does not cover
these fixes.

Read `AGENTS.md`, [the completion plan](../windows-correctness-plan.md),
[Windows results](windows-amd-2026-11-17-correctness-completion.md),
[native workflow](native-workflow.md) and [report template](report-template.md).
Preserve branches, unrelated changes, original failures and all evidence.
Do not commit, push, merge, create PRs or clean evidence without approval.

## Pristine upstream is mandatory

**Never modify `vendor/vectorclass/`, including on the Mac M2.** Its 28 files
must remain byte-exact to Agner Fog's original VCL at
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. Do not format, normalize line endings,
regenerate into this directory or change its pin/hash inventory to accept a
failure. Verify before and after execution. On a mismatch, retain evidence and
stop. Fixes belong in canonical adaptation patches or backend sources, followed
by root-header regeneration and a newly approved source checkpoint.
Generation self-tests may corrupt their isolated copies under `build/` only.

## Freeze the approved publication

Prefer the exact full publication SHA supplied by Windows. When only this
handoff path is supplied, resolve its publication commit from fetched origin
as below, then compare its source with the recorded Windows inventory. Do not
reuse the earlier `a099098` checkpoint or use `e27134b` alone: Windows tested
that base plus captured uncommitted fixes.
The published source must include patches 0003 and 0004, the new regression
headers, canonical x86 helpers and generated products.

From the primary Mac checkout (or set `M2_VALIDATION_COMMIT` to the exact full
SHA supplied by Windows instead of the file-history lookup):

```sh
git status --short --branch
git worktree list
git fetch origin
M2_VALIDATION_COMMIT=$(git log -1 --format=%H origin/master -- docs/validation/mac-arm-correctness-handoff.md)
git show --no-patch --format=fuller "$M2_VALIDATION_COMMIT"
M2_PRIMARY_ROOT=$(git rev-parse --show-toplevel)
M2_TESTBENCH_DIR="$M2_PRIMARY_ROOT/build/_deps/testbench"
M2_SHORT_COMMIT=$(git rev-parse --short=12 "$M2_VALIDATION_COMMIT")
mkdir -p build
git worktree add --detach "build/mac-m2-correctness-$M2_SHORT_COMMIT" "$M2_VALIDATION_COMMIT"
cd "build/mac-m2-correctness-$M2_SHORT_COMMIT"
mkdir -p build
```

Require every prerequisite to succeed. Never overwrite an existing worktree;
inspect it and choose a fresh attempt suffix. Capture full HEAD/tree, native
arm64/Rosetta status, tool versions, original and final inventories, raw hashes,
flags and binary architecture. Keep the worktree frozen throughout execution.

Compare non-documentation source with `code_files` in
[Windows source metadata](windows-correctness-2026-11-17/source.json) before
running. For each listed relative path, require the recorded raw SHA256 or an
explicitly recorded, reproducible CRLF-to-LF conversion matching its
`lf_normalized_sha256`. Retain both raw hashes and the transformation for every
different path. Vendor files must match the raw hash exactly. Do not assume
line endings explain a mismatch. Record new publication/documentation paths
separately; do not replace the original Windows source inventory.
When using portable hashes alone, reconstruct the Windows bytes from the Mac
bytes by the stated LF/CRLF transformation and require the result to match the
recorded Windows raw hash. If this cannot be demonstrated, obtain the retained
source bytes and investigate; a different raw hash alone is not an explanation.

The original Windows fingerprint is
`af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528`.
This includes its original documentation and raw Windows file bytes. A clean
publication/Mac checkout can have a different whole-source fingerprint; explain
the actual per-file differences instead of claiming the fingerprints match.
Any other code difference requires review before execution.

## Native execution and controls

Use native arm64 Python 3.9+, Apple Clang, CMake/CTest and Ninja or make. Reuse
the clean testbench at `a7293702385e5169a5f58b83177f2a43cd59139b`; inspect an
existing checkout rather than overwriting it. If absent, fetch it with the
maintained script. Keep the Mac awake while running.

```sh
if [ ! -e "$M2_TESTBENCH_DIR" ]; then
  sh tools/fetch_testbench.sh "$M2_TESTBENCH_DIR"
fi
M2_AUDIT_DIR=$(mktemp -d build/mac-m2-correctness-audit.XXXXXX)
python3 -B tools/regenerate_headers.py --check --json "$M2_AUDIT_DIR/before.json"
git diff --exit-code "$M2_VALIDATION_COMMIT" -- vendor/vectorclass/ vendor/vectorclass.json
python3 -B tools/check_repository.py
python3 -B tools/check_header_generation.py
python3 -B tools/run_native_arm_validation.py --plan
python3 -B tools/run_native_arm_validation.py \
  --expected-commit "$M2_VALIDATION_COMMIT" \
  --testbench-dir "$M2_TESTBENCH_DIR"
```

Require native compatibility maximum 512 and experimental Direct Neon maxima
128/256/512, each Release and strict nonrecovering UBSan Debug: eight profiles,
8,036 CTest instances and eight example executions. Reconcile actual complete
registered inventories; preserve 60-second case deadlines and concurrency
three. The added boundary/conversion assertions run inside existing edge cases;
they do not reduce or replace the matrix.

Keep fixtures, seeds, literal conversion expectations, rounding modes, NaN and
signed-zero gates, exact-page boundaries/canaries and FP flags intact. Check
positive runtime/guard controls and all four intentional boundary faults using
the actual macOS page size. Require a passing UBSan positive and an intentional
overflow diagnostic with no continuation; Apple's expected negative exit can
be `1` or SIGABRT (`-6`), and the actual exit must be retained. Inspect arm64
Mach-O binaries. MSVC Debug results are separate from these UBSan checks.
Do not run Rosetta, QEMU/WSL, Intel, ASan or benchmark timings in this phase.

Even if native execution fails, retain its actual exit and run read-only checks:

```sh
python3 -B tools/regenerate_headers.py --check --json "$M2_AUDIT_DIR/after.json"
git diff --exit-code "$M2_VALIDATION_COMMIT" -- vendor/vectorclass/ vendor/vectorclass.json
git status --short --branch
```

## Results and subsequent performance gate

Save raw command journals, compiler/flag inventories, source manifests, binary
hashes, controls, JUnit, full logs and original failed attempts under ignored
`build/`. Verify compact archive checksums and supplement any omitted generated
probe/reference sources before considering future cleanup. Prepare a new dated
portable report/data bundle with relative paths only, using the shared template;
retain actual failures and compare source with Windows. Update the validation
index and integration plan accurately. Publication requires fresh approval.

If ARM runtime changes are needed, preserve the failing snapshot, leave vendor
untouched, fix through the canonical layer and arrange renewed Windows checks
on the new source. Do not carry passes to modified code.

After both hosts validate the same approved code, the separate
[Windows upstream comparison](../benchmarks/x86-upstream-comparison.md) can
measure the final fork against original Agner Fog VCL at its recorded pin.
Use a new source/run ID; match flags and workloads, retain all slower/unstable
rows and assembly provenance. Earlier optimized measurements do not establish
the performance of these newer correctness fixes. This handoff does not claim
a new cross-host corpus parity comparison or universal bitwise identity.
