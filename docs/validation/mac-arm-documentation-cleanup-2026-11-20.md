# Mac ARM validation documentation cleanup — 20 November 2026

The maintainer requested removal of pre-19 November Mac ARM validation material
that is no longer needed for current master. Earlier passes removed 95 files;
retiring the remaining five obsolete instruction/preparation pages brings the
total to **100 files**.
Existing Git history retains the originals at
[d796768](https://github.com/DigitalExpressions/vcl2-xa64/tree/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs);
no replacement archive was created. This is documentation maintenance, not a
new native pass, performance result or runtime change.

## Completed local workspace cleanup

On 20 November the maintainer requested discarding completed Mac ARM planning,
four-blend, TRS-recovery and architecture-separation sessions. Obsolete builds,
binaries, raw trials/diagnostics, source captures, archives and planning backups
were discarded without replacement archives. Those directories occupied about
**15.86 GiB** before removal; **eight temporary worktrees** were removed through
Git. Only the primary master worktree remains. Branches, installed tools, the
pristine pinned testbench, canonical source/tests and protected originals remain.
Historical receipts describe their original availability; deleted artifacts are
no longer available for exact raw-trial/binary reinspection. No new runtime
change, native matrix or timing was part of cleanup. The
[TRS decision](trs-inverse-decision-2026-11-20.md) retains the useful findings.

## Removed

- Superseded 14/15/17 November migration, publication and native-progress reports.
- The completed 18 November architecture plan, Mac execution handoff and
  superseded candidate-04 report/audit; current ownership is documented by
  [backend layout](../backend-layout.md) and the retained file map.
- Three complete old portable bundles: arch-separation-m2-2026-11-18,
  arm-header-cleanup-m2-2026-11-18 and
  mac-arm-correctness-2026-11-17-ab5bb87. Their later Mac raw sessions were
  already discarded in the [workspace cleanup](#completed-local-workspace-cleanup).
- The retired Mac final-verification prompt and obsolete compact-retention policy.

The 19 November [M2 correctness report](mac-arm-2026-11-19-blend4.md) and its
full portable inventory retain the later 8,036-case/eight-example checkpoint.
Current source ownership, contracts and maintained runners cover future work;
none depends on the deleted documentation. Source-manifest path strings remain
historical inventories of their frozen trees, not requirements on today's files.
Historical prose links now resolve through the exact Git revision. Current
entry points use the retained workflow, layout and 19 November reports.

## Older material retained for a verified purpose

- Shared corpus/policies, Mac/Windows captures, comparator follow-up and the
  135-path source-byte reconciliation are the only retained evidence for the
  recorded 32 cross-host pairings. Separate 19 November native matrices do not
  supersede that comparison or prove parity at current master.
- Reusable native workflow, test guidance, report template, numerical/ARM lessons
  and consolidated benchmark interpretation remain current procedures or useful
  contracts. Historical benchmark tables retain distinct measured checkpoints.
- Native Windows result datasets and mixed-host records, including the
  18 November Windows cleanup/recovery receipt and shared-harness preparation,
  are retained. The separately approved extension below retires Windows
  instruction pages only; no Windows failure is removed or relabeled.

## Approved eight-page consolidation and removal

The maintainer approved consolidating measurement notes and benchmark regression
contracts into the [benchmark guide](../../benchmarks/README.md#correctness-contracts),
then deleting all eight recommended pages:

- docs/benchmarks/measurement-notes.md
- docs/validation/regression-tests.md
- docs/changes.md
- docs/integration-review.md
- docs/validation/mac-arm-correctness-handoff.md
- docs/validation/mac-arm-x86-optimization-handoff.md
- docs/validation/windows-arch-separation-handoff.md
- docs/windows-correctness-plan.md

The consolidated guide preserves the historical compiler/flags, trial aggregation,
layouts, range interpretation, slower-row policy, metadata redactions, exact
source/binary meaning and pre-/post-migration distinctions. It also retains the
independent regression/benchmark correctness contracts, exceptional inputs,
canaries and namespace/ABI scope. Current links use these maintained sections;
historical handoff links use frozen Git. Test and benchmark executable sources,
table measurements and numerical gates are unchanged.

Three portable bundle README indexes required historical-link repairs. Only
those links and their README checksum entries changed. Every other portable
payload/hash and all recorded native source/result identities remain exact.
The original indexes and checksum inventories remain in Git at d796768; original
SHA256 values below preserve the distinction from the repaired navigation.
The historical x86 index keeps its CRLF bytes, with an explicit whitespace
attribute for that documentation file.

| Bundle | Original README SHA256 | Original checksums.sha256 SHA256 |
| --- | --- | --- |
| [windows-correctness-2026-11-17](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/windows-correctness-2026-11-17/checksums.sha256) | 3f69659e55a60066545682106fe8ee5b180035b3e32b10da8510c5268363c38f | 8269fb7a4d88ed13d72b1a61d7521ebcaa43752d05967555b39a63015e8c7916 |
| [x86-optimization-2026-11-16](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/x86-optimization-2026-11-16/checksums.sha256) | 252a568077d3e415e4a0a54c109f2ebf5528f75848e5f0e3d0bf01136b33079a | 1efc00ffd9258b1c6a2896e9e24b57a1dada45ae227e26c966a3e57e763cbd43 |
| [arch-separation-publication-2026-11-18](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/arch-separation-publication-2026-11-18/checksums.sha256) | cf04f87d4506bc07cca688c69f973c98841705b07bbc7ae16070b498167e86d5 | 84156bc7f9e1857abd5126685dfb83b2d4b299cab1445a328593d6b69cf07ca4 |

These documentation-only repairs establish no new native pass and do not alter
any original source fingerprint or binary/recorded archive checksum.

## Retired 13/17 November benchmark reports and data

The maintainer approved removing both older Standard Math reports and any
associated data no longer needed by current master. Fourteen files were removed:

- STANDARD_MATH_RESULTS_2026-11-13.md and STANDARD_MATH_RESULTS_2026-11-17.md.
- standard_math_results.csv, standard_math_results.json and standard_math_validation.json.
- The nine-file standard-math-2026-11-17 bundle, including its checksum inventory.

These paths are under docs/benchmarks. No active test, runner, benchmark program
or build configuration consumes them. Historical source inventories and integrity
receipts still describe their original frozen paths/hashes; those records were
not rewritten as inventories of the present checkout. The originals remain
available in Git at d796768, without replacement archives. Historical planning
citations now use that frozen revision, preserving their original meaning.

The 19 November current-master checkpoint, reporting template, reading guide,
benchmark sources and consolidated correctness/measurement guidance remain.
Current entry points now use the master checkpoint and completed TRS decision.

The historical proposal required one link-only edit and the corresponding row
in its preflight checksum inventory. All numerical scope, source/result IDs,
other entries and payloads are unchanged. Original identities remain available
at the [frozen preflight inventory](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/m2-arm-preflight-2026-11-19/checksums.sha256):

- Original proposal SHA256: 12d0b762f36eb9ba795e775bb9ecd3e4533e60f02c4ae7afc004ce998432fc54.
- Original preflight checksums.sha256 SHA256: 05d588d4007cf1c439f479e7132d6480d66e919dbb7b1750910fa7c42c4ae3d0.

## Completed separation and planning records retired

The maintainer approved removing twelve further files: the 18 November Mac
header-cleanup report, the three-file architecture publication packet, the
19 November ARM planning handoff and completed task proposal, plus the preflight
report and its five-file companion packet. They describe completed work and no
active code/test/runner consumes them. Historical citations use frozen Git.
Source inventories still identify their original frozen trees; recorded results
are not reassigned to current master.

The current ownership/file map, source pins and maintained procedures remain.
The 19 November acceptance, completion and native Windows matrix/triage records
are retained; only historical links in the acceptance text changed. The
integration page now gives current status and a restart point. Seven attribute
rules confined to deleted documents/bundles were removed; protected-source
attributes and bytes are unchanged.

The retained 19 November M2 correctness report required one proposal-link repair
and one corresponding checksum entry refresh. Its cases, numerical contract,
source IDs, fields and every other portable payload/hash remain unchanged.
Original identities are available in the [frozen M2 inventory](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/m2-blend4-2026-11-19/checksums.sha256):

- Original M2 report SHA256: ca31146eb6a5e5b4d10a5b2a24c79ec1239caec9197d6c0d2c3dbce5c8b92e78.
- Original M2 checksums.sha256 SHA256: d73dc4c340d9f99a02c80dde6b414f850850169a5254987b11bbe57e891c002a.

## Final obsolete instruction and preparation pages retired

The maintainer approved removing the completed M2 TRS-inverse follow-up handoff,
the completed Windows four-blend handoff, the two retired Windows final prompts
and the superseded cross-platform preparation report. Actual native results,
source audits, shared comparison evidence and reusable procedures remain.
Historical integration citations now resolve through frozen Git at d796768.
No runtime/test/runner consumes the removed pages.

The parent Windows handoff inventory drops only the deleted handoff entry.
The integration receipt README has one historical-link repair, with its own
checksum entry refreshed. All result payloads, source identities, case outcomes
and other checksum entries remain exact. Originals remain at the
[frozen parent inventory](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/windows-blend4-handoff-2026-11-19/checksums.sha256)
and [frozen integration inventory](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/windows-blend4-handoff-2026-11-19/integration/checksums.sha256).
The checkout rule exclusive to the deleted handoff was also removed.

- Original handoff SHA256: b7bf938fe70c5094144beb1b2072a534d73778b8af17fedd22ddc9704b0b86d2.
- Original parent checksums.sha256 SHA256: 5029f5757215aa1510539f4df8d72801687d46b344096a11aa9299e5ce890330.
- Original integration README SHA256: f8c3babec20d7141bd5e0f7b1582ad3d1b4d78280b7274cc976cdd1f67fab098.
- Original integration checksums.sha256 SHA256: 7faa55a572c32853f705c05db03c793f74d46b4ca1d615e6a048723c53086684.

## Verification and limits

All 437 checksum entries across 42 retained manifests pass. Documentation-link
repairs and their checksum-entry changes are recorded above, including the
unpublished checkpoint README's updated decision link. Historical source/result identities and
all non-navigation payload hashes are preserved. Frozen Git link destinations
were verified directly from the local object database.
Repository/link, whitespace and read-only original/header checks passed.
Runtime, canonical tests/runners/fixtures/CMake, protected originals/dependencies,
installed tools, Git refs/configuration and staged content are unchanged.
Existing pending documentation work remains. No matrix, timing, dependency
installation, commit, push, merge or branch retirement was performed.
The Windows full gate remains failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples per recorded source.
