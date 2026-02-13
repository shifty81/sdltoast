@echo off
REM Quick build script for Harvest Quest on Windows
REM Requires: CMake and a C++ compiler (MSVC/MinGW)
REM Raylib is fetched automatically via CMake FetchContent

setlocal enabledelayedexpansion

set BUILD_LOG=build_output.log

echo Build started: %DATE% %TIME% > "%BUILD_LOG%"
echo ==================================
echo   Harvest Quest - Build Script
echo ==================================
echo.

REM Check for CMake
where cmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake not found. Please install CMake from https://cmake.org/download/
    echo ERROR: CMake not found. >> "%BUILD_LOG%"
    echo Build finished with errors: %DATE% %TIME% >> "%BUILD_LOG%"
    exit /b 1
)
echo [OK] CMake found

REM Create build directory
echo.
echo Creating build directory...
if not exist build mkdir build
cd build

REM Configure with CMake (Raylib is fetched automatically via FetchContent)
echo.
echo Configuring with CMake (Raylib will be downloaded automatically)...
cmake .. -DCMAKE_BUILD_TYPE=Debug >> "..\%BUILD_LOG%" 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    echo ERROR: CMake configuration failed! >> "..\%BUILD_LOG%"
    echo Build finished with errors: %DATE% %TIME% >> "..\%BUILD_LOG%"
    cd ..
    exit /b 1
)

REM Build
echo.
echo Building...
cmake --build . --config Debug >> "..\%BUILD_LOG%" 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    echo ERROR: Build failed! >> "..\%BUILD_LOG%"
    echo Build finished with errors: %DATE% %TIME% >> "..\%BUILD_LOG%"
    cd ..
    exit /b 1
)

echo.
echo ==================================
echo   Build Complete!
echo ==================================
echo.
echo To run the game:
echo   cd build\Debug
echo   HarvestQuest.exe
echo.
echo   (If you built with Release config, look in build\Release instead)
echo.
echo Build finished successfully: %DATE% %TIME% >> "..\%BUILD_LOG%"
cd ..
