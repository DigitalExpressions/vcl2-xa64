# Source ownership and architecture selection

Applications include `vcl2_xa64.h`. It selects headers from compiler-target
macros, without runtime dispatch or vector wrappers. ARM64EC and unsupported
or contradictory target/backend selections fail explicitly.

## Protected originals

All 28 paths recorded by [the audited manifest](../audit/vectorclass-2.02.04.json)
exist at root, byte-for-byte identical to upstream
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. This includes the root README,
LICENSE, changelog, examples, runtime detector and headers. Checkout attributes
protect their raw bytes. The duplicate vendor snapshot was retired with maintainer
authorization; its pin moved byte-exact into `audit/`. The
[x86 revision history](../audit_x86_history.md) identifies the current version.
Read-only integrity checks verify root SHA-256, Git blobs and the upstream tree.
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
logical width and namespace contracts. Third-party sse2neon retains its own
name/license and pristine dependency; ARM-private helpers use `_neon.h` suffixes.
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
is retired and `--write` always fails without writing. Exact root/manifest and sse2neon checks,
ARM include graph checks and inactive-output validation replace reproduction.
Integrity controls deliberately modify disposable copies under ignored `build/`,
including a newline-only root mutation. Real originals remain untouched.

The old canonical duplicates under `backends/arm64/` and `adaptation/include/`
are retired. The ordered historical patches remain inactive research under
`adaptation/patches/`. The unused former MSVC classification helper was also
retired; its [frozen source](https://github.com/DigitalExpressions/vcl2-xa64/blob/41d7fbacc45fbb679b08f0074a8f340ced3a3f20/backends/x86/x86_classification.h)
remains in Git history. Reproducing those patches requires their historical source.
No active manifest outputs or patches exist. Their old writers cannot target
protected originals. Original source and generation workflow remain reachable
in Git history and earlier dated reports.

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
