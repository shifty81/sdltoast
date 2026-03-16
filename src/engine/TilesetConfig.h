#ifndef TILESETCONFIG_H
#define TILESETCONFIG_H

#include <string>
#include <unordered_map>

// Forward declarations
enum class TileType;
enum class Season;

/**
 * TilesetConfig — Configurable mapping from semantic tile types to sprite IDs.
 *
 * Supports per-season tile variants so a single tileset (like "The Seasons
 * Collective") can render different visuals for Spring grass vs Winter grass.
 *
 * Usage:
 *   TilesetConfig cfg;
 *   cfg.LoadFromFile("assets/tilesets/seasons.cfg");
 *   int spriteId = cfg.GetSpriteId(TileType::GRASS, Season::WINTER);
 *
 * Config file format (one mapping per line):
 *   # comment
 *   TILE_SIZE 16
 *   SHEET_FILE assets/tilesets/world_tileset.png
 *   GRASS 0
 *   GRASS.SPRING 0
 *   GRASS.SUMMER 16
 *   GRASS.FALL 32
 *   GRASS.WINTER 48
 *   WALL 7
 *   WALL.AUTOTILE_BASE 7
 *   CROP.GROWTH_BASE 30
 *   WATER.ANIM_FRAMES 3 21 22 23
 */
class TilesetConfig {
public:
    TilesetConfig();

    // Load mapping from a config file. Returns true on success.
    bool LoadFromFile(const std::string& filepath);

    // Initialize with built-in default mapping (matches existing world_tileset.png)
    void LoadDefaults();

    // Get the sprite ID for a tile type, optionally adjusted for the current season.
    // Falls back to the base (non-seasonal) ID if no seasonal override exists.
    int GetSpriteId(TileType type, Season season) const;
    int GetSpriteId(TileType type) const;

    // Auto-tiling: base ID for walls (auto-tile offset is added by caller)
    int GetWallAutoTileBase() const { return m_wallAutoTileBase; }

    // Crop growth: base ID (growth stage is added as offset)
    int GetCropGrowthBase() const { return m_cropGrowthBase; }

    // Decoration: base ID (visual variant is added as offset)
    int GetDecorationBase() const { return m_decorationBase; }

    // Tree: base ID
    int GetTreeBase() const { return m_treeBase; }

    // Water animation frame IDs
    int GetWaterFrame(int frameIndex) const;
    int GetWaterFrameCount() const { return m_waterFrameCount; }

    // Tileset metadata
    int GetTileSize() const { return m_tileSize; }
    const std::string& GetSheetFile() const { return m_sheetFile; }

    // Check if seasonal variants are configured
    bool HasSeasonalSupport() const { return m_hasSeasonal; }

private:
    // Key for the seasonal lookup: combines TileType + Season
    struct SeasonKey {
        int tileType;
        int season;
        bool operator==(const SeasonKey& other) const {
            return tileType == other.tileType && season == other.season;
        }
    };

    struct SeasonKeyHash {
        size_t operator()(const SeasonKey& k) const {
            return std::hash<int>()(k.tileType) ^ (std::hash<int>()(k.season) << 16);
        }
    };

    // Base sprite IDs (no season)
    std::unordered_map<int, int> m_baseIds;

    // Seasonal overrides: (TileType, Season) → sprite ID
    std::unordered_map<SeasonKey, int, SeasonKeyHash> m_seasonalIds;

    // Special tile ranges
    int m_wallAutoTileBase;
    int m_cropGrowthBase;
    int m_decorationBase;
    int m_treeBase;

    // Water animation
    static constexpr int MAX_WATER_FRAMES = 8;
    int m_waterFrames[MAX_WATER_FRAMES];
    int m_waterFrameCount;

    // Metadata
    int m_tileSize;
    std::string m_sheetFile;
    bool m_hasSeasonal;

    // Helpers
    static int ParseTileType(const std::string& name);
    static int ParseSeason(const std::string& name);
};

#endif // TILESETCONFIG_H
