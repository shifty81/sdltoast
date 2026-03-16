// Harvest Quest — Skill Bonuses system unit tests

#include "systems/SkillBonuses.h"
#include <cassert>
#include <iostream>
#include <cmath>

static int s_passed = 0;
static int s_failed = 0;

#define TEST(name) static void name()
#define RUN_TEST(name) do { \
    std::cout << "  " #name "... "; \
    try { name(); std::cout << "PASS" << std::endl; s_passed++; } \
    catch (...) { std::cout << "FAIL" << std::endl; s_failed++; } \
} while(0)
#define ASSERT_TRUE(expr)  do { if (!(expr)) throw 1; } while(0)
#define ASSERT_FALSE(expr) do { if (expr) throw 1; } while(0)
#define ASSERT_EQ(a, b)    do { if ((a) != (b)) throw 1; } while(0)
#define ASSERT_FLOAT_NEAR(a, b, tol) do { if (std::fabs((a) - (b)) > (tol)) throw 1; } while(0)

// --- Farming ---

TEST(test_crop_value_level_0) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCropValueMultiplier(0), 1.0f, 0.01f);
}

TEST(test_crop_value_level_10) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCropValueMultiplier(10), 2.0f, 0.01f);
}

TEST(test_crop_value_increases) {
    float low = SkillBonuses::GetCropValueMultiplier(2);
    float high = SkillBonuses::GetCropValueMultiplier(8);
    ASSERT_TRUE(high > low);
}

TEST(test_growth_speed_bonus) {
    ASSERT_EQ(SkillBonuses::GetGrowthSpeedBonus(0), 0);
    ASSERT_EQ(SkillBonuses::GetGrowthSpeedBonus(3), 1);
    ASSERT_EQ(SkillBonuses::GetGrowthSpeedBonus(6), 2);
    ASSERT_EQ(SkillBonuses::GetGrowthSpeedBonus(9), 3);
    ASSERT_EQ(SkillBonuses::GetGrowthSpeedBonus(10), 3);
}

// --- Combat ---

TEST(test_damage_bonus_level_0) {
    ASSERT_EQ(SkillBonuses::GetDamageBonus(0), 0);
}

TEST(test_damage_bonus_level_10) {
    ASSERT_EQ(SkillBonuses::GetDamageBonus(10), 5);
}

TEST(test_damage_bonus_increases) {
    ASSERT_TRUE(SkillBonuses::GetDamageBonus(6) > SkillBonuses::GetDamageBonus(2));
}

TEST(test_crit_chance_level_0) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCriticalHitChance(0), 0.0f, 0.001f);
}

TEST(test_crit_chance_level_10) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCriticalHitChance(10), 0.30f, 0.001f);
}

// --- Mining ---

TEST(test_extra_ore_chance_level_0) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetExtraOreChance(0), 0.0f, 0.001f);
}

TEST(test_extra_ore_chance_level_10) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetExtraOreChance(10), 0.50f, 0.001f);
}

TEST(test_mining_energy_reduction) {
    ASSERT_EQ(SkillBonuses::GetMiningEnergyReduction(0), 0);
    ASSERT_EQ(SkillBonuses::GetMiningEnergyReduction(4), 1);
    ASSERT_EQ(SkillBonuses::GetMiningEnergyReduction(8), 2);
    ASSERT_EQ(SkillBonuses::GetMiningEnergyReduction(10), 2);
}

// --- Fishing ---

TEST(test_catch_rate_bonus_level_0) {
    ASSERT_EQ(SkillBonuses::GetCatchRateBonus(0), 0);
}

TEST(test_catch_rate_bonus_level_10) {
    ASSERT_EQ(SkillBonuses::GetCatchRateBonus(10), 50);
}

TEST(test_fish_value_multiplier) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetFishValueMultiplier(0), 1.0f, 0.01f);
    ASSERT_FLOAT_NEAR(SkillBonuses::GetFishValueMultiplier(10), 2.5f, 0.01f);
}

// --- Foraging ---

TEST(test_double_harvest_chance_level_0) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetDoubleHarvestChance(0), 0.0f, 0.001f);
}

TEST(test_double_harvest_chance_level_10) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetDoubleHarvestChance(10), 0.40f, 0.001f);
}

TEST(test_rare_find_chance) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetRareFindChance(0), 0.0f, 0.001f);
    ASSERT_FLOAT_NEAR(SkillBonuses::GetRareFindChance(10), 0.20f, 0.001f);
}

// --- Edge cases ---

TEST(test_negative_level_clamped) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCropValueMultiplier(-5), 1.0f, 0.01f);
    ASSERT_EQ(SkillBonuses::GetDamageBonus(-1), 0);
}

TEST(test_over_max_level_clamped) {
    ASSERT_FLOAT_NEAR(SkillBonuses::GetCropValueMultiplier(20), 2.0f, 0.01f);
    ASSERT_EQ(SkillBonuses::GetDamageBonus(15), 5);
}

// --- Skills object convenience ---

TEST(test_skills_object_farming) {
    Skills s;
    s.AddXP(SkillType::FARMING, 300);  // Level 2
    float mult = SkillBonuses::GetCropValueMultiplier(s);
    ASSERT_FLOAT_NEAR(mult, SkillBonuses::GetCropValueMultiplier(2), 0.01f);
}

TEST(test_skills_object_combat) {
    Skills s;
    s.AddXP(SkillType::COMBAT, 600);  // Level 3
    int bonus = SkillBonuses::GetDamageBonus(s);
    ASSERT_EQ(bonus, SkillBonuses::GetDamageBonus(3));
}

TEST(test_skills_object_mining) {
    Skills s;
    s.AddXP(SkillType::MINING, 1000);  // Level 4
    float chance = SkillBonuses::GetExtraOreChance(s);
    ASSERT_FLOAT_NEAR(chance, SkillBonuses::GetExtraOreChance(4), 0.001f);
}

int main() {
    std::cout << "=== Skill Bonuses Tests ===" << std::endl;
    RUN_TEST(test_crop_value_level_0);
    RUN_TEST(test_crop_value_level_10);
    RUN_TEST(test_crop_value_increases);
    RUN_TEST(test_growth_speed_bonus);
    RUN_TEST(test_damage_bonus_level_0);
    RUN_TEST(test_damage_bonus_level_10);
    RUN_TEST(test_damage_bonus_increases);
    RUN_TEST(test_crit_chance_level_0);
    RUN_TEST(test_crit_chance_level_10);
    RUN_TEST(test_extra_ore_chance_level_0);
    RUN_TEST(test_extra_ore_chance_level_10);
    RUN_TEST(test_mining_energy_reduction);
    RUN_TEST(test_catch_rate_bonus_level_0);
    RUN_TEST(test_catch_rate_bonus_level_10);
    RUN_TEST(test_fish_value_multiplier);
    RUN_TEST(test_double_harvest_chance_level_0);
    RUN_TEST(test_double_harvest_chance_level_10);
    RUN_TEST(test_rare_find_chance);
    RUN_TEST(test_negative_level_clamped);
    RUN_TEST(test_over_max_level_clamped);
    RUN_TEST(test_skills_object_farming);
    RUN_TEST(test_skills_object_combat);
    RUN_TEST(test_skills_object_mining);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
