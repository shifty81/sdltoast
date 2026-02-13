#include "HUD.h"
#include "../engine/Renderer.h"

void HUD::Render(Renderer* renderer) {
    // Draw health hearts
    for (int i = 0; i < m_maxHealth; ++i) {
        unsigned char r = (i < m_currentHealth) ? 255 : 100;
        renderer->FillRect(10 + i * 25, 10, 20, 20, r, 50, 50);
    }
    
    // TODO: Draw gold count, energy bar, etc.
}
