#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL.h>
#include <string>
#include <unordered_map>

class Renderer;

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    bool Initialize(Renderer* renderer);
    
    SDL_Texture* LoadTexture(const std::string& filepath);
    SDL_Texture* GetTexture(const std::string& id) const;

private:
    Renderer* m_renderer;
    std::unordered_map<std::string, SDL_Texture*> m_textures;
};

#endif // ASSETMANAGER_H
