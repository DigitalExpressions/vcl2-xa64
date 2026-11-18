# Native M2 ARM header cleanup — 2026-11-18

## Actual outcome

**8,036/8,036 native CTest instances passed, plus 8/8 examples.** Zero failed,
skipped or timed-out cases. All eight profiles ran on a new frozen uncommitted
capture after removing obsolete x86 code from nine ARM headers. This result
belongs to cleanup candidate 01; candidate 04's earlier layout result remains
[unchanged historical evidence](mac-arm-2026-11-18-arch-separation.md).

## Cleanup and preserved behavior

The [source audit](arm-header-cleanup-audit-2026-11-18.md) details the removal:
MSVC x86 exponential/inlining, AVX2 power and trig workarounds; dead x86
classification helpers/assembly; inactive AVX/AVX-512/XOP/FMA branches; native
wide-vector constructors and x86 detection/compiler fixes. Net reduction:
**3,456 lines across nine headers**. Original attribution remains.

Compatibility's SSE-shaped operations still map through unchanged sse2neon to
ARM. They are required implementation APIs. ARM classification barriers,
bounded/partial access, defined conversions/division, NaNs/signed zeros,
range reduction, compiler portability and Direct Neon FMA choices remain.
The Direct core headers are byte-identical to the previous candidate.
Compatibility stays default and Direct Neon requires experimental opt-in.

All **18/18** native full-facade preprocessing comparisons produced identical
C++ token streams and byte-identical preprocessed outputs before/after: both backends, widths 128/256/512,
ordinary/finite-math modes, with Direct Neon checked both without INSTRSET
and with legacy INSTRSET=6. See [comparison hashes](arm-header-cleanup-m2-2026-11-18/preprocessing.json).
The [scope check](arm-header-cleanup-m2-2026-11-18/scope-preservation.json) proves
all tests, numerical gates, fixtures/generators, seeds, FP flags, runners,
policies and neutral selection remained byte-identical to the pre-cleanup input.

Read-only integrity checks now reject inactive x86 backend/helper references,
high ISA macros and native wide intrinsics/types in ARM sources. Three negative
controls demonstrate rejection in disposable copies; all **11** integrity
controls passed. Existing root/vendor/newline/writer protections remain.

## Exact source and provenance

- Approved documentation/base commit: `1a5226446f14e1f4d26216ae688b40ccc9b0fa03`.
- Base tree: `e5b8579b6292cce2ea6a67b8cdf8027ec228d7bc`; **not the tested implementation tree**.
- Tested source: complete frozen uncommitted capture, **474 files**;
  tested commit/tree are null. Builds used only this immutable captured copy.
