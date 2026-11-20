# Retained backend research

The authoritative ARM sources are now root `*_neon.h` files. The former
`arm64/` duplicates were removed during the architecture separation; they remain
in Git history. Do not recreate a generation/copy pipeline or edit protected
root/vendor originals for Neon work.

The unused private MSVC classification helper was retired with maintainer
approval. Its [frozen source](https://github.com/DigitalExpressions/vcl2-xa64/blob/41d7fbacc45fbb679b08f0074a8f340ced3a3f20/backends/x86/x86_classification.h)
remains available in Git history. It had no include path from either active
architecture. Historical patches that reference it require the corresponding
frozen source; they are not a pipeline for current master. Tests and findings
remain available without resuming that campaign.
See the [layout guide](../docs/backend-layout.md) and
[retained x86 findings](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md).
