@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

build\bin\lab6.exe example/config.omfl
echo.

pause
