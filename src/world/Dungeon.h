#ifndef DUNGEON_H
#define DUNGEON_H

#include "Map.h"
#include <memory>
#include <vector>
#include <utility>

/**
 * Dungeon class - represents a procedurally generated dungeon
 * Uses WorldGenerator for the actual generation
 */
class Dungeon {
public:
    Dungeon(int width = 50, int height = 50, unsigned int seed = 0);
    ~Dungeon() = default;
    
    void Generate();
    void Update(float deltaTime);
    void Render(Renderer* renderer);
    
    Map* GetMap() { return m_map.get(); }
    
    // Room combat lock (Zelda-style)
    void LockDoors();
    void UnlockDoors();
    
private:
    std::unique_ptr<Map> m_map;
    int m_width, m_height;
    unsigned int m_seed;
    bool m_doorsLocked;
    int m_enemiesRemaining;
    std::vector<std::pair<int,int>> m_doorPositions;

    void FindDoorPositions();
};

#endif // DUNGEON_H
