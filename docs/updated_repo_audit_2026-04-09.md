# Updated Repo Audit (2026-04-09)

Source audited: `C:\Qt\repos\saturn_new_map_engine`  
Snapshot source: `git status --short`

## 1) Production-ready changes (safe to keep)
- `dataaccess/dataaccess.*`
- `dataaccess/geojsonservice.*`
- `dataaccess/geometryrepository.*`
- `dataaccess/legacycalculationservice.*`
- `dataaccess/mapobjectsrepository.*`

Reason: these files implement explicit DB-path methods and align with worker-safe snapshot flow.

## 2) Candidate changes (integrate feature-by-feature)
- `mainform/AppShell.qml`
- `mainform/components/BottomStatusBar.qml`
- `mainform/components/ContextMenuMap.qml`
- `mainform/components/ContextMenuObject.qml`
- `mainform/components/MapCanvas.qml`
- `mainform/components/RightSidebar.qml`
- `mainform/components/Toolbar.qml`
- `mainform/SATURN.qml`
- `mainform/CMakeLists.txt`
- `mapview/MyMapView.qml`
- `mapview/CMakeLists.txt`
- `mainform/entity_data_browser.*`
- `mapview/styles/europe-admin-dark.json`

Reason: potentially useful UX/functionality, but requires selective merge and runtime validation.

## 3) Experimental/noise (move out of runtime path)
- `.qtcreator/`
- `build-codex/`
- `mapview/data/` (heavy demo geojson payload)
- `mapview/demo_admin_boundaries_loader.*` (demo-only)
- `mapview/eventsmapservice.cpp` (stub-like)
- `mapview/eventsrepository.cpp` (stub-like)
- `mapview/mapsourcecontroller.cpp` (stub-like)
- `mapview/geojson_manager.*` (partial/legacy helper)
- `mapview/geojsonbuilder.*` (duplicate/partial helper)

Reason: either build/tooling artifacts, demo payloads, or low-value stub modules.

## 4) Actions completed in main repo
- Worker snapshot + coalescing integrated in `dataaccess`.
- Duplicate snapshot/runtime refresh path removed.
- Two-phase non-blocking startup integrated in `mainform/AppShell.qml`.
- Map viewport/scale metrics + improved map interactions integrated in `mapview/MyMapView.qml`.
- Bottom scale line switched to real denominator fallback in `BottomStatusBar.qml`.
- Added sanitization policy and tools:
  - `docs/repo_sanitization_policy.md`
  - `tools/repo_quality_gate.ps1`
  - `tools/classify_repo_noise.ps1`
  - CMake target `quality-check`

## 5) Follow-up checklist
1. Move or ignore experimental/noise files in source repo before future syncs.
2. Port candidate files one-by-one with build/run verification after each patch.
3. Keep temporary code marked via `TEMPORARY(...)` tag and owner comment.
