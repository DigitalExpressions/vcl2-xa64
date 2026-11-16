# Adaptation layer

Canonical upstream bytes stay in [vendor/vectorclass](../vendor/vectorclass/README.md).
The ordered patches adapt those bytes for this fork:

1. `patches/0001-portability-and-correctness.patch` records existing port,
   portability and correctness differences through fork `44f974a`. It includes
   ARM selection and compatibility, shared-helper extraction, defined integer
   operations, safe memory boundaries, rounding/conversion fixes, numerical
   fixes and compiler workarounds. It is not solely an ARM patch.
2. `patches/0002-msvc-x86-performance.patch` contains the measured native MSVC
   classification call sites and FP32 exponential inlining decision. The
   corresponding private primitives are in `backends/x86/`.

`include/` holds fork-owned/shared policy and extracted helpers. Original
attribution remains in derived files. Direct NEON implementation sources are
in `backends/arm64/`; the compatibility dependency is in `vendor/sse2neon/`.

[manifest.json](manifest.json) maps canonical sources to shipped root headers
and runtime examples. Edit canonical patches/backend sources, then regenerate
and validate. Root products are checked in so applications keep their existing
header-only usage and need no Python, Git or code-generation step.

See the [layout and update workflow](../docs/backend-layout.md). The generator
applies exact unified hunks without fuzz and verifies the pristine upstream
tree before writing any product. Existing numerical and memory contracts apply
to the adapted runtime; a pristine vendor snapshot is not a claim that original
upstream already meets this fork's regression gates.
