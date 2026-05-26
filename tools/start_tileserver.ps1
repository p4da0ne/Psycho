param(
    [string]$TileServerRoot = "",
    [string]$ConfigPath = "",
    [string]$StyleName = "maptiler-basic",
    [int]$Port = 8080,
    [string]$MbtilesSource = "E:\saturn_tiles_data",
    [switch]$Background,
    [switch]$ForceStopOnPortConflict,
    [switch]$SkipOverlaySetup
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($TileServerRoot)) {
    $repoRoot = Split-Path -Parent $PSScriptRoot
    $TileServerRoot = Join-Path $repoRoot "tileserver-gl"
}

if ([string]::IsNullOrWhiteSpace($ConfigPath)) {
    $ConfigPath = Join-Path $TileServerRoot "config.json"
}

if (-not (Test-Path -LiteralPath $ConfigPath)) {
    throw "Config not found: $ConfigPath"
}

# Junction обходит баг url.parse в @mapbox/mbtiles на пути с буквой диска: 'E:/foo'
# распарсится как URL с протоколом 'E:', и SQLite получит мусорный путь.
$mbtilesLink = Join-Path $TileServerRoot "data"
if (-not (Test-Path -LiteralPath $mbtilesLink)) {
    if (-not [string]::IsNullOrWhiteSpace($MbtilesSource) -and (Test-Path -LiteralPath $MbtilesSource)) {
        Write-Host "Creating junction: $mbtilesLink -> $MbtilesSource"
        & cmd /c mklink /J `"$mbtilesLink`" `"$MbtilesSource`" | Out-Null
        if ($LASTEXITCODE -ne 0) {
            throw "Failed to create junction $mbtilesLink -> $MbtilesSource (exit $LASTEXITCODE)"
        }
    } else {
        Write-Warning "Mbtiles source '$MbtilesSource' not found and '$mbtilesLink' does not exist. Tileserver will fail to load data."
    }
}

$nodeCmd = (Get-Command "node.exe" -ErrorAction SilentlyContinue)
if (-not $nodeCmd) {
    throw "node.exe is not found in PATH."
}

$tileServerMain = Join-Path $TileServerRoot "node_modules\tileserver-gl\src\main.js"
if (-not (Test-Path -LiteralPath $tileServerMain)) {
    $npmCmd = (Get-Command "npm.cmd" -ErrorAction SilentlyContinue)
    if ($npmCmd) {
        $npmRoot = (& $npmCmd.Source root -g).Trim()
        $globalMain = Join-Path $npmRoot "tileserver-gl\src\main.js"
        if (Test-Path -LiteralPath $globalMain) {
            $tileServerMain = $globalMain
        }
    }
}
if (-not (Test-Path -LiteralPath $tileServerMain)) {
    throw "tileserver-gl entrypoint not found. Install local copy in repo: `npx --yes npm@latest i --prefix `"$TileServerRoot`" tileserver-gl@5.4.0` or install globally: `npm i -g tileserver-gl`."
}

function Get-PortOwners {
    param([int]$CheckPort)

    $owners = @()
    try {
        $conns = Get-NetTCPConnection -State Listen -LocalPort $CheckPort -ErrorAction Stop
        foreach ($conn in $conns) {
            $owners += [int]$conn.OwningProcess
        }
    } catch {
        return @()
    }
    return $owners | Sort-Object -Unique
}

function Is-TileServerProcess {
    param([int]$Pid)
    try {
        $proc = Get-CimInstance Win32_Process -Filter "ProcessId = $Pid"
        if (-not $proc) { return $false }
        return ($proc.CommandLine -like "*tileserver-gl*")
    } catch {
        return $false
    }
}

$owners = Get-PortOwners -CheckPort $Port
if ($owners.Count -gt 0) {
    if ($ForceStopOnPortConflict) {
        foreach ($owner in $owners) {
            if (Is-TileServerProcess -Pid $owner) {
                Stop-Process -Id $owner -Force
                Write-Host "Stopped tileserver process on port $Port (PID=$owner)."
            } else {
                throw "Port $Port is occupied by PID=$owner (not tileserver-gl). Use another port or free it manually."
            }
        }
        Start-Sleep -Seconds 1
    } else {
        throw "Port $Port is already in use. Re-run with -ForceStopOnPortConflict or choose another port."
    }
}

if (-not $SkipOverlaySetup) {
    $repoRoot = Split-Path -Parent $PSScriptRoot
    $overlayScript = Join-Path $repoRoot "tools\setup_tileserver_geojson_overlay.ps1"
    if (Test-Path -LiteralPath $overlayScript) {
        & $overlayScript -TileServerRoot $TileServerRoot -StyleName $StyleName -Port $Port
    } else {
        Write-Warning "Overlay setup script not found: $overlayScript"
    }
}

Write-Host "Starting tileserver-gl:"
Write-Host "  Root:   $TileServerRoot"
Write-Host "  Config: $ConfigPath"
Write-Host "  Port:   $Port"

if ($Background) {
    $proc = Start-Process -FilePath $nodeCmd.Source -ArgumentList @($tileServerMain, "--config", $ConfigPath, "--port", "$Port") -WorkingDirectory $TileServerRoot -PassThru
    $styleUrl = "http://localhost:$Port/styles/$StyleName/style.json"
    $ok = $false
    for ($i = 0; $i -lt 12; $i++) {
        Start-Sleep -Milliseconds 500
        try {
            $resp = Invoke-WebRequest -UseBasicParsing -Uri $styleUrl -TimeoutSec 2
            if ($resp.StatusCode -eq 200) {
                $ok = $true
                break
            }
        } catch {
            # Keep waiting for startup.
        }
    }

    if ($ok) {
        Write-Host "tileserver-gl started (PID=$($proc.Id))."
        Write-Host "Style is reachable: $styleUrl [200]"
    } else {
        Write-Warning "tileserver-gl process started (PID=$($proc.Id)), but style check did not reach 200 in time."
        Write-Warning "Check logs and test URL: $styleUrl"
    }
} else {
    & $nodeCmd.Source $tileServerMain --config $ConfigPath --port $Port
}
