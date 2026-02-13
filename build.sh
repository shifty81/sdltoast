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

# Function to install Raylib build dependencies
install_raylib_deps() {
    echo "Attempting to install Raylib build dependencies..." | tee -a "$BUILD_LOG"

    if [ "$(uname)" = "Darwin" ]; then
        # macOS - Raylib builds from source via CMake FetchContent, no extra deps needed
        echo "macOS detected - no additional dependencies needed." | tee -a "$BUILD_LOG"
    elif [ -f /etc/debian_version ] || command -v apt-get >/dev/null 2>&1; then
        # Ubuntu/Debian - need X11/OpenGL dev headers for Raylib
        echo "Detected Ubuntu/Debian. Installing Raylib build dependencies..." | tee -a "$BUILD_LOG"
        sudo apt-get update 2>&1 | tee -a "$BUILD_LOG"
        sudo apt-get install -y cmake build-essential pkg-config \
            libasound2-dev libx11-dev libxrandr-dev libxi-dev \
            libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
            libxinerama-dev libwayland-dev libxkbcommon-dev 2>&1 | tee -a "$BUILD_LOG"
    elif [ -f /etc/fedora-release ] || command -v dnf >/dev/null 2>&1; then
        # Fedora
        echo "Detected Fedora. Installing Raylib build dependencies..." | tee -a "$BUILD_LOG"
        sudo dnf install -y cmake gcc-c++ alsa-lib-devel mesa-libGL-devel \
            libX11-devel libXrandr-devel libXi-devel libXcursor-devel \
            libXinerama-devel wayland-devel libxkbcommon-devel 2>&1 | tee -a "$BUILD_LOG"
    elif [ -f /etc/arch-release ] || command -v pacman >/dev/null 2>&1; then
        # Arch Linux
        echo "Detected Arch Linux. Installing Raylib build dependencies..." | tee -a "$BUILD_LOG"
        sudo pacman -S --noconfirm cmake base-devel alsa-lib mesa \
            libx11 libxrandr libxi libxcursor libxinerama wayland libxkbcommon 2>&1 | tee -a "$BUILD_LOG"
    else
        echo "WARNING: Could not detect platform. Raylib will be built from source via CMake FetchContent." | tee -a "$BUILD_LOG"
        echo "You may need to install X11 and OpenGL development libraries manually." | tee -a "$BUILD_LOG"
    fi
}

# Install Raylib build dependencies
echo "Checking/installing Raylib build dependencies..." | tee -a "$BUILD_LOG"
install_raylib_deps
echo "✓ Dependencies ready" | tee -a "$BUILD_LOG"

# Create build directory
echo "" | tee -a "$BUILD_LOG"
echo "Creating build directory..." | tee -a "$BUILD_LOG"
mkdir -p build
cd build

# Configure with CMake (Raylib is fetched automatically via FetchContent)
echo "" | tee -a "$BUILD_LOG"
echo "Configuring with CMake (Raylib will be downloaded automatically)..." | tee -a "$BUILD_LOG"
CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Debug)
if ! cmake .. "${CMAKE_ARGS[@]}" 2>&1 | tee -a "../$BUILD_LOG"; then
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
