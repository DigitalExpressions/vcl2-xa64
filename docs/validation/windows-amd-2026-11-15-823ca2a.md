# Native Windows AMD evidence review — 2026-11-15

## Outcome and tested source

**Passed at the fixed source after earlier failures and verification reruns.**
The retained final native AMD matrix has **5,627/5,627 passes** across 14
profiles, with zero failures, skips or timeouts. This report follows the
[shared template](report-template.md) and audits existing evidence; no native
matrix was executed during this review. Preserve the
[initial Windows report](windows-amd-2026-11-15.md) and
[AVX-512/ARM POSIX follow-up](windows-avx512-2026-11-15.md) as historical records.

| Source item | Verified identifier / state |
| --- | --- |
| Host label | Native AMD Windows x86-64 |
| Repository / execution branch | `DigitalExpressions/vcl2-xa64`, `windows-native-validation` |
| Final verification HEAD | `823ca2a24cd1ab6fbbda6e81cf596efc6d9d6486` |
| Git tree at that HEAD | `5133e9f290789671c1cd5062cb1d5e011508717f` |
| Working-tree state at verification audit | Two documentation differences: `tests/README.md` and the added `docs/validation/windows-avx512-2026-11-15.md`; not an entirely clean tree |
| Documentation-only patch SHA256 | `eed109790ab579f346133384372e225388b3c4c695f4585e7cf67d60f86509fd` |
| Tested non-Markdown source fingerprint | `b1970cbba4ff6b540f7dfbcd236b3a0f4f974487aa9bb545c41b1144c5c3105f` |
| Later clean report commit | `943923a4cce1b3990ee69b37ebdf1c37f0fc67b6`, tree `6e98a38ea24e8558f8edf08db609b72a3b18e52e` |

The fingerprint was recomputed from the retained 92-row source manifest. Every
file hash matches the corresponding committed file at `823ca2a`, allowing for
its recorded Windows LF/CRLF representation: 64 CRLF forms and 28 Git-blob
forms. It is a fingerprint of those files, not a whole-tree Git identifier.
The post-commit evidence independently records the same fingerprint at the
clean `943923a` checkpoint. The only changes from `823ca2a` to `943923a` are
the two Markdown files above; retain the actual execution identifiers rather
than calling this a new run at `943923a`.

The initial Windows execution used an uncommitted snapshot based on
`c627a2e129a2510ad9e5bb0673556456a76de346` (base tree
`8570d1776d0d9deee69aed459b40cc9abea90e60`). Its identifiers are:

- Full working patch SHA256:
  `06206d5ca3a47260d74b2c3e8d8121a4481b81e05990b48086ad2b5882a15830`.
- Non-Markdown source fingerprint:
  `0e26f1175081440b9441e63022e1128062fb359c513efa7eda3e6bedd5bf9a08`.
- Added files captured in that patch: `tests/protected_memory.h`,
  `tests/scalar_bits.h` and the initial Windows Markdown report.
- All 92 source-manifest entries match their committed forms at
  `680cf4538d431f7fa4e39aa49e42b492e1da5dbc`, tree
  `b647db7bbc0398ea16a6d8e4e7da84c900663dfb`, with the same explicit
  line-ending comparison. This does not make `680cf45` the execution HEAD.

Comparison with the [native Mac report](mac-arm-2026-11-15.md): its tested HEAD
is `465436de204ebdd59e9b5071c353df372148b3df`, tree
`ff966a90ebfd9a2bc07c76c8418684df3a04ffa8`. From `943923a` to that checkpoint,
the changed files are `CONTRIBUTING.md`, `README.md`, `tests/README.md`, the
added `docs/validation/native-workflow.md`, and the added
`tools/run_native_arm_validation.py`. The compiled headers, runtime, regression
sources, fixtures and build configuration are unchanged, but the added Mac
runner means the entire difference is **not documentation-only**. No native
Windows execution at `465436d` is asserted.

