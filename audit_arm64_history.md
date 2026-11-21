# Audited ARM64 compatibility dependency history

One row per verified upstream sse2neon revision, newest first; the top row is
current. The date is the upstream commit date, not our test or representation
change date. Our Direct Neon implementation has its own source checkpoints.

| Upstream date | Version / snapshot | Upstream commit | Status | Project validation and tuning |
| --- | --- | --- | --- | --- |
| 18 October 2026 | Post-v1.9.1 snapshot: `v1.9.1-27-g2272251` | [`2272251`](https://github.com/DLTcollab/sse2neon/commit/2272251cc3ad9cba8b8c47c4065811cfb396b802) | **Current; exact upstream bytes** | [21 November M2 report](docs/validation/arm64/mac-arm-validation-2026-11-21.md) and [matched M2 performance](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md) identify the former representation at their own tested sources. They do not establish a pass or dependency-version speedup for this later representation change. |

## Current provenance

- Repository: [DLTcollab/sse2neon](https://github.com/DLTcollab/sse2neon).
- Full commit: `2272251cc3ad9cba8b8c47c4065811cfb396b802`.
- Complete upstream Git tree: `09647ade30f10df942613660636f17bfe1372cea`.
- `git describe --tags --long`: `v1.9.1-27-g2272251`, a development snapshot
  after v1.9.1, not the tagged v1.9.1 release.
- Sole shipped header: root [sse2neon.h](sse2neon.h).
- [Mandatory audit pin and representation history](audit/sse2neon-2272251.json).

The current header is **507,039 bytes**, UTF-8 with LF endings and a final newline,
byte-for-byte identical to the pinned upstream file. Both raw SHA-256 identities
are `932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3`;
both Git blobs are `fbfc9da84fe249f5f1d7be559e87c6a94640a6c2`.
Read-only integrity checks verify the encoding/newline contract and require the
retained bytes, SHA-256 and Git blob to match upstream directly. Git checkout
attributes preserve those exact bytes on all hosts. No transformation or header
writer is part of normal verification.

## Representation alignment — 21 November 2026

The maintainer approved replacing the earlier representation with the exact
upstream file at the **same commit**. Only encoding, line endings and the final
newline changed. There is no intrinsic, API, algorithm, numerical-contract or
compiler-flag change, and no dependency-version performance claim. Agner's 28
original files remain separate and byte-exact. Direct Neon and x86 do not include
this compatibility dependency.

The old header was **518,891 bytes**, Windows-1252/CRLF without a final newline,
SHA-256 `cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff`,
Git blob `6e8b1fb79bc948de9775f800f5084ed8219683ac`. The 19 November proof
reproduced it by converting upstream UTF-8 to Windows-1252, LF to CRLF, then
removing the last CRLF. That proof and identity remain in the audit's
`representation_history` and the [frozen original audit](https://github.com/DigitalExpressions/vcl2-xa64/blob/569d3aac7308dfe865057abbbc3a119ce3e28999/audit/sse2neon-2272251.json).
Historical source captures, reports, checksums and their frozen verification tools
keep their original meaning; do not rewrite them to this new representation.
Fresh native verification must identify the new frozen source, separately from
its clean base commit and earlier passed checkpoints.

## Earlier provenance and future updates

The former [fork-local record](https://github.com/DigitalExpressions/vcl2-xa64/blob/7465b9833968bae97ef9fb1efa645a7ed994f8eb/vendor/sse2neon.json)
uses a fork identifier, not an upstream commit. The duplicate vendor catalog was
retired on 20 November; keep the root header as the sole shipped copy and retain
its MIT notice. The [historical layout audit](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19.md)
preserves the earlier bounded checks and representation, not a new full pass.

For a future dependency update, retain old audit records and evidence, add a new
upstream-revision row, and record the exact commit/tree, raw header hash and actual
native validation/performance results. Never label an untagged snapshot as a
release or infer a pass from its version. Representation-only changes stay dated
under the same upstream revision and require their own verification evidence.
