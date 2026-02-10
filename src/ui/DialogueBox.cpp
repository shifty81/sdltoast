#include "DialogueBox.h"
#include "../engine/Renderer.h"

void DialogueBox::Show(const std::string& text) {
    m_text = text;
    m_visible = true;
}

void DialogueBox::Hide() {
    m_visible = false;
    m_text.clear();
}

void DialogueBox::Render(Renderer* renderer) {
    if (!m_visible) return;
    
    // Draw dialogue box background
    renderer->FillRect(50, 400, 700, 150, 0, 0, 0, 200);
    renderer->DrawRect(50, 400, 700, 150, 255, 255, 255);
    
    // TODO: Render text (requires SDL_ttf integration)
}
