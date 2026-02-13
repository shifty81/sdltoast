#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <raylib.h>
#include <string>
#include <unordered_map>

class Renderer;

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    bool Initialize(Renderer* renderer);
    
    Texture2D LoadTexture(const std::string& filepath);
    Texture2D GetTexture(const std::string& id) const;

private:
    Renderer* m_renderer;
    std::unordered_map<std::string, Texture2D> m_textures;
};

#endif // ASSETMANAGER_H
