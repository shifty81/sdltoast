// Harvest Quest — TilesetConfig unit tests

#include "engine/TilesetConfig.h"
#include "world/Tile.h"
#include "systems/Calendar.h"
#include <cassert>
#include <iostream>
#include <fstream>

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

// ===== Default Mapping Tests =====

TEST(test_defaults_load) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetTileSize(), 32);
    ASSERT_FALSE(cfg.HasSeasonalSupport());
    ASSERT_EQ(cfg.GetSheetFile(), "assets/tilesets/world_tileset.png");
}

TEST(test_defaults_base_ids) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS), 0);
    ASSERT_EQ(cfg.GetSpriteId(TileType::DIRT), 1);
    ASSERT_EQ(cfg.GetSpriteId(TileType::SOIL), 2);
    ASSERT_EQ(cfg.GetSpriteId(TileType::WATER), 3);
    ASSERT_EQ(cfg.GetSpriteId(TileType::STONE), 4);
    ASSERT_EQ(cfg.GetSpriteId(TileType::SAND), 5);
    ASSERT_EQ(cfg.GetSpriteId(TileType::FLOOR), 6);
    ASSERT_EQ(cfg.GetSpriteId(TileType::WALL), 7);
    ASSERT_EQ(cfg.GetSpriteId(TileType::DOOR), 20);
    ASSERT_EQ(cfg.GetSpriteId(TileType::CROP), 30);
    ASSERT_EQ(cfg.GetSpriteId(TileType::DECORATION), 40);
    ASSERT_EQ(cfg.GetSpriteId(TileType::TREE), 50);
}

TEST(test_defaults_special_bases) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetWallAutoTileBase(), 7);
    ASSERT_EQ(cfg.GetCropGrowthBase(), 30);
    ASSERT_EQ(cfg.GetDecorationBase(), 40);
    ASSERT_EQ(cfg.GetTreeBase(), 50);
}

TEST(test_defaults_water_frames) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetWaterFrameCount(), 4);
    ASSERT_EQ(cfg.GetWaterFrame(0), 3);
    ASSERT_EQ(cfg.GetWaterFrame(1), 21);
    ASSERT_EQ(cfg.GetWaterFrame(2), 22);
    ASSERT_EQ(cfg.GetWaterFrame(3), 23);
}

TEST(test_defaults_water_frame_bounds) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    // Out-of-bounds frames fall back to first frame
    ASSERT_EQ(cfg.GetWaterFrame(-1), 3);
    ASSERT_EQ(cfg.GetWaterFrame(99), 3);
}

// ===== Seasonal Fallback Tests =====

TEST(test_no_seasonal_fallback_to_base) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    // Without seasonal support, all seasons should return the base ID
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::SPRING), 0);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::SUMMER), 0);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::FALL), 0);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::WINTER), 0);
}

// ===== Config File Loading Tests =====

TEST(test_load_from_file) {
    // Write a temporary config file
    const char* path = "/tmp/test_tileset.cfg";
    {
        std::ofstream f(path);
        f << "# Test tileset config\n";
        f << "TILE_SIZE 16\n";
        f << "SHEET_FILE my_tileset.png\n";
        f << "GRASS 10\n";
        f << "WATER 20\n";
        f << "WALL_AUTOTILE_BASE 100\n";
        f << "CROP_GROWTH_BASE 200\n";
        f << "DECORATION_BASE 300\n";
        f << "TREE_BASE 400\n";
        f << "WATER_ANIM_FRAMES 20 21 22\n";
    }

    TilesetConfig cfg;
    ASSERT_TRUE(cfg.LoadFromFile(path));
    ASSERT_EQ(cfg.GetTileSize(), 16);
    ASSERT_EQ(cfg.GetSheetFile(), "my_tileset.png");
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS), 10);
    ASSERT_EQ(cfg.GetSpriteId(TileType::WATER), 20);
    ASSERT_EQ(cfg.GetWallAutoTileBase(), 100);
    ASSERT_EQ(cfg.GetCropGrowthBase(), 200);
    ASSERT_EQ(cfg.GetDecorationBase(), 300);
    ASSERT_EQ(cfg.GetTreeBase(), 400);
    ASSERT_EQ(cfg.GetWaterFrameCount(), 3);
    ASSERT_EQ(cfg.GetWaterFrame(0), 20);
    ASSERT_EQ(cfg.GetWaterFrame(1), 21);
    ASSERT_EQ(cfg.GetWaterFrame(2), 22);
}

