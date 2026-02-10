#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>

/**
 * Rendering system for 2D graphics
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize(SDL_Window* window);
    void Clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);
    void Present();

    // Drawing functions
    void DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void FillRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void DrawTexture(SDL_Texture* texture, int x, int y);
    void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect);

    SDL_Renderer* GetSDLRenderer() const { return m_renderer; }
    
    void SetCamera(int x, int y) { m_cameraX = x; m_cameraY = y; }
    void GetCamera(int& x, int& y) const { x = m_cameraX; y = m_cameraY; }

private:
    SDL_Renderer* m_renderer;
    int m_cameraX, m_cameraY;
};

#endif // RENDERER_H
