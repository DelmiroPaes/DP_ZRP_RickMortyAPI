@echo off
:: Bootstrap wrapper - Executa bootstrap.ps1 via PowerShell
:: Compila o projeto usando Qt com MinGW

:: Executa o PowerShell script
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0bootstrap.ps1"

:: Mantem a janela aberta se houve erro
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] Bootstrap falhou com codigo %errorlevel%
    pause
)
