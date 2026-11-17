# Windows AMD ISA triage evidence

Portable metadata for the [dated native follow-up](../windows-amd-2026-11-17-isa-triage.md).
Full baseline: 5,392 passes. Additional higher-ISA selection: 196 passes and
124 failures, no skips/timeouts or new failed names. Six former Clang AVX2 Debug
failures now pass; both strict UBSan higher-ISA profiles have no suite runtime
diagnostics. These are results of an uncommitted frozen snapshot, not a clean
pass of its base commit or of every ISA configuration.

| File | Meaning |
| --- | --- |
| `source.json` | Original tested base commit/tree, whole-source fingerprint/inventory, source ZIP/patch hashes and all 35 runtime-product hashes |
| `baseline.json` | All four full native profile counts, original source IDs and observed CRT choices |
| `higher.json` | Eight full local inventories of failed/resolved names, exact flags, counts, exits and sanitizer diagnostics |
| `focused.json` | Sixteen independent conversion-regression builds, exits, sanitizer selection and executable hashes |
| `controls.json` | Fourteen native runtime/guard control runs; 56 expected guard faults and five strict UBSan control pairs |
| `diagnostics.json` | Both retained diagnostic attempts and their actual exits/value/trace observations; not CTest passes |
| `original-evidence-audit.json` | All 16 retained original higher-ISA packets verified against their SHA256 and published supplement copies |
| `runtime-vs-base.json` | Raw-identical products, exact LF-to-CRLF reconstructions and the single changed runtime header |
| `current-source-comparison.json` | Current non-Markdown source versus the tested snapshot, retaining both raw hashes and verified newline transformations; later checkout-attribute metadata identified separately |
| `receipts.json` | Host-local relative archive locations and SHA256, including the frozen-source archive and raw-source/evidence supplement |
| `checksums.sha256` | Exact bytes of this portable inventory |

Every path is relative. No machine paths, credentials, binaries or raw native
logs are published here. Original full logs, commands, JUnit, initial/final
inventories, failed runs, generated probe/reference sources and binaries remain
under ignored `build/windows-isa-triage-20261117/`. Transfer its raw archives
separately if an independent full audit needs them. Nothing has been cleaned.

Source fingerprints use SHA256 of sorted UTF-8 entries
`relative_path + NUL + file_SHA256_hex + newline`. The historical and new raw
fingerprints remain distinct. Do not substitute a normalized digest or current
checkout inventory for the inventories captured during native execution.
Pristine `vendor/vectorclass/` source bytes remain exact; newline equivalence
for other files does not relax the pristine upstream check.
