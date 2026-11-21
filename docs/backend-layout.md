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
originals. The upstream inventory remains in the protected audit manifest.

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

The `backends/` and `adaptation/` catalogs are retired. Current integrity and
include-graph checks reject their reintroduction; neither active backend applies
historical adaptation patches. Maintain one ARM implementation in root
`*_neon.h` and keep x86 originals pristine. Generation outputs and `--write`
remain rejected. Historical reproduction uses its own frozen source and tools.

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

The accepted x86 scope preserves the pinned original implementation and its
known limitations behind the facade. It is not a universal correctness pass.
The [Windows matrix](validation/x86/windows-amd-2026-11-19-arch-separation.md)
retains **5,387 passed, 249 failed and 76 not run of 5,712**, plus twelve passing
examples. The [targeted triage](validation/x86/windows-amd-2026-11-19-arch-separation-triage.md)
records bounded original/facade comparisons and inherited issues; selected
matches do not convert other failures into passes or establish universal parity.
The upstream lookup contract permits read-ahead beyond a logical table;
stronger ARM page-boundary expectations, numerical gates and sanitizer/guard
controls remain in force. Preserve independent fixtures, seeds, FP flags,
60-second deadlines and concurrency three. Do not reapply retired x86 repairs.

The [21 November M2 correctness report](validation/arm64/mac-arm-validation-2026-11-21.md)
and [standard-math results](benchmarks/arm64/STANDARD_MATH_RESULTS_2026-11-21.md)
record the latest retained ARM64 checkpoint. Every native result keeps its exact
tested source, tools and coverage; a later documentation commit inherits no pass.
Use the [native workflow](validation/native-workflow.md),
[ARM64 publication checklist](validation/arm64/results-publication-checklist.md)
and [Windows runner](validation/x86/windows-runner.md) for new work.

The [four-blend decision](validation/arm64/blend4-integration-2026-11-19.md)
retains the accepted ARM specializations and measured inverse cost. The
[TRS follow-up decision](validation/arm64/trs-inverse-decision-2026-11-20.md)
keeps that runtime. Separate native passes do not establish cross-host numerical
parity, application coverage or an x86 correctness pass.

The completed separation job's rename/guard map and integration decision are
historical records, available at their published source:
[frozen map](https://github.com/DigitalExpressions/vcl2-xa64/blob/5dace327bd7206f6e2bb87d8d1c81e2858161738/docs/arch-separation-map.json)
and [frozen decision](https://github.com/DigitalExpressions/vcl2-xa64/blob/5dace327bd7206f6e2bb87d8d1c81e2858161738/docs/validation/architecture-separation-integration-2026-11-19.md).
Current ownership and the accepted limits above supersede their execution and
cleanup instructions. No original runtime, test, numerical policy or evidence
record was changed by retiring those local documents.