- Fingerprint: `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
- Source ZIP SHA256: `3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`.
- Tracked patch SHA256: `f83b4c2cbb527be2c043c86f65ff7c84c7dc91b892f3ba754236bf3ea9548a3a`.
  Added-file bytes accompany the patch in the source ZIP; the patch alone is incomplete.
- Run ID: `arm-header-cleanup-20261118T213003Z-candidate01`.
- Controller UTC interval: `2026-11-18T21:37:21.576212+00:00` to `2026-11-18T22:40:56.050236+00:00`.

The [complete manifest](arm-header-cleanup-m2-2026-11-18/capture.json) sorts
relative paths by UTF-8 bytes; fingerprint input is path, NUL, raw SHA256 hex and
newline. Manifest, every captured source path, source ZIP and patch were
verified before/after. Captured runner and external controller/validator hashes
are [recorded separately](arm-header-cleanup-m2-2026-11-18/runner-hashes.json).
Default dirty-source validation still rejects; no clean-state spoof or commit
was used. The prior candidate's identifiers and original Windows records remain
unchanged. There is no new same-source Windows pass yet.

## Host and matrix

Native Apple M2 / Mac14,3, macOS 26.7 (25G229), Apple Clang 17.0.0,
arm64 compiler target, SDK 26.1, native Python 3.9.6,
CMake/CTest 4.4.4 and retained Ninja. Tools contain arm64 slices;
Python/binaries executed natively, with Rosetta query zero. Actual page size:
**16,384 bytes**. Pinned testbench remained clean at
`a7293702385e5169a5f58b83177f2a43cd59139b`; all three CPP hashes verified.
No tool installation or dependency overwrite occurred. Full details:
[host](arm-header-cleanup-m2-2026-11-18/host.json),
[flags](arm-header-cleanup-m2-2026-11-18/flags.json),
[controls](arm-header-cleanup-m2-2026-11-18/controls.json).

| Profile | Registered | Passed | Failed | Skipped | Timed out |
| --- | ---: | ---: | ---: | ---: | ---: |
| compat512-release | 1350 | 1350 | 0 | 0 | 0 |
| compat512-debug | 1350 | 1350 | 0 | 0 | 0 |
| neon128-release | 486 | 486 | 0 | 0 | 0 |
| neon128-debug | 486 | 486 | 0 | 0 | 0 |
| neon256-release | 890 | 890 | 0 | 0 | 0 |
| neon256-debug | 890 | 890 | 0 | 0 | 0 |
| neon512-release | 1292 | 1292 | 0 | 0 | 0 |
| neon512-debug | 1292 | 1292 | 0 | 0 | 0 |
| **Total** | **8036** | **8036** | **0** | **0** | **0** |

Each Debug profile uses strict nonrecovering UBSan. All case deadlines remain
**60 seconds**, concurrency **three**. Complete registered/JUnit names match
without intersection filtering or dropped cases. Eight separate examples
printed `2 5 10 17`. The native UBSan positive control exited zero, deliberate
overflow exited **-6**, with its required diagnostic and no
continuation. Guard positive exited zero; all four deliberate boundary faults
exited **-10**. Runtime reported
`arm64=1 neon=1 instrset_detect=0 pointer_bits=64 long_bits=64`. All **7,999** executable records were
rehashed and verified arm64 Mach-O. Installed-header consumers and include traces
passed for compatibility 512 and Direct Neon 128/256/512; ARM never included
protected originals or inactive backend sources. Six target rejection controls passed.

## Integrity and attempts

All **28 protected root originals**, **28 vendor files** and
`vendor/vectorclass.json` remained exact raw bytes, with protected hashes and
mtimes unchanged. Pin: `667577495efaa36a10cbb5d6cb8c942c23913848`, vendor tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. The sse2neon alias/dependency remain
exact. No originals or inventory were edited, normalized or regenerated.

The frozen native matrix passed in one attempt, with no profile retries.
Before freezing, an initial temporary reducer draft mishandled a conditional
inside a C++ comment; the preprocessing comparison caught the token mismatch.
That draft/output is retained. Corrected comment-aware pruning then passed all
18 comparisons before any frozen native run. This preparation failure is
recorded in [attempts](arm-header-cleanup-m2-2026-11-18/attempts.json).
Previous candidate 04 and its earlier migration failures remain separate.

## Evidence and next use

- [Portable bundle and checksums](arm-header-cleanup-m2-2026-11-18/README.md):
  complete cases/results, source/runner identities, native flags/controls,
  executable hashes, pruning decisions, preprocessing and integrity records.
- Frozen source: `build/arm-header-cleanup-20261118T213003Z/candidate-01/source/`.
- Exact transferable source ZIP: `build/arm-header-cleanup-20261118T213003Z/candidate-01/captured-source.zip`.
- Raw native run: `build/arm-header-cleanup-20261118T213003Z/candidate-01/source/build/mac-m2-native-20261118T213729896840Z`.
- Complete raw packet: `build/arm-header-cleanup-20261118T213003Z/arm-header-cleanup-evidence.zip`;
  SHA256 `ffcefcd5b32984c94a4f5f68681b40bb3c87936a34460ed20cb67fa6414dfe55`. Every entry is checked against its inventory.
  It includes captures/patches, failed preparation, preprocessing inputs/outputs,
  full logs/JUnit, generated reference/configuration sources, flags/controls and
  binary hashes. Original directories and binaries are retained; nothing cleaned.
- [Updated Windows return handoff](windows-arch-separation-handoff.md) selects
  this cleanup capture. Frozen source contains older historical documentation;
  follow the updated handoff supplied separately for the new source identity.

Source/results remain uncommitted on `arch-separation-m2` in the implementation
worktree; primary master and remotes remain unchanged. Reports, handoff and
current-status links prepared after execution are metadata outside the frozen
capture. They do not acquire a new source identity or test pass.

Remaining gates: native AMD Windows on this exact implementation, original/facade
x86 include/output/code comparison, other ARM compilers/hosts, physical Intel,
ARM64EC (rejected), native FP16/SVML, full FP exception emulation, universal-binary
build and external consumers. Direct Neon stays experimental. No benchmark,
ASan, translated/emulated tests, x86 repairs, commit/push/merge/PR, branch retirement
or evidence cleanup was performed. This is selected correctness validation,
not universal cross-host bitwise identity or a performance result.
