param(
    [Parameter(Mandatory = $true)]
    [string]$TargetRepo,
    [string]$OutputPath = ""
)

$ErrorActionPreference = "Stop"
$TargetRepo = (Resolve-Path $TargetRepo).Path

function Is-StubLikeCpp {
    param([string]$Content)
    $trimmed = $Content.Trim()
    if ($trimmed.Length -eq 0) {
        return $false
    }
    $stubPattern = '::\w+\s*\([^)]*\)\s*\r?\n\s*:\s*QObject\{parent\}\s*\r?\n\s*\{\s*\}'
    if ($trimmed -notmatch $stubPattern) {
        return $false
    }
    $openBraces = ([regex]::Matches($trimmed, "\{")).Count
    $lineCount = ([regex]::Matches($trimmed, "\r?\n")).Count + 1
    return ($openBraces -le 2 -and $lineCount -le 30)
}

function RelativePath {
    param([string]$Full, [string]$Root)
    return $Full.Substring($Root.Length + 1).Replace("\", "/")
}

$production = New-Object System.Collections.Generic.List[string]
$candidate = New-Object System.Collections.Generic.List[string]
$experimental = New-Object System.Collections.Generic.List[string]

$entries = @()
Push-Location $TargetRepo
try {
    $tracked = git ls-files 2>$null
    $untracked = git ls-files --others --exclude-standard 2>$null
    if ($LASTEXITCODE -eq 0) {
        $entries = @($tracked + $untracked | Sort-Object -Unique)
    }
} finally {
    Pop-Location
}

if (-not $entries -or $entries.Count -eq 0) {
    $entries = Get-ChildItem -Path $TargetRepo -Recurse -File | ForEach-Object {
        RelativePath -Full $_.FullName -Root $TargetRepo
    }
}

foreach ($rel in $entries) {
    $fullPath = Join-Path $TargetRepo $rel
    if (-not (Test-Path -LiteralPath $fullPath)) {
        continue
    }
    $file = Get-Item -LiteralPath $fullPath
    $ext = $file.Extension.ToLowerInvariant()

    if ($rel.StartsWith(".git/")) { continue }
    if ($rel.StartsWith("build/") -or $rel.StartsWith("build-codex/") -or $rel.StartsWith(".qtcreator/")) {
        $experimental.Add($rel)
        continue
    }
    if ($rel.StartsWith("mapview/data/") -and $ext -eq ".geojson") {
        if ($file.Length -gt 1024kb) { $experimental.Add($rel) } else { $candidate.Add($rel) }
        continue
    }
    if ($rel -match "/demo_" -or $rel -match "demo_") {
        $candidate.Add($rel)
        continue
    }

    if ($ext -eq ".cpp") {
        $content = Get-Content -LiteralPath $fullPath -Raw
        if (Is-StubLikeCpp -Content $content) {
            $experimental.Add($rel)
            continue
        }
    }

    if ($rel.StartsWith("dataaccess/")) {
        $production.Add($rel)
        continue
    }
    if ($rel.StartsWith("mainform/") -or $rel.StartsWith("mapview/")) {
        $candidate.Add($rel)
        continue
    }
    $production.Add($rel)
}

$report = @()
$report += "# Repo Noise Classification"
$report += ""
$report += "Target: $TargetRepo"
$report += "Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$report += ""
$report += "## Production"
foreach ($p in ($production | Sort-Object -Unique)) { $report += "- $p" }
$report += ""
$report += "## Candidate (review and integrate feature-by-feature)"
foreach ($c in ($candidate | Sort-Object -Unique)) { $report += "- $c" }
$report += ""
$report += "## Experimental/Noise (move out of runtime path)"
foreach ($e in ($experimental | Sort-Object -Unique)) { $report += "- $e" }
$report += ""
$report += "## Summary"
$report += "- Production: $(($production | Sort-Object -Unique).Count)"
$report += "- Candidate: $(($candidate | Sort-Object -Unique).Count)"
$report += "- Experimental: $(($experimental | Sort-Object -Unique).Count)"

$content = ($report -join "`r`n")
if (-not [string]::IsNullOrWhiteSpace($OutputPath)) {
    $target = $OutputPath
    $dir = Split-Path -Parent $target
    if (-not [string]::IsNullOrWhiteSpace($dir) -and -not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
    Set-Content -LiteralPath $target -Encoding UTF8 -Value $content
}

Write-Output $content
