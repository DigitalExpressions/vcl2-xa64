# Four-blend integration and M2 follow-up — 19 November 2026

The maintainer approved retaining the four-blend candidate, integrating the
reviewed Windows evidence into `master`, publishing to origin and cleaning
rebuildable local artifacts after evidence verification. The known FP32 AoS
TRS-inverse elapsed costs, 2.2% / 2.7% at 128 / 4,096 objects, are accepted for
this integration and assigned to a [separate M2 follow-up](m2-trs-inverse-follow-up-2026-11-19.md).
They have not been fixed. The approximately 6.0% small-transpose throughput gain
and the measured costs retain their original [benchmark provenance](../benchmarks/STANDARD_MATH_RESULTS_2026-11-19.md).

## Integrated source and actual validation

Local `master` fast-forwarded from `a04eb834c0e6707256009d007c77f4ccaf66beca`
to implementation/report commit `ccde9e43674b77975d1f375d1ecff02aa761aef3`,
tree `1f174faec6d791f92936bf9b8524d62af2435819`. The source candidate is
`0c69b9992d0fac09f71482a0db403276cdd8f1a1`, tree
`fd580a67ad4707116dbc7bf7b230050d635f8704`. The report commit changes only
documentation/results; it does not create a new native pass.

The [Windows report](windows-blend4-handoff-2026-11-19/results/windows-amd-2026-11-19-blend4.md)
preserves the complete gate: **5,387 passed, 249 failed, 76 not run of 5,712,
plus twelve passing examples; failed/incomplete**. All per-case pass/fail/not-run
statuses match the pinned original-source baseline. Eight failures timed out,
seven more than in the historical run; paired current-control and RTC evidence
preserve that distinction. This is not universal x86 behavioral equivalence.

The separately bounded assessment passed 96 isolated regression instances and
96 selected generated-code comparisons. Include traces kept ARM headers out of
the x86 graph. All 60 protected originals/dependency paths retained exact raw
bytes and nanosecond mtimes through the fast-forward. The original M2 and
Windows captured-source identities remain separate from publication metadata.
No numerical gate, fixture, runner or source was repaired or weakened.

The next M2 work starts from the integrated implementation/report commit above,
using fresh worktrees/captures and matched native measurements. Its objective is
to recover inverse performance while retaining transpose gains and correctness;
a successful fix is not assumed. Any proposed runtime change requires its own
native M2 matrix and subsequent Windows preservation verification.

## Publication and local cleanup

This decision and the M2 handoff are later documentation. Their publication SHA
must be recorded separately from the tested candidate and report commit. The
approved push is only to the fork's `origin/master`, without force or history
rewrites. All other branches, remotes and reusable resources are retained.

Before cleanup, the original main evidence packet and both late supplements
passed archive checksums, CRC and all recorded entry hashes again: 7,835, 56 and
seven recorded files respectively. All 44 portable checksum manifests (482
entries) passed. The failed first packaging attempt remains preserved.
Cleanup must additionally account for every selected noncompiled file and
verify recorded binary hashes before removing builds. Remove the two clean
temporary worktrees with Git only after their commits are reachable from
master and their raw tracked bytes match Git blobs. Preserve source captures,
logs, failures, generated inputs, archives, manifests, hashes, installed tools,
the pristine pinned testbench, unrelated work and branches.

At preparation time the first publication and cleanup are pending. A completion
addendum will record actual pushed identities, removal counts and retained
archive receipts after those actions; this plan is not a cleanup result.
