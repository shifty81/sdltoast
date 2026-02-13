#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "Tile.h"
#include <vector>
#include <random>

class Map;

// Biome types for world generation
enum class Biome {
    PLAINS,
    FOREST,
    DESERT,
    MOUNTAINS,
    BEACH
};

// Area types within biomes
enum class AreaType {
    FARM,
    VILLAGE,
    DUNGEON,
    WILDERNESS,
    WATER
};

/**
 * World Generation Pipeline:
 * 1. Seed → Random generation
 * 2. Biome/Area Type → High-level structure
 * 3. Logical Layout → Rooms, fields, paths
 * 4. Collision & Rules Layer → Semantic tiles
 * 5. Tile Painting → Auto-tiling for visuals
 * 6. Decoration Pass → Optional detail
 */
class WorldGenerator {
public:
    explicit WorldGenerator(unsigned int seed = 0);
    
    // Generate different world types
    void GenerateFarm(Map* map, int width, int height);
    void GenerateDungeon(Map* map, int width, int height);
    void GenerateOverworld(Map* map, int width, int height, Biome biome);
    
    void SetSeed(unsigned int seed);

private:
    // === STEP 1: Layout Generation (Logical) ===
    struct Room {
        int x, y;
        int width, height;
        bool isBoss;
        bool isStart;
        bool hasTreasure;
    };
    
    struct Corridor {
        int x1, y1, x2, y2;
    };
    
    std::vector<Room> GenerateRooms(int mapWidth, int mapHeight);
    std::vector<Corridor> ConnectRooms(const std::vector<Room>& rooms);
    void CarveRoom(Map* map, const Room& room);
    void CarveCorridor(Map* map, const Corridor& corridor);
    
    // === STEP 2: Farm Generation ===
    struct FarmZone {
        int x, y, width, height;
        enum Type { SOIL, WATER, BUILDING, PATH } type;
    };
    
    std::vector<FarmZone> GenerateFarmZones(int width, int height);
    void ApplyFarmZone(Map* map, const FarmZone& zone);
    
    // === STEP 3: Overworld with Noise ===
    float Noise2D(int x, int y, int octaves = 4);
    TileType DetermineTileFromNoise(float height, float moisture);
    
    // === STEP 4: Auto-Tiling (Visual) ===
    int GetWallAutoTile(bool north, bool south, bool east, bool west,
                        bool nw, bool ne, bool sw, bool se);
    void ApplyAutoTiling(Map* map);
    
    // === STEP 5: Decoration ===
    void AddDecorations(Map* map, float density = 0.1f);
    void AddTrees(Map* map, float density = 0.1f);
    
    // Utilities
    bool RoomsOverlap(const Room& a, const Room& b);
    int Random(int min, int max);
    float RandomFloat(float min = 0.0f, float max = 1.0f);
    
    unsigned int m_seed;
    std::mt19937 m_rng;
};

#endif // WORLDGENERATOR_H