# Canonical third-party sources

`vectorclass/` is an untouched snapshot of original VCL2 at commit
`667577495efaa36a10cbb5d6cb8c942c23913848`, tree
`36aee120c28c9c64e62903bfc81139003e37cedd`. All 28 files, their original bytes,
copyright notices and Apache license remain intact. [vectorclass.json](vectorclass.json)
records each original Git blob and SHA256. The generator independently rebuilds
the Git tree hash and rejects changed, missing or extra files. Directory-level
Git attributes prevent line-ending conversion of these canonical bytes.

Do not edit files inside `vectorclass/` to make this fork work. Maintain
adaptations separately; see the [layout guide](../docs/backend-layout.md).
An upstream update is an explicit reviewed pin/snapshot/patch-rebase operation,
not a fetch that silently changes the vendored source.

`sse2neon/sse2neon.h` is the unchanged MIT-licensed header already retained by
this fork at `44f974a`; [its record](sse2neon.json) identifies the bytes. This
reorganization does not update that dependency or claim a new external pin.
The root copy is a shipped generated product for existing include paths.
