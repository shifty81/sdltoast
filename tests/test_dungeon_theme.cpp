// Harvest Quest — Dungeon Theme system unit tests

#include "world/DungeonTheme.h"
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

TEST(test_theme_count) {
    DungeonThemeManager mgr;
    ASSERT_EQ(mgr.GetThemeCount(), 5);
}

TEST(test_get_theme_by_type) {
    DungeonThemeManager mgr;
    const DungeonTheme* theme = mgr.GetTheme(DungeonThemeType::FOREST_TEMPLE);
    ASSERT_TRUE(theme != nullptr);
    ASSERT_EQ(theme->name, "Forest Temple");
    ASSERT_EQ(theme->difficultyTier, 1);
}

TEST(test_get_theme_by_index) {
    DungeonThemeManager mgr;
    const DungeonTheme* theme = mgr.GetThemeByIndex(0);
    ASSERT_TRUE(theme != nullptr);
    ASSERT_EQ(theme->type, DungeonThemeType::FOREST_TEMPLE);
}

TEST(test_get_theme_invalid_index) {
    DungeonThemeManager mgr;
    ASSERT_TRUE(mgr.GetThemeByIndex(-1) == nullptr);
    ASSERT_TRUE(mgr.GetThemeByIndex(999) == nullptr);
}

TEST(test_all_themes_present) {
    DungeonThemeManager mgr;
    ASSERT_TRUE(mgr.GetTheme(DungeonThemeType::FOREST_TEMPLE) != nullptr);
    ASSERT_TRUE(mgr.GetTheme(DungeonThemeType::CRYSTAL_CAVERNS) != nullptr);
    ASSERT_TRUE(mgr.GetTheme(DungeonThemeType::DESERT_PYRAMID) != nullptr);
    ASSERT_TRUE(mgr.GetTheme(DungeonThemeType::SHADOW_CATACOMBS) != nullptr);
    ASSERT_TRUE(mgr.GetTheme(DungeonThemeType::SKY_TOWER) != nullptr);
}

TEST(test_theme_names) {
    ASSERT_EQ(DungeonThemeManager::GetThemeName(DungeonThemeType::FOREST_TEMPLE), "Forest Temple");
    ASSERT_EQ(DungeonThemeManager::GetThemeName(DungeonThemeType::CRYSTAL_CAVERNS), "Crystal Caverns");
    ASSERT_EQ(DungeonThemeManager::GetThemeName(DungeonThemeType::DESERT_PYRAMID), "Desert Pyramid");
    ASSERT_EQ(DungeonThemeManager::GetThemeName(DungeonThemeType::SHADOW_CATACOMBS), "Shadow Catacombs");
    ASSERT_EQ(DungeonThemeManager::GetThemeName(DungeonThemeType::SKY_TOWER), "Sky Tower");
}

TEST(test_difficulty_ordering) {
    DungeonThemeManager mgr;
    for (int i = 1; i < mgr.GetThemeCount(); ++i) {
        const DungeonTheme* prev = mgr.GetThemeByIndex(i - 1);
        const DungeonTheme* curr = mgr.GetThemeByIndex(i);
        ASSERT_TRUE(curr->difficultyTier >= prev->difficultyTier);
    }
}

TEST(test_recommended_level_ordering) {
    DungeonThemeManager mgr;
    for (int i = 1; i < mgr.GetThemeCount(); ++i) {
        const DungeonTheme* prev = mgr.GetThemeByIndex(i - 1);
        const DungeonTheme* curr = mgr.GetThemeByIndex(i);
        ASSERT_TRUE(curr->recommendedLevel >= prev->recommendedLevel);
    }
}

TEST(test_boss_stats_increase) {
    DungeonThemeManager mgr;
    for (int i = 1; i < mgr.GetThemeCount(); ++i) {
        const DungeonTheme* prev = mgr.GetThemeByIndex(i - 1);
        const DungeonTheme* curr = mgr.GetThemeByIndex(i);
        ASSERT_TRUE(curr->bossHP >= prev->bossHP);
        ASSERT_TRUE(curr->bossDamage >= prev->bossDamage);
    }
}

