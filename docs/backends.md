# Backend support

## Implementation boundaries

The [source layout](backend-layout.md) protects all 28 original VCL paths at
root and in vendor. The neutral facade `vcl2_xa64.h` selects original x86 headers
or the separate authoritative ARM `*_neon.h` family. No generation is required.

Keep one public VCL API, numerical contract and independent test suite. Register
operations, mask representation and compiler workarounds belong to the selected
backend. Backend selection is compile-time; x86 execution does not route through
NEON. Shared mathematical algorithms may retain narrowly guarded compiler
inlining decisions, but must keep their coefficients, operation grouping and
special-value policy consistent.

The [ARM/Neon lessons](neon-arm-lessons.md) and [strategy reset](integration-plan.md)
are the current planning entry points. The architecture migration preserves the ARM backend behavior below while
restoring pristine x86 behavior. Prior x86 repair/optimization work is inactive
research and is not part of the x86 include graph.

## Backend selection and coverage

| Backend | Header selection | Status |
| --- | --- | --- |
| x86 | Automatic on x86, or `VCL_BACKEND=1` | Existing VCL implementation; ISA selected by compiler flags |
| ARM64 sse2neon | Automatic on ARM64, or `VCL_BACKEND=2` | Compatibility implementation using native Neon |
| ARM64 Direct Neon | `VCL_BACKEND=3` and `VCL_ENABLE_EXPERIMENTAL_NEON` | Experimental native implementation |

C++17 is required for the library. Define backend/width options before the first
VCL include. `MAX_VECTOR_SIZE` accepts 128, 256 or 512; header inclusion defaults
to 512. CMake's explicit Direct Neon maximum defaults to 128 and is controlled
with `VCL_NATIVE_MAX_VECTOR_SIZE`. All modules exchanging VCL objects must agree
on backend, width, namespace and other ABI-affecting settings.

The portable core includes signed/unsigned 8/16/32/64-bit integers, FP32/FP64,
broad masks, construction, memory operations, classification, conversions,
division and movement. Wider native vectors compose 128-bit registers. Direct
Neon exposes `native()` and `registertype`; raw SSE interoperability uses x86
or sse2neon. FP16 and Intel SVML are outside Direct Neon support.

On ARM64, legacy x86 capability queries report no x86 hardware features and
`has_neon()` reports Neon. ARM FP control maps rounding and coupled FTZ/DAZ;
exception flags and masks are not emulated. The header does not change the
caller's FP environment merely by being included.

Recorded native validation includes Apple M2/Apple Clang and AMD Ryzen Windows
MSVC/Clang. Earlier translated x86 and QEMU/Linux ARM results are separate
evidence. Read the [validation index](validation/README.md) for exact tested
revisions and coverage limits, and the [manual Windows runner](validation/windows-runner.md) for reproducible
local checks. Other hosts/toolchains and unselected APIs remain separate
gates; historical automated results do not imply later native passes. Maintainer physical Intel
testing is omitted for now.

Native x86 FP16 in upstream VCL 2.02.04 requires the standard-library
`<stdfloat>` header. The retained migration and Windows probes lacked the
required standard-library type, and the AMD host lacks AVX-512 FP16. Historical
instruction-level 12 syntax checks do not establish native FP16 execution. See
[migration validation](validation/migration.md) and the
[Windows review](validation/windows-amd-2026-11-15-823ca2a.md).
