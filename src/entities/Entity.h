#ifndef ENTITY_H
#define ENTITY_H

class Renderer;

class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(Renderer* renderer) = 0;

    void SetPosition(float x, float y) { m_x = x; m_y = y; }
    void GetPosition(float& x, float& y) const { x = m_x; y = m_y; }
    
    void SetSize(float w, float h) { m_width = w; m_height = h; }
    void GetSize(float& w, float& h) const { w = m_width; h = m_height; }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

protected:
    float m_x, m_y;
    float m_width, m_height;
    bool m_active;
};

#endif // ENTITY_H
