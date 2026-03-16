#include "Mining.h"
#include <cstdlib>
#include <algorithm>

MiningSystem::MiningSystem() {
    InitOres();
}

void MiningSystem::InitOres() {
    // Name, value, hardness(1-10), minSkillLevel
    m_ores.push_back({"Stone",        10, 1, 0});
    m_ores.push_back({"Coal",         25, 2, 0});
    m_ores.push_back({"Copper Ore",   40, 3, 1});
    m_ores.push_back({"Iron Ore",     70, 4, 2});
    m_ores.push_back({"Gold Ore",    120, 6, 4});
    m_ores.push_back({"Ruby",        200, 7, 5});
    m_ores.push_back({"Sapphire",    200, 7, 5});
    m_ores.push_back({"Emerald",     250, 8, 6});
    m_ores.push_back({"Diamond",     500, 9, 8});
    m_ores.push_back({"Mythril Ore", 750, 10, 9});
}

std::vector<const OreType*> MiningSystem::GetAvailableOres(int skillLevel) const {
    std::vector<const OreType*> result;
    for (const auto& ore : m_ores) {
        if (skillLevel >= ore.minSkillLevel) {
            result.push_back(&ore);
        }
    }
    return result;
}

int MiningSystem::AttemptMine(int skillLevel) const {
    int roll = std::rand() % 100;
    return AttemptMineWithRoll(skillLevel, roll);
}

int MiningSystem::AttemptMineWithRoll(int skillLevel, int roll) const {
    auto available = GetAvailableOres(skillLevel);
    if (available.empty()) return -1;

    // Base success chance: 50% + 4% per skill level, capped at 90%
    int successChance = 50 + skillLevel * 4;
    if (successChance > 90) successChance = 90;

    if (roll >= successChance) return -1;  // Miss (no ore found)

    // Select an ore — lower-indexed (common) ores are more likely
    // Use roll to deterministically pick from available ores
    int oreRoll = roll % static_cast<int>(available.size());

    // If selected ore is too hard (hardness > skill + 2), fall back to easiest
    const OreType* picked = available[oreRoll];
    if (picked->hardness > skillLevel + 2) {
        const OreType* easiest = available[0];
        for (const auto* o : available) {
            if (o->hardness < easiest->hardness) easiest = o;
        }
        picked = easiest;
    }

    // Find original index in m_ores
    for (int i = 0; i < static_cast<int>(m_ores.size()); ++i) {
        if (&m_ores[i] == picked) return i;
    }

    return -1;
}

const OreType* MiningSystem::GetOre(int index) const {
    if (index < 0 || index >= static_cast<int>(m_ores.size())) return nullptr;
    return &m_ores[index];
}

int MiningSystem::GetEnergyCost(int skillLevel) const {
    // Base cost 5, reduced by 0.3 per skill level (minimum 2)
    int cost = 5 - (skillLevel * 3) / 10;
    return std::max(2, cost);
}

int MiningSystem::GetXPReward(int oreIndex) const {
    const OreType* ore = GetOre(oreIndex);
    if (!ore) return 0;
    // XP scales with hardness: hardness * 5
    return ore->hardness * 5;
}