TEST(test_room_ranges_valid) {
    DungeonThemeManager mgr;
    for (int i = 0; i < mgr.GetThemeCount(); ++i) {
        const DungeonTheme* theme = mgr.GetThemeByIndex(i);
        ASSERT_TRUE(theme->minRooms > 0);
        ASSERT_TRUE(theme->maxRooms >= theme->minRooms);
    }
}

TEST(test_loot_tier_range) {
    DungeonThemeManager mgr;
    for (int i = 0; i < mgr.GetThemeCount(); ++i) {
        const DungeonTheme* theme = mgr.GetThemeByIndex(i);
        ASSERT_TRUE(theme->lootTier >= 1);
        ASSERT_TRUE(theme->lootTier <= 5);
    }
}

TEST(test_themes_for_level_low) {
    DungeonThemeManager mgr;
    auto themes = mgr.GetThemesForLevel(1);
    ASSERT_TRUE(!themes.empty());
    // At level 1, only Forest Temple (recommended level 1) should be accessible
    ASSERT_EQ(static_cast<int>(themes.size()), 1);
    ASSERT_EQ(themes[0]->type, DungeonThemeType::FOREST_TEMPLE);
}

TEST(test_themes_for_level_mid) {
    DungeonThemeManager mgr;
    auto themes = mgr.GetThemesForLevel(5);
    // At level 5, Forest Temple (1), Crystal Caverns (3), Desert Pyramid (5)
    ASSERT_EQ(static_cast<int>(themes.size()), 3);
}

TEST(test_themes_for_level_max) {
    DungeonThemeManager mgr;
    auto themes = mgr.GetThemesForLevel(10);
    ASSERT_EQ(static_cast<int>(themes.size()), 5);
}

TEST(test_themes_for_level_zero) {
    DungeonThemeManager mgr;
    auto themes = mgr.GetThemesForLevel(0);
    ASSERT_TRUE(themes.empty());
}

TEST(test_hardest_accessible_level_1) {
    DungeonThemeManager mgr;
    const DungeonTheme* theme = mgr.GetHardestAccessible(1);
    ASSERT_TRUE(theme != nullptr);
    ASSERT_EQ(theme->type, DungeonThemeType::FOREST_TEMPLE);
}

TEST(test_hardest_accessible_level_10) {
    DungeonThemeManager mgr;
    const DungeonTheme* theme = mgr.GetHardestAccessible(10);
    ASSERT_TRUE(theme != nullptr);
    ASSERT_EQ(theme->type, DungeonThemeType::SKY_TOWER);
}

TEST(test_hardest_accessible_level_0) {
    DungeonThemeManager mgr;
    const DungeonTheme* theme = mgr.GetHardestAccessible(0);
    ASSERT_TRUE(theme == nullptr);
}

int main() {
    std::cout << "=== Dungeon Theme Tests ===" << std::endl;
    RUN_TEST(test_theme_count);
    RUN_TEST(test_get_theme_by_type);
    RUN_TEST(test_get_theme_by_index);
    RUN_TEST(test_get_theme_invalid_index);
    RUN_TEST(test_all_themes_present);
    RUN_TEST(test_theme_names);
    RUN_TEST(test_difficulty_ordering);
    RUN_TEST(test_recommended_level_ordering);
    RUN_TEST(test_boss_stats_increase);
    RUN_TEST(test_room_ranges_valid);
    RUN_TEST(test_loot_tier_range);
    RUN_TEST(test_themes_for_level_low);
    RUN_TEST(test_themes_for_level_mid);
    RUN_TEST(test_themes_for_level_max);
    RUN_TEST(test_themes_for_level_zero);
    RUN_TEST(test_hardest_accessible_level_1);
    RUN_TEST(test_hardest_accessible_level_10);
    RUN_TEST(test_hardest_accessible_level_0);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
