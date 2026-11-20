# Numerical contracts

Correctness tests retain the existing inputs, seeds, numerical gates and
special-value policies. These functions are bounded vector-math APIs rather
than full-range scalar-libm replacements. Use the documented FP environment;
validation uses nearest rounding and flush-to-zero disabled unless a test
explicitly exercises another supported mode. Explicit FMA helpers follow the
selected backend/ISA: Direct Neon and FMA-enabled x86 paths fuse, while legacy
fallbacks may round the multiply and add separately. ARM correctness, strict
comparison and benchmark builds disable implicit contraction with
`-ffp-contract=off`; strict MSVC comparisons use `/fp:strict`. Routine Windows
`upstream-optimized` tests instead use MSVC `/O2 /fp:precise /fp:except-` or
Clang `-O2 -fno-trapping-math -fno-math-errno`. Keep these profiles and their
results separate, as described in the [x86 guide](x86-build-and-test.md).
The classification stress target alone uses `/fp:fast` or finite-math flags;
no broad fast-math option applies to the ordinary suite.

The table records retained regression expectations and ARM behavior. Pristine
original x86 headers keep their upstream contracts, including documented table
padding requirements; the suite may demand stronger behavior in selected cases.
Its [failed/incomplete Windows gate](validation/README.md#windows-gate-and-architecture-preservation)
preserves those differences without changing originals or weakening tests.

| Surface | Scope |
| --- | --- |
| Integer operations | Wrapping arithmetic, shifts, masks, conversions and supported divisor templates; invalid compile-time cases are rejected |
| Memory/movement | Partial-count policies, canaries, protected-page boundaries and compile-time index rejection |
| Exp/log | Conservative VCL exponent cutoffs and documented subnormal policies; independent Decimal fixtures |
| Powers/cube roots | Intermediate overflow/underflow limits and explicit exceptional-value tests |
| Native radian trig | Accuracy-gated within ±1e7f / ±1e15; beyond cutoffs including infinity, sin/tan return signed zero and cos returns one |
| Inverse/pi-scaled trig | Domain, quadrant, parity, pole and signed-zero checks at all selected widths |
| Sinh/cosh | Conservative overflow cutoffs at absolute input 89.0f / 709.7 |
| Reciprocal/rsqrt | Backend-specific estimates, precision and special-value/underflow behavior |

Legacy x86 RCPPS tiny estimates flush to correctly signed zero. The selected
test retains its prior 2^-11 relative tolerance outside tiny/boundary intervals;
the boundary accepts only the explicitly permitted zero or normal estimate.
ARM accuracy rules and rsqrt gates remain unchanged. Protected-page tests and
strict nonrecovering UBSan are independent safeguards; they do not establish
ASan coverage on the host where ASan startup hangs.

The implementation and executable tests are the precise contracts. See
the [shared comparison policies](validation/cross-platform/README.md) for
source-derived FMA and non-default `roundi` behavior differences. These are
reported explicitly rather than treated as universal bitwise equality. See
[tests](../tests/README.md) and the [validation index](validation/README.md)
for current instructions and dated selected matrices. No general emulation
of ARM exception flags/masks or Intel SVML is provided.
