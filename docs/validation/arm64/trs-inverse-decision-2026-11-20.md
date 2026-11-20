# TRS-inverse decision — 20 November 2026

Keep current master runtime **d796768b30cb375ea62afa72dcab061b1a3e54ca**
(tree 0fad402083c42891850639c4c0242b9d2d8ab70f).
Do not integrate frozen candidate **3b4af0dc3e02e043f851022fd2db1f68895e3536**
unchanged. The Mac investigation and Windows return are complete; the earlier
execution handoffs are historical. No new runtime work is scheduled here.

## Performance tradeoffs

Master already retains the approximately 6% small-transpose throughput gain and
its accepted 2.2% / 2.7% inverse elapsed costs. In the later matched M2 session,
the candidate improved FP32 AoS inverse elapsed medians by 3.153% / 3.071%
at 128 / 4,096 objects, with overlapping invocation ranges. Small-transpose
throughput changed -0.448% against master, also overlapping.

The candidate's FP32 SoA dot elapsed costs at 4,096 objects were material:
12.434% at width 128 against master (disjoint invocation ranges), and 6.446%
at width 256 (overlap). Against pre-blend, those costs were 12.601% / 10.343%,
both disjoint. Small-transpose throughput gained 6.540% against pre-blend
(disjoint); inverse elapsed changed -0.961% / +1.087% (overlap). FP64 small
transpose elapsed costs were 3.043% against master and 4.170% against pre-blend
(overlap). No application workload weighting establishes a net benefit.

All twelve corresponding historical/current Release executables were
byte-identical, but the old integrated/candidate dot128 comparison recorded
+0.717% elapsed versus +12.434% in the later session. The mechanism remains
unresolved. The measurements cannot be dismissed as noise or attributed to an
instruction or placement cause. The review accounted for every slower row:
159 slower Neon pairs and 47 within-source rows slower than scalar. Published
full tables remain in Git history; this note selects the material tradeoffs.

Windows remains **failed/incomplete per source: 5,387 passed, 249 failed,
76 not run of 5,712, plus twelve passing examples**. Complete named statuses
match; candidate/control timeout subtypes differ in three cases (four versus
one timeout). Six matching RTC first-failure captures are diagnostics, not
passes or reconstructions of all original wait stacks and later behavior.
Bounded movement/code preservation results do not convert the full gate.

## Source and validation references

- [Current-master benchmark checkpoint](../../benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md):
  all 160 historical baseline rows, invocation ranges, source/tool metadata and
  checksums. A future experiment needs a fresh matched baseline, not ratios
  against these historical timings.
- [Published M2 report and source mappings](https://github.com/DigitalExpressions/vcl2-xa64/blob/31ab4c4ee5a758ddc7b6f980f235b36848ca5290/docs/validation/mac-arm-2026-11-19-trs-inverse-current-master.md).
  The actual native candidate input was an 807-file uncommitted capture,
  fingerprint 9dda5bfc685c7e72102368bbff7d4bba3b01834e7fc86ec70429ace9575fc93f,
  with tested commit/tree null; its 8,036-case/eight-example result is historical
  and is not transferred to a later publication.
- [Windows return and RTC records](https://github.com/DigitalExpressions/vcl2-xa64/tree/b5a671a72fe59888ce0a661741a2b65c6b02a4b6/docs/validation/trs-inverse-current-master-2026-11-19).
  Exact publication 31ab4c4ee5a758ddc7b6f980f235b36848ca5290 and Windows return
  b5a671a72fe59888ce0a661741a2b65c6b02a4b6 retain their own source identities.

Obsolete raw Mac sessions, source captures and benchmark binaries were discarded
at the maintainer's request. Exact raw-trial/binary reinspection is no longer
available locally. The [cleanup report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md#completed-local-workspace-cleanup)
records disposal. This completed decision schedules no new execution and claims
no fresh native pass or timing measurement.
