# Mac Debug comparison follow-up — 2026-11-16

## Outcome and source identity

**All eight previously blocked Debug comparisons passed**, with zero unexpected
differences. Combined with the original 24 Release results, all **32 planned
Windows/Mac pairings** now have successful comparisons.

The maintainer explicitly authorized fixing the validator and rerunning these
eight comparisons. This is offline reanalysis of existing, immutable captures
from `simd-parity-20261116`, source
`bd7807e91f5888bc2ad708ca46c9305d166d6c4b`, tree
`2a0a5d3250201773cccb06ba0562df728f4e9387`. No native tests or captures were
repeated. The 8,036 native matrix passes, eight examples and 3,996 independent
corpus reference passes retain the source and attempts in the
[original Mac report](mac-arm-2026-11-16-bd7807e-final.md).

The validator correction is an uncommitted patch based on report checkout
`3c6d5617ceebf4e3a31b2b0fdc26a893af371dc8`. It is identified separately from
the captured source:

- Validator SHA256: `d1cb37b63bf56c49e75143ce192de3289381e219b4fc7a5505bde32cb719d119`.
- Two-file code/test patch SHA256: `acf01c07a1e7ac508e4a3b381eef6d578a81c2222c840fa682cd73dcf3a28547`.
- [Portable provenance and patch](cross-platform/simd-parity-20261116/comparisons/mac-validator-02/README.md).

## Correction, controls and comparison results

Strict UBSan control validation now accepts exit `1` or `SIGABRT` (`-6`) for
`mac-m2` bundles; Windows still requires exit `1`. Both hosts still require
successful positive controls. Missing controls, zero/unknown negative exits,
timeouts, kills, segmentation faults, bus errors and Windows access violations
are rejected. Existing runtime and guard checks are unchanged.

The capture producer already checks the intentional-overflow diagnostic and
absence of continuation. All four retained Mac Debug control logs were verified
again: positive exit zero, negative exit `-6`, expected signed-overflow diagnostic,
and no `UB continued` output. Their original raw archive checksums also match.
No exit code, manifest, raw output word, checksum or capture failure was edited.

The new regression reproduced the rejection before the fix. After the fix,
all **10 comparator controls passed**, including Mac exit `1`/`-6`, Windows
exit `1`, rejection of Windows `-6`, unrelated signals/timeouts, missing controls
and failed positive controls. Corpus regeneration verification and repository
checks also passed. Syntax-tree comparison confirms that only `load_bundle`
changed: reference validation, numerical bounds, rounding/FMA policies, comparison
classification and the CLI are unchanged.

Each Windows Debug profile (MSVC and Clang strict UBSan) was compared with
compatibility 512 and Direct Neon maxima 128/256/512. Both complete input
inventories were revalidated before the explicit width scope was applied.

| Lane classification | Eight Debug reruns | Combined with original 24 Release comparisons |
| --- | ---: | ---: |
| Exact bit matches | 24,166 | 96,780 |
| Numerically acceptable differences | 272 | 1,074 |
| Documented FMA differences | 132 | 432 |
| Documented rounding differences | 102 | 402 |
| Unexpected differences | 0 | 0 |
| Total compared lanes | 24,672 | 98,688 |
| Repeated vector cases | 3,996 | 15,984 |

The combined result uses the original frozen validator for 24 Release pairings
and the identified correction for eight Debug pairings. It is not a new native
run or a pristine 32-pair frozen-validator attempt. All earlier rejection records
remain under `mac-m2/comparisons-01/`; the new results are a separate analysis
attempt under `comparisons/mac-validator-02/`. Passing declared contracts does
not assert universal bitwise identity. The unresolved Windows whole-file source
fingerprint discrepancy and other coverage limits remain as originally reported.

## Evidence and review

All 104 original portable files, including captures, manifests, summaries,
checksums and rejection records on both hosts, were verified byte-identical.
The frozen validation worktree remains clean. New portable results include
every differing lane, all eight pairing records, the validator/test patch,
provenance and their own complete checksum inventory.

Raw evidence: `build/validator-fix-simd-parity-20261116/validator-reanalysis-evidence.zip`;
SHA256 `ddcfc5230a51f2c126a86d35e20149a057312342c0cd0aa84d353080a6971b7d`.
It retains the pre-fix regression failure, passing controls, eight CLI invocations,
source/patch hashes, original control logs, immutable-input proof and new results.
Archive CRCs were verified. Original native/capture evidence remains in place.

The code fix, regression tests and follow-up results are prepared for review.
Nothing was committed, pushed, merged or published. Future native captures
continue to require an approved frozen source shared by both hosts; this
authorized offline reanalysis does not relabel the captured source revision.
