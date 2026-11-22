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
- Historical [earlier 20 November optimized-profile report](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20.md)
  and [frozen portable results](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-upstream-optimized-2026-11-20/README.md).
- [16 November Windows checkpoint](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/windows-amd-2026-11-16-bd7807e-final.md) and
  [Windows archive-recovery receipt](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/arm-strategy-cleanup-2026-11-18.md).

The frozen cleanup receipt records Windows archive recovery despite its ARM
strategy title. Shared [cross-platform procedures](../cross-platform/README.md)
stay at the validation root. Current [backend acceptance rules](../../backend-layout.md#validation-and-source-identity)
retain the original x86 scope and limits. Closed comparison, separation,
dependency-audit and publication records are cited through frozen Git.

Frozen helper scripts and source inventories inside result bundles retain
their historical checkout-relative paths and accepted hashes. Use them with
their original frozen source, as documented; relocation does not adapt them
for current-master validation or establish a new native pass.

## Historical separation records

The completed job's [19 November matrix](https://github.com/DigitalExpressions/vcl2-xa64/blob/7df464937b38260f29bca1dd7e1096c7853ca812/docs/validation/x86/windows-amd-2026-11-19-arch-separation.md),
[targeted triage](https://github.com/DigitalExpressions/vcl2-xa64/blob/7df464937b38260f29bca1dd7e1096c7853ca812/docs/validation/x86/windows-amd-2026-11-19-arch-separation-triage.md)
and complete [matrix](https://github.com/DigitalExpressions/vcl2-xa64/tree/7df464937b38260f29bca1dd7e1096c7853ca812/docs/validation/x86/windows-arch-separation-2026-11-19)
and [triage](https://github.com/DigitalExpressions/vcl2-xa64/tree/7df464937b38260f29bca1dd7e1096c7853ca812/docs/validation/x86/windows-arch-separation-triage-2026-11-19)
portable bundles were retired locally on 21 November. The historical gate stays
failed/incomplete: 5,387 passed, 249 failed, 76 not run of 5,712, plus twelve
passing examples. Selected original/facade comparisons do not establish universal
correctness or parity. See the [retirement record](../README.md#x86-separation-documentation-retirement)
for preservation scope. The later build reset discarded the earlier local raw
Windows payloads; the [22 November report](windows-x86-validation-2026-11-22.md)
identifies the fresh evidence retained under ignored `build/`.

## Build reset and fresh native collection — 2026-11-22

Fresh native AMD Windows collection at clean 08f2d2903d9bfdfdb22aa187329e99ce0f44d177: 6,796 passed, 235 failed and 109 not run of 7,140, plus fifteen passing examples. The gate remains failed/incomplete.
Read the [reset report](windows-x86-validation-2026-11-22.md) and [separate performance report](../../benchmarks/x86/x86-upstream-2026-11-22.md). All prior local Windows build evidence, including the archives described in historical packets, was discarded under explicit reset authorization; those dated availability statements are superseded for this host. Fresh evidence belongs to its own source and session.
