# Canonical backend implementation sources

- `arm64/`: fork-owned Direct NEON and ARM compatibility/control primitives.
- `x86/`: native MSVC integer-classification workarounds and optimized primitives.

These are canonical maintenance sources. The generator copies them into the
existing root include layout, preserving public names and direct header-only
usage. Applications include root `vectorclass.h`; they do not need to switch
include directories or run generation.

Shared selection/extracted helpers are under `adaptation/include/`. Original
VCL2 bytes are under `vendor/vectorclass/`, with explicit adaptation patches
outside that pristine directory. Read the [layout guide](../docs/backend-layout.md)
before editing or updating a backend.
