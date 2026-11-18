# ARM header cleanup audit — 2026-11-18

The architecture separation candidate 04 isolated the include graphs but still
carried inactive x86 implementations inside the ARM copies. The maintainer
requested their removal. This follow-up removes obsolete x86 branches without
changing the selected ARM algorithms or the compatibility default.

## Removed

- The old MSVC x86 `exp_f` force-inline workaround, AVX2 composed `pow`
  workaround and conditional-add workaround in trigonometric code.
- Native x86 classification fallback assembly and `vcl_detail_x86` calls.
- Inactive AVX/AVX2/AVX-512, XOP and x86 FMA branches in the compatibility,
  conversion, wide-composition and math headers, including native 512-bit
  constructors that cannot apply to the ARM composition types.
- x86 target normalization, instruction-set autodetection, CPUID assembly,
  AVX compiler warnings and the Clang 6 x86 mask-register workaround in the
  compatibility instruction helpers. ARM CPUID-shaped queries still return zero.
- Comments that described removed x86 workarounds or nonexistent ARM AVX paths.

Nine ARM headers change, removing 3,456 lines net. Original attribution and
licenses remain. Protected root originals, vendor files and pin inventory are
outside this cleanup; historical x86 research patches remain inactive assets.

## Retained because ARM uses them

The compatibility backend uses SSE-shaped `_mm_*` operations through the
unchanged sse2neon dependency. These names are its ARM implementation interface;
they are required, even though their spelling comes from SSE. Broad masks,
portable split-word operations and lower-level permutation choices remain.
Compiler portability for the compatibility vector aliases and MSVC ARM bit-scan
intrinsics remains. These are separate from the removed x86 ISA workarounds.

Keep bounded/partial memory access, defined integer operations and conversions,
NaN/finite classification barriers, signed-zero behavior and trigonometric
range handling. Keep Direct Neon's explicit FMA choice and compatibility's
existing arithmetic choices. Direct core headers are byte-identical to candidate
04. No numerical gates, fixtures, seeds, FP flags, UBSan/guard controls, case
limits, registered test selections or backend defaults change.

## Verification method

Before freezing, native Apple Clang preprocessing compares the full facade
before/after across both backends, widths 128/256/512 and ordinary/finite-math
modes. Direct Neon is checked both without `INSTRSET` and with the legacy
`INSTRSET=6` test define: **18/18 identical token streams**. Both inputs and
outputs, compiler commands, hashes and the cleanup diff are retained under
ignored `build/`. Native include traces and include-only installed consumers
pass for compatibility 512 and Direct Neon 128/256/512.

An initial unfrozen preparation comparison exposed a comment-handling bug in
the temporary conditional reducer. Its files and comparison output are kept.
The reducer was corrected to ignore directives inside C++ comments before the
18 passing comparisons. No frozen test or native matrix used that draft.

`tools/regenerate_headers.py --check` now also rejects old x86 backend/helper
references, native AVX/XOP/FMA macros, native wide intrinsics/types and high-ISA
selection in ARM source, including inactive branches. Integrity controls inject
three representative failures into disposable build copies and require rejection.
The protected-source and retired-writer checks remain intact.

Fresh full native validation is required at a new complete source capture.
Candidate 04's earlier results remain tied to its original fingerprint; the
follow-up dated report must identify its own capture and actual results.
Native Windows verification remains a separate gate. No timings are collected.
