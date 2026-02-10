#ifndef HUD_H
#define HUD_H

class Renderer;

class HUD {
public:
    HUD() = default;
    ~HUD() = default;

    void Render(Renderer* renderer);
    
    void SetHealth(int current, int max) { m_currentHealth = current; m_maxHealth = max; }
    void SetGold(int gold) { m_gold = gold; }

private:
    int m_currentHealth = 3;
    int m_maxHealth = 3;
    int m_gold = 0;
};

#endif // HUD_H
