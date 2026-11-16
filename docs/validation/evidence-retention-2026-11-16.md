# Compact evidence retention — 2026-11-16

The maintainer approved keeping compact verification records and reusable test
resources, then removing rebuildable outputs and the completed Mac validation
worktree. This is a storage cleanup, not a new validation pass.

## Removed and retained

The clean detached worktree `build/final-mac-bd7807e/` was removed through Git.
It occupied about **1.5 GB**, mostly compiled objects, test executables and debug
symbols. It contained no unique commit or uncommitted source work. Frozen commit
`bd7807e91f5888bc2ad708ca46c9305d166d6c4b`, tree
`2a0a5d3250201773cccb06ba0562df728f4e9387`, remains reachable from `master`.
Future checks can create a fresh worktree at an approved source and rebuild.

Before removal, every build record was matched by path, size and SHA256 to an
archive copy. **9,000 records were preserved**: 8,446 in the original native
packets, 17 in the existing combined archive, and 537 in a new supplement.
All nine original per-run evidence packets were confirmed byte-identical to
the nested copies in the unchanged combined archive; ZIP CRCs also passed.

The retained records include commands/exits, compiler/OS/source details,
compile/link flags, case inventories, JUnit and raw test output, binary hashes,
controls, original failures, output bits, input files and generated diagnostic
or reference sources. NUL-delimited Git logs were preserved as records too.
The new supplement also contains `_retention/coverage.json`, mapping every
retained record to its archive location and original hash.

Only rebuildable binary/cache files and their generated directories were
discarded. Original archives, published portable data/checksums, dated reports,
tracked source, fixtures, generators, runners, the pinned testbench and local
CMake/CTest/Ninja installations remain intact. Older migration and historical
build directories outside this worktree were not part of this cleanup.

## Archive locations

All archives remain local under ignored `build/`; they transfer separately.

| Archive | Location under `build/` | SHA256 |
| --- | --- | --- |
| `mac-final-evidence.zip` | `final-mac-control-simd-parity-20261116/` | `6ded9321c727c8c847aecf60ddb0caddab942f117dcc8927f1167ae595c5508c` |
| `validator-reanalysis-evidence.zip` | `validator-fix-simd-parity-20261116/` | `ddcfc5230a51f2c126a86d35e20149a057312342c0cd0aa84d353080a6971b7d` |
| `retained-text-records.zip` | `worktree-retirement-20261116T201424Z/` | `e014ae41ea98ee8fb67e6e2784f5e1af1424f356403a9899357c8107a5aa373f` |

Original per-run archives are nested at `native/<original-run>/return-evidence.zip`
inside `mac-final-evidence.zip`. Their original names and checksums are unchanged.
The supplement is about 3.2 MB; it preserves previously uncovered records without
creating a full archive of the compiled build. Cleanup manifests, coverage checks
and the worktree-removal receipt remain beside it.

## Current retention policy

Keep complete builds until their records and failures have verified archive
coverage. After a completed run and explicit cleanup authorization, generated
objects, executables, debug symbols and temporary worktrees can be removed.
Keep source commits reachable, original binary hashes and archive receipts,
portable outputs, fixtures, dependency checkouts and reusable tools.

Compact evidence is an audit record, not a backup of the original executables.
New native passes rebuild in fresh directories and record their own source,
compiler, cases and attempts. Historical reports retain their execution-time
paths and original results; this follow-up records the later storage change.
No historical result, tolerance, manifest or checksum was rewritten, and no
pull request, remote change, history rewrite or upstream push was performed.
