// Harvest Quest — Map system unit tests
// Tests tile operations, farming interactions, coordinate conversion,
// and collision detection (pure logic, rendering stubs provided via linking)

#include "world/Map.h"
#include "world/Tile.h"
#include <cassert>
#include <iostream>

static int s_passed = 0;
static int s_failed = 0;

#define TEST(name) static void name()
#define RUN_TEST(name) do { \
    std::cout << "  " #name "... "; \
    try { name(); std::cout << "PASS" << std::endl; s_passed++; } \
    catch (...) { std::cout << "FAIL" << std::endl; s_failed++; } \
} while(0)
#define ASSERT_TRUE(expr)  do { if (!(expr)) throw 1; } while(0)
#define ASSERT_FALSE(expr) do { if (expr) throw 1; } while(0)
#define ASSERT_EQ(a, b)    do { if ((a) != (b)) throw 1; } while(0)

// ---- Constructor tests ----

TEST(test_default_constructor) {
    Map map;
    ASSERT_EQ(map.GetWidth(), 25);
    ASSERT_EQ(map.GetHeight(), 19);
}

TEST(test_parameterized_constructor) {
    Map map(10, 8);
    ASSERT_EQ(map.GetWidth(), 10);
    ASSERT_EQ(map.GetHeight(), 8);
}

TEST(test_default_tiles_are_grass) {
    Map map(5, 5);
    const Tile* tile = map.GetTileAt(0, 0);
    ASSERT_TRUE(tile != nullptr);
    ASSERT_EQ(tile->GetType(), TileType::GRASS);
}

// ---- Tile access tests ----

TEST(test_get_tile_at_valid) {
    Map map(5, 5);
    Tile* tile = map.GetTileAt(2, 3);
    ASSERT_TRUE(tile != nullptr);
}

TEST(test_get_tile_at_invalid) {
    Map map(5, 5);
    Tile* tile = map.GetTileAt(-1, 0);
    ASSERT_TRUE(tile == nullptr);
    tile = map.GetTileAt(5, 0);
    ASSERT_TRUE(tile == nullptr);
    tile = map.GetTileAt(0, -1);
    ASSERT_TRUE(tile == nullptr);
    tile = map.GetTileAt(0, 5);
    ASSERT_TRUE(tile == nullptr);
}

TEST(test_set_tile) {
    Map map(5, 5);
    Tile wallTile(TileType::WALL, 3);
    map.SetTile(2, 2, wallTile);
    const Tile* tile = map.GetTileAt(2, 2);
    ASSERT_TRUE(tile != nullptr);
    ASSERT_EQ(tile->GetType(), TileType::WALL);
    ASSERT_EQ(tile->GetVisualId(), 3);
}

// ---- Collision tests ----

TEST(test_is_solid_wall) {
    Map map(5, 5);
    Tile wallTile(TileType::WALL);
    map.SetTile(1, 1, wallTile);
    ASSERT_TRUE(map.IsSolid(1, 1));
}

TEST(test_is_solid_grass) {
    Map map(5, 5);
    ASSERT_FALSE(map.IsSolid(0, 0));
}

TEST(test_is_solid_out_of_bounds) {
    Map map(5, 5);
    // Out of bounds should be treated as solid
    ASSERT_TRUE(map.IsSolid(-1, 0));
    ASSERT_TRUE(map.IsSolid(5, 0));
}

// ---- Coordinate conversion tests ----

TEST(test_world_to_tile) {
    Map map(10, 10);
    int tx, ty;
    map.WorldToTile(64.0f, 96.0f, tx, ty);
    // TILE_SIZE is 32, so 64/32=2, 96/32=3
    ASSERT_EQ(tx, 2);
    ASSERT_EQ(ty, 3);
}

TEST(test_tile_to_world) {
    Map map(10, 10);
    float wx, wy;
    map.TileToWorld(2, 3, wx, wy);
    // TILE_SIZE is 32, so 2*32=64, 3*32=96
    ASSERT_EQ(static_cast<int>(wx), 64);
    ASSERT_EQ(static_cast<int>(wy), 96);
}

TEST(test_world_to_tile_origin) {
    Map map(10, 10);
    int tx, ty;
    map.WorldToTile(0.0f, 0.0f, tx, ty);
    ASSERT_EQ(tx, 0);
    ASSERT_EQ(ty, 0);
}

// ---- Farming interaction tests ----

TEST(test_till_soil_on_grass) {
    Map map(5, 5);
    // Default tile is GRASS, tilling should work
    ASSERT_TRUE(map.TillSoil(2, 2));
    const Tile* tile = map.GetTileAt(2, 2);
    ASSERT_EQ(tile->GetType(), TileType::SOIL);
    ASSERT_EQ(tile->GetSoilState(), SoilState::HOE);
}

TEST(test_till_soil_on_wall_fails) {
    Map map(5, 5);
    Tile wallTile(TileType::WALL);
    map.SetTile(1, 1, wallTile);
    ASSERT_FALSE(map.TillSoil(1, 1));
}

TEST(test_water_tile) {
    Map map(5, 5);
    map.TillSoil(2, 2);
    ASSERT_TRUE(map.WaterTile(2, 2));
    const Tile* tile = map.GetTileAt(2, 2);
    ASSERT_EQ(tile->GetSoilState(), SoilState::WATERED);
}

TEST(test_water_tile_on_grass_fails) {
    Map map(5, 5);
    ASSERT_FALSE(map.WaterTile(0, 0));
}

