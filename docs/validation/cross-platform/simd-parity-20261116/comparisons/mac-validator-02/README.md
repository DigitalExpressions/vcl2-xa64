# Debug comparison reanalysis — validator correction

All **eight previously blocked Debug comparisons passed**, with **zero unexpected
differences**. These are offline reruns explicitly authorized by the maintainer,
using original `simd-parity-20261116` captures at
`bd7807e91f5888bc2ad708ca46c9305d166d6c4b`, tree
`2a0a5d3250201773cccb06ba0562df728f4e9387`.

The corrected validator accepts intentional UBSan termination with exit `1` or
`SIGABRT` (`-6`) on the Mac; Windows still requires exit `1`. Positive, guard and
runtime controls remain mandatory. The producer's diagnostic/no-continuation
checks and all numerical policies are unchanged. Retained raw Mac controls were
independently inspected before reanalysis.

The eight pairings cover both Windows Debug compilers against compatibility
512 and Direct Neon maxima 128/256/512. Full profile inventories are validated
before explicit width selection. The results contain 24,166 exact lane matches,
272 acceptable numerical differences, 132 documented FMA differences and 102
documented rounding differences, totaling 24,672 lanes over 3,996 repeated vector
cases. Every differing lane is retained in its comparison JSON.

Combined with the original 24 Release comparisons, all **32 planned pairings**
are covered: 96,780 exact matches, 1,074 numerical differences, 432 FMA differences,
402 rounding differences and zero unexpected differences. These totals use two
identified validator revisions, not a new capture run or universal bitwise identity.

- [All eight results and aggregate metadata](matrix-summary.json).
- [Validator identity](validator-provenance.json) and [code/test patch](validator.patch).
- [Full follow-up report](../../../../mac-arm-2026-11-16-bd7807e-validator-followup.md).
- [Original captures and first comparison attempt](../../mac-m2/README.md), retained unchanged.

The validator SHA256 is
`d1cb37b63bf56c49e75143ce192de3289381e219b4fc7a5505bde32cb719d119`;
the code/test patch SHA256 is
`acf01c07a1e7ac508e4a3b381eef6d578a81c2222c840fa682cd73dcf3a28547`.
Ten regression controls passed after reproducing the original failure. All
104 original portable files remain byte-identical; native execution was not repeated.
The original four capture failures and eight rejection records are preserved.

Raw archive: `validator-reanalysis-evidence.zip`, SHA256
`ddcfc5230a51f2c126a86d35e20149a057312342c0cd0aa84d353080a6971b7d`.
Raw evidence stays under ignored `build/`. This directory has its own checksums;
the original host checksum inventories are unchanged. The Windows whole-file
source fingerprint discrepancy remains unresolved. Nothing was committed or pushed.
