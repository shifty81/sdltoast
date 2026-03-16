// Harvest Quest — Mining system unit tests

#include "systems/Mining.h"
#include <cassert>
#include <iostream>

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

TEST(test_ore_count) {
    MiningSystem ms;
    ASSERT_EQ(ms.GetOreCount(), 10);
}

TEST(test_get_ore_valid) {
    MiningSystem ms;
    const OreType* ore = ms.GetOre(0);
    ASSERT_TRUE(ore != nullptr);
    ASSERT_FALSE(ore->name.empty());
    ASSERT_TRUE(ore->value > 0);
}

TEST(test_get_ore_invalid) {
    MiningSystem ms;
    ASSERT_TRUE(ms.GetOre(-1) == nullptr);
    ASSERT_TRUE(ms.GetOre(999) == nullptr);
}

TEST(test_available_ores_skill_0) {
    MiningSystem ms;
    auto available = ms.GetAvailableOres(0);
    ASSERT_TRUE(!available.empty());
    // At skill 0, should have Stone and Coal (minSkillLevel 0)
    for (const auto* ore : available) {
        ASSERT_EQ(ore->minSkillLevel, 0);
    }
}

TEST(test_available_ores_skill_5) {
    MiningSystem ms;
    auto atSkill0 = ms.GetAvailableOres(0);
    auto atSkill5 = ms.GetAvailableOres(5);
    // Higher skill should unlock more ores
    ASSERT_TRUE(atSkill5.size() > atSkill0.size());
}

TEST(test_available_ores_skill_10) {
    MiningSystem ms;
    auto atSkill10 = ms.GetAvailableOres(10);
    // At max skill, all ores should be available
    ASSERT_EQ(static_cast<int>(atSkill10.size()), ms.GetOreCount());
}

TEST(test_attempt_mine_success) {
    MiningSystem ms;
    // Skill 10, roll=0 → should succeed (success chance = 90%)
    int result = ms.AttemptMineWithRoll(10, 0);
    ASSERT_TRUE(result >= 0);
}

TEST(test_attempt_mine_miss) {
    MiningSystem ms;
    // Skill 0, roll=99 → miss (success chance = 50%, roll 99 >= 50)
    int result = ms.AttemptMineWithRoll(0, 99);
    ASSERT_EQ(result, -1);
}

TEST(test_attempt_mine_high_skill_boundary) {
    MiningSystem ms;
    // Skill 10: success chance = 90%. Roll of 89 should succeed.
    int result = ms.AttemptMineWithRoll(10, 89);
    ASSERT_TRUE(result >= 0);
    // Roll of 90 should miss.
    int result2 = ms.AttemptMineWithRoll(10, 90);
    ASSERT_EQ(result2, -1);
}

TEST(test_mined_ore_is_valid) {
    MiningSystem ms;
    int idx = ms.AttemptMineWithRoll(5, 0);
    ASSERT_TRUE(idx >= 0);
    const OreType* ore = ms.GetOre(idx);
    ASSERT_TRUE(ore != nullptr);
    ASSERT_TRUE(ore->value > 0);
}

TEST(test_ore_hardness_range) {
    MiningSystem ms;
    for (int i = 0; i < ms.GetOreCount(); ++i) {
        const OreType* ore = ms.GetOre(i);
        ASSERT_TRUE(ore->hardness >= 1);
        ASSERT_TRUE(ore->hardness <= 10);
    }
}

TEST(test_ore_min_skill_range) {
    MiningSystem ms;
    for (int i = 0; i < ms.GetOreCount(); ++i) {
        const OreType* ore = ms.GetOre(i);
        ASSERT_TRUE(ore->minSkillLevel >= 0);
        ASSERT_TRUE(ore->minSkillLevel <= 10);
    }
}

TEST(test_energy_cost_decreases_with_skill) {
    MiningSystem ms;
    int costAt0 = ms.GetEnergyCost(0);
    int costAt5 = ms.GetEnergyCost(5);
    int costAt10 = ms.GetEnergyCost(10);
    ASSERT_TRUE(costAt0 >= costAt5);
    ASSERT_TRUE(costAt5 >= costAt10);
    ASSERT_TRUE(costAt10 >= 2);  // Minimum cost
}

TEST(test_energy_cost_values) {
    MiningSystem ms;
    ASSERT_EQ(ms.GetEnergyCost(0), 5);   // Base cost
    ASSERT_EQ(ms.GetEnergyCost(10), 2); // Minimum cost
}

TEST(test_xp_reward_valid_ore) {
    MiningSystem ms;
    // Stone has hardness 1, so XP = 1 * 5 = 5
    ASSERT_EQ(ms.GetXPReward(0), 5);
}

TEST(test_xp_reward_invalid_ore) {
    MiningSystem ms;
    ASSERT_EQ(ms.GetXPReward(-1), 0);
    ASSERT_EQ(ms.GetXPReward(999), 0);
}

TEST(test_xp_reward_scales_with_hardness) {
    MiningSystem ms;
    // Higher hardness ores give more XP
    int xpStone = ms.GetXPReward(0);      // hardness 1 → 5
    int xpDiamond = ms.GetXPReward(8);    // hardness 9 → 45
    ASSERT_TRUE(xpDiamond > xpStone);
}

int main() {
    std::cout << "=== Mining Tests ===" << std::endl;
    RUN_TEST(test_ore_count);
    RUN_TEST(test_get_ore_valid);
    RUN_TEST(test_get_ore_invalid);
    RUN_TEST(test_available_ores_skill_0);
    RUN_TEST(test_available_ores_skill_5);
    RUN_TEST(test_available_ores_skill_10);
    RUN_TEST(test_attempt_mine_success);
    RUN_TEST(test_attempt_mine_miss);
    RUN_TEST(test_attempt_mine_high_skill_boundary);
    RUN_TEST(test_mined_ore_is_valid);
    RUN_TEST(test_ore_hardness_range);
    RUN_TEST(test_ore_min_skill_range);
    RUN_TEST(test_energy_cost_decreases_with_skill);
    RUN_TEST(test_energy_cost_values);
    RUN_TEST(test_xp_reward_valid_ore);
    RUN_TEST(test_xp_reward_invalid_ore);
    RUN_TEST(test_xp_reward_scales_with_hardness);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
