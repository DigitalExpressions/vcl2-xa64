# ARM64 / Neon lessons to carry forward

The architecture separation and native validation are complete and integrated
into master. Use [current ownership](backend-layout.md),
[integration status](integration-plan.md) and the [validation index](validation/README.md).
These lessons remain useful for future ARM work. The old x86 repair campaign
and Mac migration handoffs are retired; they are not implementation requests.

## What retained native M2 testing established

The [19 November correctness report](validation/arm64/mac-arm-2026-11-19-blend4.md)
records **8,036 CTest passes plus eight examples** across compatibility maximum
512 and Direct Neon maxima 128/256/512, each Release and strict UBSan Debug.
Its tested implementation is a 571-file uncommitted capture, fingerprint
3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36;
tested commit/tree are null, with base 5e039b0 identified separately. This
result is not a fresh pass at current master's later metadata/layout tree.
[Portable source/results](validation/arm64/m2-blend4-2026-11-19/README.md) remain.
Obsolete raw Mac sessions/binaries were discarded on 20 November.

The earlier [shared checkpoint](validation/cross-platform/README.md) and
[source-byte audit](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/source-byte-audit-2026-11-16-bd7807e.md) retain
32 selected pre-separation comparisons and their exact source identities in
frozen Git. Their local historical records were retired on 20 November. Future
comparisons require new captures of the separated implementation.
No checkpoint validates every VCL API, full scalar-libm behavior, application
integration, a future redesign or universal bitwise equality across architectures.

## Hardware width and representation

- Native Neon registers are 128 bits. The tested logical 256/512-bit vectors
  compose two/four registers. Wider C++ types do not imply wider ARM hardware.
- Both sse2neon compatibility and Direct Neon execute native ARM64 instructions.
  Compatibility is the current default; Direct Neon requires explicit
  experimental opt-in. This describes the tested implementation, not a decision
  that the next design must retain both backends.
- Separate operation semantics from register and mask representation. Test lane
  order, boolean selection, bit-preserving casts, cross-register permutations,
  halves and cross-translation-unit ABI explicitly. Avoid assuming that a mask
  conversion supported by one architecture has the same representation elsewhere.
- Backend/width/namespace settings must agree across modules exchanging vector
  objects. Avoid leaking test or ISA flags into application consumer targets.
- x86 capability queries should not pretend ARM has x86 instruction sets.
  The existing ARM runtime exposes Neon separately. Raw SSE interoperability,
  Intel SVML and unimplemented FP16 APIs are outside the Direct Neon contract.

See [backend coverage](backends.md) and [test guidance](../tests/README.md).
Keep the useful behavioral tests even if the eventual implementation changes.

## Floating-point behavior needs an explicit contract

- Preserve independent answers for rounding ties, all supported rounding modes,
  conversion boundaries, NaNs, infinities, subnormals and signed zero. Do not
  implement exceptional conversions with undefined out-of-range scalar casts.
- ARM exception flags/masks are not emulated by this port. Its FP control maps
  rounding and coupled FTZ/DAZ behavior; x86's independent FTZ/DAZ controls are
  not an ARM hardware guarantee. Including headers must not change caller state.
- Explicit FMA and separately rounded operations can produce different bits.
  Keep contraction flags and fused-operation policies visible; do not turn on
  broad fast-math to hide a discrepancy. References must match the stated policy.
- The tested transcendental functions have bounded domains/cutoffs and explicit
  special-value policies. They are not unrestricted scalar-libm replacements.
  Preserve [numerical contracts](numerical-contracts.md) as evidence of what
  was actually implemented and checked, rather than silently extending claims.
- ARM trig work used FP64 reduction for accuracy, including widened FP32 inputs.
  Accuracy, register pressure, conversion cost and compiler inlining must be
  assessed together on the target architecture.

The Windows performance follow-up showed why an algorithm/compiler decision
appropriate for one backend must not automatically become a policy for every
backend. Carry forward architecture isolation and target-specific measurement;
the x86 repair implementations themselves are outside the new ARM agenda.

## Memory boundaries must be tested on the actual host

The M2 report records **16,384-byte pages**; the AMD Windows host used 4,096.
Discover page size instead of hardcoding it. Keep positive writable controls
and all four intentional read/write boundary faults to prove the guard setup.
Use exact page-end tables/buffers, partial counts and canaries. Padding a table
can conceal a vector overread; a pass with padding is weaker evidence.

Check lookup/gather/scatter, partial load/store, permutations and wide composed
operations at exact boundaries. Assertions should use independent lane values,
not the vector implementation as its own expected-value generator.

