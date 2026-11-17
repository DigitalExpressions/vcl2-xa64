# Native Windows AMD higher-ISA correctness follow-up

**Conversion fix verified; baseline passed; higher-ISA matrix still failing.** Based on
`3abf7af84d5ed8375f00c2a184d8a0e55a7b951e`
(tree `19e520999effe6acea50135e5dc8bc6f0ca38a71`). This investigation does not
extend the earlier Windows or Mac validation claims to changed runtime code.

The checkout was clean and matched fetched `origin/master`. The Mac publication
at `3abf7af` reports native results at `a099098`, not at the runtime changes
prepared here. The retained detached candidate worktree is preserved.

The new detached validation worktree contains that base plus the explicit
uncommitted adaptation/regression changes. All twelve baseline/higher-ISA profiles have
matching original initial/final whole-source SHA256
`ca073990eff3c19aa5dd4445628c0ebc6202d472d094dbf3e0bd43ff06b7c43c`.
The 35-product raw runtime fingerprint is
`9ea1ea10ea4e9fc0bd72a543e35c2a6b7728ef57124d090a4a7e533596007cd0`.
Fingerprint entries are sorted UTF-8 `relative_path + NUL + file_SHA256_hex + newline`.

Comparing shipped products with original base Git blobs finds 24 identical raw
hashes, ten exact reproducible LF-to-CRLF transformations and one runtime change
(`vectorf256.h`). Both hashes and each transformation are retained; no line-ending
explanation is assumed from differing fingerprints alone. The pristine vendor
inventory always requires exact raw bytes. The frozen source archive also
retains the initial report draft; later documentation/metadata edits do not
acquire native passes. Runtime, tests, fixtures and maintenance tooling stay
frozen during execution.

The final primary-source comparison separately checks 144 non-Markdown files:
137 raw-identical, six exact snapshot-CRLF-to-primary-LF reconstructions and the
later portable-data `.gitattributes` addition. No runtime/test content mismatch
is accepted. Every pristine vendor source file remains raw-identical. This
comparison retains the original tested fingerprint rather than replacing it
with the final primary checkout's inventory.

## Completed scope and boundaries

1. Verify original failed packets and initial/final source inventories. Reproduce
   the Clang AVX2 Debug UBSan conversion at `vectorf256.h:2066` before a fix.
2. Add independent integer-conversion boundary/mixed-lane regression coverage.
   Fix through an explicit adaptation patch and regenerate shipped headers.
   Keep all 28 `vendor/vectorclass/` files byte-exact before and after.
3. Run focused native conversion checks across the relevant ISA/compiler modes,
   all eight higher-ISA local profiles, and all four full baseline profiles.
   Retain 60-second deadlines, concurrency three, numerical/FP flags, pinned
   independent references, strict UBSan controls and protected-page coverage.
4. Classify remaining failures from actual diagnostics; record results and gaps
   here, in the validation index and integration plan. Preserve historical data.

Raw evidence is retained under `build/windows-isa-triage-20261117/`. All 16
original higher-ISA packets match their sidecar SHA256 and their embedded copies
in the published supplement (`10c4c5bd728594c0df46766f406df6f2516d403364c75ab598c01bf3371bb109`).
Their original initial/final inventories agree. No original evidence was edited.

No benchmark timings, native Intel tests, commits, pushes, PRs or cleanup are
authorized for this follow-up. Runtime changes require new native M2 validation
before making a new shared-platform claim.

## Host, tools and controls

Native AMD Ryzen 9 7950X, Windows 11 Pro 10.0.26200, native x64 Python 3.11.
`IsWow64Process2` reports native machine `0x8664`, process machine `0`; inspected
test/control executables are x64 PE (`0x8664`) with recorded SHA256. There is no
WSL, QEMU, Rosetta or physical Intel execution.

| Tool | Observed version / selection |
| --- | --- |
| MSVC | 19.50.35723, native x64 driver |
| Clang | GNU-style driver 20.1.8, target `x86_64-pc-windows-msvc` |
| CMake / CTest | 4.2.1 / 4.2.1 |
| Ninja | 1.12.1 |
| Upstream testbench | Clean `a7293702385e5169a5f58b83177f2a43cd59139b`; original three source hashes checked |
| UBSan | Clang Debug only, static release CRT `MultiThreaded`; MSVC Debug is unsanitized |

