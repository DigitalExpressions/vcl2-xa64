# x86 VCL comparison benchmarks

Compares this fork's native x86 implementation with a pinned revision of Agner
Fog's original VCL on the same Windows x64 machine, using MSVC and Clang.
The runner compiles identical workloads and inputs against each library.

Workloads include FP32/FP64 arithmetic, square root, rounding, classification,
conversion, array arithmetic, sine/exp, and integer division and lookups. The
selected cases cover logical vector widths of 128, 256 and 512 bits, with CPU
and OS support checked before higher-ISA execution.

Correctness against scalar references and protected-memory controls precedes
timing. `edge_probe.cpp` provides separate untimed exceptional-value checks.
Paired timing samples compare the two library builds; they measure different
workloads from the ARM64 vector/matrix harness.

See the [comparison protocol and runner commands](../../docs/benchmarks/x86/x86-upstream-comparison.md).
