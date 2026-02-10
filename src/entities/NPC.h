#ifndef NPC_H
#define NPC_H

#include "Entity.h"
#include <string>

class NPC : public Entity {
public:
    NPC();
    ~NPC() override = default;

    void Update(float deltaTime) override;
    void Render(Renderer* renderer) override;

    void SetName(const std::string& name) { m_name = name; }
    const std::string& GetName() const { return m_name; }

private:
    std::string m_name;
    int m_friendshipLevel;
};

#endif // NPC_H
