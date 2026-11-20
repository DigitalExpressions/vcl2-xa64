# Native Windows optimized test profiles - 20 November 2026

## Outcome and implementation

The owned harness now defaults routine Windows validation to optimized native
x64 MSVC and GNU-driver Clang Release, following the reviewed upstream guidance.
All fifteen authorized configurations were collected: **7,140 registered cases
and fifteen passing canonical examples**. Individual nonpassing gates below are
retained. This is not a claim that the library passes the full suite.

The implementation is uncommitted in a fresh detached worktree based on
`d796768b30cb375ea62afa72dcab061b1a3e54ca`, tree `0fad402083c42891850639c4c0242b9d2d8ab70f`.
The exact tested 656-file capture fingerprint is
`e6d83a55d7c8f535b928d4cfc709cad63b383c071b82cacb7d37aeb500877230`; patch SHA256 `10ed51b8d90e6b199499b07e6f5b3c855de6d756b220c8e43fff9dbfaa16523f`.
[Source proofs](windows-upstream-optimized-2026-11-20/source-vs-base.json)
record six intentional tracked content changes, three additions, and 76 existing
metadata checkout-EOL differences separately from Git blobs. All **98 tracked
C++ source/header files** match base Git bytes exactly. The 28 Agner originals,
audit pin, both sse2neon headers and pin remain byte-exact with unchanged mtimes.
No runtime source, fixture, seed, expectation or numerical gate was changed.
Report/index/usage metadata prepared after execution does not acquire a new
tested source identity. No new M2 source or performance comparison was run.

## Compiler policy and use

MSVC upstream-optimized uses O2, C++17, precise FP and `fp:except-`, without
RTC or Od. Clang uses O2, C++17, no-trapping-math and no-math-errno.
The retained strict Release comparison preserves strict FP/no-contraction and
rounding controls. Strict UBSan is a separate GNU-driver Clang Debug profile,
with disabled recovery, frame pointers and the verified static release CRT.
Classification-only fast/finite-math stress stays separate in every profile.

