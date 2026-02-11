#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy();
    ~Enemy() override = default;

    void Update(float deltaTime) override;
    void Render(Renderer* renderer) override;

    void SetHealth(int health) { m_health = health; }
    int GetHealth() const { return m_health; }

    void SetTarget(float x, float y) { m_targetX = x; m_targetY = y; }

    enum class AIState { IDLE, PATROL, CHASE };
    void SetAIState(AIState state) { m_aiState = state; }
    AIState GetAIState() const { return m_aiState; }

private:
    int m_health;
    float m_speed;
    AIState m_aiState;

    // Patrol state
    float m_patrolOriginX, m_patrolOriginY;
    float m_patrolTargetX, m_patrolTargetY;
    float m_patrolTimer;
    static constexpr float PATROL_RADIUS = 64.0f;
    static constexpr float PATROL_INTERVAL = 3.0f;

    // Chase state
    float m_targetX, m_targetY;
    static constexpr float CHASE_RANGE = 150.0f;
};

#endif // ENEMY_H