The unchanged native runner checks pinned reference sources, regenerates
independent fixtures, reconciles every registered/executed case, captures actual
compiler commands, and rejects changed baseline flags. Ordinary MSVC cases keep
`/fp:strict`; only `fp_classification` uses `/fp:fast`. Clang keeps
`-ffp-contract=off`, selected `-frounding-math` and classification-only
`-ffinite-math-only`. Strict UBSan retains `-fsanitize=undefined`,
`-fno-sanitize-recover=undefined`, `-fno-omit-frame-pointer` and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. All suite deadlines remain
60 seconds and build/test concurrency remains three. No fixture seed,
tolerance, expected value, guard/canary or numerical policy was relaxed.

## Full baseline matrix

All four profiles passed on their initial native invocation. Each registers and
executes 40 local baseline-ISA cases plus the 1,308-case pinned SSE4.2 selection.
The new regression extends `edges`; no case was removed or substituted.

| Driver | Mode / sanitizer / observed CRT | Registered | Passed | Failed | Skipped | Timed out |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| MSVC | Release, unsanitized, dynamic Release `/MD` | 1348 | 1348 | 0 | 0 | 0 |
| MSVC | Debug, unsanitized, dynamic Debug `/MDd` | 1348 | 1348 | 0 | 0 | 0 |
| Clang | Release, unsanitized, dynamic Release `msvcrt` | 1348 | 1348 | 0 | 0 | 0 |
| Clang | Debug, strict UBSan, static Release `libcmt` | 1348 | 1348 | 0 | 0 | 0 |
| Total | | **5392** | **5392** | **0** | **0** | **0** |

All registered and JUnit names/counts reconcile. Each example separately prints
`2 5 10 17`. Each profile retains passing native runtime/writable-page controls
and all four expected access violations at the real 4,096-byte boundaries.
Strict Clang Debug's positive control exits 0; intentional signed overflow exits
1 with its diagnostic and no continuation. There are no suite UBSan diagnostics.

## Higher-ISA matrix

All rows register and execute 40 local cases. Each is a fresh first invocation
at the frozen changed source. The previous column refers to the original
optimized candidate, not a new execution of its baseline or a pristine upstream
matrix. All new failed-name sets are subsets of their original sets.

| Driver / ISA | Mode | Previous passed / failed | New passed / failed | New failed names | Suite UBSan diagnostics |
| --- | --- | --- | --- | ---: | ---: |
| MSVC / AVX2 | Release | 27 / 13 | 27 / 13 | 0 | 0 |
| MSVC / AVX2 | Debug | 31 / 9 | 31 / 9 | 0 | 0 |
| Clang / AVX2 | Release | 30 / 10 | 30 / 10 | 0 | 0 |
| Clang / AVX2 | strict UBSan Debug | 25 / 15 | **31 / 9** | 0 | **0** |
| MSVC / AVX-512 | Release | 17 / 23 | 17 / 23 | 0 | 0 |
| MSVC / AVX-512 | Debug | 20 / 20 | 20 / 20 | 0 | 0 |
| Clang / AVX-512 | Release | 20 / 20 | 20 / 20 | 0 | 0 |
| Clang / AVX-512 | strict UBSan Debug | 20 / 20 | 20 / 20 | 0 | 0 |
| Total | | **190 / 130** | **196 / 124** | **0** | **0** |

All rows have zero skips/timeouts and retain failing CTest exit 8. The higher-ISA
collector exits 1 because real failures remain. Zero diagnostics in unsanitized
rows is not UBSan coverage; only Clang Debug is instrumented. All eight examples
separately print `2 5 10 17`. Higher-ISA profiles retain their original static
Release CRT (`MultiThreaded`) selection, including MSVC Debug.

