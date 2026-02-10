#include "Map.h"
#include "../engine/Renderer.h"
#include <iostream>

Map::Map()
    : m_width(25)
    , m_height(19)
{
    // Create a simple test map
    m_tiles.resize(m_width * m_height, 0);
}

Map::~Map() {
}

bool Map::LoadFromFile(const std::string& filepath) {
    // TODO: Implement map loading from file
    std::cout << "Map loading not yet implemented. Using default map." << std::endl;
    return true;
}

void Map::Update(float deltaTime) {
    // TODO: Update animated tiles, etc.
}

void Map::Render(Renderer* renderer) {
    // Render a simple grid for now
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int tileType = m_tiles[y * m_width + x];
            
            // Checkerboard pattern
            Uint8 g = ((x + y) % 2 == 0) ? 100 : 120;
            renderer->FillRect(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                50, g, 50
            );
            
            // Draw grid lines
            renderer->DrawRect(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                30, 80, 30
            );
        }
    }
}
