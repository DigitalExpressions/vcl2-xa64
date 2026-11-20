# ARM64 vector and matrix benchmarks

Compares scalar FP32/FP64 arithmetic, sse2neon compatibility and Direct Neon on
the same ARM64 machine. Scalar builds disable automatic SIMD vectorization.

Workloads include vector arithmetic, dot/cross products, length/normalization,
matrix-vector and matrix multiplication, transpose and TRS inverse. They cover
object-by-object and batched storage, small and larger arrays, and logical SIMD
widths of 128, 256 and 512 bits.

Each executable supports `--check` for independent numerical-reference and
canary checks before timing. Timing reports measure complete operations and
retain input/output hashes and repeated samples. Results depend on the exact
source, compiler and host.

See the [build instructions and correctness contracts](../README.md) and
[ARM64 report reading guide](../../docs/benchmarks/arm64/standard_math_reading_guide.md).
