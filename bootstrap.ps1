#Requires -RunAsAdministrator
<#
.SYNOPSIS
    Bootstrap script para RickMortyAPI
.DESCRIPTION
    Instala dependencias necessarias, compila o projeto e prepara para execucao.
    Pre-requisito: Qt 6.8+ deve estar instalado (veja README.md)
.NOTES
    Executar como Administrador
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
# 1. Verificar/Instalar MSVC Build Tools
# ------------------------------------------------------------------------------
Write-Step "Verificando MSVC Build Tools..."

$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$msvcInstalled = $false
$msvcJustInstalled = $false

if (Test-Path $vsWhere) {
    $vsInstallPath = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
    if ($vsInstallPath) {
        $msvcInstalled = $true
        Write-Ok "MSVC Build Tools encontrado em: $vsInstallPath"
    }
}

if (-not $msvcInstalled) {
    Write-Warn "MSVC Build Tools nao encontrado. Iniciando instalacao..."

    $installerUrl = "https://aka.ms/vs/17/release/vs_buildtools.exe"
    $installerPath = "$env:TEMP\vs_buildtools.exe"

    Write-Host "    Baixando Visual Studio Build Tools..." -ForegroundColor Gray
    try {
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
        $ProgressPreference = 'SilentlyContinue'  # Acelera o download
        Invoke-WebRequest -Uri $installerUrl -OutFile $installerPath -UseBasicParsing
        $ProgressPreference = 'Continue'
    }
    catch {
        Write-Err "Falha ao baixar o instalador: $_"
        exit 1
    }

    Write-Host "    Instalando (isso pode levar varios minutos)..." -ForegroundColor Gray
    $installArgs = @(
        "--quiet",
        "--wait",
        "--norestart",
        "--nocache",
        "--add", "Microsoft.VisualStudio.Workload.VCTools",
        "--includeRecommended"
    )

    $process = Start-Process -FilePath $installerPath -ArgumentList $installArgs -Wait -PassThru

    if ($process.ExitCode -ne 0 -and $process.ExitCode -ne 3010) {
        Write-Err "Instalacao do MSVC falhou com codigo: $($process.ExitCode)"
        exit 1
    }

    Write-Ok "MSVC Build Tools instalado com sucesso!"
    $msvcJustInstalled = $true

    # Limpar instalador
    Remove-Item $installerPath -Force -ErrorAction SilentlyContinue

    # Atualizar variavel para proximas etapas
    if (Test-Path $vsWhere) {
        $vsInstallPath = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
    }

    if (-not $vsInstallPath) {
        Write-Warn "MSVC foi instalado mas requer reinicializacao do sistema."
        Write-Host ""
        Write-Host "Por favor, reinicie o computador e execute o bootstrap novamente." -ForegroundColor Yellow
        Read-Host "Pressione ENTER para sair"
        exit 0
    }
}

# ------------------------------------------------------------------------------
# 2. Verificar Qt no PATH
# ------------------------------------------------------------------------------
Write-Step "Verificando Qt..."

$qmake = Get-Command qmake -ErrorAction SilentlyContinue

if (-not $qmake) {
    # Tentar encontrar Qt em locais comuns usando wildcard
    $qtBasePath = "C:\Qt"
    $qtFound = $false

    if (Test-Path $qtBasePath) {
        # Buscar qualquer versao 6.8.x
        $qtVersionDirs = Get-ChildItem -Path $qtBasePath -Directory -Filter "6.8*" | Sort-Object Name -Descending

        foreach ($versionDir in $qtVersionDirs) {
            $msvcPath = Join-Path $versionDir.FullName "msvc2022_64\bin"
            if (Test-Path "$msvcPath\qmake.exe") {
                $env:Path = "$msvcPath;$env:Path"
                Write-Ok "Qt encontrado em: $msvcPath"
                $qtFound = $true
                break
            }
        }
    }

    if (-not $qtFound) {
        Write-Err "Qt nao encontrado!"
        Write-Host ""
        Write-Host "Por favor, instale o Qt 6.8+ seguindo as instrucoes do README.md" -ForegroundColor Yellow
        Write-Host "Apos instalar, adicione ao PATH ou reinstale em C:\Qt" -ForegroundColor Yellow
        exit 1
    }
}

