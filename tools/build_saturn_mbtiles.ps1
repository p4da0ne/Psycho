param(
    [string]$SourceDir = "",
    [string]$WorkDir = "",
    [string]$OutputMbtiles = "",
    [string]$OsmInputPath = "",
    [string]$Bbox = "",
    [string]$PlanetilerJavaXmx = "4g",
    [int]$Threads = 12,
    [string]$OsmiumImage = "iboates/osmium:latest",
    [switch]$SkipMerge,
    [switch]$ForceRebuild
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

function Resolve-RepoRoot {
    $scriptDir = Split-Path -Parent $PSCommandPath
    return (Resolve-Path (Join-Path $scriptDir "..")).Path
}

function Ensure-Directory([string]$path) {
    if (-not (Test-Path -LiteralPath $path)) {
        New-Item -ItemType Directory -Path $path -Force | Out-Null
    }
}

function Resolve-Abs([string]$path) {
    return (Resolve-Path -LiteralPath $path).Path
}

function Require-File([string]$path) {
    if (-not (Test-Path -LiteralPath $path)) {
        throw "Required file not found: $path"
    }
}

function Download-FileWithResume([string]$url, [string]$destination) {
    $destDir = Split-Path -Parent $destination
    Ensure-Directory $destDir

    Write-Host "Downloading dependency:"
    Write-Host "  $url"
    Write-Host "  -> $destination"

    $curlCmd = Get-Command "curl.exe" -ErrorAction SilentlyContinue
    if ($curlCmd) {
        & $curlCmd.Source -L -C - --retry 10 --retry-delay 3 --output $destination $url
        if ($LASTEXITCODE -ne 0) {
            throw "curl download failed with code ${LASTEXITCODE}: $url"
        }
    } else {
        Invoke-WebRequest -Uri $url -OutFile $destination -UseBasicParsing -TimeoutSec 1800
    }

    Require-File $destination
}

$repoRoot = Resolve-RepoRoot
$tileServerRoot = Join-Path $repoRoot "tileserver-gl"
if ([string]::IsNullOrWhiteSpace($SourceDir)) {
    $SourceDir = Join-Path $tileServerRoot "data-src"
}
if ([string]::IsNullOrWhiteSpace($WorkDir)) {
    if (Test-Path -LiteralPath "E:\") {
        $WorkDir = "E:\saturn_tiles_work"
    } else {
        $WorkDir = Join-Path $tileServerRoot "data-work"
    }
}
if ([string]::IsNullOrWhiteSpace($OutputMbtiles)) {
    if (Test-Path -LiteralPath "E:\") {
        $OutputMbtiles = "E:\saturn_tiles_data\saturn-russia-eu.mbtiles"
    } else {
        $OutputMbtiles = Join-Path $tileServerRoot "data\saturn-russia-eu.mbtiles"
    }
}

Ensure-Directory $SourceDir
Ensure-Directory $WorkDir
Ensure-Directory (Split-Path -Parent $OutputMbtiles)

$sourceDirAbs = Resolve-Abs $SourceDir
$workDirAbs = Resolve-Abs $WorkDir
$outputAbs = [System.IO.Path]::GetFullPath($OutputMbtiles)

docker info 1>$null 2>$null
if ($LASTEXITCODE -ne 0) {
    throw "Docker daemon is not available."
}

$mergedPathHost = Join-Path $workDirAbs "russia-eu-neighbors.osm.pbf"
$mergedPathCtr = "/work/russia-eu-neighbors.osm.pbf"
$mergedRawPathHost = Join-Path $workDirAbs "russia-eu-neighbors-raw-merge.osm.pbf"
$mergedRawPathCtr = "/work/russia-eu-neighbors-raw-merge.osm.pbf"
$clippedPathHost = Join-Path $workDirAbs "russia-eu-neighbors-bbox.osm.pbf"
$clippedPathCtr = "/work/russia-eu-neighbors-bbox.osm.pbf"
$outputCtr = "/work/saturn-russia-eu.mbtiles"

$osmInputHost = ""
$osmInputCtr = ""
if (-not [string]::IsNullOrWhiteSpace($OsmInputPath)) {
    $osmInputHost = [System.IO.Path]::GetFullPath($OsmInputPath)
    Require-File $osmInputHost
    if ($osmInputHost.StartsWith($workDirAbs, [System.StringComparison]::OrdinalIgnoreCase)) {
        $relative = $osmInputHost.Substring($workDirAbs.Length).TrimStart('\')
        $relative = $relative -replace '\\','/'
        $osmInputCtr = "/work/$relative"
    } elseif ($osmInputHost.StartsWith($sourceDirAbs, [System.StringComparison]::OrdinalIgnoreCase)) {
        $relative = $osmInputHost.Substring($sourceDirAbs.Length).TrimStart('\')
        $relative = $relative -replace '\\','/'
        $osmInputCtr = "/src/$relative"
    } else {
        throw "OsmInputPath must be inside SourceDir or WorkDir."
    }
} else {
    $required = @(
        "russia-latest.osm.pbf",
        "norway-latest.osm.pbf",
        "finland-latest.osm.pbf",
        "estonia-latest.osm.pbf",
        "latvia-latest.osm.pbf",
        "lithuania-latest.osm.pbf",
        "poland-latest.osm.pbf",
        "belarus-latest.osm.pbf",
        "ukraine-latest.osm.pbf"
    )

    foreach ($name in $required) {
        Require-File (Join-Path $sourceDirAbs $name)
    }

    # ForceRebuild should recreate output artifacts, but with -SkipMerge we keep existing merged PBF.
    $shouldMerge = (-not $SkipMerge) -or (-not (Test-Path -LiteralPath $mergedPathHost))
    if ($shouldMerge) {
        $mergeInputCtr = @()
        foreach ($name in $required) {
            $mergeInputCtr += "/src/$name"
        }

        Write-Host "Merging OSM extracts into one PBF..."
        $mergeArgs = @(
            "run", "--rm",
            "-v", "${sourceDirAbs}:/src",
            "-v", "${workDirAbs}:/work",
            $OsmiumImage,
            "merge",
            "-O",
            "-o", $mergedRawPathCtr
        ) + $mergeInputCtr

        & docker @mergeArgs
        if ($LASTEXITCODE -ne 0) {
            throw "osmium merge failed."
        }

        # Geofabrik extracts can contain different object versions across borders.
        # Keep only the state at a single point in time to remove duplicate IDs.
        $snapshotUtc = [DateTime]::UtcNow.ToString("yyyy-MM-ddTHH:mm:ssZ")
        Write-Host "Normalizing merged snapshot with osmium time-filter at $snapshotUtc ..."
        $timeFilterArgs = @(
            "run", "--rm",
            "-v", "${workDirAbs}:/work",
            $OsmiumImage,
            "time-filter",
            $mergedRawPathCtr,
            $snapshotUtc,
            "-O",
            "-o", $mergedPathCtr
        )

        & docker @timeFilterArgs
        if ($LASTEXITCODE -ne 0) {
            throw "osmium time-filter failed."
        }
    }

    Require-File $mergedPathHost

    if (-not [string]::IsNullOrWhiteSpace($Bbox)) {
        $shouldExtract = $shouldMerge -or (-not (Test-Path -LiteralPath $clippedPathHost)) -or $ForceRebuild
        if ($shouldExtract) {
            Write-Host "Extracting merged PBF by bbox ($Bbox) ..."
            $extractArgs = @(
                "run", "--rm",
                "-v", "${workDirAbs}:/work",
                $OsmiumImage,
                "extract",
                "-b", $Bbox,
                "--strategy=complete_ways",
                "-O",
                "-o", $clippedPathCtr,
                $mergedPathCtr
            )

            & docker @extractArgs
            if ($LASTEXITCODE -ne 0) {
                throw "osmium extract failed for bbox $Bbox."
            }
        }

        Require-File $clippedPathHost
        $osmInputHost = $clippedPathHost
        $osmInputCtr = $clippedPathCtr
    } else {
        $osmInputHost = $mergedPathHost
        $osmInputCtr = $mergedPathCtr
    }
}

if (Test-Path -LiteralPath $outputAbs) {
    if ($ForceRebuild) {
        Remove-Item -LiteralPath $outputAbs -Force
    } else {
        Write-Host "Output already exists: $outputAbs"
        Write-Host "Use -ForceRebuild to recreate."
        exit 0
    }
}

$planetilerSourcesHost = Join-Path $workDirAbs "planetiler-sources"
Ensure-Directory $planetilerSourcesHost

$auxSources = @(
    @{
        Name = "lake_centerline.shp.zip";
        Url = "https://github.com/acalcutt/osm-lakelines/releases/download/v12/lake_centerline.shp.zip";
    },
    @{
        Name = "water-polygons-split-3857.zip";
        Url = "https://osmdata.openstreetmap.de/download/water-polygons-split-3857.zip";
    },
    @{
        Name = "natural_earth_vector.sqlite.zip";
        Url = "https://naciscdn.org/naturalearth/packages/natural_earth_vector.sqlite.zip";
    }
)

foreach ($source in $auxSources) {
    $localPath = Join-Path $planetilerSourcesHost $source.Name
    if ((-not (Test-Path -LiteralPath $localPath)) -or ((Get-Item -LiteralPath $localPath).Length -le 0)) {
        Download-FileWithResume -url $source.Url -destination $localPath
    }
    Require-File $localPath
}

$planetilerSourcesCtr = "/work/planetiler-sources"
$lakeCenterlinesCtr = "$planetilerSourcesCtr/lake_centerline.shp.zip"
$waterPolygonsCtr = "$planetilerSourcesCtr/water-polygons-split-3857.zip"
$naturalEarthCtr = "$planetilerSourcesCtr/natural_earth_vector.sqlite.zip"

$planetilerArgs = @(
    "run", "--rm",
    "-e", "JAVA_TOOL_OPTIONS=-Xmx$PlanetilerJavaXmx",
    "-v", "${sourceDirAbs}:/src",
    "-v", "${workDirAbs}:/work",
    "ghcr.io/onthegomap/planetiler:latest",
    "--download=false",
    "--download_osm_tile_weights=false",
    "--fetch-wikidata=false",
    "--lake_centerlines_path=$lakeCenterlinesCtr",
    "--water_polygons_path=$waterPolygonsCtr",
    "--natural_earth_path=$naturalEarthCtr",
    "--osm_path=$osmInputCtr",
    "--output=$outputCtr",
    "--tmpdir=/work/tmp",
    "--threads=$Threads",
    "--minzoom=0",
    "--maxzoom=14",
    "--force=true"
)

if (-not [string]::IsNullOrWhiteSpace($Bbox)) {
    $planetilerArgs += "--bounds=$Bbox"
}

Write-Host "Building MBTiles with planetiler..."
& docker @planetilerArgs
if ($LASTEXITCODE -ne 0) {
    throw "planetiler build failed."
}

$builtOutput = Join-Path $workDirAbs "saturn-russia-eu.mbtiles"
Require-File $builtOutput

if ($builtOutput -ne $outputAbs) {
    Copy-Item -LiteralPath $builtOutput -Destination $outputAbs -Force
}

$sizeGb = [math]::Round((Get-Item -LiteralPath $outputAbs).Length / 1GB, 3)
Write-Host "MBTiles ready: $outputAbs"
Write-Host "Size: ${sizeGb} GB"
