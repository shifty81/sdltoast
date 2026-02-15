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
