# FindSDL2_mixer.cmake
# ---------------------
# Find the SDL2_mixer library.
#
# Result variables:
#   SDL2_mixer_FOUND        - True if SDL2_mixer was found
#   SDL2_MIXER_INCLUDE_DIRS - SDL2_mixer include directories
#   SDL2_MIXER_LIBRARIES    - SDL2_mixer libraries to link
#
# Imported targets:
#   SDL2_mixer::SDL2_mixer  - The SDL2_mixer library

if(NOT SDL2_mixer_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_SDL2_MIXER QUIET SDL2_mixer)
    endif()

    # Gather vcpkg hints when available
    include(${CMAKE_CURRENT_LIST_DIR}/VcpkgHints.cmake)

    find_path(SDL2_MIXER_INCLUDE_DIR
        NAMES SDL_mixer.h
        HINTS
            ${PC_SDL2_MIXER_INCLUDEDIR}
            ${PC_SDL2_MIXER_INCLUDE_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2MIXERDIR}
            ${_VCPKG_HINTS}
        PATH_SUFFIXES SDL2 include/SDL2 include
    )

    find_library(SDL2_MIXER_LIBRARY
        NAMES SDL2_mixer
        HINTS
            ${PC_SDL2_MIXER_LIBDIR}
            ${PC_SDL2_MIXER_LIBRARY_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2MIXERDIR}
            ${_VCPKG_HINTS}
        PATH_SUFFIXES lib
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_mixer
        REQUIRED_VARS SDL2_MIXER_LIBRARY SDL2_MIXER_INCLUDE_DIR
    )

    if(SDL2_mixer_FOUND)
        set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})
        set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})

        if(NOT TARGET SDL2_mixer::SDL2_mixer)
            add_library(SDL2_mixer::SDL2_mixer UNKNOWN IMPORTED)
            set_target_properties(SDL2_mixer::SDL2_mixer PROPERTIES
                IMPORTED_LOCATION "${SDL2_MIXER_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIR}"
            )
        endif()
    endif()

    mark_as_advanced(SDL2_MIXER_INCLUDE_DIR SDL2_MIXER_LIBRARY)
endif()
