# Audited ARM64 compatibility dependency history

One row per verified upstream sse2neon revision, newest first; the top row is
current. The date is the upstream commit date, not our test date. This ledger
identifies the sse2neon compatibility dependency. Our experimental Direct Neon
implementation has its own source checkpoints in the project reports.

| Upstream date | Version / snapshot | Upstream commit | Status | Project validation and tuning |
| --- | --- | --- | --- | --- |
| 18 October 2026 | Post-v1.9.1 snapshot: `v1.9.1-27-g2272251` | [`2272251`](https://github.com/DLTcollab/sse2neon/commit/2272251cc3ad9cba8b8c47c4065811cfb396b802) | **Current; representation differences recorded below** | [Native M2 correctness](docs/validation/arm64/mac-arm-validation-2026-11-21.md); [matched M2 performance](docs/benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-19.md). Timings compare scalar, compatibility and Direct Neon; they are not a dependency-version speedup claim. |

Current provenance:

- Repository: [DLTcollab/sse2neon](https://github.com/DLTcollab/sse2neon).
- Full commit: `2272251cc3ad9cba8b8c47c4065811cfb396b802`.
- Complete upstream Git tree: `09647ade30f10df942613660636f17bfe1372cea`.
- `git describe --tags --long`: `v1.9.1-27-g2272251`. This is a development
  snapshot after v1.9.1, not the tagged v1.9.1 release.
- [Upstream/retained byte identities and transformation proof](audit/sse2neon-2272251.json).
- [Earlier fork-local retention record](https://github.com/DigitalExpressions/vcl2-xa64/blob/7465b9833968bae97ef9fb1efa645a7ed994f8eb/vendor/sse2neon.json), preserved in frozen Git.
  Its `44f974a...` identifier belongs to this fork, not DLTcollab's upstream.

The sole shipped dependency is root [sse2neon.h](sse2neon.h), with SHA-256:
`cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff`.
The upstream UTF-8/LF header has SHA-256:
`932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3`.

The retained header is not raw-byte identical to upstream. On 19 November 2026, an exact
comparison verified that decoding upstream UTF-8, encoding Windows-1252,
converting LF to CRLF and removing the final CRLF reproduces every retained
byte. There are no other source-content changes. Both source identities are
recorded; the protected header was not normalized or updated during this audit.
Read-only integrity checks require the audit above and verify the root header's
byte count, SHA-256 and Git blob, then reconstruct the upstream representation
in memory to verify its byte count, SHA-256 and Git blob.

On 20 November 2026 the maintainer approved retiring the remaining `vendor/`
catalog: its README, duplicate header and superseded fork-local pin. The root
header and its existing MIT notice, Windows-1252 encoding, CRLF line endings
and absent final newline remain byte-exact. The mandatory audit above is the
current pin; the earlier record stays available through the frozen Git citation.
Historical source inventories keep their original paths and hashes. Reproduce
them with the tools from their frozen source, not the current-layout checker.
The [layout migration record](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19.md)
distinguishes this provenance audit from a new dependency update or native run.

For a future update, retain the old audit record and test evidence, add one new
row above the current row, mark the old row superseded, and record the exact
upstream commit/tree, retained header hash, any representation/local changes and
actual native validation/performance reports. Never label an untagged snapshot
as a release or infer a pass from a version number.

## Root-only layout verification — 20 November 2026

The uncommitted retirement based on `7465b9833968bae97ef9fb1efa645a7ed994f8eb`
passed the read-only integrity and repository checks, all 31 isolated integrity
controls, and native M2 include/standalone-consumer checks for compatibility 512
and Direct Neon 128/256/512, including six target-rejection controls. All nine
retained portable checksum inventories verified (169 entries).

All 28 original VCL root files, root `sse2neon.h` and the VCL audit manifest
retain their exact bytes, modes and modification times. Runtime, numerical
tests, runners, fixtures and CMake are unchanged. Logs and control outputs are
under ignored `build/`. This bounded maintenance check does not establish a new
full correctness matrix, benchmark result or Windows pass; the existing full
Windows gate remains failed/incomplete.
