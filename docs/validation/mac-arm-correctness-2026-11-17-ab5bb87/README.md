# Native M2 correctness results — 2026-11-17

This bundle records the native ARM run at approved commit
`ab5bb8754a33724e05a170fa2867475077efd023`, tree
`48376a53dfa5f5f2139651e3d44f5e1a9ed841b1`. See the
[dated Mac report](../mac-arm-2026-11-17-ab5bb87-correctness.md) for the actual
outcome, attempts and remaining gaps. This is native correctness evidence,
not a new shared corpus or universal bitwise-parity comparison.

- `summary.json`, `profiles.json`: actual counts, controls and outcome.
- `test-inventory.json`: every registered case, deadline and executed result.
- `binaries.json`: native test/example/probe executable hashes; no binaries.
- `source-comparison.json`: all 157 Windows-listed code paths, retained raw
  hashes and verified newline reconstructions, plus publication differences.
- `source-manifest.json`: exact tested Mac source hashes and fingerprint.
- `vendor-integrity.json`: before/after raw hashes, sizes and timestamps.
- `host.json`, `flags.json`, `controls.json`: toolchain, native proof and gates.
- `archive-receipt.json`: original raw packets, verified checksums and coverage.
- `cleanup-receipt.json`: authorized cleanup, relocated original packet and
  checksummed supplement retaining all nonbinary build evidence.

Original Windows source identifiers remain unchanged. Windows executed base
`e27134b` plus captured fixes, fingerprint
`af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528`;
these portable results do not relabel that run as an executed whole-tree pass
of the later publication commit. No original Windows inventory was replaced.

Raw archives/logs remain under ignored `build/` and transfer separately.
The maintainer subsequently authorized publication and evidence-preserving
cleanup. See the dated report follow-up for retained packet locations. Original
source IDs, execution results and archive bytes remain unchanged.
