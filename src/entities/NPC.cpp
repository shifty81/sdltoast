#include "NPC.h"
#include "../engine/Renderer.h"

NPC::NPC()
    : Entity()
    , m_name("NPC")
    , m_friendshipLevel(0)
{
}

void NPC::Update(float deltaTime) {
    // TODO: Implement NPC behavior
}

void NPC::Render(Renderer* renderer) {
    if (!m_active) return;
    renderer->FillRect(
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height),
        100, 200, 100
    );
}
