# MBTiles Data Sources and Download

## What we need for the proposed pipeline

1. Base vector data for roads/buildings/POI:
- `russia-latest.osm.pbf` (Geofabrik extract)

2. Optional reference/admin datasets for boundary overlays:
- `ne_10m_admin_0_countries.zip` (Natural Earth)
- `ne_10m_admin_1_states_provinces.zip` (Natural Earth)

## Direct sources

- Geofabrik Russia extract:
  - https://download.geofabrik.de/russia-latest.osm.pbf
- Natural Earth admin 0:
  - https://naciscdn.org/naturalearth/10m/cultural/ne_10m_admin_0_countries.zip
- Natural Earth admin 1:
  - https://naciscdn.org/naturalearth/10m/cultural/ne_10m_admin_1_states_provinces.zip

## One-command download (Windows PowerShell)

From repo root:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\download_map_data.ps1
```

Files will be downloaded to:

`tileserver-gl\data-src`

## Useful options

```powershell
# custom output directory
powershell -ExecutionPolicy Bypass -File .\tools\download_map_data.ps1 -OutputDir "D:\saturn-map-data"

# skip large Russia .pbf file
powershell -ExecutionPolicy Bypass -File .\tools\download_map_data.ps1 -SkipRussiaPbf

# skip Natural Earth archives
powershell -ExecutionPolicy Bypass -File .\tools\download_map_data.ps1 -SkipNaturalEarth
```

## Notes

- `russia-latest.osm.pbf` is large (several GB), so download time depends on your channel.
- If BITS is restricted by policy, the script falls back to `Invoke-WebRequest`.
- This step only downloads source datasets; MBTiles generation is the next stage.
