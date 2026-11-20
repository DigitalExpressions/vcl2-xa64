# Native AMD manual-runner preparation — 2026-11-15

## Outcome and tested source

**Passed after preparation changes and a retained prototype failure.** The
final manual Windows selection passed **160/160 cases** across four fresh
profiles, with zero failures, skips or timeouts. An earlier CPUID-corrected
snapshot also passed 2,776/2,776 cases including pinned upstream selections.
The maintainer selected manual execution on Mac M2 and AMD Windows only;
hosted test automation is removed and the existing workflow is disabled.

| Source identifier | Recorded value |
| --- | --- |
| Repository / execution branch | `DigitalExpressions/vcl2-xa64`, `neon-port` |
| Execution HEAD / its base tree | `08c3fe899ab9306c11eb58f101aece392c5d7374` / `f15a7cbb799d5cc14bee0e949fb3239da66f5160` |
| Working-tree state | Uncommitted preparation changes before and after execution; the manifest remained unchanged throughout each final profile |
| Final captured patch SHA256 | `a92fde3a4a91a09df3f7e629d15011cbe6266f31bb07184f10bb563d10c322ca` |
| Final source manifest fingerprint | `283542959ab7fbbc474d7ee8ad5e50c24a2f622c16f321a62139c86ec17fd0bb` |
| Reconstructed tested snapshot tree | `63b8941153ffd0fa2f0f816563ae7b71283afebc` |
| Manifest scope | 121 entries including the deleted workflow; present file contents or deleted state verified against the reconstructed tree with explicit LF/CRLF comparison |

The reconstructed tree was produced from retained tracked/added-file patches
in a temporary index. It is not a commit and did not move the real index or
any branch. Added files in this snapshot are `docs/README.md`,
`docs/integration-plan.md`, `docs/integration-review.md`,
`docs/validation/windows-runner.md`, `tools/run_windows_validation.ps1` and
`tools/run_windows_validation.py`. The existing GitHub Actions workflow is
deleted. Later report/plan edits and a later commit are not the execution HEAD.
Pre-commit cleanup also removed one extra EOF blank line in the Windows Python
runner; its parsed syntax tree was verified unchanged.

The earlier pinned preparation used the same base HEAD with source fingerprint
`c9405df9ac16a563b1df0347dbb83871ef157772dca1fd54e576261eaeff1325`,
patch SHA256 `90667aaf46b6cea8136bcd7e26c17cef5bcd784a8c117251da689306e82cbfe1`,
and reconstructed tree `438b5fb407537fc5093c8ef01c7022ee2d39a2f4`.
It predates the workflow decision and controller changes; it is not relabeled
as the final manual snapshot. Its library source and original serial
configuration probes match the final manual source. The final runner additionally
checks required ISA level before execution and handles deleted-file manifests.

Comparison with the [Mac report](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/mac-arm-2026-11-15.md): the Mac tested
`465436de204ebdd59e9b5071c353df372148b3df`, tree
`ff966a90ebfd9a2bc07c76c8418684df3a04ffa8`. New preparation changes include the
x86-guarded CPUID correction, manual Windows tools, UTF-8 repository checking,
workflow removal and developer documentation. The ARM library paths and
original configuration contracts are unchanged. This is a reviewed source
comparison, not a new native Mac execution or a whole-tree identity claim.

## Host, tools and dependencies

| Item | Observed value |
| --- | --- |
| Physical CPU / OS / process | AMD Ryzen 9 7950X; Windows 11 Pro 10.0.26200; native x64 Python |
| Native execution proof | `IsWow64Process2`: native machine `0x8664`, process machine `0`; control/test executables inspected as x64 PE with SHA256 records |
| Compilers | MSVC 19.50.35723.0 and GNU-driver Clang 20.1.8 targeting Windows x86-64 |
| Build tools | CMake/CTest 4.2.1, Ninja 1.12.1, Python 3.11.9, Windows SDK 10.0.26100.0 |
| UBSan / CRT | Compiler-rt x86-64 standalone/C++ runtime; static release CRT `MultiThreaded` in Clang Debug, with `-O0` and debug information |
| UBSan control | Positive exit 0; intentional overflow exit 1 with the signed-overflow diagnostic and no continued execution |
| Pinned upstream dependency | `a7293702385e5169a5f58b83177f2a43cd59139b`, clean, original three source hashes verified for the pinned selection |
| ISA requirement | Final local suite requires detected level 2; pinned selection requires level 6; observed level 11. An impossible level-99 control correctly failed |

## Executed matrix

The final manual-only snapshot uses the original serial configuration fixture.
Timeout counts are a subset of failed counts; both are zero in the final run.

| Compiler/driver | Backend / ISA / logical width | Build mode / sanitizer / CRT | Registered | Passed | Failed | Skipped | Timed out | Attempt |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | --- |
| MSVC 19.50 | x86 SSE2, 128/256/512 | Release, no UBSan, release DLL CRT | 40 | 40 | 0 | 0 | 0 | final manual |
| MSVC 19.50 | x86 SSE2, 128/256/512 | Debug, no UBSan, debug DLL CRT | 40 | 40 | 0 | 0 | 0 | final manual |
| clang++ 20 | x86 SSE2, 128/256/512 | Release, no UBSan, release DLL CRT | 40 | 40 | 0 | 0 | 0 | final manual |
| clang++ 20 | x86 SSE2, 128/256/512 | Debug, strict UBSan, static release CRT | 40 | 40 | 0 | 0 | 0 | final manual |

