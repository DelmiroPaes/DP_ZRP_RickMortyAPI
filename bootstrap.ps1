<#
.SYNOPSIS
    Bootstrap script para RickMortyAPI
.DESCRIPTION
    Configura o ambiente, compila o projeto e prepara para execucao.
    Pre-requisito: Qt 6.8+ com MinGW deve estar instalado (veja README.md)
.NOTES
    Requer Qt com MinGW 64-bit instalado
#>

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

# Cores para output (nomes que nao conflitam com cmdlets nativos)
function Write-Step { param($msg) Write-Host "`n[$([char]0x2192)] $msg" -ForegroundColor Cyan }
function Write-Ok { param($msg) Write-Host "[OK] $msg" -ForegroundColor Green }
function Write-Warn { param($msg) Write-Host "[!] $msg" -ForegroundColor Yellow }
function Write-Err { param($msg) Write-Host "[ERRO] $msg" -ForegroundColor Red }

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "   RickMortyAPI - Bootstrap Script" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# ------------------------------------------------------------------------------
# 1. Verificar Qt e MinGW no PATH
# ------------------------------------------------------------------------------
Write-Step "Verificando Qt e MinGW..."

$qmake = Get-Command qmake -ErrorAction SilentlyContinue
$gcc = Get-Command gcc -ErrorAction SilentlyContinue

# Tentar encontrar Qt em locais comuns
$qtBasePath = "C:\Qt"
$qtFound = $false
$mingwFound = $false

if (-not $qmake) {
    if (Test-Path $qtBasePath) {
        # Buscar qualquer versao 6.x com MinGW
        $qtVersionDirs = Get-ChildItem -Path $qtBasePath -Directory -Filter "6.*" | Sort-Object Name -Descending

        foreach ($versionDir in $qtVersionDirs) {
            $mingwPath = Get-ChildItem -Path $versionDir.FullName -Directory -Filter "mingw*" -ErrorAction SilentlyContinue | Select-Object -First 1
            if ($mingwPath) {
                $qtBinPath = Join-Path $mingwPath.FullName "bin"
                if (Test-Path "$qtBinPath\qmake.exe") {
                    $env:Path = "$qtBinPath;$env:Path"
                    Write-Ok "Qt encontrado em: $qtBinPath"
                    $qtFound = $true
                    break
                }
            }
        }
    }
} else {
    $qtFound = $true
}

if (-not $qtFound) {
    Write-Err "Qt nao encontrado!"
    Write-Host ""
    Write-Host "Por favor, instale o Qt 6.8+ com MinGW seguindo as instrucoes do README.md" -ForegroundColor Yellow
    exit 1
}

# Verificar versao do Qt
$qmake = Get-Command qmake -ErrorAction SilentlyContinue
if ($qmake) {
    $qtVersion = & qmake -query QT_VERSION
    Write-Ok "Qt encontrado: $qtVersion"
}

# Verificar MinGW
if (-not $gcc) {
    # Tentar encontrar MinGW na instalacao do Qt
    $mingwToolsPath = Get-ChildItem -Path "$qtBasePath\Tools" -Directory -Filter "mingw*" -ErrorAction SilentlyContinue |
        Sort-Object Name -Descending | Select-Object -First 1

    if ($mingwToolsPath) {
        $mingwBinPath = Join-Path $mingwToolsPath.FullName "bin"
        if (Test-Path "$mingwBinPath\gcc.exe") {
            $env:Path = "$mingwBinPath;$env:Path"
            $mingwFound = $true
            Write-Ok "MinGW encontrado em: $mingwBinPath"
        }
    }
} else {
    $mingwFound = $true
}

$gcc = Get-Command gcc -ErrorAction SilentlyContinue
if ($gcc) {
    $gccVersion = & gcc --version | Select-Object -First 1
    Write-Ok "GCC: $gccVersion"
} else {
    Write-Err "MinGW (gcc) nao encontrado!"
    Write-Host ""
    Write-Host "Certifique-se de que MinGW 64-bit esta instalado com o Qt" -ForegroundColor Yellow
    exit 1
}

# ------------------------------------------------------------------------------
# 2. Verificar CMake e Ninja
# ------------------------------------------------------------------------------
Write-Step "Verificando CMake e Ninja..."

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    $qtCmakePath = "$qtBasePath\Tools\CMake_64\bin"
    if (Test-Path "$qtCmakePath\cmake.exe") {
        $env:Path = "$qtCmakePath;$env:Path"
    }
}

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Write-Err "CMake nao encontrado! Verifique a instalacao do Qt."
    exit 1
}

$cmakeVersion = & cmake --version | Select-Object -First 1
Write-Ok "CMake encontrado: $cmakeVersion"

