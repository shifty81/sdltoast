#include "Map.h"
#include "../engine/Renderer.h"
#include "../engine/SpriteSheet.h"
#include <iostream>

Map::Map()
    : m_width(25)
    , m_height(19)
{
    m_tiles.resize(m_width * m_height, Tile(TileType::GRASS, 0));
}

Map::Map(int width, int height)
    : m_width(width)
    , m_height(height)
{
    m_tiles.resize(m_width * m_height, Tile(TileType::GRASS, 0));
}

Map::~Map() {
}

bool Map::LoadFromFile(const std::string& filepath) {
    // TODO: Implement map loading from JSON
    std::cout << "Map loading not yet implemented. Using procedural generation." << std::endl;
    return true;
}

void Map::Update(float deltaTime) {
    // Update all tiles (for animations, crop growth, etc.)
    for (auto& tile : m_tiles) {
        tile.Update(deltaTime);
    }
}

void Map::Render(Renderer* renderer) {
    // Try to use sprite sheets if available
    SpriteSheet* worldTiles = SpriteSheetManager::Instance().GetSpriteSheet("world_tiles");
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const Tile* tile = GetTileAt(x, y);
            if (!tile) continue;
            
            int screenX = x * TILE_SIZE;
            int screenY = y * TILE_SIZE;
            
            // Use sprite sheet if loaded
            if (worldTiles && worldTiles->IsLoaded()) {
                // Map tile type to sprite sheet tile ID
                int tileId = GetTileSpriteId(tile);
                worldTiles->RenderTile(renderer, tileId, screenX, screenY, TILE_SIZE, TILE_SIZE);
            } else {
                // Fallback: Render colored rectangles (current method)
                RenderTileFallback(renderer, tile, screenX, screenY);
            }
        }
    }
}

void Map::RenderTileFallback(Renderer* renderer, const Tile* tile, int screenX, int screenY) {
    unsigned char r = 50, g = 100, b = 50;  // Default grass
    
    switch (tile->GetType()) {
        case TileType::VOID:
            r = 20; g = 20; b = 20;
            break;
        case TileType::FLOOR:
            r = 80; g = 80; b = 80;
            break;
        case TileType::WALL:
            r = 60; g = 60; b = 70;
            break;
        case TileType::DOOR:
            r = 100; g = 60; b = 30;
            break;
        case TileType::WATER:
            r = 50; g = 100; b = 200;
            break;
        case TileType::SOIL:
            r = 90; g = 60; b = 30;
            break;
        case TileType::CROP:
            r = 50; g = 150; b = 50;
            break;
        case TileType::GRASS:
            r = 50; g = ((screenX/TILE_SIZE + screenY/TILE_SIZE) % 2 == 0) ? 100 : 120; b = 50;
            break;
        case TileType::DIRT:
            r = 120; g = 90; b = 60;
            break;
        case TileType::STONE:
            r = 100; g = 100; b = 110;
            break;
        case TileType::SAND:
            r = 194; g = 178; b = 128;
            break;
        case TileType::DECORATION:
            r = 30; g = 140; b = 30;
            break;
        case TileType::TREE:
            r = 20; g = 80; b = 20;
            break;
    }
    
    renderer->FillRect(screenX, screenY, TILE_SIZE, TILE_SIZE, r, g, b);
    renderer->DrawRect(screenX, screenY, TILE_SIZE, TILE_SIZE, r/2, g/2, b/2);

    // Draw tree trunk and canopy on top of base tile
    if (tile->GetType() == TileType::TREE) {
        // Brown trunk (center bottom)
        renderer->FillRect(screenX + 12, screenY + 18, 8, 14, 100, 60, 20);
        // Green canopy (top area, kept within tile bounds)
        renderer->FillRect(screenX + 4, screenY + 2, 24, 18, 30, 140, 30);
        renderer->FillRect(screenX + 8, screenY + 0, 16, 6, 40, 160, 40);
    }
}

