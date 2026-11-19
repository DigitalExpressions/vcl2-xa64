# Next ARM/Neon task proposal — 19 November 2026

Recommendation for review: specialize four Direct Neon four-lane blend patterns
used by matrix transpose, while retaining their exact bit semantics. This is a
bounded implementation proposal; no runtime change has been selected or made.
Compatibility remains default and Direct Neon remains explicitly experimental.

## Source and evidence

Preflight used a fresh detached worktree at
`5e039b01b49abc00548123763be85898932e3ad8`, tree
`ea90c1b40fc50936db7c3509aaa11ab51c02d890`: all 564 files matched their Git
blobs and raw fingerprint
`fa9efdded211c6cbec2ede4fb0f12f334350c17b5c27c10149829a481a29ac06`.
The fetched handoff/documentation tip is separately identified as
`a04eb834c0e6707256009d007c77f4ccaf66beca`. This proposal and the
[preflight readiness report](validation/m2-arm-preflight-2026-11-19.md) are later,
uncommitted metadata. HEAD/tree still identify the clean base; they do not
identify the added planning documents or a newly tested implementation.

Evidence inspected:

- [Retained M2 measurements](benchmarks/STANDARD_MATH_RESULTS_2026-11-17.md),
  [reading guide](benchmarks/standard_math_reading_guide.md), and its CSV,
  metadata, correctness and code-generation records. Measured source:
  `3abf7af84d5ed8375f00c2a184d8a0e55a7b951e`, tree
  `19e520999effe6acea50135e5dc8bc6f0ca38a71`. All eight checksummed benchmark
  metadata files verified. These times belong to that earlier source.
- [Later M2 cleanup result](validation/mac-arm-2026-11-18-arm-header-cleanup.md):
  frozen 474-file fingerprint
  `5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d`,
  tested commit/tree null, 8,036 CTest passes plus eight examples. The complete
  retained capture and original source/raw-archive hashes verified again.
- [Publication comparison](validation/arch-separation-publication-2026-11-18/README.md),
  [integration acceptance](validation/architecture-separation-integration-2026-11-19.md),
  and [Windows triage](validation/windows-amd-2026-11-19-arch-separation-triage.md).
  Windows publication `2d251bc72bcc861edad371499fa732e8eb45a5f7`, tree
  `8c16f0135a597db27b963dff6c241b533a5b91b4`, 497-file fingerprint
  `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2`
  retains its failed/incomplete gate: 5,387 passed, 249 failed, 76 not run of
  5,712, plus twelve passing examples. No earlier pass transfers to this base.
- Current [movement implementation](../vectori128_perm_neon.h),
  [FP entry points](../vectorf128_neon.h),
  [movement tests](../tests/movement_slice.cpp),
  [reduction tests](../tests/fp_reduction.cpp), and
  [benchmark call sites](../benchmarks/standard_math/kernels.cpp).
  The benchmark-era movement helper lived at
  `backends/arm64/vectori128_neon_perm.h`; its scoped comparison to the current
  helper found the include-guard rename after ignoring end-of-line whitespace.
  This limited comparison establishes no whole-checkpoint or performance identity.

## Ranked candidates

The ranking combines source evidence, bounded scope and contract risk. Timing
rows include loads/stores and loop overhead; wrapper instruction counts cannot
identify a workload winner. Wider logical types still compose 128-bit registers.