# Verificar Ninja
$ninja = Get-Command ninja -ErrorAction SilentlyContinue
if (-not $ninja) {
    $qtNinjaPath = "$qtBasePath\Tools\Ninja"
    if (Test-Path "$qtNinjaPath\ninja.exe") {
        $env:Path = "$qtNinjaPath;$env:Path"
    }
}

$ninja = Get-Command ninja -ErrorAction SilentlyContinue
if (-not $ninja) {
    Write-Err "Ninja nao encontrado! Verifique a instalacao do Qt."
    exit 1
}

$ninjaVersion = & ninja --version
Write-Ok "Ninja encontrado: $ninjaVersion"

# ------------------------------------------------------------------------------
# 3. Configurar projeto com CMake
# ------------------------------------------------------------------------------
Write-Step "Configurando projeto com CMake..."

Push-Location $ProjectRoot

try {
    $buildDir = "build\release"

    # Limpar build anterior se existir
    if (Test-Path $buildDir) {
        Write-Host "    Limpando build anterior..." -ForegroundColor Gray
        Remove-Item -Recurse -Force $buildDir
    }

    Write-Host "    Executando cmake configure..." -ForegroundColor Gray

    # Configurar com Ninja e MinGW
    cmake -B $buildDir -S . -G Ninja -DCMAKE_BUILD_TYPE=Release

    if ($LASTEXITCODE -ne 0) {
        Write-Err "Falha na configuracao do CMake"
        exit 1
    }

    Write-Ok "Projeto configurado com sucesso"

    # ------------------------------------------------------------------------------
    # 4. Compilar em Release
    # ------------------------------------------------------------------------------
    Write-Step "Compilando projeto (Release)..."

    Write-Host "    Executando build..." -ForegroundColor Gray

    cmake --build $buildDir

    if ($LASTEXITCODE -ne 0) {
        Write-Err "Falha na compilacao"
        exit 1
    }

    Write-Ok "Compilacao concluida com sucesso"

    # ------------------------------------------------------------------------------
    # 5. Deploy
    # ------------------------------------------------------------------------------
    Write-Step "Executando deploy..."

    cmake --build $buildDir --target deploy

    if ($LASTEXITCODE -ne 0) {
        # Deploy pode falhar se target nao existir, tentar windeployqt manual
        Write-Warn "Target deploy nao disponivel, executando windeployqt manualmente..."

        $installDir = Join-Path $ProjectRoot "install\bin"
        if (-not (Test-Path $installDir)) {
            New-Item -ItemType Directory -Path $installDir -Force | Out-Null
        }

        $exePath = Join-Path $ProjectRoot "$buildDir\appRickMortyAPI.exe"
        if (Test-Path $exePath) {
            Copy-Item $exePath $installDir -Force

            $windeployqt = Get-Command windeployqt -ErrorAction SilentlyContinue
            if ($windeployqt) {
                $deployExe = Join-Path $installDir "appRickMortyAPI.exe"
                $qmlDir1 = Join-Path $ProjectRoot "qml"
                $qmlDir2 = Join-Path $ProjectRoot "modules\RickMortyUI\qml"

                Write-Host "    Executando windeployqt..." -ForegroundColor Gray
                & windeployqt --no-translations --no-system-d3d-compiler --no-opengl-sw `
                    --qmldir "$qmlDir1" --qmldir "$qmlDir2" "$deployExe"
            }
            else {
                Write-Warn "windeployqt nao encontrado. DLLs do Qt podem estar faltando."
            }
        }
        else {
            Write-Err "Executavel nao encontrado em: $exePath"
            exit 1
        }
    }

    Write-Ok "Deploy concluido"

    # ------------------------------------------------------------------------------
    # 6. Verificar resultado
    # ------------------------------------------------------------------------------
    $appPath = Join-Path $ProjectRoot "install\bin\appRickMortyAPI.exe"

    if (-not (Test-Path $appPath)) {
        # Tentar caminho alternativo
        $appPath = Join-Path $ProjectRoot "$buildDir\appRickMortyAPI.exe"
    }

    if (Test-Path $appPath) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "   Bootstrap concluido com sucesso!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Aplicacao disponivel em:" -ForegroundColor White
        Write-Host "  $appPath" -ForegroundColor Cyan
        Write-Host ""

        # ------------------------------------------------------------------------------
        # 7. Perguntar se deseja executar
        # ------------------------------------------------------------------------------
        $response = Read-Host "Deseja executar a aplicacao agora? (S/N)"

        if ($response -match "^[Ss]") {
            Write-Host ""
            Write-Host "Iniciando aplicacao..." -ForegroundColor Cyan
            Start-Process $appPath
        }
    }
    else {
        Write-Err "Aplicacao nao encontrada apos o build"
        exit 1
    }
}
finally {
    Pop-Location
}

Write-Host ""
