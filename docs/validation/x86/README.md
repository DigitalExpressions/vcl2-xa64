# x86 validation

Use the [shared validation index](../README.md) for current status and limits.
The full Windows correctness gate remains failed/incomplete.
Use the [results publication checklist](results-publication-checklist.md) after
the correctness round and any requested separate performance tests. At the
start of each new validation request, ask whether to include performance tests
and record the answer for that round.

- [Windows runner guide](windows-runner.md) and
  [MSVC/Clang build and test guide](../../x86-build-and-test.md).
- [21 November native Windows validation report](windows-x86-validation-2026-11-21.md):
  clean commit `dc802e4`; 6,798 passed, 233 failed and 109 not run across 7,140
  registrations, plus fifteen passing examples. All case statuses match the
  20 November collection; the correctness gate remains failed/incomplete.
- [20 November integrated harness report](windows-x86-harness-integration-2026-11-20.md)
  and [portable results](windows-x86-harness-integration-2026-11-20/README.md).
- Historical [earlier 20 November optimized-profile report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20.md)
  and [frozen portable results](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20/README.md).
- [19 November architecture matrix](windows-amd-2026-11-19-arch-separation.md) and
  [targeted triage](windows-amd-2026-11-19-arch-separation-triage.md).
- [Four-blend Windows return](windows-blend4-handoff-2026-11-19/results/README.md)
  and [integration receipt](windows-blend4-handoff-2026-11-19/integration/README.md).
- [16 November Windows checkpoint](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-amd-2026-11-16-bd7807e-final.md) and
  [Windows archive-recovery receipt](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/arm-strategy-cleanup-2026-11-18.md).

The frozen cleanup receipt records Windows archive recovery despite its ARM
strategy title. Shared [cross-platform procedures](../cross-platform/README.md)
and the architecture acceptance decision stay at the validation root. Closed
comparison, dependency-audit and publication records are cited through frozen Git.

Frozen helper scripts and source inventories inside result bundles retain
their historical checkout-relative paths and accepted hashes. Use them with
their original frozen source, as documented; relocation does not adapt them
for current-master validation or establish a new native pass.
