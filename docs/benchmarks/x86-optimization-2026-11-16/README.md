# Portable optimized x86 comparison data

Read the [dated report](../x86-optimization-2026-11-16.md),
[native correctness report](../../validation/windows-amd-2026-11-16-x86-layout.md)
and [repeatable protocol](../x86-upstream-comparison.md).

- `04-before-after/`: optimized uncommitted candidate versus unmodified fork
  `44f974a`. Legacy result labels call this reference `upstream`; it is the
  pre-change fork, not original VCL.
- `05-upstream/`: the same candidate versus original VCL `6675774`.
- `edge-controls/`: separate untimed IEEE classification/large-rounding checks
  and CMake examples against the exact retained second-experiment sources.
- `verification.json`: separate ratio/source/archive audit, timestamp-only
  Clang object control and slower-case byte/placement observations.
- `alignment-diagnostic.json`: short relink-only mechanism probe, separate
  from all official comparison samples.

Each experiment retains all 816 rows as CSV/JSON, per-pair/sample ranges, source
commit or explicit base/patch identity, native executable hashes, exact harness
hashes and raw evidence archive receipt. There are no absolute machine paths,
credentials, binaries or raw command logs in these portable files. Candidate
`commit: null` is intentional; do not label it as a tested clean base commit.

Ratios are medians of four paired invocation-median ratios. Values above one
mean the candidate was slower. Case units and descriptive threshold rules are
unchanged from the original protocol. Preserve all directories and checksum
inventories when adding a later run; never pool data across source checkpoints.
Root checksum inventory covers all other files recursively; each experiment
also retains its original local checksum inventory.
