#ifndef MAP_H
#define MAP_H

#include "Tile.h"
#include <vector>
#include <string>

class Renderer;

/**
 * Map class represents a tile-based world
 * Uses the smart data approach - tiles have meaning, not just visuals
 */
class Map {
public:
    Map();
    Map(int width, int height);
    ~Map();

    bool LoadFromFile(const std::string& filepath);
    void Update(float deltaTime);
    void Render(Renderer* renderer);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    
    // Tile access
    Tile* GetTileAt(int x, int y);
    const Tile* GetTileAt(int x, int y) const;
    void SetTile(int x, int y, const Tile& tile);
    
    // Check tile properties at position
    bool IsSolid(int x, int y) const;
    bool IsAreaSolid(float worldX, float worldY, float width, float height) const;
    bool CanPlantCrop(int x, int y) const;
    
    // Farming interactions
    bool TillSoil(int x, int y);
    bool WaterTile(int x, int y);
    bool PlantCrop(int x, int y, int cropType);
    bool HarvestCrop(int x, int y);
    
    // World position to tile coordinates
    void WorldToTile(float worldX, float worldY, int& tileX, int& tileY) const;
    void TileToWorld(int tileX, int tileY, float& worldX, float& worldY) const;

private:
    int m_width, m_height;
    std::vector<Tile> m_tiles;
    static constexpr int TILE_SIZE = 32;
    
    int GetIndex(int x, int y) const;
    bool IsValidPosition(int x, int y) const;
    
    // Helper methods for rendering
    void RenderTileFallback(Renderer* renderer, const Tile* tile, int screenX, int screenY);
    int GetTileSpriteId(const Tile* tile) const;
};

#endif // MAP_H