Clang AVX2 Debug now passes `power_roots`, `power_roots_namespace`,
`power_roots_signed_zero`, `inverse_pi`, `inverse_pi_namespace` and
`inverse_pi_signed_zero`. Its three trig cases complete without the former
conversion diagnostic but still fail numerical gates. This is six resolved case
names, not resolution of every diagnostic in previously failing cases.

## Remaining failures and concrete triage

The [portable result inventory](windows-isa-triage-2026-11-17/higher.json) records
every failed name per profile. Unmodified numerical gates remain authoritative.
Two separately retained diagnostic attempts use instrumented **copies** under
`build/`, with unchanged `/fp:strict`, AVX2/AVX-512 selection and 60-second process
deadlines. The second adds entry/exit scope markers to locate the faults more
precisely; neither changes or replaces a registered suite case. Diagnostic
program exits/output are observations, not additional CTest passes.

| Area / affected cases | Observed result and unresolved work |
| --- | --- |
| `movement_slice` and namespace, both ISAs | Protected-memory access violation `0xc0000005`. Both diagnostic attempts reproduce it; the second enters `memory_lookups<17, Vec8s, Vec8s>` without returning. Investigate the short-index large-table lookup path; the exact faulting instruction is not yet captured. |
| `wide_composition`, `composition512` and namespace variants | Actual access violations as well as assertion failures. Entry/exit diagnostics localize 256-bit failures to `table_boundary<1, Vec32c, Vec32c>` and 512-bit failures to `table_boundary<1, Vec16i, Vec16i>` on both ISAs. These tables end at an inaccessible page; preserve that boundary. Native packing/arithmetic/blend assertions also remain unresolved. |
| AVX-512 `integer_slice` variants | A focused native probe of `[INT32_MIN,0,INT32_MAX,-1] / const_int(INT32_MIN)` returns `[1,1,1,1]`, expected `[1,0,0,0]`. The upstream-derived special case constructs `Vec4i` from a compact comparison mask, which broadcasts scalar mask bits rather than expanding lanes. That source expression is present in pristine upstream too. A separate adaptation fix/regression is still needed. |
| AVX-512 `fp_helpers` variants | The probe confirms `Vec4i(mask)` with mask bits `1` gives `[1,1,1,1]`; the broad-mask oracle expects `[-1,0,0,0]`. Distinguish this ISA-specific construction assumption in the test from the actual division bug above. The AVX-512 `abs` range instruction also quiets signaling NaNs and can retain their sign, unlike the bit-clear oracle. No oracle was relaxed. |
| Signed-zero helpers / AVX2 Release power cases | Existing `if_add` and `pow(-inf,-3)` differences remain. The Debug probe observes inactive `if_add(-0,5)` as `+0` at AVX2 and `-0` at AVX-512. MSVC AVX2 Release still returns `+0` instead of the required `-0` for the wide power case. Resolve the concrete optimized path/contract without changing expectations to fit it. |
| `trig` variants, both ISAs | Independent fixture errors remain above the original gate, including normalized FP32 errors 6, 25 and 44 against gate 3. Removing undefined conversion does not fix range-reduction accuracy. |
| AVX-512 `exp_log` / `hyperbolic` variants | Subnormal/domain and NaN special-value failures remain. A Debug probe shows negative minimum subnormal log producing NaN at AVX-512 versus the legacy `-inf` policy at AVX2. Hyperbolic logs also retain NaN-to-finite failures. These require separate implementation/ISA-contract triage. |
| MSVC AVX-512 Release `inverse_pi` variants | Tiny finite FP64 atan inputs still produce NaN in the actual optimized suite. The Debug special-value probe returns the finite tiny value, so that probe does not resolve the Release failure. |
| Clang AVX2 Release `fp_classification` | Existing finite-math stress fails wide NaN finiteness/mixed-lane expectations. The classification-only flags remain unchanged; compiler lowering and composition need further isolation. |

The `INSTRSET=9` unchecked conversion noted below is another unexecuted gap.
No ASan, native FP16, physical Intel, unselected API/ISA coverage or benchmark
timings were added. The Mac's published `a099098` pass does not cover this new
test/adaptation snapshot; native M2 compatibility and Direct Neon validation
remain necessary at a separately approved frozen checkpoint.