This review started clean at documentation commit
`306a4a4c98d7493551cbe99548695372e9795b05`, tree
`c23bf003477a9f01323f24ff748b9f9870d90068`. It is available locally and was
confirmed after fetching `origin`. Compared with `465436d`, it changes only
`README.md` and four Markdown files under `docs/validation/`: the index, Mac
report, workflow and template. This does not warrant rerunning a matrix solely
for those documentation changes, and does not establish an executed Windows
pass at the review HEAD. No raw Mac evidence packet is present on this Windows
host; the Mac results and archive identifiers were read from its portable report.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| Physical CPU, OS and process | AMD Ryzen 9 7950X, AuthenticAMD; Windows 11 Pro 10.0.26200; native x64 process |
| Native execution proof | Retained machine inventories; CMake x64 compiler records; per-case executables inspected as PE machine `0x8664`, with recorded hashes rechecked |
| Primary compiler | MSVC 19.50.35723.0, x64 driver, Visual Studio 2026 |
| Secondary MSVC | 19.44.35221.0, x64; final selected AVX2 Release and AVX-512 Release profiles |
| Clang drivers | Native GNU-driver `clang++` and `clang-cl`, version 20.1.8, Windows x86-64 target; not the separately installed default i686 executable |
| SDK / build tools | Windows SDK 10.0.26100.0; CMake/CTest 4.2.1; Ninja 1.12.1; Python 3.11.9 |
| Clang UBSan runtime | Compiler-rt `ubsan_standalone` and C++ x86-64 libraries; Debug builds use static release CRT `MultiThreaded` / `libcmt`, retaining `-O0` and debug information |
| Intentional-UB control | Ordinary Clang control exit 0; signed-overflow control exit 1 with the expected diagnostic and no continued execution |
| Upstream testbench | Separate clean checkout at `a7293702385e5169a5f58b83177f2a43cd59139b`; all three original source hashes match both retained manifests; provenance and scalar adapters preserved |
| Higher-ISA support | Runtime consumer: `instrset_detect=11`, `XCR0=0xe7`, FMA3/F16C enabled, AVX-512 FP16 absent; pointers 64 bits, Windows `long` 32 bits |

## Executed matrix

These are the final retained `-verified` attempts at the fixed source, checked
against JUnit, registration JSON, command exits, complete test logs, compiler
metadata and executable hashes. Timeout counts are a subset of failures; there
are none in this selected matrix. MD/MDd below mean release/debug DLL CRT;
MT means static release CRT. Ordinary profiles exercise logical widths
128/256/512; selected higher-ISA profiles have the narrower scope below.

| Compiler/driver | Backend / ISA / logical width | Build mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | --- |
| MSVC 19.50 | x86 SSE2, 128/256/512 | Release, no UBSan, MD | 40 | 40 | 0 | 0 | 0 | verified rerun |
| MSVC 19.50 | x86 SSE2, 128/256/512 | Debug, no UBSan, MDd | 40 | 40 | 0 | 0 | 0 | verified rerun |
| MSVC 19.50 | selected SSE4.2, 128/256/512 | Release, no UBSan, MD | 1348 | 1348 | 0 | 0 | 0 | verified rerun |
| MSVC 19.50 | selected SSE4.2, 128/256/512 | Debug, no UBSan, MDd | 1348 | 1348 | 0 | 0 | 0 | verified rerun |
| MSVC 19.50 | selected AVX-512 | Release, no UBSan, MD | 7 | 7 | 0 | 0 | 0 | verified rerun |
| MSVC 19.50 | selected AVX-512 | Debug, no UBSan, MDd | 7 | 7 | 0 | 0 | 0 | verified rerun |
| MSVC 19.44 | selected AVX2 | Release, no UBSan, MD | 7 | 7 | 0 | 0 | 0 | verified rerun |
| MSVC 19.44 | selected AVX-512 | Release, no UBSan, MD | 7 | 7 | 0 | 0 | 0 | verified rerun |
| clang++ 20 | x86 SSE2, 128/256/512 | Release, no UBSan, release DLL CRT | 40 | 40 | 0 | 0 | 0 | verified rerun |
| clang++ 20 | x86 SSE2, 128/256/512 | Debug, strict UBSan, MT | 40 | 40 | 0 | 0 | 0 | verified rerun |
| clang++ 20 | selected SSE4.2, 128/256/512 | Release, no UBSan, release DLL CRT | 1348 | 1348 | 0 | 0 | 0 | verified rerun |
| clang++ 20 | selected SSE4.2, 128/256/512 | Debug, strict UBSan, MT | 1348 | 1348 | 0 | 0 | 0 | verified rerun |
| clang++ 20 | selected AVX-512 | Release, no UBSan, release DLL CRT | 7 | 7 | 0 | 0 | 0 | verified rerun |
| clang-cl 20 | x86 SSE2, 128/256/512 | Release, no UBSan, MD | 40 | 40 | 0 | 0 | 0 | verified rerun |
| **Final selected total** | | | **5627** | **5627** | **0** | **0** | **0** | |

