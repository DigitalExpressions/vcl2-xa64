# Manual native Windows runner

Run tests manually on the maintainer's AMD Windows x86-64 machine. The Mac M2
uses the separate [native Mac runner](../../tools/run_native_arm_validation.py)
and [native workflow](native-workflow.md). Hosted CI is disabled by the
maintainer's decision; pushes and pull requests do not launch test workflows.
Read [validation status](README.md) for actual executed results and the
[integration plan](../integration-plan.md) for remaining review/approval work.

| Manual selection | Expected case instances |
| --- | ---: |
| Windows MSVC Release and unsanitized Debug, local suite | 80 |
| Windows GNU-driver Clang Release and strict UBSan Debug, local suite | 80 |
| Each Windows profile with the separately pinned upstream selection | 1348 |

Counts are registration checkpoints, not claims of successful execution.
The four full pinned profiles total 5,392 cases. Higher-ISA matrices remain
separate native checks requiring CPU/OS support probes. Physical Intel is omitted.

## Developer shell and execution

`tools/run_windows_validation.ps1` locates Visual Studio through its installer, initializes
the x64 developer environment, and selects a GNU-driver Clang whose reported
target is Windows x86-64. It delegates to
[the Windows runner](../../tools/run_windows_validation.py). Missing tools or
unsupported sanitizer runtimes fail the run; they are not skipped or counted
as passing. MSVC Debug is not UBSan coverage.

From an x64 Visual Studio developer shell at the repository root:

```powershell
python -B tools/run_windows_validation.py --driver msvc --mode Release
python -B tools/run_windows_validation.py --driver msvc --mode Debug
python -B tools/run_windows_validation.py --driver clang --mode Release --compiler 'C:/path/to/native/x64/clang++.exe'
python -B tools/run_windows_validation.py --driver clang --mode Debug --sanitize --compiler 'C:/path/to/native/x64/clang++.exe'
```

Use an installed compiler's actual path. `--plan` prints registration and mode
settings without launching validation. An existing pristine upstream checkout
can be supplied with `--testbench-dir build/_deps/testbench`; its revision and
source hashes remain enforced. The PowerShell wrapper's `-Upstream` option
restores the pinned checkout if absent and preserves any existing directory.

For example, from a shell whose execution policy permits local scripts:

```powershell
./tools/run_windows_validation.ps1 -Driver msvc -Mode Release -Upstream
./tools/run_windows_validation.ps1 -Driver clang -Mode Debug -Sanitize
```

Do not change machine-wide execution policy for this tool; an existing x64
developer shell can invoke the Python entry point directly.

The runner verifies native Windows x64 execution through `IsWow64Process2` and
PE machine types, records the CPU/OS and compiler, and captures HEAD/tree,
working patches, added-file patches and source fingerprints. It verifies a
runtime consumer and independent protected-page controls before the suite.
The consumer requires detected ISA level 2 for the local suite and level 6
before executing the optional SSE4.2 upstream selection.
The writable control must pass; read and write accesses at both inaccessible
boundaries must return `0xc0000005` using the actual Windows page size.

Clang UBSan uses static release CRT (`MultiThreaded`) even in Debug, with
`-O0`, debug information, `-fsanitize=undefined`, disabled recovery, frame
pointers and `UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. An ordinary
control must pass and intentional signed overflow must stop with its expected
diagnostic. A link or control failure is an unavailable/failing gate. The
known local runtime emits LNK4217 import warnings, retained in raw logs.

The suite retains all six Python configuration/fixture checks, original
numerical gates, the 60-second per-case timeout, and build/test concurrency
three. Ordinary MSVC tests use `/fp:strict`; classification alone uses
`/fp:fast`. Clang retains no contraction, selected rounding-mode options and
classification-only finite-math stress. No ASan or timing benchmarks run.

Configuration probes retain their original serial execution, positive flag
controls, negative contracts and 60-second CTest deadline.

## Evidence and failed runs

Each Windows invocation creates `build/windows-native-<driver>-<mode>-<UTC>/`.
It retains commands/exits, patches, manifests, configure/build/test logs,
compile commands, CTest inventories, JUnit, control/binary hashes and results.
The runner produces `return-evidence.zip` and its SHA256 on success or failure
after evidence capture starts. Preserve full build directories locally.

Transfer evidence packets separately and record their SHA256 in portable
reports. Keep failed attempts and their original archives. A setup failure
before the runner starts may have only shell output; record that coverage gap.
There are no hosted uploads or retention guarantees. The Mac ASan startup hang
is not retried. Physical Intel testing remains omitted.
