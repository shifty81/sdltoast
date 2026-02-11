#include "AssetManager.h"
#include "Renderer.h"
#include "Logger.h"
#include <SDL2/SDL_image.h>
#include <iostream>

AssetManager::~AssetManager() {
    for (auto& pair : m_textures) {
        SDL_DestroyTexture(pair.second);
    }
    m_textures.clear();
}

bool AssetManager::Initialize(Renderer* renderer) {
    m_renderer = renderer;
    
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        Logger::Instance().Error(std::string("SDL_image initialization failed: ") + IMG_GetError());
        return false;
    }
    
    return true;
}

SDL_Texture* AssetManager::LoadTexture(const std::string& filepath) {
    auto it = m_textures.find(filepath);
    if (it != m_textures.end()) {
        return it->second;
    }

    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface) {
        Logger::Instance().Error("Failed to load image " + filepath + ": " + IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer->GetSDLRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        Logger::Instance().Error("Failed to create texture from " + filepath + ": " + SDL_GetError());
        return nullptr;
    }

    m_textures[filepath] = texture;
    return texture;
}

SDL_Texture* AssetManager::GetTexture(const std::string& id) const {
    auto it = m_textures.find(id);
    return (it != m_textures.end()) ? it->second : nullptr;
}