Each ordinary 40-case profile includes 34 compiled cases plus six Python
configuration/fixture checks. Each 1,348-case profile adds 1,308 pinned
upstream cases. Each selected seven-case ISA profile comprises four core/
namespace cases, one classification stress case and two SSE2 partial-memory
cases; it is not a full higher-ISA matrix and does not include the six Python
checks. Nine ordinary profile examples each printed `2 5 10 17`. The separately
linked runtime consumer and guard controls are additional controls, excluded
from suite totals.

All retained native suite attempts reconcile as follows. These rows include
repeated case executions and multiple changing source checkpoints; they are
not additional unique coverage at the final source.

| Retained attempt set | Suite invocations | Case executions | Passed | Failed | Skipped | Timed out |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Initial Windows session, all named CTest logs | 15 | 5733 | 5729 | 4 | 0 | 0 |
| Follow-up, initial named stage | 14 | 5627 | 5623 | 4 | 0 | 0 |
| Follow-up, `-final` stage | 14 | 5627 | 5624 | 3 | 0 | 0 |
| Follow-up, `-verified` stage selected above | 14 | 5627 | 5627 | 0 | 0 | 0 |
| **All retained native suite invocations** | **57** | **22614** | **22603** | **11** | **0** | **0** |

The initial session's audited selection itself was 5,611/5,613, including the
two MSVC AVX-512 failures; its other native suite attempts account for the
remaining initial-session executions. Compilation failures, standalone
reproducers, intentional negative controls, examples and ARM emulation are
excluded from these suite totals.

## Controls, flags and numerical contracts

- Rechecked all 11,240 per-case executable hash records across the two audited
  selections and their x64 PE headers, plus 126 initial-session and 127
  follow-up additional executable hash records. Counts include repeated
  references to shared interpreters; they are not unique binary counts.