Each profile includes six Python configuration/fixture checks and 34 compiled
regressions. Each example printed `2 5 10 17`. Runtime/guard/intentional-UB
controls are separate from suite case counts.

| Preparation snapshot | Case executions | Passed | Failed | Skipped | Timed out |
| --- | ---: | ---: | ---: | ---: | ---: |
| Initial runner prototype | 160 | 160 | 0 | 0 | 0 |
| CPUID-corrected pinned preparation: MSVC Release / Clang UBSan 1348 each, MSVC Debug / Clang Release 40 each | 2776 | 2776 | 0 | 0 | 0 |
| Parallel configuration prototype with shared PDB | 160 | 159 | 1 | 0 | 0 |
| Isolated-directory prototype | 160 | 160 | 0 | 0 | 0 |
| Final manual-only source with original serial probes | 160 | 160 | 0 | 0 | 0 |
| **All native suite executions in preparation** | **3416** | **3415** | **1** | **0** | **0** |

These are 20 suite invocations across changing preparation snapshots, not
3,416 distinct cases at one source. Four additional startup failures executed
no suite cases and are recorded below. Compiler-negative probes and old hosted
runs are excluded from these native suite totals.

## Controls, flags and numerical contracts

- Every final profile verified native process/binary architecture, a runtime
  consumer, a writable-page control and all four forbidden read/write boundary
  accesses. Actual page size was 4,096 bytes; each forbidden access returned
  `0xc0000005`. The original partial counts, boundaries and canaries remain.
- Clang UBSan retains `-fsanitize=undefined`,
  `-fno-sanitize-recover=undefined`, frame pointers, matching link instrumentation
  and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. No suite diagnostic
  occurred; MSVC Debug is not sanitizer coverage. LNK4217 import warnings remain
  in the retained Clang link logs.
- MSVC ordinary cases retain `/fp:strict`; classification alone uses `/fp:fast`.
  Clang retains no implicit contraction, selected rounding options and isolated
  finite-math classification stress. Numerical gates, references and seeds did
  not change. Fixture checks and original 60-second deadlines remain active;
  build/test concurrency is three.
- Original serial configuration probes are retained. The discarded parallel
  prototype did not remove contracts or raise deadlines. No ASan or benchmark
  timing collection took place.

## Failures, retries and coverage limits

- Review reproduced a GNU/Clang i386 CPUID compile regression against the port,
  while the extracted upstream helper compiled. Restoring GNU selection and
  i386 intrinsic inclusion fixed the probe; i386/x86-64 syntax checks pass.
  Complete 32-bit runtime coverage is not claimed.
- A discarded parallel configuration prototype failed one MSVC Debug positive
  contract with C1041 due to shared `vc140.pdb` (39/40). Private directories
  corrected that prototype and 160/160 passed. The maintainer then chose manual
  native testing, so the original serial fixture was restored. Both prototypes
  and the final source remain distinguishable in the evidence.
- The first four manual-only startup attempts stopped before configuration
  because a manifest tried to read the deleted workflow. The runner now records
  deleted entries explicitly; fresh final profiles passed. No suite execution
  or control pass is claimed for those four startup failures.
- Existing hosted ARM timeouts and the cancelled stuck old run are historical
  evidence. Hosted execution is disabled and does not substitute for either
  native hardware report. Native Mac was not rerun in this Windows session.
- Full higher-ISA matrices, FP16, ASan and additional ARM toolchains/hosts remain
  separate coverage limits. Physical Intel testing is omitted. Direct Neon
  remains experimental. The earlier [AMD review](windows-amd-2026-11-15-823ca2a.md)
  retains its original 5,627-case scope and source identifiers.

## Evidence and handoff

| Evidence packet | Host-local ignored directory | SHA256 | Contents / attempt |
| --- | --- | --- | --- |
| `manual-integration-evidence.zip` | `build/master-integration-20261115/` | `d5aaeaf09cb0a00f9b79d541dec7e6fc6dd3eff6ecc9defd6318afb2f0126a4d` | Original prototype packets, startup failures, final manual packets, reconstructed source patches/trees, controls and workflow-disable/cancellation records |

The outer packet passed CRC and all entry checksums; individual runner packets
retain commands/exits, inventories, manifests, flags, configure/build/test logs,
JUnit and executable hashes. Keep full build directories locally; binaries are
not included in the compact transfer packets. Public links depend only on
repository documents; transfer raw packets separately.

The original execution HEAD stayed at `08c3fe8` throughout preparation tests.
These result/documentation changes are prepared for the approved commit/push
and draft PR into `master`; the maintainer's manual-only workflow replaces the
initial hosted-CI proposal. No merge, branch retirement or rename is authorized
by this report. The [integration plan](../integration-plan.md) records the next
review/approval steps, and the [native workflow](native-workflow.md) describes
manual validation on both physical hosts.
