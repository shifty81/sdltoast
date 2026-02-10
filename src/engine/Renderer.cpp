#include "Renderer.h"
#include <iostream>

Renderer::Renderer()
    : m_renderer(nullptr)
    , m_cameraX(0)
    , m_cameraY(0)
{
}

Renderer::~Renderer() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
}

bool Renderer::Initialize(SDL_Window* window) {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Enable alpha blending
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    
    return true;
}

void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Rect rect = { x - m_cameraX, y - m_cameraY, w, h };
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderDrawRect(m_renderer, &rect);
}

void Renderer::FillRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Rect rect = { x - m_cameraX, y - m_cameraY, w, h };
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::DrawTexture(SDL_Texture* texture, int x, int y) {
    if (!texture) return;

    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    SDL_Rect dstRect = { x - m_cameraX, y - m_cameraY, w, h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &dstRect);
}

void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
    if (!texture) return;

    SDL_Rect adjustedDst = *dstRect;
    adjustedDst.x -= m_cameraX;
    adjustedDst.y -= m_cameraY;
    SDL_RenderCopy(m_renderer, texture, srcRect, &adjustedDst);
}
