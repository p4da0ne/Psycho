param(
    [string]$RepoRoot = ""
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($RepoRoot)) {
    $RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
} else {
    $RepoRoot = (Resolve-Path $RepoRoot).Path
}

function Get-TrackedFiles {
    param([string]$Root)
    Push-Location $Root
    try {
        $tracked = git ls-files 2>$null
        if ($LASTEXITCODE -eq 0 -and $tracked) {
            return $tracked
        }
    } finally {
        Pop-Location
    }
    return Get-ChildItem -Path $Root -Recurse -File | ForEach-Object {
        $_.FullName.Substring($Root.Length + 1).Replace("\", "/")
    }
}

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

$issues = New-Object System.Collections.Generic.List[string]
$trackedFiles = Get-TrackedFiles -Root $RepoRoot

$bannedPrefixes = @(
    ".qtcreator/",
    "build-codex/",
    "mapview/data/"
)

foreach ($file in $trackedFiles) {
    foreach ($prefix in $bannedPrefixes) {
        if ($file.StartsWith($prefix, [System.StringComparison]::OrdinalIgnoreCase)) {
            $issues.Add("Banned path in tracked files: $file")
            break
        }
    }
}

foreach ($file in $trackedFiles) {
    if (-not $file.EndsWith(".cpp", [System.StringComparison]::OrdinalIgnoreCase)) {
        continue
    }
    $fullPath = Join-Path $RepoRoot $file
    if (-not (Test-Path -LiteralPath $fullPath)) {
        continue
    }
    $content = Get-Content -LiteralPath $fullPath -Raw
    if (Is-StubLikeCpp -Content $content) {
        $issues.Add("Stub-like cpp implementation: $file")
    }
}

if ($issues.Count -gt 0) {
    Write-Output "Repository quality gate: FAILED"
    foreach ($issue in $issues) {
        Write-Output " - $issue"
    }
    exit 1
}

Write-Output "Repository quality gate: PASSED"
exit 0
