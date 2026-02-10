#include "Map.h"
#include "../engine/Renderer.h"
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
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const Tile* tile = GetTileAt(x, y);
            if (!tile) continue;
            
            // Render based on tile TYPE (semantic), not visual ID yet
            Uint8 r = 50, g = 100, b = 50;  // Default grass
            
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
                    r = 50; g = ((x + y) % 2 == 0) ? 100 : 120; b = 50;
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
            }
            
            renderer->FillRect(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                r, g, b
            );
            
            // Draw grid lines for clarity
            renderer->DrawRect(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                r/2, g/2, b/2
            );
        }
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
