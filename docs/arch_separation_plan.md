# Plan: pristine upstream at root, separate ARM / Neon headers

**Approved implementation sequence, updated 18 November 2026.** The maintainer has agreed that
applications will use `#include "vcl2_xa64.h"` as the platform-neutral entry point.
Publish the reviewed documentation first, implement and validate on native
Mac M2, then verify the same frozen implementation on native AMD Windows.
The [Mac handoff](validation/mac-arch-separation-handoff.md) defines execution
scope and preservation controls. The initial `1a52264` publication contained
only documentation. The maintainer subsequently approved implementation/results
publication on `arch-separation-m2`, after native M2 validation and ARM cleanup.

The Mac implementation is now prepared for topic-branch publication. See the
[actual M2 report](validation/mac-arm-2026-11-18-arm-header-cleanup.md) and
[Windows return handoff](validation/windows-arch-separation-handoff.md) for its
exact captured source, retained attempts and pending cross-host verification.
The documentation-only publication described above remains the base checkpoint.

## Intended result

1. Agner Fog's original files occupy their original names in the repository
   root, byte-for-byte identical to the pinned upstream snapshot.
2. Native x86/x64 uses those originals. None of our ARM adaptations or old
   x86 correctness/performance patches enters that include path.
3. ARM64 uses a separately maintained header family ending in `_neon.h`.
   Future Neon changes never modify the protected original files.
4. Applications include the agreed fork-owned header, `vcl2_xa64.h`, on
   either architecture. It selects the implementation during preprocessing.
5. Ordinary SIMD vector operations keep their direct types and inline code;
   the facade adds no runtime architecture branch or per-operation wrapper.

The protected source is original VCL2 at
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. The current 28-file inventory in
`vendor/vectorclass.json` is the authority. This plan does not update upstream.
`vendor/sse2neon/` is a different dependency, not Agner Fog's source inventory.

## Proposed layout and public include

```text
repository root/
  vectorclass.h                  original upstream, protected
  vectori128.h                   original upstream, protected
  vectorf128.h                   original upstream, protected
  vectormath_common.h            original upstream, protected
  vectormath_exp.h                original upstream, protected
  ...                            all 28 original upstream paths

  vcl2_xa64.h                    new platform-neutral public entry point
  vectorclass_neon.h             ARM aggregate
  vectori128_neon.h              ARM implementation, existing name reconciled
  vectorf128_neon.h              ARM implementation, existing name reconciled
  vector_convert_neon.h          ARM conversions/composition
  vectori256e_neon.h             ARM logical wide-vector composition
  vectormath_common_neon.h       ARM math foundation
  vectormath_exp_neon.h          ARM math
  vectormath_trig_neon.h         ARM math
  ...                            remaining needed ARM counterparts

  vendor/vectorclass/            unchanged reference snapshot retained
  vendor/vectorclass.json        integrity inventory retained
  docs/                          fork guidance and ARM lessons
```

The consumer-facing change is:

```cpp
#include "vcl2_xa64.h"
```

The facade includes the original root vector and supported math headers on
x86/x64, and their `_neon.h` counterparts on ARM64. It should expose the common
supported vector/FP32/FP64 math surface needed by the application. Intel SVML,
unimplemented FP16 and other architecture-only APIs are outside that common
surface unless separately implemented and tested.

Existing ARM clients that directly include `vectorclass.h` or unsuffixed math
headers must migrate to the facade. Keeping those ARM includes working by
editing the original `vectorclass.h` would contradict the proposed separation.
Original upstream includes remain available to x86-only users.

## Architecture selection recommendation

Use **compile-time selection**, automatic from compiler target macros. Typical
ARM64 macros are `__aarch64__`, `__arm64__` and `_M_ARM64`; x86/x64 targets have
their corresponding compiler macros. This detects the compilation target,
including cross-compilation, rather than the machine running the build.

An explicit project define can select a supported ARM implementation or test
configuration. Validate it against the actual compiler target; do not let an
override pretend an x86 compilation can emit a Neon implementation. Unsupported
targets should produce a clear compile error. ARM64EC needs explicit treatment
or rejection because MSVC also defines x64 target macros for it.

The preprocessor includes one implementation, so no runtime architecture test
is inserted into SIMD calls. Runtime SSE/AVX feature dispatch within x86 is a
separate optional issue and can continue using untouched upstream facilities.
A universal macOS binary still compiles separate architecture slices; the
facade selects correctly while compiling each slice.

