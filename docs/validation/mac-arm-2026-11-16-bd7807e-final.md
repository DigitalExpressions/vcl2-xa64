# Native Mac M2 final validation — 2026-11-16

## Outcome and tested source

**Native matrix passed; cross-platform comparison incomplete.** The full selected
matrix passed **8,036/8,036 CTest cases**, plus eight examples. Separately, all
**3,996/3,996 corpus vector cases** (24,672 lanes) passed independent references.
Four Debug capture invocations then failed frozen bundle validation because
Apple UBSan's intentional-overflow control aborts with `SIGABRT` (`-6`), while
the shared loader requires exit code `1`. These are retained workflow failures,
not numerical mismatches or missing captured cases.

- Run ID: `simd-parity-20261116`.
- Tested HEAD: `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
- Tested tree: `2a0a5d3250201773cccb06ba0562df728f4e9387`.
- Execution: clean detached worktree under ignored `build/final-mac-bd7807e/`.
  Initial/final source checks passed; all 135 tracked files matched frozen Git
  blobs byte-for-byte. No code, harness, policy or environment workaround was applied.
- The main checkout remained on `master` at report-only
  `3c6d5617ceebf4e3a31b2b0fdc26a893af371dc8`, initially clean and current with
  origin after fetch. The retained branches were preserved.
- The [Windows final report](windows-amd-2026-11-16-bd7807e-final.md) and all eight
  portable bundles identify the same commit/tree. Their complete checksum lists,
  reference results, run ID, corpus and policy hashes were verified before execution.

Corpus SHA256: `0e18a9aecb2b15b31b1ff0aeaa61e1d8f91867df675900114c6ecbf3e385946c`.
Policies SHA256: `610be76973950ba9eea1c3779ccdb4397e4c82fe4bf2e73f3226039edf152fd3`.

Using the Windows runner's sorted path/NUL/content-SHA256/newline algorithm,
the Mac whole-source byte fingerprint is
`fc0700a5163d98bd7c80a2ab705984d12a22d839568fa297e83b65f47137127e`;
Windows reports `0b1504d4d225f42661763dfc6d17cf5933bfdee52ac38531ee3ef9a5f0d7ab79`.
The Windows raw source manifest/archive was unavailable on this Mac. A simple
CRLF simulation did not reconcile the difference, so no line-ending explanation
or whole-checkout byte identity is claimed. Frozen Git identifiers and shared
corpus/policy/input hashes match; the complete Mac source manifest is retained.

## Host, tools and dependencies

Native Apple M2, macOS 26.7 (25G229), arm64 Python 3.9.6;
`sysctl.proc_translated=0`. Apple Clang 17.0.0 (`clang-1700.4.4.1`), target
`arm64-apple-darwin25.6.0`. CMake/CTest 4.4.4 and Ninja
`1.11.1.git.kitware.jobserver-1` (local Python package `ninja==1.11.1.4`).
Existing local CMake/CTest were selected through the invocation PATH; Ninja
was installed only under ignored `build/`. Its universal executable includes
arm64; the native shell/Python selected that architecture. Sleep prevention
was active during execution.

The pristine upstream testbench remained pinned to
`a7293702385e5169a5f58b83177f2a43cd59139b`; Git cleanliness and all three source
hashes passed. The unchanged scalar-only adaptations were recorded by CMake.

## Executed matrix

All rows use Apple Clang and the macOS system runtime. Each is an initial attempt;
failed/skipped/timed-out counts are all zero. No matrix or capture was retried.

| Backend / logical maximum | Mode | Registered | Passed |
| --- | --- | ---: | ---: |
| sse2neon / 512 | Release | 1350 | 1350 |
| sse2neon / 512 | Debug, strict UBSan | 1350 | 1350 |
| Direct Neon / 128 | Release | 486 | 486 |
| Direct Neon / 128 | Debug, strict UBSan | 486 | 486 |
| Direct Neon / 256 | Release | 890 | 890 |
| Direct Neon / 256 | Debug, strict UBSan | 890 | 890 |
| Direct Neon / 512 | Release | 1292 | 1292 |
| Direct Neon / 512 | Debug, strict UBSan | 1292 | 1292 |

JUnit names exactly match each registered inventory, including all six
configuration/fixture checks. All eight examples produced `2 5 10 17`.
Every compiled test/example was verified as an arm64 Mach-O executable and its
hash audited. Separate corpus counts per mode are 672 compatibility, 218 Direct
Neon 128, 436 Direct Neon 256 and 672 Direct Neon 512; their complete output
inventories and raw bits are in the [portable results](cross-platform/simd-parity-20261116/mac-m2/README.md).

## Controls, flags and numerical contracts

- Native runtime consumers reported ARM64/Neon, x86 instruction level zero,
  64-bit pointers and 64-bit `long`. Writable guards passed at the actual
  16,384-byte page size. All four protected-boundary read/write controls
  terminated with `SIGBUS` (`-10`) in the suite and every capture.
- UBSan positive controls exited zero. Intentional signed overflow emitted the
  expected diagnostic, terminated with `SIGABRT` (`-6`), and did not continue.
  The unchanged native runner accepts this strict nonrecovering behavior;
  the shared bundle loader's exit-code restriction rejects Debug manifests.
- Ordinary tests retain `-ffp-contract=off`, selected rounding flags and
  `-mcpu=apple-m1`. Only the classification stress target uses
  `-ffinite-math-only`. Captures retain `-frounding-math`, nearest initial
  rounding, FTZ/DAZ disabled, and exercise all four selected rounding modes.
- Debug instrumentation retains `-fsanitize=undefined`,
  `-fno-sanitize-recover=undefined`, `-fno-omit-frame-pointer`, debug information,
  sanitizer linking and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`.
