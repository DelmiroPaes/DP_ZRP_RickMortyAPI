@echo off
:: Bootstrap wrapper - Executa bootstrap.ps1 via PowerShell
:: Requer execucao como Administrador para instalar dependencias

:: Verifica se esta rodando como admin
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERRO] Este script requer privilegios de Administrador.
    echo        Clique com botao direito e selecione "Executar como administrador"
    pause
    exit /b 1
)

:: Executa o PowerShell script
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0bootstrap.ps1"

:: Mantem a janela aberta se houve erro
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] Bootstrap falhou com codigo %errorlevel%
    pause
)