References: [GCC preprocessor conditionals](https://gcc.gnu.org/onlinedocs/cpp/Ifdef.html),
[Microsoft target macros](https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170),
and [Apple universal-binary guidance](https://developer.apple.com/documentation/apple-silicon/building-a-universal-macos-binary).

## Details that make this more than a bulk rename

**Existing names collide.** `vectori128_neon.h`, `vectorf128_neon.h` and
`instrset_neon.h` already contain Direct Neon implementations. Renaming the
current generic files onto those paths would overwrite distinct implementations.
Reconcile these explicitly. If compatibility remains available during the move,
its private counterparts can use names such as `vectori128_compat_neon.h` and
`instrset_compat_neon.h`, preserving the `_neon.h` suffix without collisions.
Use the same suffix for ARM-owned helpers, for example
`vectori128_div_neon.h` and `vectorf128_helpers_neon.h`. Neutral configuration
and third-party dependency names remain explicit exceptions.

**Dependencies need their own ARM path.** Even the currently unchanged
`vectormath_common.h` includes `vectorclass.h`. Wide emulation and conversion
headers also refer back to the aggregate. An ARM counterpart is needed wherever
these dependencies must change, even if the mathematical body initially matches
upstream. Rewrite ARM includes and guards as a coherent family. Verify compiler
include traces so an ARM path cannot silently return to the restored x86 tree.

**Different does not mean Neon-related.** Several current native AVX headers
contain only x86-side changes. They do not become Neon implementations by
renaming. Restore their originals and remove their old modified versions from
active use; their history and the separate
[x86 findings and fix candidates](x86_findings_and_fix_candidates.md) are retained.
Audit changes by purpose and
reachability, not only by text markers such as `NEON`.

**C++ implementation files stay C++.** If ARM runtime-query support needs a
separate compiled implementation, use `instrset_detect_neon.cpp`, not a `.h`
rename. Select the correct implementation in the build. Preserve original
upstream dispatch examples unchanged; put any fork-owned ARM/portable examples
under `examples/`, using the new facade.

**Original metadata files also need protection.** The full original inventory
includes `README.md`, `LICENSE` and `changelog.txt`. For literal preservation
of all 28 paths, restore these too. Relocate fork-specific README content to
`README-vcl2-xa64.md` and/or `docs/`; the original root README remains unchanged.
This changes the GitHub landing README, so make the fork guide easy to find.

## Initial file-disposition inventory

Every row restores its original file to the same root path. The ARM column is
the proposed treatment to confirm through the dependency audit before renaming.

| Original file | ARM-side disposition |
| --- | --- |
| `LICENSE` | Preserve original; retain fork/dependency notices separately |
| `README.md` | Preserve original; relocate fork usage documentation |
| `changelog.txt` | Preserve original; record fork history separately |
| `dispatch_example1.cpp` | Original x86 example; portable/ARM examples separate |
| `dispatch_example2.cpp` | Original x86 example; portable/ARM examples separate |
| `instrset.h` | Reconcile existing `instrset_neon.h`; separate compatibility support if kept |
| `instrset_detect.cpp` | ARM-owned `_neon.cpp` only if compiled support is needed |
| `vector_convert.h` | `vector_convert_neon.h` |
| `vectorclass.h` | `vectorclass_neon.h` |
| `vectorf128.h` | Reconcile native `vectorf128_neon.h`; compatibility copy gets a distinct name |
| `vectorf256.h` | Native x86/AVX implementation; no automatic ARM copy |
| `vectorf256e.h` | `vectorf256e_neon.h` for ARM composition |
| `vectorf512.h` | Native x86/AVX-512 implementation; no automatic ARM copy |
| `vectorf512e.h` | `vectorf512e_neon.h` for ARM composition |
| `vectorfp16.h` | Original x86-only surface; ARM FP16 is separate scope |
| `vectorfp16e.h` | No automatic ARM FP16 claim or copy |
| `vectori128.h` | Reconcile native `vectori128_neon.h`; compatibility copy gets a distinct name |
| `vectori256.h` | Native x86/AVX2 implementation; no automatic ARM copy |
| `vectori256e.h` | `vectori256e_neon.h` for ARM composition |
| `vectori512.h` | Native x86/AVX-512 implementation; no automatic ARM copy |
| `vectori512e.h` | `vectori512e_neon.h` for ARM composition |
| `vectori512s.h` | Native x86/AVX-512 implementation; no automatic ARM copy |
| `vectori512se.h` | `vectori512se_neon.h` for ARM composition |
| `vectormath_common.h` | `vectormath_common_neon.h`, including ARM dependency changes |
| `vectormath_exp.h` | `vectormath_exp_neon.h` |
| `vectormath_hyp.h` | `vectormath_hyp_neon.h` |
| `vectormath_lib.h` | Original optional x86/SVML surface; exclude from common ARM facade initially |
| `vectormath_trig.h` | `vectormath_trig_neon.h` |

## Approved migration sequence

1. **Freeze and audit.** Preserve local edits and retained evidence. Record the
   reviewed base commit and use an isolated topic branch when implementation
   starts on the Mac; no new branch or worktree is needed for documentation publication.
   Verify all 28 original hashes and map every current root/ARM counterpart, dependency,
   include guard and generated copy. Classify ARM requirements separately from
   old x86-only repairs. Produce an explicit old-to-new path map before moving.
   Preserve the x86 findings, canonical fix references, independent regressions
   and evidence receipts listed in the separate x86 document before retiring
   any old adaptation machinery. These are historical/research assets, not
   patches to apply to the protected originals.
2. **Set the source ownership rule.** Recommended: root `_neon.h` files become
   the authoritative editable ARM sources. Move/reconcile their existing
   implementations rather than maintain duplicate editable/generated copies.
   Private ARM helpers can remain separate. Retire the old patch-to-root write
   pipeline; no generator, patch, formatter or attribution tool may rewrite the
   protected upstream files. Adjust tooling before restoring originals.
3. **Build the ARM family.** Resolve the three name collisions, preserve the
   ARM-relevant behavior, and redirect all ARM includes/guards to ARM counterparts.
   Keep currently tested backend selection during this structural migration;
   changing the ARM default to Direct Neon is a separate review/measurement step.
   This avoids mixing layout changes with a behavioral/backend switch.
4. **Restore originals.** Copy the 28 pinned files into the root with exact raw
   bytes. Keep the vendor snapshot as the integrity reference. Old x86 patches
   must no longer be applied to the root or compiled into the x86 path.
5. **Add the single public facade.** Implement compiler-target selection without
   runtime branching or vector wrappers. Include the common supported vector
   and math surface. Preserve namespace/width settings and document optional
   compiled runtime support through the same portable build target.
6. **Migrate consumers and build logic.** Update owned tests, examples, capture
   tools, benchmarks and CMake rules to the facade/selected runtime source.
   Handle generated header-isolation probes too. The pristine external testbench
   stays untouched: adapt its generated test copies/includes under `build/`.
   Do not edit external application/SDK consumers implicitly.
7. **Enforce and validate.** Run the acceptance checks below on a fresh frozen
   checkpoint, then prepare the M2 handoff. Preserve failed attempts and raw
   evidence. No commit, push, merge, PR or new timing is implicit in approval
   of the layout unless that work is explicitly included in the next task.

## Acceptance checks

- Every one of the 28 protected root files is raw-byte identical to the pinned
  vendor file. Check this on both hosts before/after validation. Use per-path
  checksum-preserving checkout attributes; newline equivalence is insufficient
  for the originals. No upstream-version refresh is part of this migration.
- No maintenance writer can target a protected original. A future explicit
  upstream refresh is the only reason to replace the pinned originals;
  adapting/optimizing Neon never is.
- Compiler include traces show x86 uses protected root upstream runtime/math headers
  and the neutral facade only; no `_neon.h`, sse2neon or old adapted runtime
  enters that path. ARM uses the ARM family and its owned/private dependencies;
  it does not fall back to protected original x86 headers.
- The same application source including only `vcl2_xa64.h` builds for native
  Windows x64 and native M2 ARM64. Unsupported architecture/backend combinations
  fail clearly. ABI/namespace/width contracts remain consistent within a binary.
- Compare an x86 facade consumer with a direct pinned-upstream consumer using
  the same compiler flags, include surface and inputs. Check outputs and
  generated code; do not add runtime wrappers or claim performance equivalence
  without appropriate evidence. No new x86 fix campaign is part of this check.
- Restoring upstream also restores its behavior, including known limitations.
  Some old fork-specific x86 assertions will no longer describe that baseline.
  Report inherited differences separately; do not patch the originals or weaken
  ARM numerical gates to force a single green result. Keep the separate
  [x86 findings](x86_findings_and_fix_candidates.md) available for future
  upstream discussion or explicitly authorized x86 research. A boundary fault
  or UBSan failure remains a reported failure, even when inherited.
- Run relevant native M2 coverage for the retained ARM backend(s), logical
  widths, Release and strict UBSan, independent fixtures, all original numerical
  gates, exact-page memory controls, 60-second deadlines and concurrency three.
  Earlier 8,036-case results remain historical until the new layout actually runs.
- Keep [ARM lessons](neon-arm-lessons.md), provenance, licenses and reproducible
  commands. Only after correctness should separately authorized ARM optimization
  change algorithms, defaults or compiler/inlining decisions.

## Improvements to carry forward

Keep one authoritative ARM implementation per public header, a small facade and
an explicit supported API. Optimize native 128-bit Neon first; treat logical
wide vectors as composition to measure, not a hardware-width promise. Maintain
independent numerical references rather than treating x86 output as the sole
oracle. Keep architecture-specific precision/inlining choices inside the ARM
family and validate them on the M2. Preserve original upstream attribution and
the sse2neon license if compatibility remains available.

The agreed initial scope is the portable integer/FP32/FP64 and in-tree math
surface, upstream's original root README with a separate fork guide, and
compatibility retained as default during the move. A Direct Neon default,
native FP16 or SVML support is separate scope. No runtime files have been
renamed/copied and no library, test, CMake or generator source has been changed
by this documentation publication.
