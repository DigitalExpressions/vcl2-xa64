# Source ownership and architecture selection

Applications include `vcl2_xa64.h`. It selects headers from compiler-target
macros, without runtime dispatch or vector wrappers. ARM64EC and unsupported
or contradictory target/backend selections fail explicitly.

## Protected originals

All 28 files recorded by [the audited manifest](../audit/vectorclass-2.02.04.json)
remain byte-for-byte identical to upstream
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. The 27 source/license files remain
at root; the [original upstream README](../audit/vectorclass-README.md) is retained
under `audit/`. On 20 November the maintainer approved consolidating fork usage
and upstream resource links into root [README.md](../README.md), which is now
editable fork documentation. The upstream pin, hashes and tree are unchanged.
Checkout attributes
protect their raw bytes. The duplicate vendor snapshot was retired with maintainer
authorization; its pin moved byte-exact into `audit/`. The
[x86 revision history](../audit_x86_history.md) identifies the current version.
Read-only integrity checks verify all 28 SHA-256 hashes, Git blobs and the upstream tree.
`python -B tools/restore_upstream_vcl.py` reconstructs the exact comparison copy
under ignored `build/upstream/vectorclass-6675774/`; existing copies are verified,
never overwritten. No network or generation is required by library consumers.
Only a separately authorized upstream update may replace these originals.

