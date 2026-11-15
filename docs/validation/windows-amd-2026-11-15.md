# Native Windows x64 validation — 2026-11-15

Native execution on an **AMD Ryzen 9 7950X**, Windows 11 Pro build 26200,
x64 process. This establishes physical AMD coverage. Physical Intel remains
an outstanding gate. No WSL or translated execution is included.

The clean starting checkout was `DigitalExpressions/vcl2-xa64`, `neon-port`,
HEAD `c627a2e129a2510ad9e5bb0673556456a76de346`, with port checkpoint
`4d220c79da62e96acd1022a56dc24e8ee73d634d` verified as an ancestor. Work is on
`windows-native-validation`; the final source commit remains the starting HEAD
with a reviewable uncommitted patch. No push, merge or publication was performed.
Historical migration evidence and numerical fixtures were preserved.

## Toolchain and execution

Primary compiler: MSVC 19.50.35723, Visual Studio 2026, Windows SDK 10.0.26100.0.
Secondary drivers: native x64 Clang 20.1.8, both `clang++` and `clang-cl`.
CMake/CTest 4.2.1, Ninja 1.12.1, Python 3.11.9. MSVC ran in an x64 developer
environment. The installed default LLVM `bin` executable targets i686; the
explicit x64 LLVM executable was selected and its target verified before builds.

An explicit `vcl::runtime` consumer reported `instrset_detect=11`, FMA3 and
F16C supported, AVX-512 FP16 absent. A separate XCR0 read returned `0xe7`,
confirming the OS enables the AVX and AVX-512 register state before higher-ISA
execution. Pointers are 64 bits and Windows `long` is 32 bits.

| Compiler/driver | Profile | Passed/registered | Failed/skipped |
| --- | --- | --- | --- |
| MSVC | SSE2 Release | 40/40 | 0/0 |
| MSVC | SSE2 Debug | 40/40 | 0/0 |
| MSVC | selected SSE4.2 Release | 1348/1348 | 0/0 |
| MSVC | selected SSE4.2 Debug | 1348/1348 | 0/0 |
| clang-cl | SSE2 Release | 40/40 | 0/0 |
| clang++ | SSE2 Release | 40/40 | 0/0 |
| clang++ | SSE2 Debug, strict UBSan, static release CRT | 40/40 | 0/0 |
| clang++ | selected SSE4.2 Release | 1348/1348 | 0/0 |
| clang++ | selected SSE4.2 Debug, strict UBSan, static release CRT | 1348/1348 | 0/0 |

The registered lists reconcile to 34 compiled local cases plus six Python
checks, and another 1308 pinned upstream cases when enabled. Local profiles
exercise logical widths 128/256/512, namespaces, cross-translation-unit ABI,
FP-environment behavior, signed zero and ordinary independent fixture regeneration.
Every case retained its 60-second deadline. There were no timeouts or suite
UBSan diagnostics. Each profile's example printed `2 5 10 17`; the initial
ordinary header-only MSVC smoke build passed as well. No benchmark timing
measurements were collected.

The optional GPL-3.0 testbench remains a clean, separate ignored checkout at
`a7293702385e5169a5f58b83177f2a43cd59139b`. Its three original source hashes,
provenance and existing scalar adapters were retained. MSVC accepted
`/arch:SSE4.2` for its upstream targets; ordinary local targets kept the SSE2
baseline. GNU-style Clang used `-msse4.2` only for the upstream targets.

## Harness changes and numerical safeguards

The test-only memory abstraction uses GetSystemInfo, VirtualAlloc,
VirtualProtect and VirtualFree on Windows, retaining the POSIX implementation.
The observed page size is 4096 bytes; allocation granularity is 65536 bytes.
Protection uses the page size and persistent PAGE_NOACCESS boundaries. Four
separate negative controls produced the expected `0xc0000005` access violation
for reads and writes at both boundaries; their writable-page control passed.
All original partial-count loops, exact boundaries and canary checks remain.
The standalone `fp_partial.cpp` was additionally executed in global and
isolated namespaces through an evidence-only consumer project.

MSVC ordinary targets use `/fp:strict`, which preserves FP-environment behavior
and disables implicit contraction. Only the classification stress target uses
`/fp:fast`; this is broader than GNU finite-math-only and is documented as such.
Clang keeps `-ffp-contract=off`, the selected `-frounding-math` options and its
separate `-ffinite-math-only` target. See Microsoft's
[floating-point option reference](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior).
Numerical inputs, seeds and acceptance thresholds were unchanged.

Portable independent scalar bit-count/first-bit references replace GNU-only
builtins in tests. Explicit vector wrappers and integer-mask intermediates
replace GNU native-vector indexing/conversions without changing assertions.
The noinline rounding probe uses MSVC's attribute on Windows.