TEST(test_load_seasonal_config) {
    const char* path = "/tmp/test_seasonal.cfg";
    {
        std::ofstream f(path);
        f << "TILE_SIZE 16\n";
        f << "GRASS 0\n";
        f << "GRASS.SPRING 0\n";
        f << "GRASS.SUMMER 64\n";
        f << "GRASS.FALL 128\n";
        f << "GRASS.WINTER 192\n";
        f << "TREE 50\n";
        f << "TREE.SPRING 50\n";
        f << "TREE.WINTER 242\n";
    }

    TilesetConfig cfg;
    ASSERT_TRUE(cfg.LoadFromFile(path));
    ASSERT_TRUE(cfg.HasSeasonalSupport());

    // Seasonal overrides
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::SPRING), 0);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::SUMMER), 64);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::FALL), 128);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS, Season::WINTER), 192);

    // Tree: only spring and winter have overrides
    ASSERT_EQ(cfg.GetSpriteId(TileType::TREE, Season::SPRING), 50);
    ASSERT_EQ(cfg.GetSpriteId(TileType::TREE, Season::WINTER), 242);
    // Summer/fall fall back to base
    ASSERT_EQ(cfg.GetSpriteId(TileType::TREE, Season::SUMMER), 50);
    ASSERT_EQ(cfg.GetSpriteId(TileType::TREE, Season::FALL), 50);
}

TEST(test_load_nonexistent_file) {
    TilesetConfig cfg;
    ASSERT_FALSE(cfg.LoadFromFile("/tmp/nonexistent_tileset_file.cfg"));
}

TEST(test_load_comments_and_empty_lines) {
    const char* path = "/tmp/test_comments.cfg";
    {
        std::ofstream f(path);
        f << "# This is a comment\n";
        f << "\n";
        f << "TILE_SIZE 24\n";
        f << "# Another comment\n";
        f << "GRASS 5\n";
        f << "\n";
    }

    TilesetConfig cfg;
    ASSERT_TRUE(cfg.LoadFromFile(path));
    ASSERT_EQ(cfg.GetTileSize(), 24);
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS), 5);
}

// ===== Edge Cases =====

TEST(test_unknown_tile_type_returns_zero) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetSpriteId(TileType::VOID), 0);
}

TEST(test_overrides_persist_on_reload) {
    const char* path = "/tmp/test_override.cfg";
    {
        std::ofstream f(path);
        f << "GRASS 99\n";
    }

    TilesetConfig cfg;
    cfg.LoadDefaults();
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS), 0);

    ASSERT_TRUE(cfg.LoadFromFile(path));
    ASSERT_EQ(cfg.GetSpriteId(TileType::GRASS), 99);
}

TEST(test_sheet_file_with_spaces) {
    const char* path = "/tmp/test_spaces.cfg";
    {
        std::ofstream f(path);
        f << "SHEET_FILE assets/my tilesets/world tileset.png\n";
    }

    TilesetConfig cfg;
    ASSERT_TRUE(cfg.LoadFromFile(path));
    ASSERT_EQ(cfg.GetSheetFile(), "assets/my tilesets/world tileset.png");
}

TEST(test_all_tile_types_have_defaults) {
    TilesetConfig cfg;
    cfg.LoadDefaults();
    // Every meaningful tile type should have a base ID >= 0
    ASSERT_TRUE(cfg.GetSpriteId(TileType::GRASS) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::DIRT) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::SOIL) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::WATER) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::STONE) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::SAND) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::FLOOR) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::WALL) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::DOOR) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::CROP) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::DECORATION) >= 0);
    ASSERT_TRUE(cfg.GetSpriteId(TileType::TREE) >= 0);
}

int main() {
    std::cout << "=== TilesetConfig Tests ===" << std::endl;
    RUN_TEST(test_defaults_load);
    RUN_TEST(test_defaults_base_ids);
    RUN_TEST(test_defaults_special_bases);
    RUN_TEST(test_defaults_water_frames);
    RUN_TEST(test_defaults_water_frame_bounds);
    RUN_TEST(test_no_seasonal_fallback_to_base);
    RUN_TEST(test_load_from_file);
    RUN_TEST(test_load_seasonal_config);
    RUN_TEST(test_load_nonexistent_file);
    RUN_TEST(test_load_comments_and_empty_lines);
    RUN_TEST(test_unknown_tile_type_returns_zero);
    RUN_TEST(test_overrides_persist_on_reload);
    RUN_TEST(test_sheet_file_with_spaces);
    RUN_TEST(test_all_tile_types_have_defaults);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
