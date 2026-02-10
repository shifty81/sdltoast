#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class Renderer;

/**
 * SpriteSheet - Manages tileset/sprite sheet loading and rendering
 * Supports both world tilesets and character sprite sheets
 */
class SpriteSheet {
public:
    SpriteSheet();
    ~SpriteSheet();

    // Load a sprite sheet from file
    bool Load(Renderer* renderer, const std::string& filepath, int tileWidth, int tileHeight);
    
    // Render a specific tile/sprite from the sheet
    void RenderTile(Renderer* renderer, int tileId, int x, int y, bool flipH = false, bool flipV = false);
    void RenderTile(Renderer* renderer, int tileId, int x, int y, int width, int height, bool flipH = false, bool flipV = false);
    
    // Get tile dimensions
    int GetTileWidth() const { return m_tileWidth; }
    int GetTileHeight() const { return m_tileHeight; }
    int GetColumns() const { return m_columns; }
    int GetRows() const { return m_rows; }
    int GetTileCount() const { return m_columns * m_rows; }
    
    // Check if loaded
    bool IsLoaded() const { return m_texture != nullptr; }

private:
    SDL_Texture* m_texture;
    int m_tileWidth, m_tileHeight;
    int m_columns, m_rows;
    int m_sheetWidth, m_sheetHeight;
    
    void CalculateSourceRect(int tileId, SDL_Rect& srcRect);
};

/**
 * SpriteSheetManager - Global manager for all sprite sheets
 * Caches loaded sheets for reuse
 */
class SpriteSheetManager {
public:
    static SpriteSheetManager& Instance();
    
    // Load and cache a sprite sheet
    SpriteSheet* LoadSpriteSheet(Renderer* renderer, const std::string& name, 
                                 const std::string& filepath, int tileWidth, int tileHeight);
    
    // Get a cached sprite sheet
    SpriteSheet* GetSpriteSheet(const std::string& name);
    
    // Preload common sprite sheets
    void LoadDefaultAssets(Renderer* renderer);
    
    // Cleanup
    void Clear();

private:
    SpriteSheetManager() = default;
    ~SpriteSheetManager();
    
    std::unordered_map<std::string, SpriteSheet*> m_sheets;
};

#endif // SPRITESHEET_H