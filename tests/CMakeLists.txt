cmake_minimum_required(VERSION 3.22)

# Harvest Quest unit tests
# Lightweight tests — no external framework required

# Test: Tile system (pure logic, no Raylib windowing needed)
add_executable(test_tile
    test_tile.cpp
    ${CMAKE_SOURCE_DIR}/src/world/Tile.cpp
)
target_include_directories(test_tile PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME TileTests COMMAND test_tile)

# Test: Inventory system
add_executable(test_inventory
    test_inventory.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Inventory.cpp
)
target_include_directories(test_inventory PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME InventoryTests COMMAND test_inventory)

# Test: Calendar system
add_executable(test_calendar
    test_calendar.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Calendar.cpp
)
target_include_directories(test_calendar PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME CalendarTests COMMAND test_calendar)

# Test: Quest system
add_executable(test_quest
    test_quest.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Quest.cpp
)
target_include_directories(test_quest PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME QuestTests COMMAND test_quest)

# Test: Crafting system (depends on Inventory for ingredient checks)
add_executable(test_crafting
    test_crafting.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Crafting.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Inventory.cpp
)
target_include_directories(test_crafting PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME CraftingTests COMMAND test_crafting)

# Test: Dialogue system (pure logic, no Raylib needed)
add_executable(test_dialogue
    test_dialogue.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Dialogue.cpp
)
target_include_directories(test_dialogue PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME DialogueTests COMMAND test_dialogue)

# Test: Farming system (pure logic, no Raylib needed)
add_executable(test_farming
    test_farming.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Farming.cpp
)
target_include_directories(test_farming PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME FarmingTests COMMAND test_farming)

# Test: Combat system (static methods only; entities need engine stubs for linking)
add_executable(test_combat
    test_combat.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Combat.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Player.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Enemy.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Input.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/SpriteSheet.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Logger.cpp
)
target_include_directories(test_combat PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_combat raylib)
add_test(NAME CombatTests COMMAND test_combat)

# Test: Map system (tile operations, farming interactions, collision)
add_executable(test_map
    test_map.cpp
    ${CMAKE_SOURCE_DIR}/src/world/Map.cpp
    ${CMAKE_SOURCE_DIR}/src/world/Tile.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/SpriteSheet.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Logger.cpp
)
target_include_directories(test_map PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_map raylib)
add_test(NAME MapTests COMMAND test_map)

# Test: NPC system (friendship, schedule, proximity)
add_executable(test_npc
    test_npc.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/NPC.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Dialogue.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/SpriteSheet.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Logger.cpp
)
target_include_directories(test_npc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_npc raylib)
add_test(NAME NPCTests COMMAND test_npc)

# Test: SaveSystem (save/load round-trip with file I/O)
add_executable(test_savesystem
    test_savesystem.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/SaveSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Inventory.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/Calendar.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/entities/Player.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Input.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/SpriteSheet.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/Logger.cpp
)
target_include_directories(test_savesystem PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_savesystem raylib)
add_test(NAME SaveSystemTests COMMAND test_savesystem)
