// Harvest Quest — Skills system unit tests

#include "systems/Skills.h"
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

TEST(test_initial_state) {
    Skills s;
    for (int i = 0; i < Skills::SKILL_COUNT; ++i) {
        auto type = static_cast<SkillType>(i);
        ASSERT_EQ(s.GetLevel(type), 0);
        ASSERT_EQ(s.GetXP(type), 0);
    }
}

TEST(test_skill_names) {
    ASSERT_EQ(Skills::GetSkillName(SkillType::FARMING), "Farming");
    ASSERT_EQ(Skills::GetSkillName(SkillType::COMBAT), "Combat");
    ASSERT_EQ(Skills::GetSkillName(SkillType::MINING), "Mining");
    ASSERT_EQ(Skills::GetSkillName(SkillType::FISHING), "Fishing");
    ASSERT_EQ(Skills::GetSkillName(SkillType::FORAGING), "Foraging");
}

TEST(test_add_xp_no_levelup) {
    Skills s;
    bool leveled = s.AddXP(SkillType::FARMING, 10);
    ASSERT_FALSE(leveled);
    ASSERT_EQ(s.GetXP(SkillType::FARMING), 10);
    ASSERT_EQ(s.GetLevel(SkillType::FARMING), 0);
}

TEST(test_add_xp_levelup) {
    Skills s;
    // XP for level 1 = 1 * 2 * 50 = 100
    bool leveled = s.AddXP(SkillType::COMBAT, 100);
    ASSERT_TRUE(leveled);
    ASSERT_EQ(s.GetLevel(SkillType::COMBAT), 1);
}

TEST(test_add_xp_multi_levelup) {
    Skills s;
    // XP for level 2 = 2 * 3 * 50 = 300
    bool leveled = s.AddXP(SkillType::MINING, 300);
    ASSERT_TRUE(leveled);
    ASSERT_EQ(s.GetLevel(SkillType::MINING), 2);
}

TEST(test_xp_accumulates) {
    Skills s;
    s.AddXP(SkillType::FISHING, 50);
    s.AddXP(SkillType::FISHING, 50);
    ASSERT_EQ(s.GetXP(SkillType::FISHING), 100);
    ASSERT_EQ(s.GetLevel(SkillType::FISHING), 1);
}

TEST(test_max_level_cap) {
    Skills s;
    // XP for level 10 = 10 * 11 * 50 = 5500
    s.AddXP(SkillType::FORAGING, 6000);
    ASSERT_EQ(s.GetLevel(SkillType::FORAGING), Skills::MAX_LEVEL);

    // Additional XP should not level further
    bool leveled = s.AddXP(SkillType::FORAGING, 1000);
    ASSERT_FALSE(leveled);
}

TEST(test_xp_required_for_level) {
    ASSERT_EQ(Skills::XPRequiredForLevel(0), 0);
    ASSERT_EQ(Skills::XPRequiredForLevel(1), 100);   // 1*2*50
    ASSERT_EQ(Skills::XPRequiredForLevel(2), 300);   // 2*3*50
    ASSERT_EQ(Skills::XPRequiredForLevel(3), 600);   // 3*4*50
    ASSERT_EQ(Skills::XPRequiredForLevel(10), 5500);  // 10*11*50
}

TEST(test_xp_for_next_level) {
    Skills s;
    // At level 0, 0 XP: need 100 to reach level 1
    ASSERT_EQ(s.GetXPForNextLevel(SkillType::FARMING), 100);
    s.AddXP(SkillType::FARMING, 50);
    ASSERT_EQ(s.GetXPForNextLevel(SkillType::FARMING), 50);
}

TEST(test_xp_for_next_level_at_max) {
    Skills s;
    s.AddXP(SkillType::COMBAT, 6000);
    ASSERT_EQ(s.GetXPForNextLevel(SkillType::COMBAT), 0);  // Already maxed
}

TEST(test_skills_independent) {
    Skills s;
    s.AddXP(SkillType::FARMING, 100);
    ASSERT_EQ(s.GetLevel(SkillType::FARMING), 1);
    ASSERT_EQ(s.GetLevel(SkillType::COMBAT), 0);
    ASSERT_EQ(s.GetLevel(SkillType::MINING), 0);
}

TEST(test_add_xp_zero_or_negative) {
    Skills s;
    ASSERT_FALSE(s.AddXP(SkillType::FARMING, 0));
    ASSERT_FALSE(s.AddXP(SkillType::FARMING, -10));
    ASSERT_EQ(s.GetXP(SkillType::FARMING), 0);
}

TEST(test_set_level) {
    Skills s;
    s.SetLevel(SkillType::COMBAT, 5);
    ASSERT_EQ(s.GetLevel(SkillType::COMBAT), 5);
}

TEST(test_set_level_clamped) {
    Skills s;
    s.SetLevel(SkillType::MINING, 15);
    ASSERT_EQ(s.GetLevel(SkillType::MINING), Skills::MAX_LEVEL);
    s.SetLevel(SkillType::MINING, -3);
    ASSERT_EQ(s.GetLevel(SkillType::MINING), 0);
}

TEST(test_set_xp) {
    Skills s;
    s.SetXP(SkillType::FISHING, 250);
    ASSERT_EQ(s.GetXP(SkillType::FISHING), 250);
}

int main() {
    std::cout << "=== Skills Tests ===" << std::endl;
    RUN_TEST(test_initial_state);
    RUN_TEST(test_skill_names);
    RUN_TEST(test_add_xp_no_levelup);
    RUN_TEST(test_add_xp_levelup);
    RUN_TEST(test_add_xp_multi_levelup);
    RUN_TEST(test_xp_accumulates);
    RUN_TEST(test_max_level_cap);
    RUN_TEST(test_xp_required_for_level);
    RUN_TEST(test_xp_for_next_level);
    RUN_TEST(test_xp_for_next_level_at_max);
    RUN_TEST(test_skills_independent);
    RUN_TEST(test_add_xp_zero_or_negative);
    RUN_TEST(test_set_level);
    RUN_TEST(test_set_level_clamped);
    RUN_TEST(test_set_xp);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
