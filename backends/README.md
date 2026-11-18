# Retained backend research

The authoritative ARM sources are now root `*_neon.h` files. The former
`arm64/` duplicates were removed during the architecture separation; they remain
in Git history. Do not recreate a generation/copy pipeline or edit protected
root/vendor originals for Neon work.

`x86/` retains the old private MSVC classification implementation as inactive
research, with no include path from either active architecture. Historical
patches, tests and findings remain available without resuming that campaign.
See the [layout guide](../docs/backend-layout.md) and
[retained x86 findings](../docs/x86_findings_and_fix_candidates.md).
