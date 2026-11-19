# Architecture-separation failure triage — portable results

See the [dated follow-up](../windows-amd-2026-11-19-arch-separation-triage.md).
The original matrix remains failed. These records classify all 325 earlier
nonpassing instances and document targeted direct-original reproductions.

- `summary.json`, `categories.json`, `classification.json`: complete original status/name accounting and qualified causal categories.
- `unavailable-targets.json`: all 76 direct-original compile reproductions with diagnostics and matching-flags attempt IDs.
- `paired-runtime.json`: twenty paired consumers, actual failure statuses, outputs, sanitizer diagnostics and native executable hashes.
- `timeout-and-debugger.json`: the original timeout's waiting UI stack plus RTC and lookup-fault debugger evidence; debugger success is not a native pass.
- `upstream-only-probes.json`: minimal valid-input branch and AVX-512 reproductions.
- `direct-consumer-changes.json`, `representative-builds.json`, `command-journal.json`: exact include-only changes, compile/link flags and attempt provenance.
- `source-document.json`: version/URL/hash of the separately consulted author manual.
- `integrity.json`, `testbench.json`, `validator-hashes.json`, `compiled-artifact-hashes.json`: retained source, dependency, validator and executable identities.
- `attempts.json`, `archive-receipt.json`: original orchestration failures and checked compact evidence coverage.

The raw packet precedes final report preparation; this bundle/report have their
own checksums. No prior result, source checkpoint, gate or archive was replaced.

`final-review.json` records final source/portable/link checks and the separate
metadata-review supplement. The original triage packet remains unchanged.
