#ifndef TILE_H
#define TILE_H

class Tile {
public:
    enum Type {
        GRASS,
        DIRT,
        WATER,
        STONE,
        SAND
    };

    Tile(Type type = GRASS) : m_type(type), m_walkable(true) {}
    
    Type GetType() const { return m_type; }
    bool IsWalkable() const { return m_walkable; }

private:
    Type m_type;
    bool m_walkable;
};

#endif // TILE_H