Source inspection identifies concrete boundary hazards behind those localized
faults: the 128-bit 16-bit-element lookup uses 32-bit AVX2 gathers, so a lookup of
the last 16-bit element can read two bytes beyond the table. Native 256-bit
byte lookup loads 16 bytes for `n=1`; native 512-bit int32 lookup loads 16 lanes
for `n=1`. These are incompatible with the retained exact-page-boundary tests.
Keep the guard pages and fix bounded access in a separate adaptation follow-up;
do not add table padding to make the tests pass. Other assertion failures still
need independent isolation beyond those first faults.

## Evidence and handoff

The [portable inventory](windows-isa-triage-2026-11-17/README.md) contains counts,
all failed/resolved names, original source inventories, executable hashes,
control audits, both diagnostic attempts, source-byte comparisons and exact
archive receipts. Its `checksums.sha256` protects the portable file bytes.
Raw evidence stays under ignored `build/windows-isa-triage-20261117/`; the
separate original `build/x86-optimization-20261116/` evidence is unchanged.

| New full-baseline packet | SHA256 |
| --- | --- |
| MSVC Release | `9268c27fb53c9d8eb4963657aaee6b8e28851d989166ffefbb64cc6b82969710` |
| MSVC Debug | `634d979303e65c19c5e199fcf37542e3b0d5b5d6842a0b61f87649481886127d` |
| Clang Release | `b5d7fadd47099895db74e1f8de4acc0199f926f085edb72a39b183aef4374454` |
| Clang strict UBSan Debug | `0fa0c437f2849c818f811fe6b27c83c6fa877d45c2f60e0e021049e4ba9674ee` |

The raw-source/evidence supplement covers 1,454 entries, each read back and
verified against its internal SHA256 inventory. It includes the compact packets
and generated control/probe/reference sources they omit. Its SHA256 is
`dcf8439d8b830bbcd950fe85fcaa621daab9c490283cc62c520edfe8be27e156`.
The frozen source ZIP and dirty-source patch have separate hashes in
`source.json`. Fourteen control invocations verify 56 expected guard faults and
five strict UBSan positive/negative pairs. All eight generation integrity
controls pass; primary and frozen vendor checks preserve all 28 pristine files.

Two initial reproduction-helper setup errors occurred before compilation; the
successful `before03` native invocation is preserved separately. Two export
guards initially rejected raw newline differences; exact per-file byte
reconstruction resolved them without editing source bytes or validation results.
Original packets, both diagnostic attempts and failed helper attempts remain.

The current branch remains `master` at `3abf7af`; the validation checkout is
detached at that base plus captured changes. No commit, push, merge, PR,
benchmark run, evidence cleanup or branch deletion was performed. Review the
canonical patch, generated header, independent regression and this report
together. Remaining priorities are protected-table access, compact-mask division,
special-value/accuracy failures and the untested `INSTRSET=9` conversion; a new
approved source then needs native M2 validation. Existing reports stay historical.

## Defect and focused regression

The native AVX2 fallback in `truncatei(Vec4d)` directly cast four doubles to
`int64_t`, including invalid inputs. It is inherited from pinned original VCL:
the pre-fix `vectorf256.h` Git blob is
`b6d8e4592624d3047e4c793a585cefc7f6b6e96b`. Original Clang Debug logs show both
NaN and finite out-of-range diagnostics through `roundi`, used by power/roots,
trig and inverse/pi code. The fresh boundary regression terminates with strict
UBSan exit 1 at the same original line 2066 for positive `2^63`.

The third canonical adaptation patch replaces the unchecked scalar casts with
two calls to the existing guarded 128-bit conversion. Valid lanes truncate
toward zero; invalid lanes retain `INT64_MIN`, matching the existing adaptation
and native AVX-512 signed-integer conversion. No vendor file or native AVX-512
instruction was changed. Regeneration produces the shipped `vectorf256.h`.

The separate AVX-512F-only `INSTRSET=9` fallback in `vectorf512.h` also contains
unchecked scalar casts. That branch is not selected by the retained/fresh
AVX-512 profiles (`INSTRSET=10`); it remains an explicitly identified,
unexecuted conversion gap. This fix is not a claim that all ISA configurations
or all conversion fallbacks are free of undefined behavior.

