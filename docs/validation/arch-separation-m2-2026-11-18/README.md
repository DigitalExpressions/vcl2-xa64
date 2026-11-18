# Native M2 architecture separation — 2026-11-18

See the [dated report](../mac-arm-2026-11-18-arch-separation.md) for the actual
outcome, retained attempts and Windows gap. This candidate is uncommitted;
`capture.json` records the base separately from its 454-file frozen inventory.
The checksum/fingerprint algorithm sorts paths by UTF-8 bytes and hashes each
relative path, NUL, raw file SHA256 hex and newline.

- `summary.json`, `profiles.json`, `test-inventory.json`: actual execution counts,
  complete case names/results and unchanged 60-second deadlines.
- `capture.json`, `runner-hashes.json`: exact source, patch and runner identities.
- `attempts.json`, `failed-attempt-inventories.json`: all four retained candidates,
  build failures/source changes and all names registered by stopped attempts.
- `protected-integrity.json`: exact before/after root/vendor/pin hashes and mtimes.
- `include-traces.json`: ARM family and isolated installed-header consumer proof.
- `host.json`, `controls.json`, `flags.json`: actual native tools, controls and flags.
- `binaries.json`: native executable hashes, with no binary payload.
- `scope-preservation.json`: unchanged regression bodies, references and gates.
- `archive-receipt.json`: verified raw packet location, coverage and checksum.

Original source archives, patches, failed attempts, logs, binaries and generated
probe/reference inputs remain on the Mac under ignored `build/`, transferring
separately from portable metadata. Nothing is committed, pushed or cleaned.
Native Windows verification of these same implementation bytes remains pending;
use the [return handoff](../windows-arch-separation-handoff.md) after review.
This is no new benchmark or universal cross-host bitwise-parity result.