- Clang Debug strict UBSan retains `-fsanitize=undefined`,
  `-fno-sanitize-recover=undefined`, frame pointers and matching link options.
  The retained launch scripts explicitly set
  `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. The intentional-overflow
  and writable controls are retained from the initial native session; this
  review did not rerun them. Final suite logs contain no UBSan diagnostics.
- Native Windows guard controls used 4,096-byte pages and 65,536-byte allocation
  granularity. Writable access returned 0; all four independent forbidden
  read/write accesses at both boundaries returned `0xc0000005`. The protected
  memory abstraction and partial-count/canary contracts are retained unchanged
  in the fixed source. These are Windows controls, distinct from the separate
  QEMU/POSIX guard controls.
- Ordinary MSVC regressions retain `/fp:strict`; only classification uses
  `/fp:fast`, which is broader than Clang's finite-math-only option. Clang and
  clang-cl retain `-ffp-contract=off`, selected `-frounding-math`, and isolated
  `-ffinite-math-only` classification stress. The selected ISA consumers name
  this target `classification`; ordinary builds call it `fp_classification`.
- All six configuration/independent fixture checks passed in each ordinary
  profile. Numerical gates, inputs, seeds, original expected values and
  60-second deadlines remain intact. The source comparison found no fixture,
  regression or build-configuration changes between the final Windows source
  and the later Mac-tested source.
- Build/test concurrency was three. Retained launch scripts and command/exit
  journals identify exact invocations; verbose build logs preserve actual
  compile and link options. No benchmark timings were collected in this review.

## Failures, retries and coverage limits

- Initial native MSVC and Clang SSE2 Release each failed `fp_classification`.
  The harness checkpoint passed their final profiles after the focused SSE2
  classification changes. Earlier MSVC compilation attempts also failed during
  harness adaptation; their logs remain, and they are not suite executions.
- At that initial checkpoint, MSVC AVX-512 `core_512` and `core_namespace`
  failed. The compiler reproducer also failed against pristine upstream under
  MSVC 19.50 and 19.44. Commit `aa3836a` supplied the integer-mask workaround;
  the final selected higher-ISA profiles pass. This is a source fix followed
  by new execution, not a passing replay of the unfixed source.
- Follow-up initial attempts failed `fp_classification` in Clang SSE2 Release,
  clang-cl SSE2 Release and Clang SSE4.2 Release, plus `classification` in
  Clang AVX-512 Release. The middle `-final` stage still failed the three
  ordinary classification cases. The independently referenced classification
  regression and GNU finite-math correction in the fixed source passed the
  final `-verified` profiles. Every failed invocation is retained under its
  own command label; assertions were not disabled.
- The first standalone runtime-consumer build failed because the probe used
  `xgetbv` instead of the available intrinsic spelling. The corrected probe
  built and produced the recorded ISA/OS-state values. Its initial failure is
  retained and excluded from suite case totals.
- Initial Clang UBSan Debug CRT and release DLL CRT builds failed to link on
  iterator-debug/runtime-library mismatches. Fresh static release CRT Debug
  builds passed; LNK4217 import warnings remain in their logs. Debug CRT UBSan
  is not validated with this installed runtime.
- MSVC ignored the undefined-sanitizer option and continued the deliberate UB
  with exit 0. The harness refuses that configuration. MSVC Debug and
  unsanitized clang-cl are not UBSan passes. ASan was not executed.
- Earlier ARM/Linux Clang 14 and GCC 12 attempts failed; the later 336 local
  ARM/POSIX passes used Clang 19 and QEMU under WSL. They contribute zero
  native AMD or native Mac case instances. The subsequent native Mac result is
  described in its own report and was not re-audited from raw Mac evidence here.
- Full AVX2/AVX-512 API matrices, AVX-512 FP16, other ARM toolchains/hosts,
  unselected APIs, ASan and a Windows run at the exact Mac-tested/current
  revision remain unexecuted in this evidence. Physical Intel is omitted.
- Exact per-invocation source trees are not captured for every intermediate
  harness/fix attempt. The selected checkpoints have source manifests and
  patches; earlier attempt counts/names come from their unique command logs.
  Retained initial/pre-verified JUnit and logs supplement the final JUnit, but
  do not imply a complete source/binary snapshot for every intermediate state.

## Evidence and handoff

| Evidence packet | Host-local ignored directory | SHA256 | Contents / attempt |
| --- | --- | --- | --- |
| `return-evidence.zip`, assembled during this review | `build/windows-report-review-20261115/` | `bb5508b6d436d6ad6167376a8aa6517224095be7de73f77480a9249d9cbd43e2` | 1,201 entries: retained Windows inputs, clearly separate ARM emulation records, per-profile metadata/JUnit/logs and the retrospective audits |

The new archive passed CRC and every entry's SHA256 manifest check. This is a
new transfer packet of existing evidence, not an archive recorded at original
execution time. The two historical evidence directories remain unchanged:
`build/windows-validation-20261115/` and `build/windows-avx512-20261115/`, with
their original build directories. Retain executables/objects locally; the
transfer packet includes hash records and raw logs rather than those binaries.
Raw paths inside the private packet are host-local; public links depend only
on repository documents.

The original working patches, source manifests and patch-series checksums were
recomputed and match their retained records. The fixed net review patch SHA256
is `169e8e058281aa0efb90a7110c2f700bda32b6b3f2570625ed50fe80e85db40e`;
the original initial-session patch and both source fingerprints are given above.
The independent audit also retains its initial checker diagnostics: selected
ISA classification target names needed to be recognized alongside ordinary
target names. Rechecking the actual flags resolved those checker errors without
changing any test, build or historical artifact.

During the evidence review, HEAD and local branches stayed at their starting
values; only `origin` was fetched. The maintainer subsequently approved
committing and pushing the report and index on `neon-port`. These documentation
changes do not become a tested source checkpoint, and raw evidence stays under
ignored `build/`. The next comparison can use the Mac report and this
source-difference record without relabeling either tested HEAD. Fresh Windows
execution is appropriate when Windows-relevant source or coverage changes
require it; documentation-only changes alone do not require a repeat of the
existing matrix. Further integration or evidence distribution follows the
maintainer's approval requirements.
