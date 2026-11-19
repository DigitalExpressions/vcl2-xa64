# Architecture separation integrated and cleanup completed — 19 November 2026

The approved architecture separation and reviewed Windows matrix/triage reports
were integrated into local `master` by fast-forward at `a1b149a9e4d231244926e149bdc12ec8b6b76416`, tree
`5393228fe8e3e0aa534026d50149e46d3007708e`. The [acceptance decision](architecture-separation-integration-2026-11-19.md)
defines x86 preservation/equivalence scope. This completion record changes only
documentation and checkout rules; it does not claim a native pass at a later commit.

The original gate stays **failed/incomplete: 5,387 passed, 249 failed, 76 not run
out of 5,712 instances**, plus twelve passing examples. Tests and numerical gates
are unchanged. The [matrix](windows-amd-2026-11-19-arch-separation.md) and
[triage](windows-amd-2026-11-19-arch-separation-triage.md), with their portable
checksums, remain exact. No runtime/test/runner change or native replay occurred.

All 28 root originals, 28 vendor originals, pin and dependency bytes matched the
frozen capture after integration and cleanup. The immutable Windows 497-file
capture still verifies fingerprint
`d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2`;
its protected mtimes stayed unchanged. The Mac capture identity remains separate.
All 490 unchanged publication paths matched exact raw bytes on integrated master.
Repository links, integrity and whitespace checks passed.

Git initially retained the older primary checkout's CRLF bytes for the unchanged
`LICENSE` blob, plus 63 nonprotected unchanged paths. The original representations
were preserved and checksummed before checking those paths out from exact Git
blobs using command-scoped LF settings. No protected blob, pin, capture or
historical report content changed. Saved Git configuration remains unchanged.

## Completed local cleanup

Every original main, triage, review and source archive passed its checksum, CRC
and per-entry checks before removal. All **5,688 noncompiled records** selected
for removal already had verified archive coverage: **zero uncovered records**.
The retained inventory also records SHA256 for **20,070 compiled artifacts**.
Removal covered **43 generated build directories and two individual build files**,
**25,758 files / 16,102,326,211 logical bytes** (about 15.00 GiB).

The clean temporary worktree's **558 raw files** matched Git blobs reachable
from master; it contained no untracked/ignored files and was removed with Git.
Only the primary worktree remains. Original source capture/manifests, all original
archives (including four runner return archives), failures, diagnostic/probe
sources, tools and pristine pinned testbench remain. Unrelated builds and all
branches/remotes were preserved. Compiled objects/binaries/debug symbols were
removed; compact evidence retains their hashes and reachable source, so a future
run must rebuild in fresh directories.

Raw integration/cleanup evidence remains under ignored
`build/integration-20261119-01/`. Its compact packet
`integration-cleanup-evidence.zip` has SHA256 `38dafa9ce41dc727370acd20770bc2ed45b8e4acc69a53c1217ce8c4d29592da`;
all 194 recorded files passed CRC and per-entry hashes.
The [portable receipt and summary](architecture-separation-integration-2026-11-19/README.md)
identify actual counts, preserved sources and archive hashes without machine paths.
No push, PR, branch retirement, benchmark, emulation or consumer edit occurred.

The next development step is to select a bounded ARM/Neon improvement and its
API/numerical contract from the [retained lessons](../neon-arm-lessons.md), work
only in ARM-owned sources, and freeze a new checkpoint for relevant native M2
validation. Default compatibility and explicit experimental Direct Neon remain.
