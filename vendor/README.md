# Active third-party dependency

`sse2neon/sse2neon.h` is the active MIT-licensed compatibility dependency.
The root `sse2neon.h` is an exact shipped alias; generation is retired.
Both copies and the [original fork-local record](sse2neon.json) are unchanged.
The [ARM64 history](../audit_arm64_history.md) identifies upstream snapshot
`2272251cc3ad9cba8b8c47c4065811cfb396b802` and records the retained encoding and
line-ending differences with separate raw hashes. Do not normalize either copy.

The duplicate VCL snapshot and its old pin path were retired with maintainer
authorization. The 28 upstream originals remain exact at repository root.
The [x86 history](../audit_x86_history.md) and
[relocated original manifest](../audit/vectorclass-2.02.04.json) retain provenance.
Use `python -B tools/restore_upstream_vcl.py` to reconstruct a verified comparison
directory under ignored build when required. It is not an active dependency.

An upstream update requires separate scope and fresh validation. See the
[ownership guide](../docs/backend-layout.md).