## Test infrastructure worth keeping

- Independent integer-bit/scalar references and higher-precision fixture
  generators, fixed seeds, exceptional-value cases and namespace/ABI tests.
- Compatibility and Direct Neon width coverage from the old implementation as
  a behavioral reference. A redesigned API must document its actual supported
  surface and newly registered cases; old counts do not automatically transfer.
- Native arm64 Python/Apple Clang and arm64 Mach-O inspection, with Rosetta off.
  QEMU and translated x86 runs are useful diagnostics but separate evidence.
- Strict nonrecovering UBSan Debug, frame pointers, a passing positive control
  and intentional signed-overflow diagnostic with no continuation. Apple UBSan
  may abort with SIGABRT (`-6`) or exit `1`; retain the actual result.
- Original 60-second case deadlines and runner concurrency three. Preserve
  failures/replays and stop to diagnose differences instead of relaxing gates.
- The documented Mac ASan startup hang remains a coverage gap; do not retry it
  as routine validation or infer ASan coverage from UBSan.
- The maintained native ARM runner, CMake configuration checks, shared corpus
  and policy-aware comparator. Preserve the optional pristine testbench pin
  `a7293702385e5169a5f58b83177f2a43cd59139b` and its separate license boundary.

Use the [native workflow](validation/native-workflow.md) and
[report template](validation/report-template.md) as reference procedures after
a new source checkpoint is explicitly selected. Old handoffs are historical.

## What M2 performance evidence says

Use the [19 November master checkpoint](benchmarks/arm64/standard-math-master-checkpoint-2026-11-19/README.md)
and [completed TRS decision](validation/arm64/trs-inverse-decision-2026-11-20.md) for
the retained performance context. Master keeps the approximately 6% small-
transpose gain and its accepted inverse cost. The proposed inverse recovery
had material dot-product costs and was declined. Results remain workload- and
source-specific; no application weighting establishes a universal winner.
Earlier 13/17 November reports and datasets remain in frozen Git history and
are no longer local baseline files.

Keep the following measurement practices:

- Distinguish object/AoS layout from batch/SoA layout, FP32 from FP64, small
  versus larger working sets and logical vector widths.
- Define the timing unit. A complete vector/matrix operation, SIMD chunk and
  individual instruction are different measurements.
- Use independent correctness checks before timing; preserve all samples,
  ranges, slower rows and compiler flags. Keep scalar auto-vectorization policy
  explicit and do not pool different experiments.
- Inspect full called helper bodies, inlining, conversions and spills. Wrapper
  instruction counts alone can conceal costs.
- The tested source-layout separation produced byte-identical M2 benchmark
  binaries versus its pre-layout source. Directory layout alone does not prove
  a performance change. Recheck the actual code produced by the next design.
- Keep application-level 3D workload validation separate. These library tests
  and microbenchmarks do not validate a scene, renderer or external consumer.

The [reading guide](benchmarks/arm64/standard_math_reading_guide.md),
[benchmark protocol](../benchmarks/README.md) and
[report template](benchmarks/arm64/STANDARD_MATH_RESULTS_TEMPLATE.md) remain useful.

## Source provenance and dependencies

Keep Agner Fog's original attribution/license and third-party notices. The
current pristine upstream pin is `667577495efaa36a10cbb5d6cb8c942c23913848`;
all 28 original root paths must remain byte-exact under the
[audited inventory](../audit/vectorclass-2.02.04.json). The duplicate VCL vendor
copy is retired. Do not edit, format, normalize or regenerate originals to fix a
port failure.
The cleanup leaves the pin, compatibility dependency, canonical sources and
all shipped headers unchanged. Header generation is retired.

Source inventories need raw hashes, exact commits/trees and validator/tool
versions. Windows/Mac newline differences were reconciled by reproducing the
recorded hashes in memory, not by assuming a cause or rewriting inventories.
Keep original failed attempts and checksum inventories. A documentation or
cleanup commit does not inherit an earlier native pass.

## Restart point

1. Review these lessons and agree the new ARM strategy and supported API scope.
2. Choose source/dependency boundaries and an approved frozen checkpoint.
3. Implement only the newly agreed scope, then run relevant native M2 correctness
   and separately authorized performance/application checks.

New source work needs an agreed scope and fresh native validation. The
[TRS decision](validation/arm64/trs-inverse-decision-2026-11-20.md)
keeps master and declines the TRS-recovery candidate; the
[documentation cleanup](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md)
explains which older records remain useful. Neither authorizes a new x86 fix,
benchmark run, commit or push.
