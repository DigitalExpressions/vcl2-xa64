# Audited ARM64 compatibility dependency history

One row per verified upstream sse2neon revision, newest first; the top row is
current. The date is the upstream commit date, not our test date. This ledger
identifies the sse2neon compatibility dependency. Our experimental Direct Neon
implementation has its own source checkpoints in the project reports.

| Upstream date | Version / snapshot | Upstream commit | Status | Project validation and tuning |
| --- | --- | --- | --- | --- |
| 18 October 2026 | Post-v1.9.1 snapshot: `v1.9.1-27-g2272251` | [`2272251`](https://github.com/DLTcollab/sse2neon/commit/2272251cc3ad9cba8b8c47c4065811cfb396b802) | **Current; representation differences recorded below** | [Native M2 correctness](docs/validation/arm64/mac-arm-2026-11-19-blend4.md); [matched M2 performance](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md). Timings compare scalar, compatibility and Direct Neon; they are not a dependency-version speedup claim. |

Current provenance:

- Repository: [DLTcollab/sse2neon](https://github.com/DLTcollab/sse2neon).
- Full commit: `2272251cc3ad9cba8b8c47c4065811cfb396b802`.
- Complete upstream Git tree: `09647ade30f10df942613660636f17bfe1372cea`.
- `git describe --tags --long`: `v1.9.1-27-g2272251`. This is a development
  snapshot after v1.9.1, not the tagged v1.9.1 release.
- [Upstream/retained byte identities and transformation proof](audit/sse2neon-2272251.json).
- [Earlier fork-local retention record](vendor/sse2neon.json), preserved unchanged.
  Its `44f974a...` identifier belongs to this fork, not DLTcollab's upstream.

The retained root `sse2neon.h` and `vendor/sse2neon/sse2neon.h` are byte-identical
to each other, with SHA-256:
`cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff`.
The upstream UTF-8/LF header has SHA-256:
`932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3`.

The files are not raw-byte identical to upstream. On 19 November 2026, an exact
comparison verified that decoding upstream UTF-8, encoding Windows-1252,
converting LF to CRLF and removing the final CRLF reproduces every retained
byte. There are no other source-content changes. Both source identities are
recorded; neither protected copy was normalized or updated during this audit.
Read-only integrity checks verify both retained copies against their pin and
reconstruct the upstream representation in memory to verify its SHA-256/blob.

The active sse2neon dependency stays under `vendor/sse2neon/`, separate from the
VCL history and comparison cache. Preserve its MIT notice and existing bytes.
The [layout migration record](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19.md)
distinguishes this provenance audit from a new dependency update or native run.

For a future update, retain the old audit record and test evidence, add one new
row above the current row, mark the old row superseded, and record the exact
upstream commit/tree, retained header hash, any representation/local changes and
actual native validation/performance reports. Never label an untagged snapshot
as a release or infer a pass from a version number.
