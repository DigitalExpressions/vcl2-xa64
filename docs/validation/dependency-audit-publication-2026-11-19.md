# Dependency audit publication and local cleanup — 19 November 2026

The maintainer authorized publication of the completed
[x86](../../audit_x86_history.md) and [ARM64 compatibility](../../audit_arm64_history.md)
revision ledgers, retained hash manifests, VCL deduplication and supporting
integrity/reconstruction tools, followed by evidence-backed cleanup.
The [original validation/layout report](dependency-audit-history-2026-11-19.md)
and its checksummed results retain their earlier source identity and scope.

## Verified publication

The complete reviewed change was committed and pushed to the fork's
`origin/master` at `2f9837dcd5fe3d5b94c5abc5af82dedf54c54eb9`.
The remote SHA and local tracking ref matched. No force push, branch retirement,
remote/configuration change or history rewrite occurred. This later completion
record is documentation only; its final publication SHA is reported separately.

All 28 original x86 root files, both sse2neon header copies and the existing
sse2neon pin retain exact raw hashes and nanosecond mtimes: 31 protected paths.
The relocated VCL manifest remains byte-exact with SHA-256
`9c3a9a777426ffac4daf8dce53673629e5b5ff5a16716089a6d2b51cb3b8212e`.
All 117 recorded runtime/test/CMake/native-runner inputs remain unchanged.
All 46 existing portable checksum manifests, covering 494 entries, passed.

The earlier 32 controls and eight selected native smoke cases keep their exact
641-file source fingerprint
`f5077bb8018aa2c9e0aa726cdedd3c76fc640fd1df17292ba7d97360b6f4745e`.
Neither publication commit acquires a new full-matrix pass. No native test,
benchmark, ASan, hosted CI or emulation was run during this publication task.

## Completed cleanup

Before removal, the original `dependency-audit-evidence.zip` again passed its
SHA-256, ZIP CRC and all 324 recorded entry hashes. Every one of the **188
noncompiled records** selected for removal had exact archive coverage, with
**zero uncovered records**. All **96 compiled artifacts** were rehashed against
the retained original inventory before removal.

Cleanup removed only eight generated control/smoke build directories:
**284 files / 55,106,943 logical bytes**, about **52.6 MiB**. Original compact
evidence retains compiled hashes rather than executables. A future run rebuilds
in fresh directories instead of treating removed binaries as available.

Retained resources include the original native logs/JUnit/commands/flags and
generated inputs in their verified packet, the complete 641-file source capture,
the earlier 625/564-file captures, pre-removal vendor archive, both reconstructed
comparison snapshots, full negative-test fixtures, upstream inspection Git
history, installed tools and pristine pinned testbench. The three complete
source manifests were verified again after removal. Tool/runtime hashes and
testbench revision/cleanliness/all three source hashes also passed. No temporary
worktree, unrelated build, branch, external consumer or evidence archive was removed.

The [portable publication/cleanup receipts](dependency-audit-history-2026-11-19/publication/README.md)
record counts, verified publication and archive identities. Full operation logs,
coverage maps and raw hash inventories are retained under ignored
`build/audit-publication-20261119-01/`. One post-cleanup bookkeeping check initially
compared the old and newly published worktree HEAD as though they should be
equal; the corrected audit verifies unchanged paths/branches separately from
the approved HEAD advance. Its original validator, failed check and correction
remain retained; neither removal nor native execution was repeated.

The historical Windows gate stays failed/incomplete: **5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. The M2 TRS-inverse costs
remain unresolved under the separate pinned follow-up. Publication and cleanup
do not alter those conclusions.