Configuration probes now use temporary source files and driver-specific
syntax-only options, carrying configured target flags. Positive controls for
each flag set run before contract checks. An intentional unsupported MSVC
option and unsupported Clang ISA option both failed the probe positively;
MSVC's D9002 warning is checked explicitly because `/WX` alone did not reject it.
Both original x86 negative controls remain active.

MSVC Release initially failed four NaN classifications under `/fp:fast`.
Pristine upstream revision `667577495efaa36a10cbb5d6cb8c942c23913848`
reproduced the same failure with identical MSVC flags. The focused SSE2 fix
uses integer NaN classification for MSVC and enables the existing assembly
comparison path for Windows Clang. The unchanged classification test then
passed under all three Windows drivers. The separate pristine checkout is
unchanged.

## Sanitizers

The intentional signed-overflow Clang probe passed its ordinary control and
terminated with a UBSan overflow diagnostic and exit 1 under
`-fsanitize=undefined -fno-sanitize-recover=undefined` and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. The same strict instrumentation
and runtime settings were used for the two successful suite profiles.

The installed UBSan C++ runtime requires the static release CRT. The initial
Debug CRT attempt failed to link on `_ITERATOR_DEBUG_LEVEL`, and a release DLL
CRT attempt failed on runtime-library mismatch. Fresh successful Debug builds
used `CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded`, retaining `-O0` and debug
information. Linker LNK4217 import warnings remain in the raw evidence; no
sanitizer checks were disabled to obtain these results. Debug CRT UBSan
coverage remains unavailable with this installed runtime.

MSVC ignored `/fsanitize=undefined`; its intentional overflow continued with
exit 0, confirming no UBSan instrumentation. The harness now refuses that
configuration. MSVC Debug is recorded separately from UBSan. clang-cl sanitizer
configuration is also explicitly refused; validated Windows UBSan uses the GNU
Clang driver. ASan was not run in this session.

## Additional ISA checks and remaining gates

An evidence-only consumer registered seven cases per selected ISA profile:
four higher-ISA core/namespace cases, a classification stress case, and two
additional SSE2 partial-memory cases. These are selected checks, not full
higher-ISA regression matrices.

| Compiler | Profile | Passed/registered | Failed/skipped |
| --- | --- | --- | --- |
| MSVC | AVX2, instruction level 8 | 7/7 | 0/0 |
| MSVC | AVX-512, instruction level 11 | 5/7 | 2/0 |
| clang++ | AVX-512, instruction level 11 | 7/7 | 0/0 |

The MSVC AVX-512 failures are `core_512` and `core_namespace`: NaN and infinity
classification at physical width 512 returns incomplete masks. A minimal
reproducer returns `0x000f`/`0x03` instead of `0xffff`/`0xff` under both MSVC
19.50.35723 and the separately selected 19.44.35221. Pristine pinned upstream
reproduces the same outputs with matching flags. The corresponding Clang
reproducer returns the expected full masks. MSVC disassembly shows 128-bit
memory-broadcast `vfpclass*` encodings in the optimized 512-bit reproducer;
this points to a compiler code-generation issue. This gate remains failing,
with its assertions and references preserved.

Native AVX-512 FP16 execution is unavailable on this CPU. Both installed
toolchains' `<stdfloat>` probes also lack `std::float16_t`; no upstream FP16
requirement was bypassed. Physical Intel, the full AVX2/AVX-512 matrices, ASan,
and affected ARM/POSIX profiles remain outstanding. Rerun the shared harness
changes on ARM before merging. ARM validation was not substituted with x64
compile probes or prior historical results.

## Raw evidence and reproducibility

All raw evidence stays under ignored `build/`. The evidence root is
`build/windows-validation-20261115/`; each profile also has its own
`build/windows-<profile>/` directory. Retained artifacts include:

- Initial/final machine and toolchain inventories, full source commits,
  ancestry check, exact commands and their exit codes in `commands.jsonl`.
- Configure/build logs, `compile_commands.json`, CTest inventory JSON, per-case
  `Testing/Temporary/LastTest.log` and `results.xml` JUnit reports.
- Per-profile audit JSON with x64 PE verification, tested executable SHA256,
  compile commands, registration/pass/fail/skip counts and deadline checks.
- `source-manifest.json`, `working-tree.patch` including added files, and
  `audit-summary.json` with the tested non-Markdown source fingerprint and
  complete patch SHA256. Fingerprints identify this uncommitted checkpoint.
- Initial failures, failed CRT configurations, intentional negative controls,
  pristine-upstream comparisons, ISA reproducers and disassembly.

The public [test instructions](../../tests/README.md) contain portable command
shapes. No consumer SDKs, application tests, credentials or machine-specific
paths were added to public sources. The patch and this record are prepared
for review; publication and merging require the user's approval.
