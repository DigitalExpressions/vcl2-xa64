# Provenance and third-party notices

- VCL2 originates with Agner Fog. Existing source copyright/license notices and
  the root [LICENSE](../LICENSE) and [changelog](../changelog.txt) are retained.
  All 28 original files at `6675774` remain exact: 27 at root and the
  [upstream README](../audit/vectorclass-README.md) under `audit/`, with a complete
  [blob/SHA inventory](../audit/vectorclass-2.02.04.json) and
  [x86 revision history](../audit_x86_history.md). The duplicate vendor snapshot
  is retired; an audited comparison copy can be restored under ignored build.
  Fork adaptations remain separate; see the [layout guide](backend-layout.md).
- The added native port sources carry DigitalExpressions Sweden's full
  Apache-2.0 notice. Extracted/adapted helpers retain their original Agner Fog
  attribution; this notice identifies the port-specific contributions.
- Earlier ARM compatibility work came through
  [yuygfgg/vectorclass_arm](https://github.com/yuygfgg/vectorclass_arm). This fork
  carries that compatibility work and the Direct Neon port onto official VCL
  2.02.04 while retaining the original source notices.
- The sole bundled [sse2neon.h](../sse2neon.h) is at root and retains its existing
  MIT notice and exact bytes. The vendor catalog is retired; the required
  [audit pin](../audit/sse2neon-2272251.json) verifies this header directly. The
  [ARM64 dependency history](../audit_arm64_history.md) identifies its upstream
  snapshot and documents representation differences with both raw hashes.
- The optional upstream testbench is a separate GPL-3.0 checkout at the pinned
  revision documented in [tests](../tests/README.md). Its generated copies and
  embedded matching fragments in the adapter/test configuration retain that
  provenance. It is not bundled with the public runtime headers.
- GfxMath is an optional external consumer illustrated by the inline examples
  in the fork README. Its sources, tests and license are not copied into this library.

Upstream repositories: [VCL2](https://github.com/vectorclass/version2),
[testbench](https://github.com/vectorclass/testbench),
[sse2neon](https://github.com/DLTcollab/sse2neon).
