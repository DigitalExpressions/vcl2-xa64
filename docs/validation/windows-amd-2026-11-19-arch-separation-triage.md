# Native AMD Windows architecture-separation failure triage — 19 November 2026

## Outcome and scope

**No facade/separation-specific failure was reproduced in the selected checks.**
All **76 unavailable target instances** reproduce compile failures through pristine
original headers. All **20 paired runtime checks** reproduce the same failure
status through the facade and direct-original routes. Sixteen pairs have identical
captured output; four sanitizer pairs have the same first diagnostic, with different
source paths/addresses in their retained stacks. This is targeted causal evidence,
not a rerun of every original failing instance or a new full-matrix pass.

All **325 original nonpassing instances** are classified: **249 failed** and
**76 unavailable** across the original 5,712-case registration. Categories identify
observed mechanisms, contract mismatches and representative/family evidence; they
do not declare every inherited difference an upstream defect. The original
[Windows report and gate](windows-amd-2026-11-19-arch-separation.md) remain unchanged
and failed/incomplete. No test was removed, padded, weakened or marked passing.

The new runtime selection comprises **40 executions: 26 failed, six access faults
and eight timeouts; zero passes**, as expected for selected failures. Four paired
MSVC Debug cases time out at the original 60-second deadline. These are new
diagnostic attempts and do not replace the earlier single recorded timeout.

## Exact source and method

