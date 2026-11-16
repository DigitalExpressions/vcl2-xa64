# Mac M2 final native results

Run ID: `simd-parity-20261116`.
Tested source: `bd7807e91f5888bc2ad708ca46c9305d166d6c4b`.
Tested tree: `2a0a5d3250201773cccb06ba0562df728f4e9387`. Clean throughout execution.

**8,036/8,036 CTest cases and eight examples passed.** Separately,
**3,996/3,996 corpus vector cases** (24,672 lanes) passed independent references.
All attempts and the four Debug bundle-validation failures are retained.
See the [full Mac report](../../../mac-arm-2026-11-16-bd7807e-final.md).

## Portable corpus files

| Attempt | Reference cases passed | Capture workflow | Manifest / raw bits |
| --- | ---: | --- | --- |
| mac-compat-release-01 | 672 | Accepted | [manifest](mac-compat-release-01/manifest.json), [JSONL](mac-compat-release-01/outputs-apple-clang-neon-b2-w512-release.jsonl) |
| mac-compat-debug-01 | 672 | Rejected by frozen UBSan control check | [manifest](mac-compat-debug-01/manifest.json), [JSONL](mac-compat-debug-01/outputs-apple-clang-neon-b2-w512-debug-ubsan.jsonl) |
| mac-neon128-release-01 | 218 | Accepted | [manifest](mac-neon128-release-01/manifest.json), [JSONL](mac-neon128-release-01/outputs-apple-clang-neon-b3-w128-release.jsonl) |
| mac-neon128-debug-01 | 218 | Rejected by frozen UBSan control check | [manifest](mac-neon128-debug-01/manifest.json), [JSONL](mac-neon128-debug-01/outputs-apple-clang-neon-b3-w128-debug-ubsan.jsonl) |
| mac-neon256-release-01 | 436 | Accepted | [manifest](mac-neon256-release-01/manifest.json), [JSONL](mac-neon256-release-01/outputs-apple-clang-neon-b3-w256-release.jsonl) |
| mac-neon256-debug-01 | 436 | Rejected by frozen UBSan control check | [manifest](mac-neon256-debug-01/manifest.json), [JSONL](mac-neon256-debug-01/outputs-apple-clang-neon-b3-w256-debug-ubsan.jsonl) |
| mac-neon512-release-01 | 672 | Accepted | [manifest](mac-neon512-release-01/manifest.json), [JSONL](mac-neon512-release-01/outputs-apple-clang-neon-b3-w512-release.jsonl) |
| mac-neon512-debug-01 | 672 | Rejected by frozen UBSan control check | [manifest](mac-neon512-debug-01/manifest.json), [JSONL](mac-neon512-debug-01/outputs-apple-clang-neon-b3-w512-debug-ubsan.jsonl) |

All writable/runtime controls passed. All four guard faults were `SIGBUS` (`-10`)
at the actual 16,384-byte page size. UBSan positive controls exited zero, and
intentional overflow produced the expected diagnostic and stopped with
`SIGABRT` (`-6`), without continuation. The frozen native runner accepts this;
the shared bundle loader requires negative-control exit code `1` for Debug.
Debug invocations therefore exited `1` after writing their diagnostic bundles,
despite all captured cases passing independent references. Their original
manifests, raw bits and checksums are preserved unchanged.

## Windows comparison

All 32 matching-mode pairings were attempted. **24 Release pairings completed**:
72,614 exact lane matches; 802 numerically acceptable differences; 300 declared
FMA differences; 300 declared rounding differences; **zero unexpected differences**.
These span 74,016 repeated lane comparisons and do not claim bitwise identity.

All **eight Debug pairings were rejected before lane comparison** by the frozen
UBSan exit-code assumption. Their differences remain unevaluated. Complete input
inventories were required; explicit width scopes retain each backend's coverage.
[Pairing metadata](comparisons-01/matrix-summary.json) lists every planned pairing,
the original comparator results and all eight rejection records. These rejections
are explicit errors, rather than successful shared-comparator result documents.

[Host metadata](matrix-summary.json) separates suite results, reference counts,
capture workflow failures, controls, attempts and archive receipts. Each attempt
has its original checksum list; the root list covers all portable files except
itself. The frozen Mac worktree's whole-file byte fingerprint differs from the
Windows report and remains unreconciled without Windows raw source manifests;
exact frozen Git identifiers and corpus/policy/input hashes match.

No matrix/capture retries or source changes occurred. Setup comparator checks
were replayed after creating the absent ignored build directory, passing 9/9.
Full raw evidence remains under ignored `build/`; combined archive
`mac-final-evidence.zip`, SHA256
`6ded9321c727c8c847aecf60ddb0caddab942f117dcc8927f1167ae595c5508c`.
The [continuation](../../../mac-arm-2026-11-16-bd7807e-final.md#evidence-and-continuation)
requires reviewing the loader policy and a new approved shared checkpoint for
any harness correction. Full Debug parity at this frozen source is incomplete.
