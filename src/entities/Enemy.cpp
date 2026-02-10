#include "Enemy.h"
#include "../engine/Renderer.h"

Enemy::Enemy()
    : Entity()
    , m_health(3)
    , m_speed(50.0f)
{
}

void Enemy::Update(float deltaTime) {
    // TODO: Implement enemy AI
}

void Enemy::Render(Renderer* renderer) {
    if (!m_active) return;
    renderer->FillRect(
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height),
        200, 50, 50
    );
}
