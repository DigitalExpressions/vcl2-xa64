# Pristine upstream and explicit adaptation

The fork keeps a pristine pinned upstream copy and a separate adaptation layer.
Applications retain `#include "vectorclass.h"` and the same include directory,
public API, lane order and backend controls. No generation step is required to
use the checked-in runtime headers.

| Location | Maintenance role |
| --- | --- |
| `vendor/vectorclass/` | Original VCL2 bytes, including its own README/license; never edit for fork behavior |
| `vendor/vectorclass.json` | Original commit/tree and per-file Git blob/SHA256 inventory |
| `adaptation/patches/` | Ordered, explicit changes to original runtime headers |
| `adaptation/include/` | Shared fork configuration and extracted helpers |
| `backends/arm64/` | Canonical ARM/NEON implementation sources |
| `backends/x86/` | Canonical native x86 compiler-specific primitives |
| `vendor/sse2neon/` | Unchanged compatibility dependency already carried by this fork |
| Root `.h` files and registered runtime `.cpp` examples | Shipped generated products; keep existing include/build usage |
| `tests/` | Independent shared contracts, fixtures and backend regressions |

The current original VCL pin is `667577495efaa36a10cbb5d6cb8c942c23913848`
(VCL 2.02.04). All 28 original files are preserved byte-for-byte. The generator
verifies SHA256, original Git blob IDs, path inventory and the reconstructed
Git tree `36aee120c28c9c64e62903bfc81139003e37cedd`, without requiring network or
Git. Metadata and fork instructions live outside the pristine directory.
Original upstream whitespace is preserved as source evidence; its vendored
files are exempt from fork whitespace cleanup. This does not exempt adapted
code from correctness or numerical checks.

The first adaptation patch makes the existing historical port/correctness
differences explicit. The second isolates this pass's MSVC x86 call sites and
inlining choice. The shared exponential polynomial/coefficients and numerical
policies are unchanged. Correctness fixes remain necessary; the adapted build
does not pretend to execute entirely unmodified upstream bytes. The pristine
copy is the source baseline, and the adaptation layer describes every difference.

## Verify and regenerate

From the root, using Python 3.9+ (`python3` on macOS):

```sh
python -B tools/regenerate_headers.py --check
python -B tools/check_header_generation.py
python -B tools/check_repository.py
```

The first command is read-only and reports stale/hand-edited products. The
second exercises vendor-mutation, extra-path, no-fuzz patch, path-escape,
stale-output, changed-backend and line-ending controls under ignored `build/`.
The normal repository check also verifies pristine upstream and generated
products, so native runners detect stale adapted headers before execution.

After editing a canonical backend source or adaptation patch:

```sh
python -B tools/regenerate_headers.py --write
python -B tools/regenerate_headers.py --check --strict-bytes
```

`--write` replaces only the 35 registered root products after all inputs have
been checked. Review the canonical edits and generated diff together. Default
checking permits only an explicitly checked CRLF/LF difference in products;
`--strict-bytes` also requires exact declared product bytes. Pristine upstream
verification always requires exact bytes. Optional `--json build/UNIQUE.json`
saves a new checksum/provenance record without overwriting an earlier one.

Do not edit a root generated header and leave its canonical adaptation stale.
Do not fix an integrity failure by changing numerical gates, vendor hashes or
the upstream pin to match unexplained edits. Changes to canonical runtime code
still require native correctness and relevant performance checks.

## Updating upstream

Fetch/read upstream without altering branches, then deliberately choose a new
full commit. Export all original files and rebuild its pin/blob/SHA inventory,
preserving notices. Rebase the explicit patches against that snapshot without
fuzz, resolve changes in the adaptation layer, and regenerate. Never silently
replace the current baseline from a moving branch name. Review API/backend
boundaries and run native Windows/MSVC/Clang plus native M2 compatibility and
Direct NEON checks at the new frozen source. Retain prior reports/archives and
record new source identifiers. Publication requires separate authorization.

The [execution plan](x86-backend-optimization-plan.md) and
[M2 handoff](validation/mac-arm-x86-optimization-handoff.md) record the current
candidate and pending native ARM work. Canonical separation reproduces all 35
runtime products exactly from the optimized frozen bytes; it is not a claim
that a later whole repository revision has acquired earlier test results.
