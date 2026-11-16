# Windows/Mac source-byte audit — 2026-11-16

## Outcome and source identities

**Reconciled.** Run `simd-parity-20261116` has the same 135 paths on both hosts:
zero missing, zero extra, 43 identical hashes and 92 differing hashes. Every
difference is explained by a reproducible byte transformation matching the
original recorded hashes. No file-content difference remains unexplained.

| Identifier | Recorded value |
| --- | --- |
| Frozen validation commit | `bd7807e91f5888bc2ad708ca46c9305d166d6c4b` |
| Frozen tree | `2a0a5d3250201773cccb06ba0562df728f4e9387` |
| Original Windows fingerprint | `0b1504d4d225f42661763dfc6d17cf5933bfdee52ac38531ee3ef9a5f0d7ab79` |
| Published Mac fingerprint | `fc0700a5163d98bd7c80a2ab705984d12a22d839568fa297e83b65f47137127e` |
| Mac inventory/publication reviewed | `99f4a3d8d4402819d42eed7a6a65fb1a46bd712a` |

This follow-up audits existing evidence. It does not regenerate validation
inventories from a later checkout, rerun native tests, or establish a native
pass at the Mac publication or audit commit. Both original fingerprints remain.

## Original Windows evidence verification

All four full-suite profiles are available: MSVC Release/Debug and GNU-driver
Clang Release/strict UBSan Debug. Each original `source-manifest.json` and
`source-final-manifest.json` was read from its retained run directory and its
original checksummed archive. All eight JSON files match byte-for-byte, with
original JSON byte SHA256
`ebc6340b78844868db715615d99596bb4a18a6b8a3ecdc808d807e806a453f70`.

For every manifest, independently hashing sorted UTF-8 entries
`relative_path + NUL + file_SHA256_hex + newline` reproduces the original Windows
fingerprint. Initial/final entries agree in every profile and across all four
profiles. Each has 135 unique relative paths and valid original file hashes.

Original inventory/summary metadata and initial source logs agree on the frozen
HEAD/tree and clean checkout. Final HEAD logs still identify the same commit;
its immutable Git tree provides the final tree identity. No separate final-tree
log is claimed. There is no commit/tree/inventory inconsistency.

Every individual archive SHA256 matches the published Windows
`matrix-summary.json` receipt. Nested copies inside the combined archive also
match those bytes and checksums. The combined original archive remains
`build/windows-final-simd-20261116T053510Z/windows-final-evidence.zip`, SHA256
`fece4dd2f38f5dbad2da36371ef94cbee871749fec1f8d6b1d4102bc43328d3e`.
ZIP CRCs, manifest bytes and provenance entries were checked without changing
any original archive, inventory or log. Individual archive names/checksums and
original metadata byte hashes are listed in the portable export.

## File-by-file reconciliation

The [published Mac inventory](cross-platform/simd-parity-20261116/source-manifests/mac-m2-bd7807e.json)
recomputes its declared fingerprint, and all 135 entries match their frozen
Git blob bytes. Its published metadata bytes were also verified at `99f4a3d`.

The [portable Windows manifest](cross-platform/simd-parity-20261116/source-manifests/windows-amd-bd7807e/manifest.json)
preserves every original Windows relative path/hash. Its comparison section
reports missing paths, extra paths and all 92 differing hashes separately.

For each differing path, the frozen blob matches the Mac hash and contains LF
with no CR bytes. Replacing each `0a` byte with `0d0a` produces the recorded
Windows file hash exactly. Keeping the other 43 blobs unchanged then reproduces
the entire original Windows fingerprint. This proves the file-ending explanation
per path rather than assuming it from the platforms.

The 43 identical files consist of 27 blobs already containing CRLF on both hosts
and 16 files retaining LF on both. Therefore a blanket conversion changes files
that must remain unchanged. A blanket CRLF-to-LF conversion also changes the
27 original CRLF blobs. As a supplementary audit calculation only, normalizing
both reconstructed inventories to LF produces the same digest:
`9926fc0fe17e80b3436579f88f065988f5d9ca77b55fe2d4cc3365cd5eb7396f`.
This derived value is not substituted for either original fingerprint.

Current local file bytes independently match 132 original hashes; this is
corroboration only. The later checkout's modified documentation was not treated
as original evidence. All reconstruction inputs come from immutable frozen
Git blobs, checked against the published Mac inventory and original Windows hashes.

## Portable export and preservation

The [Windows inventory export](cross-platform/simd-parity-20261116/source-manifests/windows-amd-bd7807e/summary.md)
contains only `manifest.json`, `summary.md` and a new `checksums.sha256`.
Original file paths and file-hash strings are unchanged. The export adds
provenance/comparison metadata and serializes that metadata as UTF-8 JSON/LF;
the original JSON serialization's byte hash is recorded separately.
No absolute machine paths, credentials, binaries or raw logs are exported.

The original Mac manifest, source-handoff README and its checksum inventory,
all historical reports, comparison attempts and existing checksum files remain
byte-for-byte unchanged. The handoff records its pre-audit gap; this dated
follow-up and the validation index record its resolution. Raw audit scripts
and findings remain under ignored `build/source-byte-audit-20261116T182036Z/`.

## Limits and remaining gaps

No original Windows manifest is missing; no path or file-hash discrepancy is
unexplained. This closes the source-byte fingerprint evidence gap. The exact
historical Git/editor configuration that produced the mixed Windows endings
was not recorded and is not inferred. Mac execution provenance was taken from
the published inventory, independently checked against frozen Git; this audit
does not reopen the Mac raw execution archives.

The original native results and separately identified Mac validator follow-up
retain their original scope and limitations. No numerical policy, library or
harness code was changed, and no native matrix or comparison was rerun.

## Commit and handoff

Audit work is isolated on local branch
`audit/simd-parity-source-fingerprints-20261116`, based on Mac publication `99f4a3d`.
Only the portable audit and related documentation are committed. The existing
`master`, `neon-port` and `windows-native-validation` tips and unrelated work are
preserved. Nothing is pushed; explicit approval is required before publication.
