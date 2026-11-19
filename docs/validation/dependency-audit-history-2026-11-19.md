# Dependency revision ledgers and VCL deduplication — 19 November 2026

The maintainer approved replacing the duplicate VCL vendor snapshot with
separate root ledgers: [x86](../../audit_x86_history.md) and
[ARM64 compatibility](../../audit_arm64_history.md). Each records one verified
upstream revision per row, newest/current first, with actual project reports.
No example revision was added. This is a source-layout/provenance change, not
a dependency upgrade, numerical repair, performance result or new native pass.

## Source preservation and ownership

The 28 duplicate files under `vendor/vectorclass/` were removed after archiving
and verifying every byte. Their original pin moved byte-exact to
[audit/vectorclass-2.02.04.json](../../audit/vectorclass-2.02.04.json); SHA-256
remains `9c3a9a777426ffac4daf8dce53673629e5b5ff5a16716089a6d2b51cb3b8212e`.
The upstream revision remains VCL 2.02.04, commit
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. All 28 shipped original root files
retain their raw bytes and mtimes; no runtime or native test/runner input changed.

`tools/regenerate_headers.py --check` now verifies each root original against
its retained SHA-256, Git blob/mode and complete upstream tree. Generation and
`--write` remain retired. The new `tools/restore_upstream_vcl.py` reconstructs
an independent include directory under ignored build from those verified
original bytes. It verifies/reuses an existing exact snapshot and rejects
changed, missing, extra or unsafe destinations without overwriting evidence.
Consumers still need no Python, network download or generation.

Historical captures, checksum inventories and native results retain their old
layout and hashes. Maintenance checks retain support for inspecting historical
vendor-layout snapshots. One historical report's relative inventory link was
redirected to that exact inventory in the prior published Git commit so it
remains readable after relocation; its results and numerical data are unchanged.

## sse2neon provenance discovery

The retained header content matches [upstream commit 2272251](https://github.com/DLTcollab/sse2neon/commit/2272251cc3ad9cba8b8c47c4065811cfb396b802),
dated 18 October 2026, described by Git as `v1.9.1-27-g2272251`. It is a
post-v1.9.1 development snapshot, not a tagged release. Its tree is
`09647ade30f10df942613660636f17bfe1372cea`.

Upstream raw blob `fbfc9da84fe249f5f1d7be559e87c6a94640a6c2` has SHA-256
`932e41317cc325846256695d2aa83812ff6b6d065c8994b73a461efaf1f10bc3`.
The retained blob `6e8b1fb79bc948de9775f800f5084ed8219683ac` has SHA-256
`cd875ec9ec508a41d408424a5bb6fc4b6eecb6b9baee46e255cb7b53b7b1c3ff`.
Exact reconstruction proved that UTF-8 to Windows-1252, LF to CRLF and removal
of the final newline explain every byte difference. There are no source-content
edits beyond that representation. This analysis did not normalize either copy.

The root/vendor headers and existing `vendor/sse2neon.json` stay unchanged.
The [new audit record](../../audit/sse2neon-2272251.json) identifies the actual
upstream revision separately from the old fork-local retention commit. Offline
integrity checks verify the original pin, alias equality and reconstructed
upstream SHA-256/blob, without fetching or writing protected files.

## Validation and remaining scope

The following bounded checks passed:

- 15 header/dependency integrity controls, including rejection of altered root
  bytes/newlines, altered pins, reintroduced vendor copies, dependency changes,
  retired writers and ARM/x86 graph leaks. A positive historical-layout control
  verifies that earlier frozen vendor snapshots remain readable.
- Nine comparison-snapshot lifecycle controls: creation, no-write reuse,
  changed/missing/extra-file rejection, destination bounds and rejection of a
  changed source before creating a destination. Rejected inputs remain retained.
- Eight frozen-source positive/negative controls, repository/link checks and
  whitespace checks. Existing checksummed portable evidence remains exact.
- Eight selected native x64 smoke cases: facade and reconstructed-original
  routes in MSVC Release/Debug, then GNU-driver Clang Release and strict
  nonrecovering UBSan Debug. Raw movement payloads and arithmetic outputs match.
  All eight binaries are x64 PE. Recorded Ninja dependency graphs contain no
  ARM, sse2neon or retired adaptation/backend headers; the original route reads
  the reconstructed pinned snapshot.
- Four runtime/guard control profiles: positive exits zero, actual page size
  4,096 bytes and all sixteen protected boundary probes fault `0xc0000005`.
  Strict UBSan has positive exit zero and deliberate overflow exit one with its
  diagnostic and no continuation. Its static release CRT and strict flags remain
  intact. All installed tool/runtime hashes match the retained prior inventory.

Each private smoke project registered 40 imported canonical cases and two new
smoke cases; only the two named smoke cases were selected. The full canonical
matrix was not executed, and unbuilt canonical targets are not counted as passes.
The frozen smoke input was an uncommitted **641-file** capture, fingerprint
`f5077bb8018aa2c9e0aa726cdedd3c76fc640fd1df17292ba7d97360b6f4745e`,
based on `9a11b6a1fd1c1882c95af958fd43dc56c235d82a`, tree
`8b054ca83083127a7776d4fbae987f1ed193205a`. Tested commit/tree are null;
later report/checkout metadata is outside that input and acquires no native pass.
All runtime headers/sources, canonical tests/fixtures, CMake files and native
runners remain raw-byte identical to the base. The old duplicate vendor files
are the only removed library-source copies; shipped originals are untouched.

Two supplemental include-evidence checks initially rejected how Ninja presents
MSVC dependencies: it consumes `/showIncludes` lines, then records relative
paths in its database. The original validators, logs and passed CTest results
remain retained. Offline database inspection with resolved paths completed the
evidence check; no completed native case or output check was replayed.

[Portable results and receipts](dependency-audit-history-2026-11-19/README.md)
record source, preservation and native counts. Raw evidence, the pre-removal
vendor archive and upstream inspection data remain under ignored
`build/audit-history-20261119-01/`. No compiled file or earlier evidence was
removed. Branches, HEAD, index, worktrees, remotes and saved Git configuration
remain unchanged.

The historical Windows gate remains failed/incomplete: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples. The selected x86 preservation
assessment and native M2 results keep their separate captured-source identities.
The 2.2% / 2.7% TRS-inverse performance follow-up remains unresolved and uses its
already pinned historical M2 handoff base; no timing or ARM execution occurs here.
No commit, push, merge, branch retirement or evidence cleanup is included.