- Independent Fraction/Decimal references, fixtures, seed `1592594996`, numerical
  gates, partial counts, canaries and explicit FMA/rounding policies are unchanged.
  Per-case and capture deadlines remain 60 seconds; concurrency is three.
  Raw compile/link commands and binary hashes remain in the evidence packets.

## Comparisons, failures and coverage limits

All **32 planned matching-mode Windows/Mac pairings were attempted**: six
Windows Release profiles against four Mac profiles (24), and two Windows Debug
profiles against four Mac profiles (8). Explicit width scopes match each Mac
maximum; both complete input inventories are validated before width selection.

The unchanged shared comparator completed all **24 Release pairings**:

| Lane classification | Count |
| --- | ---: |
| Exact bit matches | 72,614 |
| Numerically acceptable differences under frozen policies | 802 |
| Documented FMA differences | 300 |
| Documented non-default rounding differences | 300 |
| Unexpected differences in completed comparisons | 0 |

These total 74,016 lane comparisons over 11,988 repeated vector cases, not unique
inputs or universal bitwise identity. All eight Debug pairings were rejected
before lane comparison with `Missing/failed strict UBSan controls`; Debug exact,
numerical and mismatch counts remain **unevaluated**. No missing profile or case
was silently dropped. The complete pairing inventory includes the eight rejection
records alongside the 24 comparator results.

The initial comparator self-check encountered three setup errors out of nine
checks because the clean worktree's ignored `build/` directory did not exist.
After creating it, the unchanged checks passed 9/9. Failure details and replay
logs are retained separately from native matrix/capture results. An unavailable
Ninja package version and executable-location inspection were also setup issues;
they caused no native test retry. No gates, assertions or exit codes were changed.

ASan was not retried. Emulation, hosted CI and benchmark timings were excluded.
Physical Intel, FP16, other hosts/toolchains and unselected APIs remain outside
coverage. Direct Neon remains experimental. Full Debug parity is not established.

## Evidence and continuation

Portable bundles retain the original schema, raw output bits and per-attempt
checksums, including all four rejected Debug bundles. Host metadata explicitly
separates independent reference passes from workflow rejection. Comparison
results and rejection records are under the portable host's `comparisons-01/`.

Combined raw archive: `mac-final-evidence.zip`, retained at
`build/final-mac-control-simd-parity-20261116/`; SHA256:
`6ded9321c727c8c847aecf60ddb0caddab942f117dcc8927f1167ae595c5508c`.
It includes all nine original native packets, capture failure records, command
logs, source proofs, setup failures/replay, controls, comparison records and
the independent evidence audit. Original build directories remain under the
ignored frozen worktree. Archive CRCs and all original packet hashes passed.

The frozen source and Windows results were preserved. Only portable Mac data
and reviewed documentation are prepared; nothing was committed, pushed, merged
or published. Next review the shared loader's host-specific UBSan control policy.
A harness correction requires a new approved frozen checkpoint for both hosts;
do not relabel these rejected Debug attempts as passing or rewrite their manifests.
Transfer the Windows raw source manifest separately to reconcile its byte fingerprint.
