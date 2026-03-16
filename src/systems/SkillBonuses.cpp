#include "SkillBonuses.h"
#include <algorithm>

int SkillBonuses::ClampLevel(int level) {
    return std::clamp(level, 0, Skills::MAX_LEVEL);
}

// --- Farming bonuses ---

float SkillBonuses::GetCropValueMultiplier(int farmingLevel) {
    int level = ClampLevel(farmingLevel);
    // 1.0x at level 0, +0.1x per level → 2.0x at level 10
    return 1.0f + level * 0.1f;
}

int SkillBonuses::GetGrowthSpeedBonus(int farmingLevel) {
    int level = ClampLevel(farmingLevel);
    // Every 3 levels, crops grow 1 day faster (0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3)
    return level / 3;
}

// --- Combat bonuses ---

int SkillBonuses::GetDamageBonus(int combatLevel) {
    int level = ClampLevel(combatLevel);
    // +1 damage per 2 levels → +5 at level 10
    return level / 2;
}

float SkillBonuses::GetCriticalHitChance(int combatLevel) {
    int level = ClampLevel(combatLevel);
    // 0% at level 0, +3% per level → 30% at level 10
    return level * 0.03f;
}

// --- Mining bonuses ---

float SkillBonuses::GetExtraOreChance(int miningLevel) {
    int level = ClampLevel(miningLevel);
    // 0% at level 0, +5% per level → 50% at level 10
    return level * 0.05f;
}

int SkillBonuses::GetMiningEnergyReduction(int miningLevel) {
    int level = ClampLevel(miningLevel);
    // Every 4 levels, mining costs 1 less energy → -2 at level 8+
    return level / 4;
}

// --- Fishing bonuses ---

int SkillBonuses::GetCatchRateBonus(int fishingLevel) {
    int level = ClampLevel(fishingLevel);
    // +5% catch rate per level → +50% at level 10
    return level * 5;
}

float SkillBonuses::GetFishValueMultiplier(int fishingLevel) {
    int level = ClampLevel(fishingLevel);
    // 1.0x at level 0, +0.15x per level → 2.5x at level 10
    return 1.0f + level * 0.15f;
}

// --- Foraging bonuses ---

float SkillBonuses::GetDoubleHarvestChance(int foragingLevel) {
    int level = ClampLevel(foragingLevel);
    // 0% at level 0, +4% per level → 40% at level 10
    return level * 0.04f;
}

float SkillBonuses::GetRareFindChance(int foragingLevel) {
    int level = ClampLevel(foragingLevel);
    // 0% at level 0, +2% per level → 20% at level 10
    return level * 0.02f;
}

// --- Convenience overloads using Skills object ---

float SkillBonuses::GetCropValueMultiplier(const Skills& skills) {
    return GetCropValueMultiplier(skills.GetLevel(SkillType::FARMING));
}

int SkillBonuses::GetDamageBonus(const Skills& skills) {
    return GetDamageBonus(skills.GetLevel(SkillType::COMBAT));
}

float SkillBonuses::GetExtraOreChance(const Skills& skills) {
    return GetExtraOreChance(skills.GetLevel(SkillType::MINING));
}

int SkillBonuses::GetCatchRateBonus(const Skills& skills) {
    return GetCatchRateBonus(skills.GetLevel(SkillType::FISHING));
}

float SkillBonuses::GetDoubleHarvestChance(const Skills& skills) {
    return GetDoubleHarvestChance(skills.GetLevel(SkillType::FORAGING));
}
