// Harvest Quest — Tile system unit tests
// Tests tile registry, tile properties, and crop growth

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

TEST(test_tile_registry_initialization) {
    TileRegistry::Initialize();
    const TileDef& wallDef = TileRegistry::GetDefinition(TileType::WALL);
    ASSERT_EQ(wallDef.type, TileType::WALL);
    ASSERT_TRUE(wallDef.solid);
    ASSERT_FALSE(wallDef.breakable);
    ASSERT_FALSE(wallDef.farmable);
    ASSERT_EQ(wallDef.name, "wall");
}

TEST(test_tile_floor_properties) {
    const TileDef& def = TileRegistry::GetDefinition(TileType::FLOOR);
    ASSERT_FALSE(def.solid);
    ASSERT_FALSE(def.breakable);
    ASSERT_FALSE(def.farmable);
}

TEST(test_tile_soil_is_farmable) {
    const TileDef& def = TileRegistry::GetDefinition(TileType::SOIL);
    ASSERT_FALSE(def.solid);
    ASSERT_TRUE(def.farmable);
}

TEST(test_tile_tree_properties) {
    const TileDef& def = TileRegistry::GetDefinition(TileType::TREE);
    ASSERT_TRUE(def.solid);
    ASSERT_TRUE(def.breakable);
    ASSERT_FALSE(def.farmable);
}

TEST(test_tile_default_constructor) {
    Tile tile;
    ASSERT_EQ(tile.GetType(), TileType::GRASS);
    ASSERT_EQ(tile.GetVisualId(), 0);
    ASSERT_EQ(tile.GetSoilState(), SoilState::GRASS);
    ASSERT_EQ(tile.GetCropType(), -1);
    ASSERT_EQ(tile.GetGrowthStage(), 0);
}

TEST(test_tile_parameterized_constructor) {
    Tile tile(TileType::WALL, 5);
    ASSERT_EQ(tile.GetType(), TileType::WALL);
    ASSERT_EQ(tile.GetVisualId(), 5);
    ASSERT_TRUE(tile.IsSolid());
}

TEST(test_tile_crop_growth) {
    Tile tile(TileType::SOIL);
    tile.SetSoilState(SoilState::CROP);
    tile.SetCropType(0);
    ASSERT_EQ(tile.GetGrowthStage(), 0);

    // Simulate time passing (growth interval is 5.0s)
    for (int i = 0; i < 4; ++i) {
        tile.Update(5.0f);
    }
    ASSERT_EQ(tile.GetGrowthStage(), Tile::MAX_GROWTH_STAGE);
    ASSERT_EQ(tile.GetSoilState(), SoilState::HARVEST);
}

int main() {
    std::cout << "=== Tile Tests ===" << std::endl;
    RUN_TEST(test_tile_registry_initialization);
    RUN_TEST(test_tile_floor_properties);
    RUN_TEST(test_tile_soil_is_farmable);
    RUN_TEST(test_tile_tree_properties);
    RUN_TEST(test_tile_default_constructor);
    RUN_TEST(test_tile_parameterized_constructor);
    RUN_TEST(test_tile_crop_growth);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
