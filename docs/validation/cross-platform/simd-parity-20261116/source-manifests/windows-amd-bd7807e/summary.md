# Original Windows source inventory: reconciliation

Audited 2026-11-16 for run `simd-parity-20261116`. **Reconciled: no missing/extra
paths or unexplained file hashes.** This is a source-evidence audit, not a test
rerun or a new native pass.

Frozen commit: `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`. Tree:
`2a0a5d3250201773cccb06ba0562df728f4e9387`. Mac inventory publication: `99f4a3d8d4402819d42eed7a6a65fb1a46bd712a`.

| Comparison | Count |
| --- | ---: |
| Original Windows files | 135 |
| Published Mac files | 135 |
| Missing from Windows | 0 |
| Extra on Windows | 0 |
| Identical file hashes | 43 |
| Differing file hashes | 92 |
| Unexplained file hashes | 0 |

All four original initial/final Windows inventories agree, recomputing
`0b1504d4d225f42661763dfc6d17cf5933bfdee52ac38531ee3ef9a5f0d7ab79` with the original sorted UTF-8 path/NUL/hash/newline
algorithm. All eight original JSON files have byte SHA256
`ebc6340b78844868db715615d99596bb4a18a6b8a3ecdc808d807e806a453f70`.
Local originals match archived copies byte-for-byte; individual archive hashes
match the published Windows result metadata. Nested copies in the checksummed
combined Windows packet match too. Original recorded HEAD/tree and clean state
agree across inventory, summary and source logs; final HEAD is unchanged.

## File-level explanation

The [portable manifest](manifest.json) preserves every original relative path
and Windows file hash, and lists missing, extra and differing files separately.
The [Mac inventory](../mac-m2-bd7807e.json) recomputes
`fc0700a5163d98bd7c80a2ab705984d12a22d839568fa297e83b65f47137127e`; every entry matches its frozen Git blob bytes.

For each of the 92 differing files, the frozen blob has LF and no CR bytes.
Replacing each `0a` with `0d0a` reproduces its recorded Windows hash exactly.
Keeping the other 43 files unchanged then reproduces the complete Windows
fingerprint. This is a per-file hash-verified reconstruction, not an assumed
line-ending explanation.

Of the identical files, 27 already have CRLF in their frozen Git blobs and 16
retain LF on Windows. Blanket conversion changes files that must stay unchanged;
blanket CRLF-to-LF also changes those 27 original CRLF blobs. Both inventories
produce supplementary LF-normalized fingerprint
`9926fc0fe17e80b3436579f88f065988f5d9ca77b55fe2d4cc3365cd5eb7396f`. That derived digest replaces neither
original fingerprint. The historical Git/editor configuration responsible for
the mixed checkout is not established by this audit.

## Provenance and export

| Profile | Original archive under ignored `build/` | Published SHA256 verified |
| --- | --- | --- |
| suite-msvc-sse42-release | `windows-native-msvc-release-20261116T053548744188Z/return-evidence.zip` | `7cb9260e0645074293da24f725c3157b5243f13698f5ac94a9d7a701de5c95d1` |
| suite-msvc-sse42-debug | `windows-native-msvc-debug-20261116T054103097552Z/return-evidence.zip` | `ddc2a547527ced6954bae8f6feae1835b35f482922f0190cf71397c6f15917b9` |
| suite-clang-sse42-release | `windows-native-clang-release-20261116T054642535750Z/return-evidence.zip` | `7238a2430cd502b2d817ab9397edb215f16176fa419ea1a952e2ff9798cdff65` |
| suite-clang-sse42-debug | `windows-native-clang-debug-20261116T055118185051Z/return-evidence.zip` | `c68925c1471b29b0a2e7630193a6233d44987eb9dbf36b653684a0106e2c23ee` |

The combined packet is `build/windows-final-simd-20261116T053510Z/windows-final-evidence.zip`,
SHA256 `fece4dd2f38f5dbad2da36371ef94cbee871749fec1f8d6b1d4102bc43328d3e`. ZIP CRCs and all archived original
manifest/provenance bytes were verified. Raw logs/archives and audit scripts stay
under ignored build directories and are not exported here.

This export wraps the original entries with provenance and comparison results,
using UTF-8/LF JSON formatting. Original file hashes and paths are untouched;
original JSON byte hashes identify the source serialization separately. The
new checksum inventory covers only `manifest.json` and this summary. Existing
Mac manifests, handoff README, reports, comparison attempts and checksum
inventories remain unchanged.

No current-checkout manifest was substituted. Matching current local bytes for
132 original entries only corroborated the evidence; reconstruction of all
entries used immutable frozen blobs and the recorded original hashes.

See the [dated audit report](../../../../source-byte-audit-2026-11-16-bd7807e.md).
No native test, numerical-policy, library or harness change was made. The Mac
comparison input was its published inventory, independently verified against
frozen Git; this audit does not reopen Mac raw execution archives or alter
the separately documented validator follow-up. No source-fingerprint gap remains.
