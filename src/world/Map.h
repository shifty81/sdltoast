#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Renderer;
class Tile;

class Map {
public:
    Map();
    ~Map();

    bool LoadFromFile(const std::string& filepath);
    void Update(float deltaTime);
    void Render(Renderer* renderer);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width, m_height;
    std::vector<int> m_tiles;
    static constexpr int TILE_SIZE = 32;
};

#endif // MAP_H
