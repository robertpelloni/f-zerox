@echo off
setlocal
title F-Zero X (CMake build)
cd /d "%~dp0"

echo [F-Zero X (CMake build)] Starting...
where cmake >nul 2>nul
if errorlevel 1 (
    echo [F-Zero X (CMake build)] cmake not found. Please install it.
    pause
    exit /b 1
)

cmake --build build --config Release

if errorlevel 1 (
    echo [F-Zero X (CMake build)] Exited with error code %errorlevel%.
    pause
)
endlocal