| Rank | Proposed task, API/backend/width | Evidence and hypothesis | Proposed ARM-owned scope and risk |
| --- | --- | --- | --- |
| 1 | Specialize `blend4<0,1,4,5>`, `<2,3,6,7>`, `<0,2,4,6>`, `<1,3,5,7>` for Direct Neon 128-bit four-lane FP32/int32/uint32 vectors. Exercise logical 256/512-bit composition as affected callers, without new wide specializations. | The AoS transpose uses exactly these patterns. Current mixed-source four-lane blends reach `vqtbl2q_u8`; broadcasts and two-lane patterns already have typed paths. FP32 AoS transpose at counts 128/4096 was 1.269/1.333 ns for compatibility and 1.311/1.296 ns for Direct Neon: both within 5%, overlapping ranges. Typed half combines and unzip operations might improve this path; no measured bottleneck or gain is established. | Runtime: only `vectori128_perm_neon.h`. Low numerical/ABI risk because this is bit movement; template matching and lane ordering are the main risks. Recommended for its small exact acceptance surface. |
| 2 | Specialize Direct Neon FP32 `permute4<1,2,0,3>` and `<2,0,1,3>` used by AoS `vec3_cross`; shared generic matching would also affect 128-bit `Vec4i`/`Vec4ui`. Check wider composed callers. | Current nonbroadcast four-lane permutations use a byte table. Earlier FP32 cross rows favored Direct Neon by 1.459×/1.446× at counts 128/4096 with disjoint ranges, but do not isolate shuffle cost. Replacing the two patterns with typed lane movement might help; it could also require more instructions or already compile equivalently. | Runtime: only `vectori128_perm_neon.h`, preserving lane 3 and all payload bits. Low numerical/ABI risk, weaker evidence of remaining useful work; retain the table fallback and reject any expansion into a general shuffle rewrite. |
| 3 | Review compatibility `horizontal_add(Vec4f)` for an ARM-local reduction sequence; indirect FP32 logical 256/512-bit reductions remain covered. Direct Neon and FP64 remain controls. | FP32 AoS dot was within 5% at both counts (4096: 0.362 ns compatibility, 0.365 ns Direct). Current compatibility reduction uses SSE-shaped operations; Direct already preserves `(a0+a2)+(a1+a3)`. A different sequence might reduce unused-lane work, but no timing deficit is demonstrated. | Runtime: only `vectorf128_compat_neon.h`; any additional dedicated regression would need separate scope in `tests/fp_reduction.cpp`. Higher numerical risk: grouping, NaN operand selection and FPSR flags from unused-lane arithmetic can change. Do not copy the Direct sequence without proving the compatibility contract. |

The FP64 transpose lead (4096 objects: 2.646 versus 2.344 ns) concerns a
logical 256-bit four-double object. It is not evidence that the proposed
four-FP32-lane branch explains that lead. SoA transpose is also a different
workload. Neither supplies a promised gain for candidate 1.

## Recommended task: exact scope

Permit, in a subsequently authorized implementation task, four narrowly matched
branches in `neon_detail::blend` in `vectori128_perm_neon.h`, restricted to
`V::size()==4` and the four index tuples above. Preserve existing static index
validation before selection. Use integer-bit half-register combine/unzip
operations, followed by the existing register bit cast, so no FP arithmetic or
conversion touches data. Retain the generic table path for every other tuple,
including zeroing/don't-care indices. Existing broadcast and two-lane paths stay
in place. Do not redesign the generic pattern classifier.

Public entry points remain `blend4(Vec4f,Vec4f)`, `blend4(Vec4i,Vec4i)` and
`blend4(Vec4ui,Vec4ui)`. FP64 two-lane entry points and logical-wide composition
headers need verification, not implementation edits. No layout, signature,
namespace, ABI, masks, runtime detection or compile-time selection changes.
The facade, `vcl_config.h`, all other runtime files, all 60 protected/dependency
paths and historical adaptation assets stay outside the proposed edit scope.

Future regression scope, if existing coverage does not instantiate these exact
four tuples with edge payloads, is an additive targeted check in
`tests/movement_slice.cpp` using its existing registration. No fixture, seed,
numerical gate, runner or CMake change is proposed. Current work changes none
of those files. Any discovered need to expand scope returns for review.

## Numerical and memory contract

For arbitrary input lane bits `a0..a3`, `b0..b3`, the exact output is:

| Indices | Required output bits |
| --- | --- |
| `0,1,4,5` | `a0,a1,b0,b1` |
| `2,3,6,7` | `a2,a3,b2,b3` |
| `0,2,4,6` | `a0,a2,b0,b2` |
| `1,3,5,7` | `a1,a3,b1,b3` |

