# Frozen x86 optimization candidate

Status: Windows baseline-ISA correctness and targeted performance confirmation
are complete; paired higher-ISA selections retain failures, and native M2 is
pending. Read the [Windows report](../windows-amd-2026-11-16-x86-layout.md).
Read the [execution plan](../../x86-backend-optimization-plan.md) and
[Mac handoff](https://github.com/DigitalExpressions/vcl2-xa64/blob/d796768b30cb375ea62afa72dcab061b1a3e54ca/docs/validation/mac-arm-x86-optimization-handoff.md).

`candidate.patch` is an uncommitted five-file source/test patch against
`44f974ac31e1448e0beb5157d2bd8d669f5c518e`, with SHA256
`b954c2894ecac21ad59b3ca3d9ff10401f01fb8ac5552e96eeb4dc300c5855e4`.
It isolates native MSVC x86 classification in a private helper, preserves the
existing FP32 exponential calculation while keeping it inline on that compiler,
and adds mixed-lane integer-oracle classification checks. There are no numerical
policy, rounding/conversion sentinel or ARM backend changes.

`candidate-source.json` includes the exact selected library/test/build/runner
source hashes from the frozen export, and separate LF-normalized hashes for
explicit cross-host line-ending comparison. Its full-repository fingerprint
identifies the original Git-export-plus-patch inventory, not a later commit
with additional documentation/tool files. Never label the base commit alone
as the tested optimized source. Source patches and manifests stay immutable;
use a new candidate identifier if source bytes change.

The primary checkout's existing staged work is preserved. Nothing in this
candidate has been committed or pushed by this task. Reports will retain exact
source identities, failures/replays, raw evidence receipts and coverage gaps.

The completed records are [layout provenance](source-layout.json),
[Windows counts and higher-ISA failures](windows-summary.json),
[root-only consumer checks](consumer-check.json),
[post-timing exporter controls](candidate-tooling-followup.json),
[source/probe supplement receipt](supplement-receipt.json),
[final review receipt](review-receipt.json) and
[exporter evidence receipt](tooling-export-receipt.json).
The [checksum inventory](checksums.sha256) covers the other files here.
