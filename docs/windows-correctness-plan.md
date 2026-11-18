# Retired Windows correctness plan

Retired on 18 November 2026. The maintainer requested a new ARM/Neon strategy,
not continuation of the x86-specific fixes. Start with
[ARM64 / Neon lessons](neon-arm-lessons.md) and the
[current plan](integration-plan.md).

The [dated Windows report](validation/windows-amd-2026-11-17-correctness-completion.md)
and [native M2 result](validation/mac-arm-2026-11-17-ab5bb87-correctness.md)
retain their original source IDs and coverage. This cleanup does not change,
revert or extend their source implementations.

The former plan and unpublished follow-up are preserved in the local
pre-cleanup documentation archive; see the
[cleanup record](validation/arm-strategy-cleanup-2026-11-18.md). No new test,
benchmark, commit or push is authorized by this retired plan.
