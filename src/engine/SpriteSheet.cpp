#include "SpriteSheet.h"
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include <iostream>

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
    
    // Load world tileset (16x16 tiles)
    LoadSpriteSheet(renderer, "world_tiles", "assets/tilesets/world_tileset.png", 16, 16);
    
    // Load character sprite sheet (16x16 or 32x32)
    LoadSpriteSheet(renderer, "characters", "assets/sprites/character_tileset.png", 16, 16);
    
    // Load additional tilesets as needed
    LoadSpriteSheet(renderer, "dungeon_tiles", "assets/tilesets/dungeon_tileset.png", 16, 16);
    LoadSpriteSheet(renderer, "farm_tiles", "assets/tilesets/farm_tileset.png", 16, 16);
    
    std::cout << "=== Sprite Sheets Loaded ===" << std::endl;
}

void SpriteSheetManager::Clear() {
    for (auto& pair : m_sheets) {
        delete pair.second;
    }
    m_sheets.clear();
}