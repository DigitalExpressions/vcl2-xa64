# Windows correctness completion evidence

Portable metadata for the [dated report](../windows-amd-2026-11-17-correctness-completion.md).
Final selected matrix: **5,712/5,712 CTest passes**, plus twenty focused programs
and twelve examples. All 124 earlier retained higher-ISA failures are resolved
at this captured uncommitted source. Native M2 and renewed performance testing
remain pending. Historical reports/data/checksums are preserved unchanged.

| File | Meaning |
| --- | --- |
| `source.json` | Base commit/tree, original 402-path inventory/fingerprint, dirty patch/source archive hashes, 35-product hashes and 157 non-documentation source hashes for M2 comparison |
| `baseline.json` | Four full 1,348-case profiles, exact source and observed CRT choices |
| `higher.json` | Eight final 40-case higher-ISA profiles, exact flags and resolved names |
| `focused.json` | Two newly reproduced pre-fix defects and twenty passing after-fix programs, ISA flags, sanitizer selection and PE/binary hashes |
| `controls.json` | Fourteen native runtime/guard records, 56 intentional guard faults and four strict UBSan positive/negative pairs |
| `attempts.json` | All six intermediate/final 320-case higher-ISA rounds, including every failed name and source fingerprint |
| `source-reconstructions.json` | Iterations 01-04 recovered from retained inputs/Git bytes, verifying every original raw file hash; explicitly reconstructed archives |
| `preflight.json` | Separate primary MSVC Release 40-case preflight with its own source ID |
| `current-source-comparison.json` | 155 raw-identical source paths and two verified exact LF-to-CRLF transformations; preserves original tested hashes |
| `compiler-diagnostic-provenance.json` | Excluded mislabeled 19.44 attempt and actual verified compiler retry; neither is a full matrix |
| `receipts.json` | Relative raw archive locations and verified SHA256 |
| `checksums.sha256` | Exact portable bytes, including this README |

The whole-source algorithm is SHA256 of sorted UTF-8
`relative_path + NUL + file_SHA256_hex + newline`. Preserve original inventories;
never substitute a new checkout's manifest for the captured tested source.
The separately named LF-normalized hashes help explain reviewed checkout
differences; they do not replace raw hashes or relax pristine-vendor checks.

Original raw records, binaries and all failures remain under ignored
`build/windows-correctness-20261117/`, with the older investigation under
`build/windows-isa-triage-20261117/`. The supplement includes generated sources
omitted by compact native packets and a retained executable-hash inventory.
Transfer raw archives separately for a full independent audit. No cleanup is
authorized; final documentation-check records remain under the raw root too.

For repetition, follow the maintained Windows runner and
[durable completion plan](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/windows-correctness-plan.md). Use a fresh
attempt/output directory and preserve the same controls, flags, 60-second
deadlines and concurrency three. Use the
[new M2 handoff](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/mac-arm-correctness-handoff.md) only after the new source is
approved and published. This bundle does not claim a clean-base pass, a new
cross-host corpus comparison or performance equivalence with original VCL.