Every 32-bit representation is supported: finite values, both zero signs,
subnormals, infinities, quiet/signaling NaNs with arbitrary signs/payloads, and
integer bit patterns. Require exact bits, with zero ULP/relative tolerance.
The operation must not quiet a signaling NaN, canonicalize a NaN, flush a
subnormal or change zero sign. Data movement performs no rounding, conversion,
multiply/add or FMA. Preserve results under all four rounding modes and both
FTZ/default-NaN settings; preserve FPCR and FPSR, including existing sticky bits.
Test payloads through integer/byte copies so the scalar oracle itself does not
perform FP arithmetic. Other numerical APIs retain the
[existing contracts](numerical-contracts.md), including explicit FMA choices.

The four branches operate on registers and may not add user-memory reads/writes.
Keep exact-boundary ARM lookup/gather/partial-access guarantees, negative-size
policies, integer wrapping and invalid-conversion sentinels unchanged. Original
x86 read-ahead and ISA-specific conversion/FMA behavior are distinct contracts;
they are not reasons to weaken ARM gates or edit Agner's originals. No new
cross-backend bitwise-equality claim is proposed for arithmetic.

## Future acceptance and validation

1. Confirm authorization and the four-pattern scope. Inspect actual optimized
   callers in a later task before treating source-level table removal as useful.
   If Clang already produces equivalent code, record that outcome and reconsider
   whether an extra specialization is justified. Do not claim speed from counts.
2. Keep all baseline tests and add targeted coverage only where needed. Use a
   scalar byte-index oracle built from the table above, distinct tagged lanes,
   rotating edge representations and the existing fixed random seeds. Exercise
   both source orders, all four patterns and fallback zero/`V_DC` patterns.
   `V_DC` remains unconstrained by the public oracle; required `-1` lanes remain
   positive zero. Verify FPCR/FPSR preservation separately. Existing broad
   movement/broadcast/two-lane, namespace/ABI and compile-rejection checks remain.
3. Freeze a new complete source checkpoint after runtime/test changes. Default
   is a clean checkout; an explicitly frozen uncommitted capture must contain
   every tracked and nonignored added path, exact raw inventory, patch and
   added/deleted paths. Build only that immutable source and verify it before
   and after. A documentation base is not the tested implementation.
4. Run fresh maintenance/integrity checks, include isolation and relevant
   movement/ABI/FP checks, then the full native M2 baseline. Compatibility max
   512 and Direct Neon maxima 128/256/512 must each run Release and strict
   nonrecovering UBSan Debug: preserve all 8,036 baseline case names and eight
   examples, plus any explicitly added registration. Keep 60-second deadlines,
   concurrency three, guard/canary and sanitizer positive/negative controls,
   `-ffp-contract=off`, selected rounding flags, independent fixtures and
   classification-only finite-math stress. No skips/retries/tolerance relaxation
   may create acceptance. Apple overflow control must diagnose and terminate
   without continuation at its actual exit (1 or -6), with positive control zero.
5. Verify protected inventories/raw hashes/mtimes, dependency hashes and source
   fingerprints again. Retain every failure, full inventory, JUnit, commands,
   exits, flags, generated inputs and binary hashes. Preserve the original failed
   Windows gate. A later native AMD Windows verification, if separately selected,
   evaluates pristine x86 preservation; it is not implied by M2 success.
6. Performance is a separate task after correctness. Only a newly authorized,
   matched before/after native Release run at recorded source identities can
   establish a gain. Keep all slower rows, scalar controls, output references,
   invocation variability and workload distinctions. Consumer migration remains
   separate scope. This proposal supplies no new timing or probe result.

Open questions are whether real optimized transpose callers retain material
lookup overhead, whether existing test instantiations cover all four exact tuples
with nonfinite payloads, and whether any measured benefit would justify extra
special-case code. These questions can be resolved within the proposed later
source/validation task; they do not justify changing arithmetic or shared policy.
