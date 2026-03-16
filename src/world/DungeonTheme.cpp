#include "DungeonTheme.h"
#include <algorithm>

DungeonThemeManager::DungeonThemeManager() {
    InitThemes();
}

void DungeonThemeManager::InitThemes() {
    // From GAME_DESIGN.md — 5 themed dungeons
    m_themes.push_back({
        "Forest Temple",
        "Nature themed, vine puzzles",
        DungeonThemeType::FOREST_TEMPLE,
        1,    // difficultyTier
        1,    // recommendedLevel
        5,    // minRooms
        8,    // maxRooms
        1,    // lootTier
        50,   // bossHP
        5     // bossDamage
    });

    m_themes.push_back({
        "Crystal Caverns",
        "Ice/water themed, sliding puzzles",
        DungeonThemeType::CRYSTAL_CAVERNS,
        2,    // difficultyTier
        3,    // recommendedLevel
        6,    // minRooms
        10,   // maxRooms
        2,    // lootTier
        80,   // bossHP
        8     // bossDamage
    });

    m_themes.push_back({
        "Desert Pyramid",
        "Fire themed, light reflection puzzles",
        DungeonThemeType::DESERT_PYRAMID,
        3,    // difficultyTier
        5,    // recommendedLevel
        7,    // minRooms
        12,   // maxRooms
        3,    // lootTier
        120,  // bossHP
        12    // bossDamage
    });

    m_themes.push_back({
        "Shadow Catacombs",
        "Dark themed, key/door puzzles",
        DungeonThemeType::SHADOW_CATACOMBS,
        4,    // difficultyTier
        7,    // recommendedLevel
        8,    // minRooms
        14,   // maxRooms
        4,    // lootTier
        160,  // bossHP
        15    // bossDamage
    });

    m_themes.push_back({
        "Sky Tower",
        "Wind themed, hookshot-based traversal",
        DungeonThemeType::SKY_TOWER,
        5,    // difficultyTier
        9,    // recommendedLevel
        10,   // minRooms
        16,   // maxRooms
        5,    // lootTier
        200,  // bossHP
        20    // bossDamage
    });
}

const DungeonTheme* DungeonThemeManager::GetTheme(DungeonThemeType type) const {
    for (const auto& theme : m_themes) {
        if (theme.type == type) return &theme;
    }
    return nullptr;
}

const DungeonTheme* DungeonThemeManager::GetThemeByIndex(int index) const {
    if (index < 0 || index >= static_cast<int>(m_themes.size())) return nullptr;
    return &m_themes[index];
}

std::string DungeonThemeManager::GetThemeName(DungeonThemeType type) {
    switch (type) {
        case DungeonThemeType::FOREST_TEMPLE:     return "Forest Temple";
        case DungeonThemeType::CRYSTAL_CAVERNS:   return "Crystal Caverns";
        case DungeonThemeType::DESERT_PYRAMID:    return "Desert Pyramid";
        case DungeonThemeType::SHADOW_CATACOMBS:  return "Shadow Catacombs";
        case DungeonThemeType::SKY_TOWER:         return "Sky Tower";
        default:                                  return "Unknown";
    }
}

std::vector<const DungeonTheme*> DungeonThemeManager::GetThemesForLevel(int combatLevel) const {
    std::vector<const DungeonTheme*> result;
    for (const auto& theme : m_themes) {
        if (combatLevel >= theme.recommendedLevel) {
            result.push_back(&theme);
        }
    }
    return result;
}

const DungeonTheme* DungeonThemeManager::GetHardestAccessible(int combatLevel) const {
    const DungeonTheme* hardest = nullptr;
    for (const auto& theme : m_themes) {
        if (combatLevel >= theme.recommendedLevel) {
            if (!hardest || theme.difficultyTier > hardest->difficultyTier) {
                hardest = &theme;
            }
        }
    }
    return hardest;
}
