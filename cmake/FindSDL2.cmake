# FindSDL2.cmake
# ---------------
# Find the SDL2 library.
#
# Result variables:
#   SDL2_FOUND        - True if SDL2 was found
#   SDL2_INCLUDE_DIRS - SDL2 include directories
#   SDL2_LIBRARIES    - SDL2 libraries to link
#
# Imported targets:
#   SDL2::SDL2        - The SDL2 library
#   SDL2::SDL2main    - The SDL2main library (if found)

if(NOT SDL2_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_SDL2 QUIET sdl2)
    endif()

    find_path(SDL2_INCLUDE_DIR
        NAMES SDL.h
        HINTS
            ${PC_SDL2_INCLUDEDIR}
            ${PC_SDL2_INCLUDE_DIRS}
            $ENV{SDL2DIR}
        PATH_SUFFIXES SDL2 include/SDL2 include
    )

    find_library(SDL2_LIBRARY
        NAMES SDL2
        HINTS
            ${PC_SDL2_LIBDIR}
            ${PC_SDL2_LIBRARY_DIRS}
            $ENV{SDL2DIR}
        PATH_SUFFIXES lib
    )

    find_library(SDL2MAIN_LIBRARY
        NAMES SDL2main
        HINTS
            ${PC_SDL2_LIBDIR}
            ${PC_SDL2_LIBRARY_DIRS}
            $ENV{SDL2DIR}
        PATH_SUFFIXES lib
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2
        REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
    )

    if(SDL2_FOUND)
        set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
        set(SDL2_LIBRARIES ${SDL2_LIBRARY})

        if(NOT TARGET SDL2::SDL2)
            add_library(SDL2::SDL2 UNKNOWN IMPORTED)
            set_target_properties(SDL2::SDL2 PROPERTIES
                IMPORTED_LOCATION "${SDL2_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
            )
            if(PC_SDL2_CFLAGS_OTHER)
                set_target_properties(SDL2::SDL2 PROPERTIES
                    INTERFACE_COMPILE_OPTIONS "${PC_SDL2_CFLAGS_OTHER}"
                )
            endif()
        endif()

        if(SDL2MAIN_LIBRARY AND NOT TARGET SDL2::SDL2main)
            add_library(SDL2::SDL2main UNKNOWN IMPORTED)
            set_target_properties(SDL2::SDL2main PROPERTIES
                IMPORTED_LOCATION "${SDL2MAIN_LIBRARY}"
            )
        endif()
    endif()

    mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2MAIN_LIBRARY)
endif()
