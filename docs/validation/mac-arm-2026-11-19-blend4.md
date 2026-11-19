# Native M2 Direct Neon transpose blends — 19 November 2026

**8,036/8,036 native CTest cases and 8/8 examples passed** on a new complete
frozen source capture. Zero failed, skipped or timed-out cases; no profile/test
retries. The recommended four-pattern task from the
[proposal](../arm-next-step-proposal-2026-11-19.md) was authorized after its
planning checkpoint. Compatibility remains default; Direct Neon remains
explicitly experimental.

## Change and exact contract

Only two tracked implementation paths changed:
[vectori128_perm_neon.h](../../vectori128_perm_neon.h) adds four narrowly matched
Direct Neon blend branches; [movement_slice.cpp](../../tests/movement_slice.cpp)
adds a targeted regression within its existing registration. For four 32-bit
lanes the patterns select exactly `a0,a1,b0,b1`, `a2,a3,b2,b3`, `a0,a2,b0,b2`
and `a1,a3,b1,b3`. Typed integer half-register combines/unzip operations replace
the general byte-table path for these tuples. Existing index validation,
broadcast/two-lane paths and the fallback for every other tuple stay intact.

The regression checks `Vec4f`, `Vec4i` and `Vec4ui`, both source orders, every
specified tuple, fallback zeroing/don't-care tuples and rotating edge payloads:
both signed zeros, subnormals, finite extremes, infinities and signed quiet/
signaling NaNs. Its independent scalar oracle indexes raw integer bits. Selected
lanes retain every bit; zeroing lanes are positive zero; don't-care lanes remain
unconstrained. No FP arithmetic, rounding, conversion or FMA is introduced.
Native checks preserve FPCR and FPSR across 16 rounding/FTZ/default-NaN settings,
each with initial sticky flags zero or `0x9f`, and restore the caller's state.
The original regression seed and all existing random-case ordering remain intact.

Both backends passed the expanded movement test in separate Release/strict
UBSan Debug checks before the matrix. The same new test also passed against the
immutable baseline headers. This is a behavior-preserving specialization, not a
repair to an existing numerical failure. All API/ABI/namespace/width settings,
stronger ARM access/conversion guarantees, fixtures, gates, runners and build
files remain byte-identical. All other 562 base paths are raw-byte identical.

## Frozen identity

- Working base commit: `5e039b01b49abc00548123763be85898932e3ad8`; base tree:
  `ea90c1b40fc50936db7c3509aaa11ab51c02d890`. These identify the base, not the tested implementation.
- Tested implementation: complete **571-file frozen uncommitted capture**;
  tested commit/tree null. Fingerprint:
  `3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36`.
- Source ZIP SHA256: `9883e0da75711f61d5bd5ceb3b7e2c86152b2b2975cebb897a95c179679d213c`.
- Patch SHA256: `6ef131ac0f37551994e0b9a8ac4756b1f497bf0ca56c1e5e9ed9a403020e2663`. The patch contains the two tracked
  changes; all seven added planning/preflight metadata paths are retained in
  the source ZIP and manifest. No deleted source path.
- Builds used only the immutable captured source. Every captured path and the
  complete fingerprint verified before and after; editable implementation bytes
  still match the capture. Documentation prepared after execution remains
  outside that frozen identity. No source commit was created to bypass a guard.
- Matrix controller: `2026-11-19T09:24:23.535340+00:00` to
  `2026-11-19T10:26:01.077288+00:00`, actual exit zero.

## Native validation

Native Apple M2, macOS 26.7 (25G229), Apple Clang 17.0.0 target arm64,
SDK 26.1, native arm64 Python 3.9.6, CMake/CTest 4.4.4, retained native-slice
Ninja 1.11.1. Rosetta query zero; actual page size 16,384 bytes. Existing tools
and pristine testbench `a7293702385e5169a5f58b83177f2a43cd59139b` were reused.
Its cleanliness and all three pinned hashes verified before/after.

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

Every registered name matched the complete prior M2 baseline inventory and its
new JUnit result. No intersection-only comparison, missing executable, removed
case or weakened tolerance was accepted. All eight examples returned `2 5 10 17`.
All 7,999 executable records were rehashed and verified as arm64 Mach-O.
Ordinary builds retained no implicit contraction and selected rounding flags;
finite-math stress stayed classification-only. All Debug profiles used strict
nonrecovering UBSan with halt-on-error. Every case deadline remained 60 seconds;
concurrency remained three. No ASan, emulation, hosted CI or timings ran.

The UBSan positive control exited 0; deliberate signed
overflow exited **-6**, with the required diagnostic and
no continuation. Guard positive exited 0; all four
read/write boundary controls exited **-10**.
Runtime reported `arm64=1 neon=1 instrset_detect=0 pointer_bits=64 long_bits=64`.
Four full-facade include traces and installed-header namespace consumers passed
(compatibility 512, Direct 128/256/512), with no original x86 header in ARM traces.
Six target rejection controls passed. Read-only maintenance, repository/link,
whitespace and all 11 header-integrity controls passed; disposable negative
inputs and actual expected rejection exits remain retained.

All 28 original root files, all 28 vendor originals and pin/dependency inventory
remain exact raw bytes, including line endings. Upstream pin remains
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. All 60 protected/dependency paths
retain their raw hashes and nanosecond mtimes in primary, editable, baseline and
candidate sources. Neutral policy, pristine x86 include graph and dependencies
were not edited.

## Code generation and limits

Fresh native optimized assembly of the unchanged full FP32 AoS transpose caller
retains eight table lookups at the immutable baseline. The frozen candidate uses
half-register lane movement/unzip operations with **zero table lookups** and
eliminates its four index-vector constants. The full before/after assemblies,
input/flag records and hashes remain retained. This provides evidence that the
new branch changes a real caller. It establishes no measured speedup: instruction
counts do not select a performance winner. No benchmark executable was timed.

The original 474-file M2 cleanup capture and its pass remain separate. Windows
publication `2d251bc` retains its failed/incomplete gate: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve examples. No new Windows execution or cross-host
parity is claimed. Other hosts/toolchains, physical Intel, native FP16/SVML,
ARM64EC, ASan, universal builds and application integration remain outside scope.
Performance measurement requires its own matched, authorized run after review.

## Evidence and review state

[Portable records and checksums](m2-blend4-2026-11-19/README.md) retain complete
source, cases, results, control exits, protected integrity and executable hashes.
Raw session root relative to the primary checkout:
`build/m2-blend4-20261119T092030Z-01/`. The frozen source/ZIP/manifest/patch/receipt are under
`candidate-01/`; the native run is
`candidate-01/source/build/mac-m2-native-20261119T092423584514Z/`; targeted checks and include consumers
are under `relevant-checks-01/`. Command/exit journals, all generated inputs,
raw logs, source/tool/dependency inventories and artifacts remain there.
The checksummed compact raw packet is `blend4-evidence.zip`, with a separate
per-entry inventory and receipt under the session root. It excludes compiled
payloads, which remain on disk with recorded hashes. No evidence was cleaned.

Implementation remains uncommitted in the retained detached planning worktree:
`build/m2-arm-planning-20261119T084235Z-01/checkout/`. Primary checkout, refs,
remotes, saved configuration, other worktrees and earlier planning records remain
unchanged. No commit, push, merge, PR, branch retirement, dependency update or
external consumer edit occurred. The next review is this bounded diff and native
result; publication and performance work remain separate scope.
