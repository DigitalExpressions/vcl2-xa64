# ARM strategy cleanup, 18 November 2026

The maintainer requested removal of obsolete local workspaces and retention of
the useful Mac ARM/Neon findings before choosing a new strategy. Source changes,
commit and push are outside this cleanup. Start with
[ARM64 / Neon lessons](../neon-arm-lessons.md) and the
[current plan](../integration-plan.md).

## Documentation

The ARM lessons consolidate M2 correctness, numerical and memory contracts,
native controls, performance interpretation, source provenance and architecture
isolation. The developer/validation indices now prioritize this material.
The old x86 execution plans and handoffs are retired, with short reference
pages retained for existing links. Dated reports and published portable
checksum inventories remain unchanged.

Before edits/removals, all documentation was captured in a verified snapshot,
including the unpublished final Windows performance report/data and earlier
uncommitted status edits. These are retained evidence, not an active x86 agenda.

## Retention and removal

Cleanup is complete. **Four temporary worktrees and 158 obsolete generated
roots were removed**, covering 196,635 files and
126,181,640,955 bytes (126.18 GB).
The rebuildable portion was 139,820 binary/object/debug
files (123.65 GB). Only the primary
registered worktree remains.

Before deletion, 56,599 unique raw record files,
1,460 worktree source files and
290 documentation files were archived and
verified. All 212 original archives were copied
byte-for-byte with matching SHA256. Executable/DLL hashes were recorded for
39,792 files. Six nested historical verification
repositories were preserved as verified Git bundles and complete raw records;
their discovery stopped the first backup pass before any deletion.

Use the [portable cleanup receipt](arm-strategy-cleanup-2026-11-18.json) for
counts, source protection and archive hashes. `original-archives.json` under
the retention root maps every old relative archive path to its retained copy.
`record-inventory.json` and the ZIP checksum inventories identify retained
records. Worktree source snapshots retain each original commit/tree, dirty
patch and exact source bytes. Removed commits remain reachable in Git.

| Retained archive | SHA256 |
| --- | --- |
| `pre-cleanup-documentation.zip` | `28ca7403b982a2f816b9fef27769ee35ae8bccc9e6a8cd94bc437df0dea84256` |
| `validation-records-attempt01.zip` | `a7eb238d49d3a0522151c88af684bec24b1f85d9a904efcea27727f43b4a3c1a` |
| `validation-records.zip` | `327a227bd142e43d9e6a733b2474d6b610a4a7123564b943e0768338f68e91d8` |
| `worktree-source-snapshots.zip` | `9493f8bfb34cad864767ddce931a4e0b0b828043f9a5bc34c27fa54c04ee7c4f` |

`validation-records.zip` is the complete record archive; the partial first
preparation archive is retained separately for provenance. Original archives
are stored under `original-archives/` with their original relative paths.
Nested repository bundles are under `nested-repositories/`. Read archived
files directly or restore into a fresh directory when needed.

The unpublished Windows performance report/data was removed from the active
documentation tree only after its exact bytes were verified in
`pre-cleanup-documentation.zip`. Existing published reports and portable
datasets remain in place, with their original checksum inventories unchanged.

## Source and verification

The primary HEAD remains `e41b0776d9e1d8ba51576bf2a17529a013753900`; index entries are unchanged.
Every originally tracked non-Markdown file retains its pre-cleanup SHA256.
All 28 pristine vendor files and the pin inventory are unchanged. Library,
backend, adaptation, test, fixture, CMake and tool sources were not edited.
Installed tools and the primary testbench at
`a7293702385e5169a5f58b83177f2a43cd59139b` remain available. Branches and remotes
were not changed, and no sibling checkout was touched.

Repository/documentation and whitespace checks passed after cleanup. Read-only
generation checks reproduce all 35 products exactly and verify all 28 pristine
vendor files. All 349 originally tracked non-Markdown files retain their original
raw hashes. No native matrix or benchmark was rerun for documentation and
build-output removal. No commit or push was performed; the cleaned documentation
is prepared for the maintainer's review.

Git's initial Windows removal hit a long-path limit after unregistering one
worktree. The continuation used command-local long-path handling and native
literal extended paths to remove verified residual output. No persistent Git
or system configuration was changed. Cleanup preparation/removal journals are
retained under the journal directory below.

Local retention root: `build/retained-evidence/arm-strategy-20261118/`.
Cleanup journals: `build/arm-strategy-cleanup-20261118/`.
The primary pinned testbench/tool resources remain installed. Raw Mac archives
remain on the Mac; this Windows cleanup does not operate on them or on the
legacy sibling checkout.
