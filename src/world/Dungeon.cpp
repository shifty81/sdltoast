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

void Dungeon::LockDoors() {
    m_doorsLocked = true;
    // TODO: Find all door tiles and change them to locked state
}

void Dungeon::UnlockDoors() {
    m_doorsLocked = false;
    // TODO: Find all door tiles and change them to unlocked state
}
