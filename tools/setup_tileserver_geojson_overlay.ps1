param(
    [string]$TileServerRoot = "",
    [string]$StyleName = "maptiler-basic",
    [int]$Port = 8080
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($TileServerRoot)) {
    $repoRoot = Split-Path -Parent $PSScriptRoot
    $TileServerRoot = Join-Path $repoRoot "tileserver-gl"
}

function Write-Utf8NoBom {
    param(
        [string]$Path,
        [string]$Text
    )
    $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($Path, $Text, $utf8NoBom)
}

$styleDir = Join-Path $TileServerRoot ("styles\" + $StyleName)
$stylePath = Join-Path $styleDir "style.json"
if (-not (Test-Path -LiteralPath $stylePath)) {
    throw "Style file not found: $stylePath"
}

$dynamicDir = Join-Path $styleDir "dynamic"
if (-not (Test-Path -LiteralPath $dynamicDir)) {
    New-Item -ItemType Directory -Path $dynamicDir | Out-Null
}

$emptyGeoJson = '{"type":"FeatureCollection","features":[]}'
foreach ($name in @("points.geojson", "lines.geojson", "polygons.geojson", "markers.geojson")) {
    $filePath = Join-Path $dynamicDir $name
    if (-not (Test-Path -LiteralPath $filePath)) {
        Write-Utf8NoBom -Path $filePath -Text $emptyGeoJson
    }
}

$manifestPath = Join-Path $dynamicDir "manifest.json"
if (-not (Test-Path -LiteralPath $manifestPath)) {
    $manifest = @{
        publishedAt = [DateTime]::UtcNow.ToString("o")
        points = "points.geojson"
        lines = "lines.geojson"
        polygons = "polygons.geojson"
        markers = "markers.geojson"
        publishCount = 0
    } | ConvertTo-Json -Depth 5 -Compress
    Write-Utf8NoBom -Path $manifestPath -Text $manifest
}

$style = Get-Content -LiteralPath $stylePath -Raw | ConvertFrom-Json
if (-not $style.sources) {
    $style | Add-Member -MemberType NoteProperty -Name sources -Value ([PSCustomObject]@{})
}
if (-not $style.layers) {
    $style | Add-Member -MemberType NoteProperty -Name layers -Value @()
}

$sourceName = "saturn-markers-source"
$markersUrl = "/styles/$StyleName/dynamic/markers.geojson"

$newSources = [ordered]@{}
foreach ($prop in $style.sources.PSObject.Properties) {
    if ($prop.Name -ne $sourceName) {
        $newSources[$prop.Name] = $prop.Value
    }
}
$newSources[$sourceName] = [ordered]@{
    data = $markersUrl
    type = "geojson"
}
$style.sources = [PSCustomObject]$newSources

function Ensure-Layer {
    param(
        [object[]]$Layers,
        [hashtable]$LayerDef
    )
    foreach ($layer in $Layers) {
        if ($layer.id -eq $LayerDef.id) {
            return $Layers
        }
    }
    $result = @()
    $result += $Layers
    $result += ([PSCustomObject]$LayerDef)
    return $result
}

$style.layers = Ensure-Layer -Layers $style.layers -LayerDef @{
    id = "saturn-polygons"
    type = "fill"
    source = $sourceName
    filter = @("==", '$type', "Polygon")
    paint = @{
        "fill-color" = "#4c78a8"
        "fill-opacity" = 0.24
    }
}

$style.layers = Ensure-Layer -Layers $style.layers -LayerDef @{
    id = "saturn-lines"
    type = "line"
    source = $sourceName
    filter = @("==", '$type', "LineString")
    paint = @{
        "line-color" = "#f58518"
        "line-width" = 2.0
        "line-opacity" = 0.9
    }
}

$style.layers = Ensure-Layer -Layers $style.layers -LayerDef @{
    id = "saturn-points"
    type = "circle"
    source = $sourceName
    filter = @("==", '$type', "Point")
    paint = @{
        "circle-color" = "#e45756"
        "circle-radius" = 4.5
        "circle-stroke-color" = "#ffffff"
        "circle-stroke-width" = 1.2
    }
}

$json = $style | ConvertTo-Json -Depth 100
Write-Utf8NoBom -Path $stylePath -Text $json

Write-Output "Tileserver style overlay configured:"
Write-Output " - Style: $stylePath"
Write-Output " - GeoJSON source URL: $markersUrl"
Write-Output " - Dynamic directory: $dynamicDir"

