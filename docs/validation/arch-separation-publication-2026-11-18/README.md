# Architecture separation publication — 2026-11-18

The maintainer approved committing/pushing the reviewed implementation and M2
reports on `arch-separation-m2`. This publication leaves `master`, fork/remotes,
historical reports, original source IDs and raw evidence intact. No merge, PR,
branch retirement, benchmark or evidence deletion is included.

The [M2 cleanup report](../mac-arm-2026-11-18-arm-header-cleanup.md) records
8,036/8,036 CTest passes and eight examples on frozen capture `cleanup-01`,
fingerprint `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
That result stays tied to its capture, not to the later publication commit.

`source-comparison.json` inventories every one of the 474 captured paths:
463 match exactly and 11 differ only in documentation or `.gitattributes`
(checkout rules for portable report metadata). Runtime headers, tests, tools,
fixtures, dependencies, build files and numerical controls are byte-identical.
Added paths are reports/portable metadata only. Historical checksummed bundles,
protected root/vendor bytes, pin inventory and original evidence remain unchanged.
No new native matrix was needed for these publication-only metadata edits;
repository/link/whitespace, source/hash and checkout-byte checks apply instead.

The [Windows handoff](../windows-arch-separation-handoff.md) supports a fresh
detached worktree at the exact publication commit supplied in the prompt. It
requires per-path comparison and a fresh complete Windows published-source
snapshot. Record its own fingerprint; explain any proven metadata/newline
transformations without replacing the original Mac inventory. Protected files
always require raw-byte equality. The Mac source ZIP is an optional alternative,
not a prerequisite for the Git route. Native Windows remains pending.
