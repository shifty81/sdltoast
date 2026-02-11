# FindSDL2_image.cmake
# ---------------------
# Find the SDL2_image library.
#
# Result variables:
#   SDL2_image_FOUND        - True if SDL2_image was found
#   SDL2_IMAGE_INCLUDE_DIRS - SDL2_image include directories
#   SDL2_IMAGE_LIBRARIES    - SDL2_image libraries to link
#
# Imported targets:
#   SDL2_image::SDL2_image  - The SDL2_image library

if(NOT SDL2_image_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_SDL2_IMAGE QUIET SDL2_image)
    endif()

    # Gather vcpkg hints when available
    include(${CMAKE_CURRENT_LIST_DIR}/VcpkgHints.cmake)

    find_path(SDL2_IMAGE_INCLUDE_DIR
        NAMES SDL_image.h
        HINTS
            ${PC_SDL2_IMAGE_INCLUDEDIR}
            ${PC_SDL2_IMAGE_INCLUDE_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2IMAGEDIR}
            ${_VCPKG_HINTS}
        PATH_SUFFIXES SDL2 include/SDL2 include
    )

    find_library(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS
            ${PC_SDL2_IMAGE_LIBDIR}
            ${PC_SDL2_IMAGE_LIBRARY_DIRS}
            $ENV{SDL2DIR}
            $ENV{SDL2IMAGEDIR}
            ${_VCPKG_HINTS}
        PATH_SUFFIXES lib
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_image
        REQUIRED_VARS SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR
    )

    if(SDL2_image_FOUND)
        set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})
        set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})

        if(NOT TARGET SDL2_image::SDL2_image)
            add_library(SDL2_image::SDL2_image UNKNOWN IMPORTED)
            set_target_properties(SDL2_image::SDL2_image PROPERTIES
                IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}"
            )
        endif()
    endif()

    mark_as_advanced(SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIBRARY)
endif()
