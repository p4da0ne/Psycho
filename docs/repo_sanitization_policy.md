# Repo Sanitization Policy

## Goals
- Keep runtime code clean and predictable.
- Integrate features incrementally, without bulk AI-generated dumps.
- Prevent accidental inclusion of demo payloads and stubs into production path.

## Runtime Boundaries
- Production runtime path:
  - `dataaccess/`
  - `mainform/`
  - `mapview/`
- Experimental path:
  - `experimental/` (if needed)
  - never referenced by default CMake targets.

## Integration Rules
1. Every imported feature must be isolated into a small patch.
2. No unreviewed demo data in runtime resources.
3. Empty/stub `.cpp` modules are not allowed in production path.
4. Temporary code must be explicitly marked with `TEMPORARY(...)` comment and an owner.
5. Runtime behavior must pass `build/Debug` compilation after each increment.

## Quality Gate
- Script: `tools/repo_quality_gate.ps1`
- Fails when:
  - banned noisy paths are tracked,
  - stub-like `.cpp` files are present in tracked code.

## Recommended Flow For External Repo Sync
1. Run `tools/classify_repo_noise.ps1` on source repository.
2. Move or ignore `Experimental/Noise` group.
3. Review `Candidate` group feature-by-feature.
4. Port only validated features into main repository.
5. Run build and quality gate after each feature patch.