The x86 facade path includes only the original vector and supported in-tree
math headers. It does not include ARM, sse2neon, adapted x86 repairs or compiler
workarounds. Restoring originals also restores their known limitations; see
[retained x86 findings](https://github.com/DigitalExpressions/vcl2-xa64/blob/2237ffbdc23b23cda4358acf29cd494b2d63ad8c/docs/x86_findings_and_fix_candidates.md). Inherited faults
remain failures, not reasons to patch originals or weaken ARM gates.

## ARM ownership

Root `*_neon.h` files are authoritative editable ARM sources. The existing
`vectori128_neon.h`, `vectorf128_neon.h` and `instrset_neon.h` remain Direct Neon.
Distinct `vectori128_compat_neon.h`, `vectorf128_compat_neon.h` and
`instrset_compat_neon.h` retain compatibility behavior. Common conversion,
logical-wide composition and math have their own ARM headers/guards, including
bodies originally shared with upstream. Their includes cannot reach root x86
originals. The [file map](arch-separation-map.json) records all dispositions.

`vcl_config.h` retains default compatibility, explicit experimental Direct Neon,
logical width and namespace contracts. Root `sse2neon.h` is the sole shipped
compatibility dependency and retains its exact bytes and MIT notice; ARM-private
helpers use `_neon.h` suffixes. The vendor catalog is retired.
The [ARM64 dependency history](../audit_arm64_history.md) records its upstream
snapshot and exact retained representation without changing the shipped bytes.
Consumers need only the checked-in headers, without Python or generation.

Optional runtime queries compile `instrset_detect_neon.cpp` on ARM and unchanged
`instrset_detect.cpp` on x86. CMake `vcl::runtime` selects this automatically;
`vcl_runtime.cpp` is the compile-time selector for a universal Apple target.
ARM reports Neon separately and no native x86 instruction sets. All modules
exchanging vector objects must use matching backend/width/namespace settings.

ARM headers also exclude obsolete x86-only code, including inactive branches.
SSE-shaped compatibility intrinsics remain required through sse2neon. Read-only
integrity checks reject native x86 backend, AVX and old classification-helper
references in ARM-owned source. Preserve ARM correctness and generic compiler
portability while maintaining that boundary.

## Read-only maintenance checks

```sh
python3 -B tools/regenerate_headers.py --check
python3 -B tools/check_header_generation.py
python3 -B tools/check_repository.py
```

The historical generator name remains for existing automation, but generation
is retired and `--write` always fails without writing. The required
`audit/sse2neon-2272251.json` pins the root dependency and its in-memory upstream
representation proof. The checker rejects reintroduced `vendor/` and
`adaptation/` paths.
Exact root/manifest and sse2neon checks,
ARM include graph checks and rejection of generation outputs replace reproduction.
Integrity controls deliberately modify disposable copies under ignored `build/`,
including a newline-only root mutation. Real originals remain untouched.
Historical sources use their frozen historical checkers; the current checker
requires the current root-only dependency layout.

The old canonical duplicates under `backends/arm64/` and `adaptation/include/`
are retired. On 20 November the maintainer approved retiring the entire
`adaptation/` catalog, including its empty manifest and four inactive patches:
portability/correctness, MSVC x86 performance, defined AVX2 double-to-int64
conversion and x86 correctness. The
[frozen research index](https://github.com/DigitalExpressions/vcl2-xa64/blob/0ed548100f983688091bb80fd5c47438581190e7/adaptation/README.md)
and [complete patch set and manifest](https://github.com/DigitalExpressions/vcl2-xa64/tree/0ed548100f983688091bb80fd5c47438581190e7/adaptation)
preserve their provenance. Neither current backend applies them; ARM changes
are maintained directly in root `*_neon.h`, while x86 originals remain pristine.
The unused former MSVC classification helper was also
retired; its [frozen source](https://github.com/DigitalExpressions/vcl2-xa64/blob/41d7fbacc45fbb679b08f0074a8f340ced3a3f20/backends/x86/x86_classification.h)
remains in Git history. Reproducing those patches requires their historical source.
The remaining `backends/` research index was retired on 20 November; its
[frozen README](https://github.com/DigitalExpressions/vcl2-xa64/blob/7465b9833968bae97ef9fb1efa645a7ed994f8eb/backends/README.md)
preserves historical navigation. Include-graph checks continue to reject
`backends/` headers so inactive implementations cannot reenter the ARM graph.
The checker no longer needs an adaptation manifest. Candidate comparison checks
always verify the current separated layout, and candidate patches cannot add
retired adaptation/backend paths. Generation outputs and `--write` remain
rejected. Historical reproduction uses the frozen source and tools; original
source and generation workflow remain reachable in Git history and earlier
dated reports.

## Validation programs and retired examples

On 20 November the maintainer approved retiring `examples/`. Its arithmetic
smoke program moved byte-exact to `tests/vector_arithmetic.cpp`, and the ARM
dispatch programs moved byte-exact to `tests/dispatch1_neon.cpp` and
`tests/dispatch2_neon.cpp`. `VCL_BUILD_TESTS=ON` builds `vcl_example` under the
test build directory; both native runners still execute it separately. The
registered ARM dispatch test names and correctness inventories are unchanged.

The optional external GfxMath example and its CMake options were retired.
The [frozen examples folder](https://github.com/DigitalExpressions/vcl2-xa64/tree/0ed548100f983688091bb80fd5c47438581190e7/examples)
retains the complete source, including GfxMath and its old build integration.
The fork README retains its inline VCL and GfxMath usage examples and direct
compiler commands. Historical inventories, results and their source paths stay
unchanged; they describe the tested source at their original checkpoints.

## Validation and source identity

The migration was published on `arch-separation-m2` and accepted for local
integration into `master` on 19 November. The [integration decision](validation/architecture-separation-integration-2026-11-19.md)
defines x86 preservation/equivalence acceptance despite the failed/incomplete
[Windows matrix](validation/x86/windows-amd-2026-11-19-arch-separation.md). The
[targeted triage](validation/x86/windows-amd-2026-11-19-arch-separation-triage.md)
retains inherited limitations and distinguishes documented contract differences.
No original file or test gate was changed. Native results remain tied to their
separate exact captures, and no benchmark or x86 repair is part of integration.

The [19 November M2 result](validation/arm64/mac-arm-2026-11-19-blend4.md) records the
later frozen ARM implementation. The [Windows runner guide](validation/x86/windows-runner.md)
provides the maintained procedure; the dated matrix and triage above record the
migration outcome. Historical capture/publication proofs remain accessible through
frozen Git references in the acceptance record. No pass transfers to a later tree.

The fork README's former opening status block is maintained here and in the
[integration status](integration-plan.md) and [validation index](validation/README.md).
The [four-blend integration](validation/arm64/blend4-integration-2026-11-19.md)
retains the accepted ARM specializations and unresolved 2-3% TRS-inverse cost.
The [completed TRS follow-up](validation/arm64/trs-inverse-decision-2026-11-20.md)
keeps the current runtime. The Windows full gate remains failed/incomplete.
Superseded Mac plans remain available through the
[frozen cleanup record](https://github.com/DigitalExpressions/vcl2-xa64/blob/e55c54a7fbf4f6c0c1eb5ed30153f6b569650072/docs/validation/mac-arm-documentation-cleanup-2026-11-20.md);
the [Windows cleanup index](validation/README.md#windows-documentation-cleanup)
records its historical evidence. These records retain their original source
identities and are not new validation of the README consolidation.
