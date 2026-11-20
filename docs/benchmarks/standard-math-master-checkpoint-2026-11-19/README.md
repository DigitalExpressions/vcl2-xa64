# Current-master M2 benchmark checkpoint — measured 19 November 2026

The 160-row [results.csv](results.csv) and [metadata.json](metadata.json) are
byte-exact copies of the published current-recovery-01 baseline at
master/control d796768b30cb375ea62afa72dcab061b1a3e54ca, tree
0fad402083c42891850639c4c0242b9d2d8ab70f. They retain every three-way result,
including slower rows and invocation ranges, with source and tool identities.
They describe historical measurements, not a new run or a universal speedup.

The [full report in the validation publication](https://github.com/DigitalExpressions/vcl2-xa64/blob/31ab4c4ee5a758ddc7b6f980f235b36848ca5290/docs/benchmarks/STANDARD_MATH_RESULTS_2026-11-19_TRS_CURRENT_RECOVERY_BASELINE.md)
uses STANDARD_MATH_RESULTS_TEMPLATE.md. Published candidate/control comparison
data remain in that Git history; importing the candidate branch would also
import the declined runtime and must not be used for a documentation closeout.

On 20 November the maintainer requested discarding obsolete local Mac raw
sessions, binaries and source captures without replacement archives. The
metadata's schedule/retention wording describes the original experiment;
those raw artifacts are no longer locally available after this cleanup.
This compact table is a reference checkpoint. Future timing comparisons need
a fresh matched baseline and candidate session, with separate correctness
checks; do not compare new timings directly against this historical table.

The [TRS decision](../../validation/trs-inverse-decision-2026-11-20.md)
records the decision and limitations. [Checksums](checksums.sha256) cover the
three retained checkpoint files.
