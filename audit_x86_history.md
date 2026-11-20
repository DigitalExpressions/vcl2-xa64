# Audited x86 VCL revision history

One row per upstream VCL revision, newest first; the top row is current.
The date is the upstream commit date, not our test date. Project test/tuning
checkpoints remain identified in the linked reports rather than duplicated as
new upstream rows. No example or unverified older revision is listed.

| Upstream date | Version | Upstream commit | Status | Project validation and tuning |
| --- | --- | --- | --- | --- |
| 9 September 2026 | 2.02.04 | [`6675774`](https://github.com/vectorclass/version2/commit/667577495efaa36a10cbb5d6cb8c942c23913848) | **Current** | [Windows full gate and bounded preservation](docs/validation/x86/windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md); [integration](docs/validation/arm64/blend4-integration-2026-11-19.md). Full gate remains failed/incomplete; this does not claim an x86 tuning gain. |

Current provenance:

- Repository: [Agner Fog's Vector Class Library](https://github.com/vectorclass/version2).
- Full commit: `667577495efaa36a10cbb5d6cb8c942c23913848`.
- Complete upstream Git tree: `36aee120c28c9c64e62903bfc81139003e37cedd`.
- [Original 28-file Git-blob/SHA-256 manifest](audit/vectorclass-2.02.04.json),
  moved byte-exact from the former vendor pin. Manifest SHA-256:
  `9c3a9a777426ffac4daf8dce53673629e5b5ff5a16716089a6d2b51cb3b8212e`.

All 28 original files remain byte-exact at repository root, including licenses,
notices and README. `vendor/vectorclass/` is no longer duplicated in this layout.
`python -B tools/regenerate_headers.py --check` verifies every recorded SHA-256,
Git blob and the complete upstream Git tree directly from the root originals.
This check is offline and does not rewrite or normalize any original.

For a separate pristine include directory, use:

```sh
python -B tools/restore_upstream_vcl.py
```

Use `python3` on macOS. This reconstructs the verified 28-file snapshot at
`build/upstream/vectorclass-6675774/`, entirely from the hash-verified root
originals, without downloading or changing public source. Existing destinations
are checked and reused; changed, missing or extra files cause failure and are
never overwritten. Direct-original probes should use that directory and record
its manifest/tree proof separately from facade includes.

The [layout migration record](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/dependency-audit-history-2026-11-19.md)
documents the removal and its checks. Historical frozen source captures retain
their original vendor directory and pin paths; their hashes and results are not
rewritten. This later layout does not acquire an earlier native pass.

When adopting another upstream revision, retain this manifest, add a new dated
row above it, mark the previous row superseded, and link the new full commit,
tree, per-file manifest and actual project reports. An upstream update requires
separate authorization; adding a row is not validation.