$qmake = Get-Command qmake -ErrorAction SilentlyContinue
if ($qmake) {
    $qtVersion = & qmake -query QT_VERSION
    Write-Ok "Qt encontrado: $qtVersion"
}

# Verificar CMake (pode vir do Qt ou instalacao separada)
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    $qtCmakePath = "C:\Qt\Tools\CMake_64\bin"
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
    $qtNinjaPath = "C:\Qt\Tools\Ninja"
    if (Test-Path "$qtNinjaPath\ninja.exe") {
        $env:Path = "$qtNinjaPath;$env:Path"
    }
}

$ninja = Get-Command ninja -ErrorAction SilentlyContinue
if ($ninja) {
    $ninjaVersion = & ninja --version
    Write-Ok "Ninja encontrado: $ninjaVersion"
}

# ------------------------------------------------------------------------------
# 3. Configurar ambiente MSVC
# ------------------------------------------------------------------------------
Write-Step "Configurando ambiente de compilacao..."

$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsInstallPath = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath

if (-not $vsInstallPath) {
    Write-Err "Nao foi possivel encontrar a instalacao do Visual Studio/Build Tools"
    exit 1
}

$vcvarsall = Join-Path $vsInstallPath "VC\Auxiliary\Build\vcvarsall.bat"
if (-not (Test-Path $vcvarsall)) {
    Write-Err "vcvarsall.bat nao encontrado em: $vcvarsall"
    exit 1
}

Write-Ok "Ambiente MSVC configurado: $vsInstallPath"

# ------------------------------------------------------------------------------
# 4. Configurar projeto com CMake
# ------------------------------------------------------------------------------
Write-Step "Configurando projeto com CMake..."

Push-Location $ProjectRoot

try {
    $buildDir = "build"

    # Limpar build anterior se existir
    if (Test-Path $buildDir) {
        Write-Host "    Limpando build anterior..." -ForegroundColor Gray
        Remove-Item -Recurse -Force $buildDir
    }

    Write-Host "    Executando cmake configure..." -ForegroundColor Gray

    # Executar cmake via cmd com vcvarsall
    $cmakeCmd = "`"$vcvarsall`" x64 && cmake -B build -S . -G `"Visual Studio 17 2022`" -A x64"

    # Mostrar output em tempo real
    cmd /c "$cmakeCmd"

    if ($LASTEXITCODE -ne 0) {
        Write-Err "Falha na configuracao do CMake"
        exit 1
    }

    Write-Ok "Projeto configurado com sucesso"

    # ------------------------------------------------------------------------------
    # 5. Compilar em Release
    # ------------------------------------------------------------------------------
    Write-Step "Compilando projeto (Release)..."

    Write-Host "    Executando build..." -ForegroundColor Gray

    $buildCmd = "`"$vcvarsall`" x64 && cmake --build build --config Release"

    # Mostrar output em tempo real
    cmd /c "$buildCmd"

    if ($LASTEXITCODE -ne 0) {
        Write-Err "Falha na compilacao"
        exit 1
    }

    Write-Ok "Compilacao concluida com sucesso"

    # ------------------------------------------------------------------------------
    # 6. Deploy
    # ------------------------------------------------------------------------------
    Write-Step "Executando deploy..."

    $deployCmd = "`"$vcvarsall`" x64 && cmake --build build --config Release --target deploy"

    # Mostrar output em tempo real
    cmd /c "$deployCmd"

    if ($LASTEXITCODE -ne 0) {
        # Deploy pode falhar se target nao existir, tentar windeployqt manual
        Write-Warn "Target deploy nao disponivel, executando windeployqt manualmente..."

        $installDir = Join-Path $ProjectRoot "install\bin"
        if (-not (Test-Path $installDir)) {
            New-Item -ItemType Directory -Path $installDir -Force | Out-Null
        }

        $exePath = Join-Path $ProjectRoot "build\Release\appRickMortyAPI.exe"
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
    # 7. Verificar resultado
    # ------------------------------------------------------------------------------
    $appPath = Join-Path $ProjectRoot "install\bin\appRickMortyAPI.exe"

    if (-not (Test-Path $appPath)) {
        # Tentar caminho alternativo
        $appPath = Join-Path $ProjectRoot "build\Release\appRickMortyAPI.exe"
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
        # 8. Perguntar se deseja executar
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
