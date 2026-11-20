# Four-blend integration and cleanup receipts — 19 November 2026

Read the [integration/completion report](../../../arm64/blend4-integration-2026-11-19.md)
and [historical M2 handoff](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/m2-trs-inverse-follow-up-2026-11-19.md).
The [Windows results](../results/README.md) retain the failed/incomplete full
gate separately from the passing bounded preservation assessment. The follow-up
is closed by the [TRS decision](../../../arm64/trs-inverse-decision-2026-11-20.md).

`summary.json` records the verified first master publication and actual cleanup
counts. `cleanup.json` identifies the selected generated directories and clean
worktrees; `source-comparison.json` accounts for every one of the 625 Windows
input paths and the later documentation-only additions. Original M2 and Windows
source identities remain separate; no later documentation acquires native passes.
`protected-integrity.json` and `tool-preservation.json` record retained resources.
`archives.json` retains verified packet identities and original runner archive
copies. The failed packaging, push and cleanup attempts are preserved as recorded.

Every selected noncompiled record had archive coverage before removal. Compiled
files were rehashed against the original inventory, then removed; compact
evidence retains their hashes, not runnable binaries. Both complete source
captures remain available. New native work rebuilds in fresh directories.

Raw commands, logs, generated inputs, coverage maps, full binary/worktree hashes
and machine paths stay under ignored build. Final publication verification is
retained locally after this packet closes. `checksums.sha256` covers every other
file here, with exact bytes preserved by the existing parent checkout rule.
