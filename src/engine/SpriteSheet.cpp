#include "SpriteSheet.h"
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <vector>

// For directory reading
#ifndef _WIN32
#include <dirent.h>
#endif

// ============================================================================
// SpriteSheet Implementation
// ============================================================================

SpriteSheet::SpriteSheet()
    : m_texture(nullptr)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_columns(0)
    , m_rows(0)
    , m_sheetWidth(0)
    , m_sheetHeight(0)
{
}

SpriteSheet::~SpriteSheet() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

bool SpriteSheet::Load(Renderer* renderer, const std::string& filepath, int tileWidth, int tileHeight) {
    // Load the image
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface) {
        std::cerr << "Failed to load sprite sheet " << filepath << ": " << IMG_GetError() << std::endl;
        return false;
    }

    m_sheetWidth = surface->w;
    m_sheetHeight = surface->h;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    m_columns = m_sheetWidth / m_tileWidth;
    m_rows = m_sheetHeight / m_tileHeight;

    // Create texture
    m_texture = SDL_CreateTextureFromSurface(renderer->GetSDLRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        std::cerr << "Failed to create texture from " << filepath << ": " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Loaded sprite sheet: " << filepath << std::endl;
    std::cout << "  Size: " << m_sheetWidth << "x" << m_sheetHeight << std::endl;
    std::cout << "  Tiles: " << m_columns << "x" << m_rows << " (" << GetTileCount() << " total)" << std::endl;
    std::cout << "  Tile size: " << m_tileWidth << "x" << m_tileHeight << std::endl;

    return true;
}

void SpriteSheet::RenderTile(Renderer* renderer, int tileId, int x, int y, bool flipH, bool flipV) {
    RenderTile(renderer, tileId, x, y, m_tileWidth, m_tileHeight, flipH, flipV);
}

void SpriteSheet::RenderTile(Renderer* renderer, int tileId, int x, int y, int width, int height, bool flipH, bool flipV) {
    if (!m_texture) return;

    SDL_Rect srcRect;
    CalculateSourceRect(tileId, srcRect);

    SDL_Rect dstRect = { x, y, width, height };
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipH) flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
    if (flipV) flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

    SDL_RenderCopyEx(renderer->GetSDLRenderer(), m_texture, &srcRect, &dstRect, 0.0, nullptr, flip);
}

void SpriteSheet::CalculateSourceRect(int tileId, SDL_Rect& srcRect) {
    int col = tileId % m_columns;
    int row = tileId / m_columns;
    
    srcRect.x = col * m_tileWidth;
    srcRect.y = row * m_tileHeight;
    srcRect.w = m_tileWidth;
    srcRect.h = m_tileHeight;
}

// ============================================================================
// SpriteSheetManager Implementation
// ============================================================================

SpriteSheetManager& SpriteSheetManager::Instance() {
    static SpriteSheetManager instance;
    return instance;
}

SpriteSheetManager::~SpriteSheetManager() {
    Clear();
}

SpriteSheet* SpriteSheetManager::LoadSpriteSheet(Renderer* renderer, const std::string& name, 
                                                  const std::string& filepath, int tileWidth, int tileHeight) {
    // Check if already loaded
    auto it = m_sheets.find(name);
    if (it != m_sheets.end()) {
        return it->second;
    }

    // Load new sprite sheet
    SpriteSheet* sheet = new SpriteSheet();
    if (sheet->Load(renderer, filepath, tileWidth, tileHeight)) {
        m_sheets[name] = sheet;
        return sheet;
    } else {
        delete sheet;
        return nullptr;
    }
}

SpriteSheet* SpriteSheetManager::GetSpriteSheet(const std::string& name) {
    auto it = m_sheets.find(name);
    return (it != m_sheets.end()) ? it->second : nullptr;
}

