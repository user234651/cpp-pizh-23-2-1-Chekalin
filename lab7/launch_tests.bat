@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo === Тесты программы ===
build\tests\bst_tests.exe

echo.
pause