`tests/truncate_i64_regression.h`, called by the existing `edges` case, uses 22
literal IEEE-754 inputs with independent literal integer results. It covers
signed zeros/subnormals, fractions, nearest representable values around both
int64 boundaries, largest finite doubles, infinities and signed quiet/signaling
NaNs. Every input rotates through every available lane under all four rounding
modes. It makes no out-of-range scalar cast in its reference.

Before the fix, a new Clang AVX2 Debug executable fails with the expected
diagnostic. After the fix, all 16 focused builds pass: MSVC and Clang, Release
and Debug, SSE2/SSE4.2/AVX2/AVX-512; Clang Debug uses strict UBSan. Each executes
1,232 lane assertions across 128/256/512-bit shapes (19,712 total). These
standalone assertions are separate from CTest registration counts. Positive
and intentional-overflow UBSan controls return 0 and 1 respectively, and actual
4,096-byte protected-page controls produce all four expected access violations.
The CPU/OS probe confirms native AMD, AVX2/FMA/F16C, AVX-512F/DQ/BW/VL and XCR0
`0xe7` before higher-ISA execution. No timing code is executed.

## Repeating the native checks

Use a fresh source snapshot and build directories, record its exact commit/tree
plus uncommitted patch/new-file hashes, and verify pristine vendor bytes before
and after. Do not reuse this report's passes for another snapshot. Initialize
an x64 Visual Studio developer environment and use the pristine pinned testbench
at `a7293702385e5169a5f58b83177f2a43cd59139b`.

Run all four baseline profiles using `tools/run_windows_validation.py`:

```powershell
python -B tools/run_windows_validation.py --driver msvc --mode Release --testbench-dir <pinned-testbench>
python -B tools/run_windows_validation.py --driver msvc --mode Debug --testbench-dir <pinned-testbench>
python -B tools/run_windows_validation.py --driver clang --mode Release --compiler <clang++.exe> --testbench-dir <pinned-testbench>
python -B tools/run_windows_validation.py --driver clang --mode Debug --sanitize --compiler <clang++.exe> --testbench-dir <pinned-testbench>
```

For each additional local higher-ISA profile, configure the same CMake suite
without the optional testbench; retain all 40 registered cases. Set
`VCL_BUILD_TESTS=ON`, `VCL_BUILD_EXAMPLES=ON`, `VCL_BACKEND=1`,
`CMAKE_EXPORT_COMPILE_COMMANDS=ON`, the explicit compiler/build type and these
`CMAKE_CXX_FLAGS`:

| Driver / ISA | Additional flags |
| --- | --- |
| MSVC AVX2 | `/arch:AVX2 /DINSTRSET=8` |
| MSVC AVX-512 | `/arch:AVX512 /DINSTRSET=10` |
| Clang AVX2 | `-mavx2 -mfma -mf16c -DINSTRSET=8` |
| Clang AVX-512 | `-mavx2 -mfma -mf16c -mavx512f -mavx512dq -mavx512bw -mavx512vl -DINSTRSET=10` |

Use `CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded`; enable `VCL_SANITIZE=ON` only
for strict Clang Debug here, with
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. Preserve all ordinary
per-target numerical/rounding flags and classification-only finite-math stress.
Build with `cmake --build <fresh-build> --parallel 3 --verbose`. Capture
`ctest --test-dir <fresh-build> --show-only=json-v1`, verify every timeout is 60,
then run:

```powershell
ctest --test-dir <fresh-build> --output-on-failure --no-tests=error --timeout 60 --parallel 3 --output-junit results.xml
```

Run the native binary/runtime, CPU/OS, guard-page and strict UBSan positive and
negative controls as well; a successful build alone is insufficient. Preserve
nonzero CTest exits, complete JUnit/inventories, per-case diagnostics, all source
and compiler/flag manifests, initial/final fingerprints, binary hashes and
checksummed raw packets under ignored `build/`. Failed higher-ISA profiles must
remain labeled failing even when their failed names match an older attempt.
