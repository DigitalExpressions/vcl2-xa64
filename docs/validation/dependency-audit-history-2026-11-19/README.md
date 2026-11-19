# Dependency audit/layout results — 19 November 2026

Read the [dated report](../dependency-audit-history-2026-11-19.md) and root
[x86](../../../audit_x86_history.md) / [ARM64](../../../audit_arm64_history.md)
revision ledgers. `summary.json` distinguishes the frozen smoke source, eight
selected cases, controls and unchanged historical gate. `native-smoke.json`
records bounded compiler/include/output checks; the canonical matrix was not run.
`protected-integrity.json` records the 31 remaining protected files and every
unchanged runtime/test/CMake/native-runner input. The VCL manifest relocated
byte-exact; the sse2neon provenance audit retains both representation hashes.

`archive-receipt.json` identifies the verified compact raw packet. Complete
source captures, pre-removal vendor bytes, generated fixtures, Git inspection
objects, native logs/inputs and compiled files remain under ignored build.
No evidence or compiled file was removed and no commit/push/merge was performed.
Later documentation is not a new native pass. Existing portable bundles remain
unchanged. `checksums.sha256` covers every other file here with exact checkout bytes.
