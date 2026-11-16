# Source-byte fingerprint handoff

The [Mac source manifest](mac-m2-bd7807e.json) lists all 135 tracked files from
the clean frozen `bd7807e91f5888bc2ad708ca46c9305d166d6c4b` worktree, tree
`2a0a5d3250201773cccb06ba0562df728f4e9387`. Every file matched its Git blob
byte-for-byte; this export was verified against the retained execution inventory.
Paths are repository-relative and hashes preserve original bytes and line endings.

Its full fingerprint is
`fc0700a5163d98bd7c80a2ab705984d12a22d839568fa297e83b65f47137127e`.
The Windows report records
`0b1504d4d225f42661763dfc6d17cf5933bfdee52ac38531ee3ef9a5f0d7ab79`.
Exact captured commit/tree and shared corpus/policy/input hashes match, but the
whole-file byte fingerprint difference remains unresolved. A simple CRLF
simulation did not reconcile it; do not assume the reason.

On Windows, locate the original frozen-source `source-manifest.json` retained
by the native runner or in the separately transferred raw evidence archives.
Use the original file, rather than a manifest regenerated at the publication
commit. Verify its declared fingerprint by hashing the sorted `files` entries
as UTF-8 `path + NUL + sha256 + newline`; the digest must reproduce the Windows
report before comparing it with this Mac inventory.

Compare file entries by relative path and report missing paths, extra paths
and differing content hashes separately. Inspect the original bytes for each
difference before attributing it to line endings or other causes. Preserve both
original manifests and record any reconciliation as a new evidence follow-up.
If the retained Windows inventory does not reproduce its report's fingerprint,
record that discrepancy explicitly. No validation rerun is needed merely to
perform this source-inventory review.

The Windows raw source manifest is unavailable on this Mac. The
[Mac report](../../../mac-arm-2026-11-16-bd7807e-final.md) and
[validator follow-up](../../../mac-arm-2026-11-16-bd7807e-validator-followup.md)
retain the executed results and limits. Publishing this inventory does not
establish a native pass at the later publication commit.
