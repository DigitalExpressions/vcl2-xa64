# Native M2 ARM planning preflight — 19 November 2026

Preflight is ready for review of the
[bounded ARM proposal](../arm-next-step-proposal-2026-11-19.md).
All required source, dependency, tool and maintenance gates passed. Execution
was limited to maintenance and runner planning; no new native matrix, consumer
probe, benchmark or runtime/test change occurred.

## Identities

| Item | Observed identity |
| --- | --- |
| Detached preflight HEAD | `5e039b01b49abc00548123763be85898932e3ad8` |
| Git tree | `ea90c1b40fc50936db7c3509aaa11ab51c02d890` |
| Complete clean source | 564 files; every raw file equals its Git blob |
| Raw source fingerprint | `fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06` |
| Fetched origin/master and handoff revision | `a04eb834c0e6707256009d007c77f4ccaf66beca`; base ancestry passed |
| New clean capture ZIP SHA256 | `44de4f5d3c3f95918d4ed4b9a23ee0b40f1b65058e22cda5908c34050b5af551` |

Capture preceded planning metadata: exact base commit/tree, empty tracked patch,
no added/deleted source paths, 564-file manifest and matching fingerprint.
The two dated Markdown documents and adjacent portable summary are later
uncommitted additions. HEAD/tree and clean fingerprint do not identify that
working inventory. Its separate final inventory is retained under ignored
`build/preflight-01/working-inventory-final.json`; no source commit was created.

## Real checks

- All 28 original root paths and 28 vendor originals match their pinned raw
  hashes, inventory and line endings. Pin remains
  `667577495efaa36a10cbb5d6cb8c942c23913848`, tree
  `36aee120c28c9c64e62903bfc81139003e37cedd`. Including both pin files and
  sse2neon alias/vendor header, all 60 protected/dependency paths retained their
  SHA256 and nanosecond modification times before/after checks.
- Native Apple M2, macOS 26.7 (25G229), native arm64 Python 3.9.6,
  Rosetta query zero, Apple Clang 17.0.0 target arm64-apple-darwin25.6.0,
  SDK 26.1. Python/compiler/CMake/CTest/Ninja contain arm64 slices. CMake and
  CTest 4.4.4, CTest JUnit option present; retained Ninja
  1.11.1.git.kitware.jobserver-1 and installed make available. Actual page size
  is 16,384 bytes. Absolute paths, tool hashes and raw output remain ignored.
- Retained pristine testbench verified at
  `a7293702385e5169a5f58b83177f2a43cd59139b`: clean including untracked
  inventory and all three runner `SOURCE_HASHES` match. Reuse its existing
  directory with `--testbench-dir` in a future run. Tools/dependencies were
  neither installed nor replaced. Their retained paths require an explicit
  task-local PATH when starting a future validation shell.
- `python3 -B tools/regenerate_headers.py --check`: exit 0, 28 originals/28
  vendor files exact, generation retired, zero generated products.
- `python3 -B tools/check_repository.py`: exit 0, including documentation links
  and source integrity; rerun after planning metadata. This is a static include
  boundary check, not a new compiler include trace.
- `python3 -B tools/check_header_generation.py`: exit 0, all 11 controls passed.
  Disposable mutated inputs/results remain under the worktree's ignored
  `build/header-generation-check-20261119T084423669068Z/`. The intentional
  writer rejection retained actual exit 2. Actual originals were untouched.
- `python3 -B tools/run_native_arm_validation.py --plan`: exit 0, eight profiles,
  8,036 case instances, 60-second deadlines. This prints expected registrations;
  it neither configures CTest nor establishes a native pass. Current runner
  concurrency policy remains three.
- Seven historical portable bundles passed all 112 listed hashes: cleanup M2
  (18), earlier separation M2 (16), publication (2), Windows matrix (38),
  Windows triage (20), integration (5), earlier M2 correctness (13).
  Benchmark metadata separately passed eight hashes. Historical manifests and
  reports were read without edits.
- The original retained 474-file M2 capture verifies its own fingerprint
  `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`.
  Its source ZIP retains SHA256
  `3f20dbd2655a4fca5795359281e36267403b6e038cc08cb4f4e5d05519263310`;
  raw packet retains SHA256
  `ffcefcd5b32984c94a4f5f68681b40bb3c87936a34460ed20cb67fa6414dfe55`.
  No prior inventory was regenerated or replaced.

Planned counts per mode are compatibility 512: 1,350; Direct 128: 486;
Direct 256: 890; Direct 512: 1,292, each Release and strict UBSan Debug.
The future runtime task must re-freeze source and actually execute the full
baseline and relevant new checks. Compatibility stays default; Direct is
experimental. No missing required tool/dependency gate was found.

## Readiness limits and preservation

The recommended task specializes four Direct four-lane blends used by transpose
in ARM-owned `vectori128_perm_neon.h`, retaining exact bits and the fallback.
The proposal compares cross-product shuffle and compatibility reduction
alternatives, specifies the numerical/memory contract and future acceptance.
A performance benefit remains a hypothesis.

The 8,036-pass/eight-example M2 result belongs to the earlier cleanup capture,
not the integrated base. Windows publication retains its distinct 497-file
fingerprint and failed/incomplete result (5,387 passed, 249 failed, 76 not run
of 5,712, plus twelve examples). No new cross-host parity or native pass is
claimed. ASan, physical Intel, other toolchains, FP16/SVML, universal builds
and external consumers retain their earlier limits.

Session root, relative to the original checkout:
`build/m2-arm-planning-20261119T084235Z-01/`. The detached worktree is its
`checkout/`; raw source/tool/dependency/protected inventories are in
`checkout/build/preflight-01/`. Full command/exit journals and stdout/stderr
also reside at the session root. The immutable clean capture is
`checkout/build/preflight-01/source-capture-01/`, with source ZIP, manifest,
patch and receipt. All worktrees, branches, local work, installed resources,
raw evidence and failures are retained. No reset/stash/clean/prune, cleanup,
commit/push/merge/PR, saved configuration edit or external consumer edit occurred.

An interrupted earlier inspection/fetch preceded the full session journal.
The restarted preservation snapshot was saved before the subsequent journaled
non-pruning fetch; the local sequence note retains that distinction. The
primary checkout observed at resumed inspection was already at `a04eb83`.
This task did not advance or switch it. A read-only historical-path lookup
failed because the helper had been renamed; the failure and corrected path
comparison are retained. The diff's exit 1 means differences were found, not
an integrity-gate failure. No required preflight gate failed.

The [portable preflight summary](m2-arm-preflight-2026-11-19/README.md) has
checksums and machine-path-free metadata. Its checksummed ZIP remains at
`checkout/build/preflight-01/m2-arm-preflight-2026-11-19.zip`, with receipt.
Raw command paths and installed tool inventories are excluded from that packet.
Source capture and control inputs remain separate retained local artifacts.