int Map::GetTileSpriteId(const Tile* tile) const {
    // Map semantic tile types to sprite sheet tile IDs
    // These IDs depend on your tileset layout
    // Example mapping (adjust based on actual tileset):
    
    switch (tile->GetType()) {
        case TileType::GRASS:      return 0;   // First tile in sheet
        case TileType::DIRT:       return 1;
        case TileType::SOIL:       return 2;
        case TileType::WATER:      return 3;
        case TileType::STONE:      return 4;
        case TileType::SAND:       return 5;
        case TileType::FLOOR:      return 6;
        case TileType::WALL:       return 7 + tile->GetVisualId();  // Use auto-tile ID
        case TileType::DOOR:       return 20;
        case TileType::CROP:       return 30 + tile->GetGrowthStage();
        case TileType::DECORATION: return 40 + tile->GetVisualId();
        case TileType::TREE:       return 50;
        default:                   return 0;
    }
}

Tile* Map::GetTileAt(int x, int y) {
    if (!IsValidPosition(x, y)) return nullptr;
    return &m_tiles[GetIndex(x, y)];
}

const Tile* Map::GetTileAt(int x, int y) const {
    if (!IsValidPosition(x, y)) return nullptr;
    return &m_tiles[GetIndex(x, y)];
}

void Map::SetTile(int x, int y, const Tile& tile) {
    if (IsValidPosition(x, y)) {
        m_tiles[GetIndex(x, y)] = tile;
    }
}

bool Map::IsSolid(int x, int y) const {
    const Tile* tile = GetTileAt(x, y);
    return tile ? tile->IsSolid() : true;
}

bool Map::IsAreaSolid(float worldX, float worldY, float width, float height) const {
    int tlX, tlY, trX, trY, blX, blY, brX, brY;
    WorldToTile(worldX, worldY, tlX, tlY);
    WorldToTile(worldX + width - 1, worldY, trX, trY);
    WorldToTile(worldX, worldY + height - 1, blX, blY);
    WorldToTile(worldX + width - 1, worldY + height - 1, brX, brY);
    return IsSolid(tlX, tlY) || IsSolid(trX, trY) ||
           IsSolid(blX, blY) || IsSolid(brX, brY);
}

bool Map::CanPlantCrop(int x, int y) const {
    const Tile* tile = GetTileAt(x, y);
    return tile ? tile->IsFarmable() : false;
}

bool Map::TillSoil(int x, int y) {
    Tile* tile = GetTileAt(x, y);
    if (!tile || tile->GetType() != TileType::GRASS) return false;
    
    tile->SetType(TileType::SOIL);
    tile->SetSoilState(SoilState::HOE);
    return true;
}

bool Map::WaterTile(int x, int y) {
    Tile* tile = GetTileAt(x, y);
    if (!tile || !tile->IsFarmable()) return false;
    
    if (tile->GetSoilState() == SoilState::HOE) {
        tile->SetSoilState(SoilState::WATERED);
        return true;
    }
    return false;
}

bool Map::PlantCrop(int x, int y, int cropType) {
    Tile* tile = GetTileAt(x, y);
    if (!tile || !tile->IsFarmable()) return false;
    
    if (tile->GetSoilState() == SoilState::HOE || 
        tile->GetSoilState() == SoilState::WATERED) {
        tile->SetCropType(cropType);
        tile->SetGrowthStage(0);
        tile->SetSoilState(SoilState::CROP);
        return true;
    }
    return false;
}

bool Map::HarvestCrop(int x, int y) {
    Tile* tile = GetTileAt(x, y);
    if (!tile || tile->GetSoilState() != SoilState::HARVEST) return false;
    
    // Reset to tilled soil
    tile->SetSoilState(SoilState::HOE);
    tile->SetCropType(-1);
    tile->SetGrowthStage(0);
    return true;
}

bool Map::ChopTree(int x, int y) {
    Tile* tile = GetTileAt(x, y);
    if (!tile || tile->GetType() != TileType::TREE) return false;

    // Replace tree with grass stump
    tile->SetType(TileType::GRASS);
    tile->SetVisualId(0);
    return true;
}

void Map::WorldToTile(float worldX, float worldY, int& tileX, int& tileY) const {
    tileX = static_cast<int>(worldX / TILE_SIZE);
    tileY = static_cast<int>(worldY / TILE_SIZE);
}

void Map::TileToWorld(int tileX, int tileY, float& worldX, float& worldY) const {
    worldX = static_cast<float>(tileX * TILE_SIZE);
    worldY = static_cast<float>(tileY * TILE_SIZE);
}

int Map::GetIndex(int x, int y) const {
    return y * m_width + x;
}

bool Map::IsValidPosition(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}
