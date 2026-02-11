# FindSDL2_ttf.cmake
# -------------------
# Find the SDL2_ttf library.
#
# Result variables:
#   SDL2_ttf_FOUND        - True if SDL2_ttf was found
#   SDL2_TTF_INCLUDE_DIRS - SDL2_ttf include directories
#   SDL2_TTF_LIBRARIES    - SDL2_ttf libraries to link
#
# Imported targets:
#   SDL2_ttf::SDL2_ttf    - The SDL2_ttf library

if(NOT SDL2_ttf_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_SDL2_TTF QUIET SDL2_ttf)
    endif()

    find_path(SDL2_TTF_INCLUDE_DIR
        NAMES SDL_ttf.h
        HINTS
            ${PC_SDL2_TTF_INCLUDEDIR}
            ${PC_SDL2_TTF_INCLUDE_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2TTFDIR}
        PATH_SUFFIXES SDL2 include/SDL2 include
    )

    find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS
            ${PC_SDL2_TTF_LIBDIR}
            ${PC_SDL2_TTF_LIBRARY_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2TTFDIR}
        PATH_SUFFIXES lib
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_ttf
        REQUIRED_VARS SDL2_TTF_LIBRARY SDL2_TTF_INCLUDE_DIR
    )

    if(SDL2_ttf_FOUND)
        set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})
        set(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBRARY})

        if(NOT TARGET SDL2_ttf::SDL2_ttf)
            add_library(SDL2_ttf::SDL2_ttf UNKNOWN IMPORTED)
            set_target_properties(SDL2_ttf::SDL2_ttf PROPERTIES
                IMPORTED_LOCATION "${SDL2_TTF_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SDL2_TTF_INCLUDE_DIR}"
            )
        endif()
    endif()

    mark_as_advanced(SDL2_TTF_INCLUDE_DIR SDL2_TTF_LIBRARY)
endif()