TEST(test_plant_crop) {
    Map map(5, 5);
    map.TillSoil(2, 2);
    ASSERT_TRUE(map.PlantCrop(2, 2, 0)); // Crop type 0
    const Tile* tile = map.GetTileAt(2, 2);
    ASSERT_EQ(tile->GetSoilState(), SoilState::CROP);
    ASSERT_EQ(tile->GetCropType(), 0);
    ASSERT_EQ(tile->GetGrowthStage(), 0);
}

TEST(test_plant_crop_on_watered_soil) {
    Map map(5, 5);
    map.TillSoil(2, 2);
    map.WaterTile(2, 2);
    ASSERT_TRUE(map.PlantCrop(2, 2, 1));
    const Tile* tile = map.GetTileAt(2, 2);
    ASSERT_EQ(tile->GetSoilState(), SoilState::CROP);
    ASSERT_EQ(tile->GetCropType(), 1);
}

TEST(test_plant_crop_on_grass_fails) {
    Map map(5, 5);
    ASSERT_FALSE(map.PlantCrop(0, 0, 0));
}

TEST(test_harvest_crop) {
    Map map(5, 5);
    map.TillSoil(2, 2);
    map.PlantCrop(2, 2, 0);
    // Manually set to harvest state
    Tile* tile = map.GetTileAt(2, 2);
    tile->SetSoilState(SoilState::HARVEST);
    ASSERT_TRUE(map.HarvestCrop(2, 2));
    // After harvest, tile should be back to tilled soil
    ASSERT_EQ(tile->GetSoilState(), SoilState::HOE);
    ASSERT_EQ(tile->GetCropType(), -1);
    ASSERT_EQ(tile->GetGrowthStage(), 0);
}

TEST(test_harvest_crop_not_ready_fails) {
    Map map(5, 5);
    map.TillSoil(2, 2);
    map.PlantCrop(2, 2, 0);
    // Crop is still growing (CROP state, not HARVEST)
    ASSERT_FALSE(map.HarvestCrop(2, 2));
}

TEST(test_chop_tree) {
    Map map(5, 5);
    Tile treeTile(TileType::TREE);
    map.SetTile(3, 3, treeTile);
    ASSERT_TRUE(map.IsSolid(3, 3)); // Trees are solid
    ASSERT_TRUE(map.ChopTree(3, 3));
    const Tile* tile = map.GetTileAt(3, 3);
    ASSERT_EQ(tile->GetType(), TileType::GRASS);
    ASSERT_FALSE(map.IsSolid(3, 3)); // No longer solid
}

TEST(test_chop_tree_on_grass_fails) {
    Map map(5, 5);
    ASSERT_FALSE(map.ChopTree(0, 0));
}

TEST(test_can_plant_crop) {
    Map map(5, 5);
    // Grass is not farmable
    ASSERT_FALSE(map.CanPlantCrop(0, 0));
    // Till to make soil (farmable)
    map.TillSoil(0, 0);
    ASSERT_TRUE(map.CanPlantCrop(0, 0));
}

// ---- Full farming cycle ----

TEST(test_full_farming_cycle) {
    Map map(5, 5);
    // 1. Till grass → soil
    ASSERT_TRUE(map.TillSoil(2, 2));
    ASSERT_EQ(map.GetTileAt(2, 2)->GetSoilState(), SoilState::HOE);
    // 2. Water soil
    ASSERT_TRUE(map.WaterTile(2, 2));
    ASSERT_EQ(map.GetTileAt(2, 2)->GetSoilState(), SoilState::WATERED);
    // 3. Plant crop (planting on watered soil also works)
    ASSERT_TRUE(map.PlantCrop(2, 2, 0));
    ASSERT_EQ(map.GetTileAt(2, 2)->GetSoilState(), SoilState::CROP);
    // 4. Simulate growth to harvest
    Tile* tile = map.GetTileAt(2, 2);
    tile->SetSoilState(SoilState::HARVEST);
    // 5. Harvest
    ASSERT_TRUE(map.HarvestCrop(2, 2));
    ASSERT_EQ(tile->GetSoilState(), SoilState::HOE);
}

int main() {
    TileRegistry::Initialize();
    std::cout << "=== Map Tests ===" << std::endl;
    RUN_TEST(test_default_constructor);
    RUN_TEST(test_parameterized_constructor);
    RUN_TEST(test_default_tiles_are_grass);
    RUN_TEST(test_get_tile_at_valid);
    RUN_TEST(test_get_tile_at_invalid);
    RUN_TEST(test_set_tile);
    RUN_TEST(test_is_solid_wall);
    RUN_TEST(test_is_solid_grass);
    RUN_TEST(test_is_solid_out_of_bounds);
    RUN_TEST(test_world_to_tile);
    RUN_TEST(test_tile_to_world);
    RUN_TEST(test_world_to_tile_origin);
    RUN_TEST(test_till_soil_on_grass);
    RUN_TEST(test_till_soil_on_wall_fails);
    RUN_TEST(test_water_tile);
    RUN_TEST(test_water_tile_on_grass_fails);
    RUN_TEST(test_plant_crop);
    RUN_TEST(test_plant_crop_on_watered_soil);
    RUN_TEST(test_plant_crop_on_grass_fails);
    RUN_TEST(test_harvest_crop);
    RUN_TEST(test_harvest_crop_not_ready_fails);
    RUN_TEST(test_chop_tree);
    RUN_TEST(test_chop_tree_on_grass_fails);
    RUN_TEST(test_can_plant_crop);
    RUN_TEST(test_full_farming_cycle);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
