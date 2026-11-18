# Source ownership and architecture selection

Applications include `vcl2_xa64.h`. It selects headers from compiler-target
macros, without runtime dispatch or vector wrappers. ARM64EC and unsupported
or contradictory target/backend selections fail explicitly.

## Protected originals

All 28 paths recorded by `vendor/vectorclass.json` exist both at root and under
`vendor/vectorclass/`, byte-for-byte identical to upstream
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. This includes the root README,
LICENSE, changelog, examples, runtime detector and headers. Checkout attributes
protect their raw bytes. Vendor and its pin inventory are never rewritten.
Only a separately authorized upstream update may replace these originals.

The x86 facade path includes only the original vector and supported in-tree
math headers. It does not include ARM, sse2neon, adapted x86 repairs or compiler
workarounds. Restoring originals also restores their known limitations; see
[retained x86 findings](x86_findings_and_fix_candidates.md). Inherited faults
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
Consumers need only the checked-in headers, without Python or generation.

Optional runtime queries compile `instrset_detect_neon.cpp` on ARM and unchanged
`instrset_detect.cpp` on x86. CMake `vcl::runtime` selects this automatically;
`vcl_runtime.cpp` is the compile-time selector for a universal Apple target.
ARM reports Neon separately and no native x86 instruction sets. All modules
exchanging vector objects must use matching backend/width/namespace settings.

ARM headers also exclude obsolete x86-only code, including inactive branches.
The [cleanup audit](validation/arm-header-cleanup-audit-2026-11-18.md) and
[fresh M2 result](validation/mac-arm-2026-11-18-arm-header-cleanup.md) record the
removal and preserved ARM behavior. SSE-shaped compatibility intrinsics remain
required through sse2neon. Read-only integrity checks reject native x86 backend,
AVX and old classification-helper references in ARM-owned source.

## Read-only maintenance checks

```sh
python3 -B tools/regenerate_headers.py --check
python3 -B tools/check_header_generation.py
python3 -B tools/check_repository.py
```

The historical generator name remains for existing automation, but generation
is retired and `--write` always fails without writing. Exact vendor/root checks,
ARM include graph checks and inactive-output validation replace reproduction.
Integrity controls deliberately modify disposable copies under ignored `build/`,
including a newline-only root mutation. Real originals remain untouched.

The old canonical duplicates under `backends/arm64/` and `adaptation/include/`
are retired. The ordered historical patches remain inactive research under
`adaptation/patches/`; `backends/x86/` retains the isolated former MSVC helpers.
No active manifest outputs or patches exist. Their old writers cannot target
protected originals. Original source and generation workflow remain reachable
in Git history and earlier dated reports.

## Validation and source identity

The migration is published on `arch-separation-m2`. Use the [Mac handoff](validation/mac-arch-separation-handoff.md)
for its approval/scope and the [validation index](validation/README.md) for actual
results. Capture the full implementation with exact per-path hashes, patch and
added/deleted paths before native execution. Earlier 8,036-case results apply
to their original sources. The new M2 cleanup result validates its recorded
ARM implementation; pristine x86 still requires native Windows verification.
Native Windows verification remains a separate same-source gate. No benchmark
or x86 repair campaign is part of architecture separation.

The [18 November M2 result](validation/mac-arm-2026-11-18-arm-header-cleanup.md)
records the complete frozen candidate and retained failed setup/build attempts.
Native Windows verification remains pending in the
[return handoff](validation/windows-arch-separation-handoff.md). No old report
acquires a new source identity. The [publication comparison](validation/arch-separation-publication-2026-11-18/README.md)
records metadata differences without transferring a native pass to a later commit.
