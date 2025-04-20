@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo === Пример 1: Суммирование чисел 1 2 3 ===
build\bin\labwork5.exe 1 2 3 --sum
echo.

echo === Пример 2: Умножение чисел 4 5 6 ===
build\bin\labwork5.exe 4 5 6 --mult
echo.

pause
