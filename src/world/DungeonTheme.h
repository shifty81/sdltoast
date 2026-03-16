#ifndef DUNGEON_THEME_H
#define DUNGEON_THEME_H

#include <string>
#include <vector>

/**
 * Dungeon theme identifiers — each theme has unique aesthetics and difficulty.
 * From game design: Forest Temple, Crystal Caverns, Desert Pyramid,
 * Shadow Catacombs, Sky Tower.
 */
enum class DungeonThemeType {
    FOREST_TEMPLE,
    CRYSTAL_CAVERNS,
    DESERT_PYRAMID,
    SHADOW_CATACOMBS,
    SKY_TOWER,
    COUNT
};

/**
 * Describes a dungeon theme with its properties.
 */
struct DungeonTheme {
    std::string name;
    std::string description;
    DungeonThemeType type;
    int difficultyTier;       // 1-5 difficulty rating
    int recommendedLevel;     // Minimum combat skill recommended
    int minRooms;             // Minimum room count for generation
    int maxRooms;             // Maximum room count for generation
    int lootTier;             // 1-5 loot quality rating
    int bossHP;               // Hit points for the theme's boss
    int bossDamage;           // Damage dealt by the theme's boss
};

/**
 * Manages dungeon themes and provides theme data for dungeon generation.
 */
class DungeonThemeManager {
public:
    DungeonThemeManager();

    // Access themes
    const DungeonTheme* GetTheme(DungeonThemeType type) const;
    const DungeonTheme* GetThemeByIndex(int index) const;
    int GetThemeCount() const { return static_cast<int>(m_themes.size()); }

    // Get theme name
    static std::string GetThemeName(DungeonThemeType type);

    // Query
    std::vector<const DungeonTheme*> GetThemesForLevel(int combatLevel) const;
    const DungeonTheme* GetHardestAccessible(int combatLevel) const;

private:
    std::vector<DungeonTheme> m_themes;
    void InitThemes();
};

#endif // DUNGEON_THEME_H
