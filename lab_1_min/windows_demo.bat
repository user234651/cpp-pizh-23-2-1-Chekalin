@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: Установка шрифта, чтобы консоль отображала UTF-8 корректно:
:: Это на твой страх и риск — в GUI меняется шрифт cmd. Альтернатива — вручную в свойствах консоли.
:: REG ADD "HKCU\Console" /v "FaceName" /t REG_SZ /d "Consolas" /f

echo === Проверка программы ===
Program.exe

echo.
pause
