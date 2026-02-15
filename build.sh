#!/usr/bin/env bash
#
# Harvest Quest — Atlas Forge Game Project Build Script
#
# Builds the Harvest Quest game project:
#   HarvestQuest - Top-down action-adventure RPG
#
# Usage:
#   ./build.sh [options]
#
# Options:
#   -b, --build-type TYPE   Build type: Debug, Release, Development (default: Release)
#   -j, --jobs N            Parallel build jobs (default: auto-detected)
#   -c, --clean             Clean build directory before building
#   -h, --help              Show this help message
#
# Examples:
#   ./build.sh                           # Build in Release mode
#   ./build.sh -b Debug                  # Build in Debug mode
#   ./build.sh --clean -b Release        # Clean rebuild in Release mode
#

set -euo pipefail

# --- Defaults ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$SCRIPT_DIR"
BUILD_DIR="$SOURCE_DIR/build"
BUILD_TYPE="Release"
CLEAN=false
JOBS=""

# --- Colors (disabled if not a terminal) ---
if [ -t 1 ]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    CYAN='\033[0;36m'
    BOLD='\033[1m'
    RESET='\033[0m'
else
    RED='' GREEN='' YELLOW='' CYAN='' BOLD='' RESET=''
fi

info()  { echo -e "${CYAN}[INFO]${RESET}  $*"; }
ok()    { echo -e "${GREEN}[OK]${RESET}    $*"; }
warn()  { echo -e "${YELLOW}[WARN]${RESET}  $*"; }
error() { echo -e "${RED}[ERROR]${RESET} $*" >&2; }

usage() {
    sed -n '/^# Usage:/,/^[^#]/{ /^#/s/^# \?//p }' "$0"
    exit 0
}

# --- Parse arguments ---
while [[ $# -gt 0 ]]; do
    case "$1" in
        -b|--build-type)
            BUILD_TYPE="$2"; shift 2 ;;
        -j|--jobs)
            JOBS="$2"; shift 2 ;;
        -c|--clean)
            CLEAN=true; shift ;;
        -h|--help)
            usage ;;
        -*)
            error "Unknown option: $1"; usage ;;
        *)
            shift ;;
    esac
done

# --- Map build type ---
CMAKE_BUILD_TYPE="$BUILD_TYPE"
case "$BUILD_TYPE" in
    Debug|debug)
        CMAKE_BUILD_TYPE="Debug" ;;
    Release|release)
        CMAKE_BUILD_TYPE="Release" ;;
    Development|development|Dev|dev)
        CMAKE_BUILD_TYPE="RelWithDebInfo" ;;
    *)
        error "Unknown build type: $BUILD_TYPE (expected Debug, Release, or Development)"
        exit 1 ;;
esac

# --- Detect parallel jobs ---
if [ -z "$JOBS" ]; then
    if command -v nproc &>/dev/null; then
        JOBS=$(nproc)
    elif command -v sysctl &>/dev/null; then
        JOBS=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    else
        JOBS=4
    fi
fi

# --- Setup build log ---
LOG_DIR="$SOURCE_DIR/logs"
mkdir -p "$LOG_DIR"
BUILD_LOG="$LOG_DIR/build.log"

# Redirect all output to both console and build log
exec > >(tee -a "$BUILD_LOG") 2>&1

echo ""
echo "=== Harvest Quest Build Log ==="
echo "Started: $(date '+%Y-%m-%d %H:%M:%S')"
echo ""

# --- Print build configuration ---
echo ""
echo -e "${BOLD}╔══════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}║   Harvest Quest — Atlas Forge Project    ║${RESET}"
echo -e "${BOLD}╚══════════════════════════════════════════╝${RESET}"
echo ""
info "Build type:    $BUILD_TYPE ($CMAKE_BUILD_TYPE)"
info "Parallel jobs: $JOBS"
info "Build log:     $BUILD_LOG"
echo ""

# --- Install Raylib build dependencies ---
install_raylib_deps() {
    info "Checking Raylib build dependencies..."

    if [ "$(uname)" = "Darwin" ]; then
        ok "macOS detected — no additional dependencies needed."
    elif [ -f /etc/debian_version ] || command -v apt-get >/dev/null 2>&1; then
        info "Detected Ubuntu/Debian. Installing Raylib build dependencies..."
        sudo apt-get update 2>&1 | while IFS= read -r line; do echo "  $line"; done
        sudo apt-get install -y cmake build-essential pkg-config \
            libasound2-dev libx11-dev libxrandr-dev libxi-dev \
            libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
            libxinerama-dev libwayland-dev libxkbcommon-dev 2>&1 | while IFS= read -r line; do echo "  $line"; done
    elif [ -f /etc/fedora-release ] || command -v dnf >/dev/null 2>&1; then
        info "Detected Fedora. Installing Raylib build dependencies..."
        sudo dnf install -y cmake gcc-c++ alsa-lib-devel mesa-libGL-devel \
            libX11-devel libXrandr-devel libXi-devel libXcursor-devel \
            libXinerama-devel wayland-devel libxkbcommon-devel 2>&1 | while IFS= read -r line; do echo "  $line"; done
    elif [ -f /etc/arch-release ] || command -v pacman >/dev/null 2>&1; then
        info "Detected Arch Linux. Installing Raylib build dependencies..."
        sudo pacman -S --noconfirm cmake base-devel alsa-lib mesa \
            libx11 libxrandr libxi libxcursor libxinerama wayland libxkbcommon 2>&1 | while IFS= read -r line; do echo "  $line"; done
    else
        warn "Could not detect platform. Raylib will be built from source via CMake FetchContent."
        warn "You may need to install X11 and OpenGL development libraries manually."
    fi
}

install_raylib_deps
ok "Dependencies ready"

# --- Clean ---
if [ "$CLEAN" = true ]; then
    info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    ok "Build directory cleaned"
fi

# --- Configure ---
info "Configuring CMake..."
mkdir -p "$BUILD_DIR"
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    2>&1 | while IFS= read -r line; do echo "  $line"; done

ok "CMake configured"

# --- Build ---
info "Building HarvestQuest..."
cmake --build "$BUILD_DIR" -j "$JOBS" \
    2>&1 | while IFS= read -r line; do echo "  $line"; done

ok "HarvestQuest built successfully"

# --- Summary ---
echo ""
echo -e "${BOLD}═══════════════════════════════════════════${RESET}"
echo -e "${GREEN}${BOLD} Build complete!${RESET}"
echo ""
info "To run the game:"
info "  cd build"
info "  ./HarvestQuest"
echo ""
info "Or with Atlas Runtime:"
info "  ./AtlasRuntime --project harvestquest.atlas"
echo -e "${BOLD}═══════════════════════════════════════════${RESET}"
echo ""
echo "=== Build Finished: $(date '+%Y-%m-%d %H:%M:%S') ==="
info "Build log saved to: $BUILD_LOG"
echo ""