Sources: [Agner's compiler guidance](https://github.com/vectorclass/manual/blob/c18341d00171bfe6d816ce7b24800c9d164f15aa/vcl_performance.tex)
and [pinned upstream test script](https://github.com/vectorclass/testbench/blob/a7293702385e5169a5f58b83177f2a43cd59139b/runtest.sh).
O2 is the selected reproducible Clang level; upstream also discusses/uses O3.
No timing judgment selected this policy. See the current
[MSVC/Clang commands](windows-runner.md) and
[actual per-target policy records](windows-upstream-optimized-2026-11-20/compiler-policy.json).
The profile helper exports no options through the public interface target.
Direct CMake retains strict-validation by default for portability; the Windows
runner selects upstream-optimized explicitly. Unknown/incompatible profiles,
MSVC Debug/RTC and inherited compiler-policy environment injection are rejected.

## Actual canonical results

| Selection | Registered | Passed | Failed | Not run | Timeouts | Examples passed |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| upstream-optimized | 2856 | 2737 | 81 | 38 | 0 | 6 |
| strict-validation | 2856 | 2739 | 79 | 38 | 0 | 6 |
| strict-ubsan | 1428 | 1322 | 73 | 33 | 0 | 3 |

| Compiler | Profile | ISA | Registered | Passed | Failed | Not run | Timeouts |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| msvc | upstream-optimized | baseline | 1348 | 1329 | 16 | 3 | 0 |
| msvc | strict-validation | baseline | 1348 | 1331 | 14 | 3 | 0 |
| clang | upstream-optimized | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-validation | baseline | 1348 | 1328 | 9 | 11 | 0 |
| clang | strict-ubsan | baseline | 1348 | 1284 | 53 | 11 | 0 |
| msvc | upstream-optimized | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | upstream-optimized | avx512 | 40 | 13 | 27 | 0 | 0 |
| msvc | strict-validation | avx2 | 40 | 24 | 14 | 2 | 0 |
| msvc | strict-validation | avx512 | 40 | 13 | 27 | 0 | 0 |
| clang | upstream-optimized | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | upstream-optimized | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-validation | avx2 | 40 | 24 | 4 | 12 | 0 |
| clang | strict-validation | avx512 | 40 | 19 | 11 | 10 | 0 |
| clang | strict-ubsan | avx2 | 40 | 19 | 9 | 12 | 0 |
| clang | strict-ubsan | avx512 | 40 | 19 | 11 | 10 | 0 |

Timeouts, if any, are included in failed counts. There are no skipped cases.
The baseline is 1,348 cases/profile, including all six fixture/configuration
checks and the pinned upstream selection. Supported higher-ISA local profiles
register 40 cases each. Every case retains a 60-second deadline and concurrency
three. Examples print `2 5 10 17`. The 17 development profile smoke
examples and their positive/negative controls are retained separately in
[setup attempts](windows-upstream-optimized-2026-11-20/setup-attempts.json);
they are not counted as canonical matrix cases/examples. Two additional
[standalone compiler-recipe consumers](windows-upstream-optimized-2026-11-20/standalone-consumers.json)
also pass with the facade example and recorded native include traces.

The [complete inventory](windows-upstream-optimized-2026-11-20/cases.json),
[nonpassing output](windows-upstream-optimized-2026-11-20/nonpassing-output.json),
[optimized/strict comparison](windows-upstream-optimized-2026-11-20/optimized-vs-strict.json)
and [strict/prior-control comparison](windows-upstream-optimized-2026-11-20/strict-vs-previous-control.json)
compare complete registered names, not intersections. Output in portable data
is path-redacted captured JUnit stdout and may be CTest-truncated; full LastTest
and process logs remain in raw evidence. No universal output-bit identity is claimed.

MSVC optimized baseline has two additional failures versus strict baseline:
`fp_helpers` and `fp_helpers_namespace`, at the unchanged signed-zero assertion
`equal_bits(if_add(M(false),V(T(-0.0)),T(5))[0],T(0))`.
These remain failed. Other configuration differences are enumerated in the
complete comparisons; no protected source fix or acceptance waiver is applied.
All 4,284 strict Release/UBSan registrations match the prior control's case
statuses exactly. Optimized versus strict differs only in the two MSVC baseline
cases above. Captured stdout hashes also retain other output differences;
identical statuses do not establish universal bitwise behavior identity.

## Native controls and preservation

Native AMD Ryzen 9 7950X, Windows 11 x64, MSVC 19.50.35723 and GNU-driver
Clang 20.1.8 (Windows MSVC ABI x64), Python 3.11, CMake/CTest 4.2.1 and Ninja
1.12.1 were verified. Pinned testbench `a7293702385e5169a5f58b83177f2a43cd59139b`
remains clean, with all three source hashes checked. Installed tools/runtimes
were reused and hashed, with no installation or saved configuration change.

CPU CPUID and OS/XCR0 probes permit AVX2/FMA and AVX-512 F/DQ/BW/VL on this
host. Fresh probes precede higher-ISA execution. The original runtime detector,
native x64 PE identity, actual 4,096-byte pages, writable positive control and
all four protected read/write fault controls pass for all fifteen configurations.
All three strict UBSan profiles have positive exit zero and intentional signed
overflow exit one, expected diagnostic and no continuation. The static release
CRT and halt-on-error setting were verified. Full
[control proofs](windows-upstream-optimized-2026-11-20/canonical-controls.json),
[binary hashes](windows-upstream-optimized-2026-11-20/binary-inventories.json) and
[include graphs](windows-upstream-optimized-2026-11-20/include-graphs.json) are retained.
Actual compiled dependency graphs verify facade forwarding to root-original
x86 headers, with no ARM/Neon/sse2neon implementation dependency. The existing
adapted testbench uses that facade too; this is not a new pristine-consumer matrix.

## NoInitFailures, retained failures and gaps

`NoInitFailures` remains a diagnostic category for a captured MSVC RTC
uninitialized-use event. Three historical case identities/six paired debugger
captures are retained. The public uninitialized-constructor contract and a
plausible performance rationale do not establish author intent or harmlessness
for a particular masked use. Current Release/UBSan failures are not relabelled
by case name. [Policy and historical sites](windows-upstream-optimized-2026-11-20/noinit-policy.json)
keep mechanism, outcome, attribution and intent distinct.

RTC execution is deferred and rejected; no new RTC coverage or callback/private
desktop implementation is claimed. Routine binaries have no RTC instrumentation,
so they cannot produce the observed RTC uninitialized-variable dialog. This
does not guarantee that every unrelated Windows error UI is impossible.
The historical full Windows gate remains **failed/incomplete: 5,387 passed,
249 failed, 76 not run out of 5,712, plus twelve passing examples per source**.
New profile results do not overwrite it or prove that earlier diagnostics were false.

Initial build-abort packets/false summaries and one separately logged Ninja
`-k 0` continuation per affected build remain intact. Missing binaries stay
not run. The first implementation capture was interrupted before any canonical
CTest case, corrected, and replaced with a new immutable capture. One post-CTest
private collector path error was corrected by offline ingestion of the completed
first MSVC JUnit; **no canonical case was replayed**. Development failed controls,
both captures and validator revisions remain in raw evidence. A metadata EOL
source-proof assertion was also corrected analytically without editing bytes.
An offline graph-audit startup attempt could not find Ninja outside the developer
shell; its empty log and validator are retained. The completed audit uses the
already hash-verified installed Ninja path. No native case was rerun for it.

Physical Intel, ARM/M2 execution/performance, RTC diagnostics, ASan, benchmarks,
emulation, hosted CI, unregistered APIs and external consumers are outside this
collection. The independent M2 review/integration hold is unchanged.

## Evidence and repository state

[Portable results and checksums](windows-upstream-optimized-2026-11-20/README.md)
include exact source identities, complete statuses, all nonpassing captured
outputs, flags, controls, hashes and private-validator corrections. The
[raw archive receipt](windows-upstream-optimized-2026-11-20/raw-evidence-receipt.json)
identifies the CRC/entry-SHA256-verified host-local packet
`build/uo20-01-raw-2026-11-20.zip`; retained binaries are inventoried and stay
on disk. Source captures, failed attempts, logs and generated control/probe
inputs remain under ignored `build/uo20-01/`.

The primary master HEAD, local work, prior worktrees, branches, saved repository
configuration, tools, dependencies and retained protected snapshots are preserved.
No cleanup, commit, push, merge, PR or external consumer change was performed.
This implementation and metadata are prepared for review in the detached worktree.
