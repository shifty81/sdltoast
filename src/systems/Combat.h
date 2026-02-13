#ifndef COMBAT_H
#define COMBAT_H

class Player;
class Enemy;

class Combat {
public:
    static int CalculateDamage(int attackPower, int defense);

    // Check if two rectangles overlap (AABB collision)
    static bool CheckCollision(float x1, float y1, float w1, float h1,
                               float x2, float y2, float w2, float h2);

    // Player attacks in facing direction, returns true if hit
    static bool PlayerAttack(Player* player, Enemy* enemy, int attackPower);

    // Enemy damages player on contact
    static bool EnemyContact(Enemy* enemy, Player* player, int attackPower);

    // Combat tuning constants
    static constexpr float ATTACK_RANGE = 40.0f;
    static constexpr float DAMAGE_COOLDOWN = 1.0f; // Seconds of invincibility after hit
    static constexpr int ENEMY_KILL_GOLD = 10;
};

#endif // COMBAT_H
