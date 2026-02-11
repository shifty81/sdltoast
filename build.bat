@echo off
REM Quick build script for Harvest Quest on Windows
REM Requires: CMake, a C++ compiler (MSVC/MinGW), and vcpkg for SDL2

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

REM Check for vcpkg
if defined VCPKG_ROOT (
    echo [OK] VCPKG_ROOT is set to: %VCPKG_ROOT%
) else if exist "%LOCALAPPDATA%\vcpkg\vcpkg.exe" (
    set "VCPKG_ROOT=%LOCALAPPDATA%\vcpkg"
    echo [OK] Found vcpkg at: !VCPKG_ROOT!
) else (
    where vcpkg >nul 2>&1
    if !ERRORLEVEL! neq 0 (
        echo WARNING: vcpkg not found. SDL2 must already be installed or discoverable by CMake.
        echo You can install vcpkg from https://vcpkg.io/
        echo.
        echo To install SDL2 with vcpkg:
        echo   vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
        echo.
    ) else (
        for /f "delims=" %%i in ('where vcpkg') do (
            if not defined VCPKG_ROOT set "VCPKG_ROOT=%%~dpi"
        )
        REM Remove trailing backslash if present
        if "!VCPKG_ROOT:~-1!"=="\" set "VCPKG_ROOT=!VCPKG_ROOT:~0,-1!"
        echo [OK] Found vcpkg at: !VCPKG_ROOT!
    )
)

REM Determine vcpkg toolchain file
set "TOOLCHAIN_ARG="
if defined VCPKG_ROOT (
    if exist "!VCPKG_ROOT!\scripts\buildsystems\vcpkg.cmake" (
        set "TOOLCHAIN_ARG=-DCMAKE_TOOLCHAIN_FILE=!VCPKG_ROOT!\scripts\buildsystems\vcpkg.cmake"
        echo [OK] Using vcpkg toolchain: !VCPKG_ROOT!\scripts\buildsystems\vcpkg.cmake
    )
)

REM Create build directory
echo.
echo Creating build directory...
if not exist build mkdir build
cd build

REM Configure with CMake
echo.
echo Configuring with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Debug %TOOLCHAIN_ARG% >> "..\%BUILD_LOG%" 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    echo ERROR: CMake configuration failed! >> "..\%BUILD_LOG%"
    echo.
    echo Make sure SDL2 is installed. With vcpkg:
    echo   vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
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
