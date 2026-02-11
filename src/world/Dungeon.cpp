#include "Dungeon.h"
#include "WorldGenerator.h"
#include "../engine/Renderer.h"

Dungeon::Dungeon(int width, int height, unsigned int seed)
    : m_width(width)
    , m_height(height)
    , m_seed(seed)
    , m_doorsLocked(false)
    , m_enemiesRemaining(0)
{
    m_map = std::make_unique<Map>(width, height);
    Generate();
}

void Dungeon::Generate() {
    WorldGenerator generator(m_seed);
    generator.GenerateDungeon(m_map.get(), m_width, m_height);
    FindDoorPositions();
}

void Dungeon::Update(float deltaTime) {
    if (m_map) {
        m_map->Update(deltaTime);
    }
    
    // Check for room combat lock (Zelda-style)
    if (m_doorsLocked && m_enemiesRemaining <= 0) {
        UnlockDoors();
    }
}

void Dungeon::Render(Renderer* renderer) {
    if (m_map) {
        m_map->Render(renderer);
    }
}

void Dungeon::FindDoorPositions() {
    m_doorPositions.clear();
    if (!m_map) return;
    for (int y = 0; y < m_map->GetHeight(); ++y) {
        for (int x = 0; x < m_map->GetWidth(); ++x) {
            const Tile* tile = m_map->GetTileAt(x, y);
            if (tile && tile->GetType() == TileType::DOOR) {
                m_doorPositions.emplace_back(x, y);
            }
        }
    }
}

void Dungeon::LockDoors() {
    m_doorsLocked = true;
    if (!m_map) return;
    for (const auto& pos : m_doorPositions) {
        Tile* tile = m_map->GetTileAt(pos.first, pos.second);
        if (tile) {
            tile->SetType(TileType::WALL);
        }
    }
}

void Dungeon::UnlockDoors() {
    m_doorsLocked = false;
    if (!m_map) return;
    for (const auto& pos : m_doorPositions) {
        Tile* tile = m_map->GetTileAt(pos.first, pos.second);
        if (tile) {
            tile->SetType(TileType::DOOR);
        }
    }
}
