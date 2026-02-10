#include "Combat.h"

int Combat::CalculateDamage(int attackPower, int defense) {
    int damage = attackPower - defense;
    return damage > 0 ? damage : 0;
}
