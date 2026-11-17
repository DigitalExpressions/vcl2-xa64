# Windows SIMD correctness completion plan

Started 2026-11-17 at `e27134b`, preserving existing staged/unstaged changes,
the prior AVX2 conversion fix and every historical report/evidence archive.
Agner Fog upstream is `667577495efaa36a10cbb5d6cb8c942c23913848`.
The published Mac report at `3abf7af` validates the earlier `a099098` source;
it does not cover new fixes. Pristine `vendor/vectorclass/` must stay untouched.

## Execution and restart checklist

- [x] Inspect current checkout, applicable instructions and original failures;
  preserve the existing index and working patch under ignored build output.
- [x] Reproduce and repair bounded table lookup/gather failures through a new
  canonical adaptation patch. Preserve protected pages and add focused boundary
  regressions; never pad test tables to hide overreads.
- [x] Repair compact-mask integer division and audit mask construction at the
  failing call sites. Use independent lane expectations.
- [x] Resolve remaining composition, classification, signed-zero and numerical
  failures. Distinguish a library defect from an ISA-specific API assumption;
  retain numerical thresholds, fixtures/seeds, FP flags and original failures.
- [x] Cover the identified `INSTRSET=9` FP64 conversion fallback as well as the
  previously fixed AVX2 conversion, with strict UBSan regression evidence.
- [x] Freeze the final candidate in a fresh detached validation worktree;
  capture base commit/tree, exact patches, added sources and initial/final hashes.
- [x] Run MSVC first, then native Clang: full four-profile baseline, AVX2 and
  AVX-512 local Release/Debug profiles (strict UBSan for Clang Debug), native
  CPU/OS probes, PE/runtime checks, actual-page guards and sanitizer controls.
  Preserve 60-second deadlines and concurrency three. Retain every attempt.
- [x] Verify vendor/generation integrity, portable result checksums and complete
  evidence coverage. Update dated results, validation index and integration plan.
- [x] Prepare a concrete M2 handoff for the new frozen source, with mandatory
  pristine-vendor checks and exact source/checkout-byte reconciliation.
- [x] Obtain explicit cleanup/commit/push approval for the prepared shared
  publication (2026-11-17); retain original evidence and validation worktrees.
- [x] Run compatibility 512 and Direct Neon 128/256/512, each Release/strict
  UBSan, on physical M2 at approved `ab5bb87`. See the
  [dated Mac result](validation/mac-arm-2026-11-17-ab5bb87-correctness.md).
  All eight profiles passed 8,036/8,036 cases and eight examples on the first
  Mac attempt, with no failures, skips, timeouts or retries.
- [ ] After both hosts validate the same final source, run the documented native
  Windows comparison against upstream at exact source IDs. Retain slower rows,
  variability, matched flags, edge correctness and source/assembly provenance.
  Do not collect timings during this correctness phase.

Raw root: `build/windows-correctness-20261117/`. Original triage remains under
`build/windows-isa-triage-20261117/`. The investigation preserved the original
index. The maintainer subsequently authorized cleanup, commit and push of the
prepared fixes, portable evidence and handoff on 2026-11-17. Stage only those
related files. Preserve raw evidence/worktrees; no PR, merge or branch deletion
is part of this publication.

Current status: all eight higher-ISA profiles pass 320/320 at frozen fingerprint
`af489367c33d2bddb12ab16b53639f4744ad19434ad8dfd3b4ca5502914f1528`,
base `e27134b` plus captured changes. All four full baseline profiles pass
5,392/5,392; twenty focused boundary/conversion programs pass, including level 9.
The selected final CTest total is 5,712/5,712, with no failures, skips, timeouts
or suite UBSan diagnostics. All 28 vendor files remain pristine and all 35
products reproduce exactly. See the
[dated report](validation/windows-amd-2026-11-17-correctness-completion.md) and
[portable metadata](validation/windows-correctness-2026-11-17/README.md).
The [new M2 handoff](validation/mac-arm-correctness-handoff.md) is prepared;
native execution uses this approved publication checkpoint.

Restart here: retain the original Windows frozen source and every attempt,
plus the checksummed Mac execution evidence. The maintainer subsequently
authorized publication and cleanup of the temporary Mac worktrees; the tested
commit and complete nonbinary evidence remain retained. The new Mac source comparison
reconciles all 157 listed code paths, preserving original Windows fingerprints.
Native M2 is complete at `ab5bb87`; renewed Windows performance comparison
at the final source remains, under separate authorization.
The Mac validation phase ran no benchmark and performed no publication or
cleanup. The maintainer subsequently authorized publishing its reports and
removing temporary worktrees/rebuildable output after verifying evidence
coverage. Use the [Windows final-source performance prompt](validation/windows-final-source-performance-prompt.md)
for the next separately requested native Windows step.
