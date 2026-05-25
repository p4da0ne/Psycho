param(
    [string]$OutputDir = "",
    [switch]$SkipRussiaPbf,
    [switch]$SkipNaturalEarth,
    [switch]$IncludeEuropeanNeighbors,
    [switch]$IncludeEuropeExtract,
    [switch]$ForceRecheck
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

function Download-File([string]$url, [string]$destination) {
    Write-Host "Downloading: $url"
    Write-Host "To:         $destination"

    $destDir = Split-Path -Parent $destination
    Ensure-Directory $destDir

    try {
        Start-BitsTransfer -Source $url -Destination $destination -DisplayName "saturn-map-data" -ErrorAction Stop
    } catch {
        Write-Warning "BITS failed, fallback to Invoke-WebRequest: $($_.Exception.Message)"
        Invoke-WebRequest -Uri $url -OutFile $destination -UseBasicParsing -TimeoutSec 1800
    }

    if (-not (Test-Path -LiteralPath $destination)) {
        throw "Download failed: $destination"
    }
    $sizeMb = [math]::Round((Get-Item -LiteralPath $destination).Length / 1MB, 2)
    Write-Host "Saved ($sizeMb MB)`n"
}

function Download-FileWithResume([string]$url, [string]$destination) {
    Write-Host "Downloading (resume): $url"
    Write-Host "To:                   $destination"

    $destDir = Split-Path -Parent $destination
    Ensure-Directory $destDir

    $curlCmd = Get-Command "curl.exe" -ErrorAction SilentlyContinue
    if ($curlCmd) {
        & $curlCmd.Source -L -C - --retry 10 --retry-delay 3 --output $destination $url
        if ($LASTEXITCODE -ne 0) {
            throw "curl download failed with code ${LASTEXITCODE}: $url"
        }
    } else {
        Download-File -url $url -destination $destination
    }

    if (-not (Test-Path -LiteralPath $destination)) {
        throw "Download failed: $destination"
    }
    $sizeMb = [math]::Round((Get-Item -LiteralPath $destination).Length / 1MB, 2)
    Write-Host "Saved ($sizeMb MB)`n"
}

function Get-RemoteMd5([string]$md5Url) {
    $content = (Invoke-WebRequest -Uri $md5Url -UseBasicParsing -TimeoutSec 180).Content
    $raw = ""
    if ($content -is [System.Array]) {
        $raw = [System.Text.Encoding]::UTF8.GetString($content)
    } else {
        $raw = [string]$content
    }
    if ([string]::IsNullOrWhiteSpace($raw)) {
        throw "Empty md5 response from $md5Url"
    }
    $parts = ($raw.Trim() -split '\s+')
    if ($parts.Count -lt 1) {
        throw "Unable to parse md5 from $md5Url"
    }
    return $parts[0].ToLower()
}

function Get-LocalMd5([string]$path) {
    return (Get-FileHash -Algorithm MD5 -LiteralPath $path).Hash.ToLower()
}

function Download-GeofabrikPbfChecked([string]$pbfUrl, [string]$destinationPath, [switch]$AlwaysCheck) {
    $md5Url = $pbfUrl + ".md5"
    $remoteMd5 = Get-RemoteMd5 -md5Url $md5Url
    $needDownload = $true

    if ((-not $AlwaysCheck) -and (Test-Path -LiteralPath $destinationPath)) {
        try {
            $localMd5 = Get-LocalMd5 -path $destinationPath
            if ($localMd5 -eq $remoteMd5) {
                Write-Host ((Split-Path $destinationPath -Leaf) + " is up-to-date (md5 ok): " + $localMd5)
                $needDownload = $false
            } else {
                Write-Warning "Existing PBF md5 mismatch. Will resume download."
                Write-Warning "Local=$localMd5 Remote=$remoteMd5"
            }
        } catch {
            Write-Warning "Unable to verify existing PBF md5; will re-download with resume: $($_.Exception.Message)"
        }
    }

    if ($needDownload) {
        Download-FileWithResume -url $pbfUrl -destination $destinationPath
        $localMd5 = Get-LocalMd5 -path $destinationPath
        if ($localMd5 -ne $remoteMd5) {
            throw "Downloaded PBF checksum mismatch. Local=$localMd5 Remote=$remoteMd5"
        }
        Write-Host ((Split-Path $destinationPath -Leaf) + " checksum verified: " + $localMd5)
    }
}

$repoRoot = Resolve-RepoRoot
if ([string]::IsNullOrWhiteSpace($OutputDir)) {
    $OutputDir = Join-Path $repoRoot "tileserver-gl\data-src"
}
$OutputDir = (Resolve-Path -Path (New-Item -ItemType Directory -Path $OutputDir -Force)).Path

Write-Host "Output directory: $OutputDir"

if (-not $SkipRussiaPbf) {
    Download-GeofabrikPbfChecked `
        -pbfUrl "https://download.geofabrik.de/russia-latest.osm.pbf" `
        -destinationPath (Join-Path $OutputDir "russia-latest.osm.pbf") `
        -AlwaysCheck:$ForceRecheck
}

if ($IncludeEuropeanNeighbors) {
    $neighborNames = @(
        "norway",
        "finland",
        "estonia",
        "latvia",
        "lithuania",
        "poland",
        "belarus",
        "ukraine"
    )

    foreach ($name in $neighborNames) {
        $url = "https://download.geofabrik.de/europe/$name-latest.osm.pbf"
        $dst = Join-Path $OutputDir "$name-latest.osm.pbf"
        Download-GeofabrikPbfChecked -pbfUrl $url -destinationPath $dst -AlwaysCheck:$ForceRecheck
    }
}

if ($IncludeEuropeExtract) {
    Download-GeofabrikPbfChecked `
        -pbfUrl "https://download.geofabrik.de/europe-latest.osm.pbf" `
        -destinationPath (Join-Path $OutputDir "europe-latest.osm.pbf") `
        -AlwaysCheck:$ForceRecheck
}

if (-not $SkipNaturalEarth) {
    Download-File `
        "https://naciscdn.org/naturalearth/10m/cultural/ne_10m_admin_0_countries.zip" `
        (Join-Path $OutputDir "ne_10m_admin_0_countries.zip")

    Download-File `
        "https://naciscdn.org/naturalearth/10m/cultural/ne_10m_admin_1_states_provinces.zip" `
        (Join-Path $OutputDir "ne_10m_admin_1_states_provinces.zip")
}

Write-Host "Done."
