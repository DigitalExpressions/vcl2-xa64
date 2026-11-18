# Native M2 ARM header cleanup — 2026-11-18

See the [dated report](../mac-arm-2026-11-18-arm-header-cleanup.md) and
[cleanup audit](../arm-header-cleanup-audit-2026-11-18.md).
The complete uncommitted capture is identified by `capture.json`; its base
commit is separate from the tested implementation. Fingerprint paths are sorted
by UTF-8 bytes, hashing path, NUL, raw file SHA256 hex and newline.

`summary.json`, `profiles.json` and `test-inventory.json` record all actual cases.
`preprocessing.json`, `changed-headers.json`, `conditional-pruning.json` and
`scope-preservation.json` document removal and preserved ARM behavior.
`host.json`, `controls.json`, `flags.json`, `binaries.json`, `include-traces.json`,
`protected-integrity.json` and `runner-hashes.json` retain native evidence.
`attempts.json` preserves the unfrozen preparation failure and source identities.
`archive-receipt.json` locates the checksummed raw packet, including generated
sources, preprocessing inputs/outputs and every failed attempt.

Raw source ZIPs, patches, logs, executables and failed drafts remain under
ignored `build/`; no cleanup or publication has been performed. Windows remains
a separate gate using the [updated return handoff](../windows-arch-separation-handoff.md).
The earlier candidate 04 report and portable checksums remain unchanged.
