# VcpkgHints.cmake
# -----------------
# Helper to build a list of vcpkg installed-directory hints for Find modules.
#
# After including this file, the variable _VCPKG_HINTS contains paths such as
# <vcpkg_root>/installed/x64-windows (or the appropriate triplet) that can be
# passed to find_path() / find_library() HINTS.

set(_VCPKG_HINTS "")

# Determine the vcpkg root directory
set(_VCPKG_HINT_ROOT "")
if(DEFINED ENV{VCPKG_ROOT})
    set(_VCPKG_HINT_ROOT "$ENV{VCPKG_ROOT}")
elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT})
    set(_VCPKG_HINT_ROOT "$ENV{VCPKG_INSTALLATION_ROOT}")
endif()

if(_VCPKG_HINT_ROOT)
    # Prefer the explicit triplet when set
    if(DEFINED VCPKG_TARGET_TRIPLET)
        list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
    elseif(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
        list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/$ENV{VCPKG_DEFAULT_TRIPLET}")
    else()
        # Fallback: add common triplets for the current platform
        if(WIN32)
            list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/x64-windows")
            list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/x86-windows")
        elseif(APPLE)
            list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/x64-osx")
            list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/arm64-osx")
        else()
            list(APPEND _VCPKG_HINTS "${_VCPKG_HINT_ROOT}/installed/x64-linux")
        endif()
    endif()
endif()

unset(_VCPKG_HINT_ROOT)