void SpriteSheetManager::LoadDefaultAssets(Renderer* renderer) {
    std::cout << "\n=== Loading Sprite Sheets ===" << std::endl;
    
    // Check root directory for PNG files first (user convenience)
    std::cout << "Checking root directory for PNG files..." << std::endl;
    bool foundRootPngs = CheckAndLoadRootPngs(renderer);
    if (foundRootPngs) {
        std::cout << "Loaded tilesets from root directory!" << std::endl;
    }
    
    // Load world tileset (16x16 tiles)
    if (!GetSpriteSheet("world_tiles")) {
        LoadSpriteSheet(renderer, "world_tiles", "assets/tilesets/world_tileset.png", 16, 16);
    }
    
    // Load character sprite sheet (16x16 or 32x32)
    if (!GetSpriteSheet("characters")) {
        LoadSpriteSheet(renderer, "characters", "assets/sprites/character_tileset.png", 16, 16);
    }
    
    // Load additional tilesets as needed
    LoadSpriteSheet(renderer, "dungeon_tiles", "assets/tilesets/dungeon_tileset.png", 16, 16);
    LoadSpriteSheet(renderer, "farm_tiles", "assets/tilesets/farm_tileset.png", 16, 16);
    
    std::cout << "=== Sprite Sheets Loaded ===" << std::endl;
}

bool SpriteSheetManager::CheckAndLoadRootPngs(Renderer* renderer) {
    // Common tileset filenames to check in root directory
    std::vector<std::string> tilesetNames = {
        "world_tileset.png", "tiles.png", "tileset.png", "world.png",
        "terrain.png", "map.png"
    };
    
    std::vector<std::string> characterNames = {
        "character_tileset.png", "characters.png", "char.png", "sprites.png",
        "player.png", "hero.png"
    };
    
    bool loadedAny = false;
    
    // Try to load world tileset from root
    for (const auto& name : tilesetNames) {
        if (LoadSpriteSheet(renderer, "world_tiles", name, 16, 16)) {
            std::cout << "  ✓ Loaded world tiles from: " << name << std::endl;
            loadedAny = true;
            break;
        }
    }
    
    // Try to load character sprites from root
    for (const auto& name : characterNames) {
        if (LoadSpriteSheet(renderer, "characters", name, 16, 16)) {
            std::cout << "  ✓ Loaded character sprites from: " << name << std::endl;
            loadedAny = true;
            break;
        }
    }
    
    // If exactly 2 PNGs in root, try them in order
    // (This handles the case where user just drops 2 PNGs with any names)
    if (!loadedAny) {
        std::vector<std::string> rootPngs;
        // Check for any .png files in current directory
        #ifdef _WIN32
        // Windows version would use different API
        #else
        // Unix-like systems
        DIR* dir = opendir(".");
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                std::string filename = entry->d_name;
                if (filename.length() > 4 && 
                    filename.substr(filename.length() - 4) == ".png") {
                    rootPngs.push_back(filename);
                }
            }
            closedir(dir);
        }
        #endif
        
        // Sort for consistency
        std::sort(rootPngs.begin(), rootPngs.end());
        
        if (rootPngs.size() >= 1 && !GetSpriteSheet("world_tiles")) {
            if (LoadSpriteSheet(renderer, "world_tiles", rootPngs[0], 16, 16)) {
                std::cout << "  ✓ Loaded world tiles from: " << rootPngs[0] << " (1st PNG in root)" << std::endl;
                loadedAny = true;
            }
        }
        
        if (rootPngs.size() >= 2 && !GetSpriteSheet("characters")) {
            if (LoadSpriteSheet(renderer, "characters", rootPngs[1], 16, 16)) {
                std::cout << "  ✓ Loaded character sprites from: " << rootPngs[1] << " (2nd PNG in root)" << std::endl;
                loadedAny = true;
            }
        }
    }
    
    return loadedAny;
}

void SpriteSheetManager::Clear() {
    for (auto& pair : m_sheets) {
        delete pair.second;
    }
    m_sheets.clear();
}