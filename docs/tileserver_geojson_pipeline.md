# TileServer-GL GeoJSON Pipeline

## Goal
- Serve base map tiles/styles from `tileserver-gl`.
- Publish dynamic object geometry (`Point/LineString/Polygon`) as GeoJSON without stopping server distribution.

## Options
1. **Direct static GeoJSON files (implemented)**
- App writes GeoJSON atomically into `tileserver-gl/styles/<style>/dynamic/`.
- Style references `markers.geojson` as a `geojson` source.
- Pros: simplest, no custom backend, zero restart for updates.
- Cons: dynamic layer update cadence depends on client/source reload behavior.

2. **Sidecar API for dynamic layers**
- Separate HTTP service returns GeoJSON/tiles from DB.
- TileServer keeps only basemap style/tiles.
- Pros: clean separation, better cache/version control.
- Cons: extra service and deployment complexity.

3. **Server-side vector-tile generation**
- Convert DB geometry stream into vector tiles (MVT) and serve as tile source.
- Pros: best scalability and rendering performance.
- Cons: highest implementation complexity.

## Minimal Plan (current)
1. Keep `maplibre` style URL on `tileserver-gl`.
2. Publish runtime GeoJSON snapshots to tileserver style folder via atomic writes.
3. Add GeoJSON source/layers to style (`saturn-points/lines/polygons`).
4. Keep current QML overlay rendering as fallback while validating server layer behavior.
5. After real MBTiles arrive: switch style/theme as needed, keep dynamic pipeline unchanged.

## Paths
- Default publish dir (app): `<repo>/tileserver-gl/styles/maptiler-basic/dynamic`
- Style patched by script: `<repo>/tileserver-gl/styles/maptiler-basic/style.json`

## Setup script
- `tools/setup_tileserver_geojson_overlay.ps1`

## Startup
- Preferred launcher: `tools/start_tileserver.ps1`
- First-time setup on a new machine (inside repo):
  - `npm install --prefix .\tileserver-gl`
- Foreground run:
  - `powershell -ExecutionPolicy Bypass -File tools/start_tileserver.ps1`
- Background run with auto-stop conflicting tileserver:
  - `powershell -ExecutionPolicy Bypass -File tools/start_tileserver.ps1 -Background -ForceStopOnPortConflict`
- Optional params:
  - `-TileServerRoot "<repo>\tileserver-gl"`
  - `-ConfigPath "<repo>\tileserver-gl\config.json"`
  - `-Port 8080`
  - `-StyleName "maptiler-basic"`
  - `-SkipOverlaySetup`
