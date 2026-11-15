# Port integration review

Target: integrate `neon-port` into the existing default `master` after review
and approval. PR #1 was merged on explicit maintainer approval at `732f3e9`;
both existing branches are retained as requested. The
[integration plan](integration-plan.md) records completion and where to resume.
The maintainer selected **manual native testing on Mac M2 and AMD Windows only**;
hosted CI is removed. Release publication and branch retirement require
separate approval; the default branch remains named `master`.

## Scope and method

The complete port is compared with original VCL 2.02.04 at
`667577495efaa36a10cbb5d6cb8c942c23913848`. Before preparation,
`master..08c3fe8` changes 107 files, including vendored sse2neon and generated
fixtures. Raw diffs, review inventories and controls stay under ignored
`build/master-integration-20261115/`.

| Area | Inspection and evidence |
| --- | --- |
| Backend/header selection | `vcl_config.h`, `vectorclass.h`, `instrset.h` and `instrset_neon.h`; ARM compatibility stays automatic, Direct Neon requires opt-in, and invalid architecture/backend/width combinations fail compilation |
| Shared helpers | The original helper block and `instrset_helpers.h` have identical extracted bodies; attribution, namespace boundaries and include guards retained |
| Existing x86 changes | Integer overflow/shifts, bounded lookups/gathers, conversion/rounding, finite-math classification and MSVC AVX-512 mask workarounds reviewed against regression evidence |
| Direct Neon | Integer/FP storage, masks, bit-preserving casts, division parameters, signed minima, zero-divisor rejection, permutations, gather/scatter and partial-memory boundaries inspected |
| Wide vectors/API/ABI | Typed half-register composition, namespace isolation, mask conversions, width guards and cross-translation-unit contracts reviewed |
| Runtime/dispatch | ARM exposes Neon separately and reports zero x86 capability; x86 detector/dispatch paths retained, including the CPUID correction below |
| Numerical functions | Explicit backend guards, bounded trig reduction, initialized select operands, conversion sentinels and fused helpers inspected; accuracy gates and expected fixture values preserved |
| Oracles/fixtures | Independent generators and exact-match scalar adapters reviewed; pinned source hashes and fixture regeneration retained; no vector result becomes its own oracle |
| CMake/consumers | Header-only default retained; runtime/tests/examples/benchmarks opt-in; test CPU/sanitizer flags do not leak through `vcl::vcl`; GfxMath remains optional |
| Attribution | Agner Fog notices, root license/changelog and sse2neon MIT notice retained; optional GPL testbench/adapters remain documented separately from runtime headers |
| Developer assets | Manual Windows/Mac runners, documentation navigation, evidence capture and the `neon-port` to `master` transition reviewed |

Inspection and selected tests are not exhaustive API or performance proof.
Generated fixture rows are supported by independent generators and retained
regeneration evidence, not a claim of manually proving every row. No benchmark
timings were collected.

## Findings and disposition

### Fixed: GNU/Clang i386 CPUID regression

The port narrowed GNU inline assembly to `__x86_64__`, making an i386 GNU/Clang
target enter Microsoft-style assembly. Its GNU intrinsic include was similarly
restricted. The extracted upstream helper compiled in an i386 syntax probe;
the port helper failed at `__asm {`. Restoring GNU/Clang selection inside the
existing x86 guard and including i386 intrinsics fixes the path. Corrected
i386 and x86-64 probes pass. This is compilation-only evidence, not a complete
32-bit runtime matrix. Native AMD validation also exercises the prepared source.

### Added: repeatable manual Windows controls and evidence

The Windows runner covers MSVC Release/Debug and GNU-driver Clang Release/
strict UBSan Debug. It rejects emulated Windows, verifies x64 PE files, probes
runtime ISA requirements, requires four independent protected-page faults,
and proves nonrecovering UBSan with intentional overflow. It preserves all
fixtures, flags, numerical gates and 60-second deadlines. Evidence and failed
attempts remain under `build/` with checksummed transfer packets.

The four first local profiles passed 160/160 cases. After the CPUID fix, a
snapshot passed 2,776/2,776 cases: pinned MSVC Release and Clang UBSan Debug
at 1,348 each, plus local MSVC Debug and Clang Release at 40 each. Later
controller-only ISA gating was verified with a valid level-6 requirement and
an impossible level-99 negative control. Each snapshot retains its identifiers.

The Visual Studio bootstrap initially emitted a `vswhere` PATH warning while
still initializing correctly. Its installer directory is now on the process
PATH; no machine-wide environment setting changed.

### Hosted prototype removed after the maintainer's workflow decision

The initial plan proposed hosted Windows jobs. Inspection of existing hosted
ARM runs found configuration-test timeouts, and one old compatibility Debug
job remained stuck in CTest for over 40 minutes. Logs/state were retained and
that obsolete run was cancelled. A two-worker configuration prototype preserved
the deadline and contracts but exposed a shared MSVC Debug PDB collision:
one configuration case failed (39/40), while three other profiles passed.
Private probe directories fixed that collision; a fresh four-profile snapshot
passed 160/160. Both attempts remain in the evidence.

The maintainer then chose manual testing on the two native machines. The hosted
workflow and unpublished CI additions are removed, and `tests/configuration.py`
is restored to its original serial implementation. The parallel prototype is
not part of the proposed integration. No hosted pass is required or claimed
under the revised policy, and future pushes/PRs do not start test workflows.

### Developer documentation cleanup

The README and backend page previously described physical x86 as unvalidated
despite native AMD evidence. Current pages now identify the validation index
and distinguish exact tested revisions, native AMD/M2, emulation and historical
hosted results. The Apple ASan hang is labeled precisely. The developer index
links current commands, contracts and runner guidance. Preparation used
`neon-port`; the approved integration makes `master` the common base. Dated reports
and benchmark data remain unchanged.

## Evidence for the PR and remaining limits

- [Native Mac report](validation/mac-arm-2026-11-15.md): 8,036 selected cases at
  `465436d`, passed after a sleep-related full-profile replay.
- [Historical AMD review](validation/windows-amd-2026-11-15-823ca2a.md): final
  5,627 cases, exact source fingerprints and retained earlier failures/reruns.
- [Validation index](validation/README.md): actual source comparisons and new
  preparation results; no relabeling of older runs as a later tested HEAD.
- [Manual Windows runner](validation/windows-runner.md) and
  [native workflow](validation/native-workflow.md): commands and evidence protocol.
- [New manual preparation results](validation/windows-amd-2026-11-15-08c3fe8-manual.md):
  final 160/160 local cases, separately identified intermediate 2,776/2,776
  pinned cases, discarded prototype failure, startup failures and exact snapshots.

The only library change introduced during this review is the x86-guarded CPUID
selection correction. Original serial configuration checks are retained.
Historical source comparisons alone do not establish a pass at a later HEAD;
new manual results record their own snapshots. The ARM library paths are
unchanged by the CPUID correction. Full higher-ISA matrices, FP16, other ARM
toolchains/hosts and ASan remain outside the recorded scope; physical Intel
is omitted and Direct Neon remains experimental.

Final manual native/documentation preparation checks are recorded and
[PR #1](https://github.com/DigitalExpressions/vcl2-xa64/pull/1) is merged at
`732f3e9`. The merge tree equals reviewed head `abf7a82`; no native matrix was
rerun for the merge or completion documentation. The Windows checkout is on
`master`; the Mac checkout can now follow the native workflow to update.
Keep both existing branches. Use separate topic branches for future work;
reviewed releases require separate publication approval.
