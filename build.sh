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

# Function to install SDL2 dependencies automatically
install_sdl2() {
    echo "Attempting to install SDL2 automatically..." | tee -a "$BUILD_LOG"

    if [ "$(uname)" = "Darwin" ]; then
        # macOS
        if command -v brew >/dev/null 2>&1; then
            echo "Detected macOS with Homebrew. Installing SDL2..." | tee -a "$BUILD_LOG"
            brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf 2>&1 | tee -a "$BUILD_LOG"
        else
            echo "ERROR: Homebrew not found. Please install Homebrew first:" | tee -a "$BUILD_LOG"
            echo "  /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"" | tee -a "$BUILD_LOG"
            echo "Build finished with errors: $(date)" >> "$BUILD_LOG"
            exit 1
        fi
    elif [ -f /etc/debian_version ] || command -v apt-get >/dev/null 2>&1; then
        # Ubuntu/Debian
        echo "Detected Ubuntu/Debian. Installing SDL2 and build tools..." | tee -a "$BUILD_LOG"
        sudo apt-get update 2>&1 | tee -a "$BUILD_LOG"
        sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev cmake build-essential pkg-config 2>&1 | tee -a "$BUILD_LOG"
    elif [ -f /etc/fedora-release ] || command -v dnf >/dev/null 2>&1; then
        # Fedora
        echo "Detected Fedora. Installing SDL2..." | tee -a "$BUILD_LOG"
        sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel 2>&1 | tee -a "$BUILD_LOG"
    elif [ -f /etc/arch-release ] || command -v pacman >/dev/null 2>&1; then
        # Arch Linux
        echo "Detected Arch Linux. Installing SDL2..." | tee -a "$BUILD_LOG"
        sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_mixer sdl2_ttf 2>&1 | tee -a "$BUILD_LOG"
    elif [ -f /etc/os-release ]; then
        # Fallback: parse /etc/os-release for distro family
        ID=$(grep -oP '^ID=\K.*' /etc/os-release | tr -d '"')
        ID_LIKE=$(grep -oP '^ID_LIKE=\K.*' /etc/os-release | tr -d '"')
        PRETTY_NAME=$(grep -oP '^PRETTY_NAME=\K.*' /etc/os-release | tr -d '"')
        PRETTY_NAME="${PRETTY_NAME:-unknown}"
        case "$ID $ID_LIKE" in
            *debian*|*ubuntu*)
                echo "Detected Debian-based distro ($PRETTY_NAME). Installing SDL2..." | tee -a "$BUILD_LOG"
                sudo apt-get update 2>&1 | tee -a "$BUILD_LOG"
                sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev cmake build-essential pkg-config 2>&1 | tee -a "$BUILD_LOG"
                ;;
            *fedora*|*rhel*|*centos*)
                echo "Detected RHEL-based distro ($PRETTY_NAME). Installing SDL2..." | tee -a "$BUILD_LOG"
                if command -v dnf >/dev/null 2>&1; then
                    sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel 2>&1 | tee -a "$BUILD_LOG"
                else
                    sudo yum install -y SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel 2>&1 | tee -a "$BUILD_LOG"
                fi
                ;;
            *arch*)
                echo "Detected Arch-based distro ($PRETTY_NAME). Installing SDL2..." | tee -a "$BUILD_LOG"
                sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_mixer sdl2_ttf 2>&1 | tee -a "$BUILD_LOG"
                ;;
            *suse*)
                echo "Detected SUSE-based distro ($PRETTY_NAME). Installing SDL2..." | tee -a "$BUILD_LOG"
                sudo zypper install -y libSDL2-devel libSDL2_image-devel libSDL2_mixer-devel libSDL2_ttf-devel 2>&1 | tee -a "$BUILD_LOG"
                ;;
            *)
                echo "ERROR: Unsupported platform ($PRETTY_NAME). Please install SDL2 manually:" | tee -a "$BUILD_LOG"
                echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev" | tee -a "$BUILD_LOG"
                echo "  Fedora: sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel" | tee -a "$BUILD_LOG"
                echo "  Arch: sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf" | tee -a "$BUILD_LOG"
                echo "  macOS: brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf" | tee -a "$BUILD_LOG"
                echo "  Windows: Use vcpkg or download from https://www.libsdl.org/" | tee -a "$BUILD_LOG"
                echo "Build finished with errors: $(date)" >> "$BUILD_LOG"
                exit 1
                ;;
        esac
    else
        echo "ERROR: Unsupported platform. Please install SDL2 manually:" | tee -a "$BUILD_LOG"
        echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev" | tee -a "$BUILD_LOG"
        echo "  Fedora: sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel" | tee -a "$BUILD_LOG"
        echo "  Arch: sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf" | tee -a "$BUILD_LOG"
        echo "  macOS: brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf" | tee -a "$BUILD_LOG"
        echo "  Windows: Use vcpkg or download from https://www.libsdl.org/" | tee -a "$BUILD_LOG"
        echo "Build finished with errors: $(date)" >> "$BUILD_LOG"
        exit 1
    fi
}

# Check for SDL2
echo "Checking for SDL2..." | tee -a "$BUILD_LOG"
if ! pkg-config --exists sdl2 2>/dev/null; then
    echo "SDL2 not found. Will attempt automatic installation." | tee -a "$BUILD_LOG"
    install_sdl2

    # Verify installation succeeded
    if ! pkg-config --exists sdl2 2>/dev/null; then
        echo "ERROR: SDL2 installation failed. Please install manually." | tee -a "$BUILD_LOG"
        echo "Build finished with errors: $(date)" >> "$BUILD_LOG"
        exit 1
    fi
    echo "✓ SDL2 installed successfully" | tee -a "$BUILD_LOG"
else
    echo "✓ SDL2 found" | tee -a "$BUILD_LOG"
fi

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
