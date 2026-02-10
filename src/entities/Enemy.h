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

private:
    int m_health;
    float m_speed;
};

#endif // ENEMY_H
