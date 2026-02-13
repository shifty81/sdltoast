#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>
#include <string>

/**
 * Rendering system for 2D graphics
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize(int width, int height);
    void Clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
    void Present();

    // Drawing functions
    void DrawRect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void FillRect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void DrawTextureRect(Texture2D texture, int x, int y);
    void DrawTextureRect(Texture2D texture, const Rectangle* srcRect, const Rectangle* dstRect);

    void SetCamera(int x, int y) { m_cameraX = x; m_cameraY = y; }
    void GetCamera(int& x, int& y) const { x = m_cameraX; y = m_cameraY; }

private:
    int m_cameraX, m_cameraY;
    int m_width, m_height;
};

#endif // RENDERER_H
