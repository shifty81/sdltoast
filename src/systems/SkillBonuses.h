#ifndef SKILL_BONUSES_H
#define SKILL_BONUSES_H

#include "Skills.h"

/**
 * Skill-based bonuses — calculates gameplay bonuses from skill levels.
 * Each skill provides increasing benefits as it levels up.
 *
 * Farming:  crop value multiplier, growth speed bonus
 * Combat:   damage bonus, critical hit chance
 * Mining:   extra ore chance, energy cost reduction
 * Fishing:  catch rate bonus, fish value bonus
 * Foraging: double harvest chance, rare find chance
 */
class SkillBonuses {
public:
    // Farming bonuses (based on farming skill level 0-10)
    static float GetCropValueMultiplier(int farmingLevel);
    static int   GetGrowthSpeedBonus(int farmingLevel);

    // Combat bonuses (based on combat skill level 0-10)
    static int   GetDamageBonus(int combatLevel);
    static float GetCriticalHitChance(int combatLevel);

    // Mining bonuses (based on mining skill level 0-10)
    static float GetExtraOreChance(int miningLevel);
    static int   GetMiningEnergyReduction(int miningLevel);

    // Fishing bonuses (based on fishing skill level 0-10)
    static int   GetCatchRateBonus(int fishingLevel);
    static float GetFishValueMultiplier(int fishingLevel);

    // Foraging bonuses (based on foraging skill level 0-10)
    static float GetDoubleHarvestChance(int foragingLevel);
    static float GetRareFindChance(int foragingLevel);

    // Convenience: get bonuses from a Skills object
    static float GetCropValueMultiplier(const Skills& skills);
    static int   GetDamageBonus(const Skills& skills);
    static float GetExtraOreChance(const Skills& skills);
    static int   GetCatchRateBonus(const Skills& skills);
    static float GetDoubleHarvestChance(const Skills& skills);

private:
    static int ClampLevel(int level);
};

#endif // SKILL_BONUSES_H
