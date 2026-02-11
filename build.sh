#!/bin/bash
# Quick build and test script for Harvest Quest

set -e

BUILD_LOG="build_output.log"

# Start logging
echo "Build started: $(date)" | tee "$BUILD_LOG"

echo "==================================" | tee -a "$BUILD_LOG"
echo "  Harvest Quest - Build Script   " | tee -a "$BUILD_LOG"
echo "==================================" | tee -a "$BUILD_LOG"
echo "" | tee -a "$BUILD_LOG"

# Check for SDL2
echo "Checking for SDL2..." | tee -a "$BUILD_LOG"
if ! pkg-config --exists sdl2 2>/dev/null; then
    echo "ERROR: SDL2 not found!" | tee -a "$BUILD_LOG"
    echo "" | tee -a "$BUILD_LOG"
    echo "Please install SDL2:" | tee -a "$BUILD_LOG"
    echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev" | tee -a "$BUILD_LOG"
    echo "  macOS: brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf" | tee -a "$BUILD_LOG"
    echo "  Windows: Use vcpkg or download from https://www.libsdl.org/" | tee -a "$BUILD_LOG"
    echo "Build finished with errors: $(date)" >> "$BUILD_LOG"
    exit 1
fi

echo "✓ SDL2 found" | tee -a "$BUILD_LOG"

# Create build directory
echo "" | tee -a "$BUILD_LOG"
echo "Creating build directory..." | tee -a "$BUILD_LOG"
mkdir -p build
cd build

# Configure with CMake
echo "" | tee -a "$BUILD_LOG"
echo "Configuring with CMake..." | tee -a "$BUILD_LOG"
if ! cmake .. -DCMAKE_BUILD_TYPE=Debug 2>&1 | tee -a "../$BUILD_LOG"; then
    echo "ERROR: CMake configuration failed!" | tee -a "../$BUILD_LOG"
    echo "Build finished with errors: $(date)" >> "../$BUILD_LOG"
    exit 1
fi

# Build
echo "" | tee -a "../$BUILD_LOG"
echo "Building..." | tee -a "../$BUILD_LOG"
if ! cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1 | tee -a "../$BUILD_LOG"; then
    echo "ERROR: Build failed!" | tee -a "../$BUILD_LOG"
    echo "Build finished with errors: $(date)" >> "../$BUILD_LOG"
    exit 1
fi

echo "" | tee -a "../$BUILD_LOG"
echo "==================================" | tee -a "../$BUILD_LOG"
echo "  Build Complete!                " | tee -a "../$BUILD_LOG"
echo "==================================" | tee -a "../$BUILD_LOG"
echo "" | tee -a "../$BUILD_LOG"
echo "To run the game:" | tee -a "../$BUILD_LOG"
echo "  cd build" | tee -a "../$BUILD_LOG"
echo "  ./HarvestQuest" | tee -a "../$BUILD_LOG"
echo "" | tee -a "../$BUILD_LOG"
echo "Build finished successfully: $(date)" >> "../$BUILD_LOG"