Approved publication `2d251bc72bcc861edad371499fa732e8eb45a5f7`, tree
`8c16f0135a597db27b963dff6c241b533a5b91b4`; immutable Windows capture **497 files**,
fingerprint `d51cba48722ec8e6c46395ab8a4447503f13c8d2ba2de5b9873991ff31d003c2`.
Original x86 pin: `667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. The Mac 474-file source identity
`5a6f9ad8fe448d19e05917ef2eee95f4482eb7136c67488ad3bf2b3d6eef542d` stays separate;
no Mac result is relabeled as execution of the publication or this triage.

Native AMD Ryzen 9 7950X / Windows x64; native-process and PE proof, the pinned
testbench revision/three hashes and source/old-archive hashes were reverified.
Existing MSVC 19.50.35723 and GNU-driver Clang 20.1.8, CTest 4.2.1, and installed
native CDB 10.0.20348.1 were reused. No tool was installed. An independent CPUID/OS
probe reconfirmed AVX2/FMA and AVX-512F/DQ/BW/VL (`XCR0=0xe7`) before higher-ISA
execution. Actual page size remained 4,096; writable/four negative guard controls
and strict UBSan positive/negative controls passed their original conditions.

New generated consumer copies live only under ignored `build/`. Their only test
source edit replaces facade includes with a direct-original test bridge. That
bridge includes unchanged neutral selection policy for the existing test macros,
then the pristine vendor vector/math headers explicitly. Pure upstream-only branch
and ISA probes include originals without even that neutral bridge. Include traces
exclude the facade, ARM headers and retired helpers in direct builds. Inputs,
fixtures, seeds, lane expectations and numerical gates remain the frozen bytes.
Original compiler/FP/rounding/CRT/sanitizer flags are retained; only input/output
paths and include diagnostics change. Four controller-flag mistakes were corrected
in separately identified attempts; the originals remain retained and excluded as
matching-flags evidence. Original link argument lists were reused with new output
paths and retained runtime libraries. Build/test concurrency stayed **three** and
every native/diagnostic case retained a **60-second deadline**.

## Timeout resolved: Debug runtime report, not numerical nontermination

The original `math_Vec4f_300.exe` was allowed to start normally, then inspected
with the installed native debugger. Its main thread was blocked in a Windows
modal message box reached through CRT reporting:

```text
user32!MessageBoxW
ucrtbased!CrtDbgReportW
math_Vec4f_300!_RTC_UninitUse
math_Vec4f_300!sinh_f<Vec4f>  (vectormath_hyp.h:135)
```

The original `sinh_f` declares `y1` and `y2` without initialization, calculates
only needed branch results, then passes both to `select`. Uniform lanes leave
one argument uninitialized. Original double-precision `asin_d` similarly reaches
`select` with uninitialized `rx`/`px` at `vectormath_trig.h:641`. Debugger stops
reproduced this family across sinh/tanh/asinh/acosh/atanh and asin/acos consumers.
Twenty debugger sessions captured **14 RTC stops and six lookup access faults**;
the extra attach session captured the actual waiting UI stack. Debugger exit zero
means a diagnostic was captured, never that its target passed.

A small pristine-original-only probe tests uniform-small, uniform-large and mixed
lanes for float sinh and double asin. All six Release cases pass independent
two/three-ULP scalar references. In Debug the four uniform cases report the exact
uninitialized names (`y1`, `y2`, `px`, `rx`); two mixed cases pass because both
branches initialize their temporaries. A diagnostic-only RTC callback records
and exits **86 without continuation**, avoiding the reporting UI. It does not
change the frozen tests or turn their original failures into passes. This proves
an inherited Debug/runtime-check interaction; it does not prove those Release
calculations return bad values or establish a compiler defect.

## Other concrete original-code reproductions

- Pure-original MSVC AVX-512 division of `[INT_MIN,0,INT_MAX,-1]` by `INT_MIN`
  returns **`[1,1,1,1]`**, versus independent **`[1,0,0,0]`**. The original
  compact comparison mask is broadcast as an integer before `& 1`.
- In the same original-only program under ordinary **`/fp:strict`**, broadcast
  quiet-NaN/Inf masks are **`f/3`**, versus expected **`ffff/ff`**. This is
  an inherited original/MSVC interaction, separate from finite-math stress.
  A source rewrite passing would not alone establish a compiler bug.
- Direct Clang strict UBSan reproduces invalid FP-to-int64 casts, signed scalar
  qword multiplication overflow, and the `abs(INT_MIN)` path in `Divisor_i`.
  The C++ diagnostics are real; demanding one universal invalid-conversion
  sentinel is a separate fork policy, not inferred from different output bits.
- Direct numerical consumers reproduce finite pi-scaled/trig discrepancies and
  special-value handling differences. Ordinary power/root fixture accuracy and
  exceptional class/sign assertions are distinguished in the recorded outputs.
  An inherited numerical mismatch alone does not show a separation regression.

## Contract review

The [author's manual](https://www.agner.org/optimize/vcl_manual.pdf#page=49)
allows table read-ahead and requires positive lookup sizes. Its
[FP guidance](https://www.agner.org/optimize/vcl_manual.pdf#page=92) excludes
finite-math classification and signaling-NaN use. Invalid conversion results and
some zero/NaN signs vary. Our stricter bounded access, negative-size, bit-pattern
and sentinel expectations therefore require separate contract decisions.
The retrieved manual is dated 2022-08-07 and separately checksummed; it is not a
file pinned by the library snapshot. Source comments and actual reproducers are
retained alongside that documentation, and ambiguous extreme-input guarantees
remain explicitly unproven.

The six debugger-confirmed boundary faults occur in original `lookup<1>` or
`lookup<17>` loads/gathers at unpadded protected page ends. Their read widths
are incompatible with the retained exact-boundary gate, while the documented
read-ahead allowance explains the original implementation's contract. No table
was padded and no boundary expectation was changed during this triage.

## Complete original nonpassing inventory

Each original profile/name appears exactly once in the
[classification bundle](windows-arch-separation-triage-2026-11-19/README.md).
Counts below are case instances, not independent defects. A case may exercise
additional policies; its primary category does not erase its full output.
Some class/sign and precise input-domain guarantees still need maintainer
contract review; the per-case evidence-strength labels avoid claiming that all
249 failures received individual debugger replays.

| Category | Original instances | Observed basis |
| --- | ---: | --- |
| `compile_lookup_nonpositive` | 20 | Nonpositive lookup size used by retained fork tests; original documentation requires a positive size. |
| `compile_pow_const_intmin` | 18 | Original pow_const(INT_MIN) instantiates a negated exponent outside int range. |
| `compile_signed_minimum_division` | 18 | Original constant-divisor template evaluates -INT_MIN in a constant expression. |
| `compile_unsigned_complement_expression` | 4 | Original unsigned a & ~b is ambiguous for composed Vec8uq. |
| `compile_unsigned_complement_type` | 16 | Retained assertion expects unsigned complement result; original inherited result is signed. |
| `runtime_exp_log_special_values` | 12 | NaN/subnormal/domain results differ in AVX-512 special handling. |
| `runtime_finite_math_classification` | 4 | Retained classification stress requires behavior under flags excluded by the original manual. |
| `runtime_fp_conversion_ub` | 58 | Strict UBSan reports an invalid floating-to-integer cast in original fallback code. |
| `runtime_fp_helper_bits` | 7 | Bit-exact NaN/zero helper expectations differ; includes signaling-NaN behavior and conditional signed zero. |
| `runtime_hyperbolic_special_values` | 9 | Original hyperbolic special-input outputs differ from retained reference expectations. |
| `runtime_integer_arithmetic_ub` | 4 | Strict UBSan reports original signed multiply/negation overflow. |
| `runtime_inverse_pi_accuracy` | 9 | Finite pi-scaled range reduction differs from the independent fixtures. |
| `runtime_lookup_boundary` | 40 | Protected-page table access fails through both routes; exact bounded reads exceed the documented read-ahead allowance. |
| `runtime_min_divisor_mask` | 4 | Compact-mask signed-minimum division gives wrong valid-input lane results. |
| `runtime_msvc_debug_branch` | 59 | MSVC Debug reports uninitialized branch temporaries; debugger/minimal probes establish representative mechanisms. |
| `runtime_narrow_divisor_policy` | 8 | Test expects narrowed interpretation of 65539 for smaller signed lanes. |
| `runtime_power_special_values` | 12 | Power/root special-value and sign checks differ; ordinary fixture accuracy is separate. |
| `runtime_strict_classification` | 2 | Ordinary /fp:strict AVX-512 quiet-NaN/Inf classification fails in the original/MSVC combination. |
| `runtime_trig_accuracy` | 21 | Finite trigonometric fixtures exceed retained numerical gates. |

Unavailable target breakdown: nonpositive lookup instantiations **20**;
unsigned-complement type assertions **16**; `pow_const(INT_MIN)` **18**;
signed-minimum constant division **18**; ambiguous unsigned complement expression
**4**. All 76 fail through direct originals with matching flags. Negative lookup
and exact-result-type expectations are separated from actual ordinary API
composition/representable-divisor limitations.

## Review decision and remaining gaps

The targeted evidence supports the architecture boundary: no selected failure
was introduced solely by the facade. It does **not** justify a full correctness
pass or an automatic merge. Before integration, agree the x86 acceptance scope:
which documented original limitations are accepted, which stricter guarantees
belong to the ARM/fork contract, and which inherited valid-input issues block
the intended consumers. Keep the current failed results intact. Any future
source fix or test-policy change needs its own explicit scope, frozen checkpoint
and relevant native validation; no retired patch has been reapplied here.

Unselected failing instances have family/source attribution rather than individual
direct re-execution. Compiler causation, complete mathematical-domain guarantees,
physical Intel, other hosts/toolchains, ASan, native FP16/SVML, application
integration and new cross-host numerical parity remain outside this triage.
No benchmark, emulation, hosted CI, upstream contact, consumer edit, source fix,
commit, push, merge, PR, branch retirement or evidence cleanup occurred.

## Evidence and preservation

Run ID: `windows-arch-separation-triage-20261119-01`.
Raw records, generated consumers/probes, all failed preparation, objects, binaries,
PDBs and native debugger logs remain under ignored `build/wt19-triage-01/`.
Compact packet: `windows-arch-separation-triage-evidence.zip`, SHA256
**`68c1cf4a62bf133dc0e2a846312b5dfa2ce45bbfa630319490a9e332d96f9e6a`**; **384** entries passed CRC and
every recorded SHA256 check. Binary hashes are retained; compiled files were
omitted only from the compact packet and remain on disk.

The prior main packet `e7999de002dd09191fd3c2a3d12b6a3f819a3a71bfb89cc660f9ad041f572876`
and post-review supplement `823ced8f40623153f0c7c94a0a0d8a573cb00c3b24af0bf7b104e9f03a74123b`
remain byte-exact. Earlier reports, their portable checksums and source identities
are unchanged. All 28 root originals, 28 vendor originals, pin and dependency
bytes/mtimes remain exact, as does the full 497-file library capture. Primary
master remains clean; only new review metadata and current-status notes are
prepared in the existing detached worktree. Nothing was published.
