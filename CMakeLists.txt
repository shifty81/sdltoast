cmake_minimum_required(VERSION 3.15)

# Auto-detect vcpkg toolchain when not already specified.
# This must be set before project() so the toolchain is loaded during
# compiler/platform detection.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT} AND EXISTS "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    endif()
endif()

project(HarvestQuest VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile options for better warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add cmake/ directory to module path for Find modules (fallback when
# config-mode packages are not available, e.g. Windows without vcpkg)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories for project sources.
# SDL2 include directories are provided automatically by the imported targets
# (SDL2::SDL2, SDL2_image::SDL2_image, etc.) via target_link_libraries.
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/engine/Game.cpp
    src/engine/Renderer.cpp
    src/engine/Input.cpp
    src/engine/AssetManager.cpp
    src/engine/AudioManager.cpp
    src/engine/SpriteSheet.cpp
    src/engine/Logger.cpp
    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/NPC.cpp
    src/systems/Combat.cpp
    src/systems/Farming.cpp
    src/systems/Inventory.cpp
    src/systems/Calendar.cpp
    src/systems/Crafting.cpp
    src/systems/Dialogue.cpp
    src/world/Map.cpp
    src/world/Tile.cpp
    src/world/Dungeon.cpp
    src/world/WorldGenerator.cpp
    src/ui/HUD.cpp
    src/ui/Menu.cpp
    src/ui/DialogueBox.cpp
)

# Header files
set(HEADERS
    src/engine/Game.h
    src/engine/Renderer.h
    src/engine/Input.h
    src/engine/AssetManager.h
    src/engine/AudioManager.h
    src/engine/SpriteSheet.h
    src/engine/Logger.h
    src/entities/Entity.h
    src/entities/Player.h
    src/entities/Enemy.h
    src/entities/NPC.h
    src/systems/Combat.h
    src/systems/Farming.h
    src/systems/Inventory.h
    src/systems/Calendar.h
    src/systems/Crafting.h
    src/systems/Dialogue.h
    src/world/Map.h
    src/world/Tile.h
    src/world/Dungeon.h
    src/world/WorldGenerator.h
    src/ui/HUD.h
    src/ui/Menu.h
    src/ui/DialogueBox.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SDL2 libraries
if(TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
endif()
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

# Platform-specific settings
if(WIN32)
    # Windows: Copy DLLs to build directory
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
elseif(APPLE)
    # macOS: Create app bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE ON
    )
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

install(DIRECTORY assets/
    DESTINATION share/${PROJECT_NAME}/assets
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "HarvestQuest")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A game combining Zelda and Stardew Valley")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
include(CPack)
