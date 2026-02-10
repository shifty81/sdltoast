#!/bin/bash
# Quick build and test script for Harvest Quest

set -e

echo "=================================="
echo "  Harvest Quest - Build Script   "
echo "=================================="
echo ""

# Check for SDL2
echo "Checking for SDL2..."
if ! pkg-config --exists sdl2 2>/dev/null; then
    echo "ERROR: SDL2 not found!"
    echo ""
    echo "Please install SDL2:"
    echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
    echo "  macOS: brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf"
    echo "  Windows: Use vcpkg or download from https://www.libsdl.org/"
    exit 1
fi

echo "✓ SDL2 found"

# Create build directory
echo ""
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo ""
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
echo ""
echo "Building..."
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "=================================="
echo "  Build Complete!                "
echo "=================================="
echo ""
echo "To run the game:"
echo "  cd build"
echo "  ./HarvestQuest"
echo ""
