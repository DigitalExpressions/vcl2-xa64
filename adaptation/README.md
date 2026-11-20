# Historical adaptation research

Patches 0001 through 0004 retain the exact prior portability/correctness and
x86 performance work. They are inactive research records, not an active pipeline.
See [retained x86 findings](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md) for their
provenance, independent regressions, limitations and evidence receipts.
The old MSVC classification helper is retired from the current checkout;
reproduce patches that reference it using the corresponding
[frozen helper source](https://github.com/DigitalExpressions/vcl2-xa64/blob/41d7fbacc45fbb679b08f0074a8f340ced3a3f20/backends/x86/x86_classification.h).

The schema-2 [manifest](manifest.json) has no patches or output products.
`tools/regenerate_headers.py --write` is retired and always rejects. Do not apply
these patches to protected originals. ARM sources now live in root `*_neon.h`;
the old editable include/backend duplicates are removed. All 28 originals at
root remain protected; the vendor catalog is retired. Read the [layout guide](../docs/backend-layout.md).
