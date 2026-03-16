// Harvest Quest — Fishing system unit tests

#include "systems/Fishing.h"
#include "systems/Calendar.h"
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

TEST(test_fish_count) {
    FishingSystem fs;
    ASSERT_TRUE(fs.GetFishCount() >= 10);
}

TEST(test_get_fish_valid) {
    FishingSystem fs;
    const FishType* fish = fs.GetFish(0);
    ASSERT_TRUE(fish != nullptr);
    ASSERT_FALSE(fish->name.empty());
    ASSERT_TRUE(fish->value > 0);
}

TEST(test_get_fish_invalid) {
    FishingSystem fs;
    ASSERT_TRUE(fs.GetFish(-1) == nullptr);
    ASSERT_TRUE(fs.GetFish(999) == nullptr);
}

TEST(test_spring_fish_available) {
    FishingSystem fs;
    auto available = fs.GetAvailableFish(Season::SPRING);
    ASSERT_TRUE(!available.empty());
    // All returned fish should be available in spring
    for (const auto* f : available) {
        ASSERT_TRUE(f->springAvail);
    }
}

TEST(test_summer_fish_available) {
    FishingSystem fs;
    auto available = fs.GetAvailableFish(Season::SUMMER);
    ASSERT_TRUE(!available.empty());
    for (const auto* f : available) {
        ASSERT_TRUE(f->summerAvail);
    }
}

TEST(test_fall_fish_available) {
    FishingSystem fs;
    auto available = fs.GetAvailableFish(Season::FALL);
    ASSERT_TRUE(!available.empty());
    for (const auto* f : available) {
        ASSERT_TRUE(f->fallAvail);
    }
}

TEST(test_winter_fish_available) {
    FishingSystem fs;
    auto available = fs.GetAvailableFish(Season::WINTER);
    ASSERT_TRUE(!available.empty());
    for (const auto* f : available) {
        ASSERT_TRUE(f->winterAvail);
    }
}

TEST(test_attempt_catch_success) {
    FishingSystem fs;
    // With skill level 10, roll=0 → should succeed (success chance = 90%)
    int result = fs.AttemptCatchWithRoll(Season::SPRING, 10, 0);
    ASSERT_TRUE(result >= 0);
}

TEST(test_attempt_catch_miss) {
    FishingSystem fs;
    // With skill level 0, roll=99 → miss (success chance = 40%, roll 99 >= 40)
    int result = fs.AttemptCatchWithRoll(Season::SPRING, 0, 99);
    ASSERT_EQ(result, -1);
}

TEST(test_attempt_catch_high_skill_high_success) {
    FishingSystem fs;
    // Skill 10: success chance = 90%. Roll of 89 should succeed.
    int result = fs.AttemptCatchWithRoll(Season::SUMMER, 10, 89);
    ASSERT_TRUE(result >= 0);
    // Roll of 90 should miss.
    int result2 = fs.AttemptCatchWithRoll(Season::SUMMER, 10, 90);
    ASSERT_EQ(result2, -1);
}

TEST(test_caught_fish_is_valid) {
    FishingSystem fs;
    int idx = fs.AttemptCatchWithRoll(Season::FALL, 5, 0);
    ASSERT_TRUE(idx >= 0);
    const FishType* caught = fs.GetFish(idx);
    ASSERT_TRUE(caught != nullptr);
    ASSERT_TRUE(caught->value > 0);
}

TEST(test_fish_difficulty_range) {
    FishingSystem fs;
    for (int i = 0; i < fs.GetFishCount(); ++i) {
        const FishType* f = fs.GetFish(i);
        ASSERT_TRUE(f->difficulty >= 1);
        ASSERT_TRUE(f->difficulty <= 10);
    }
}

TEST(test_each_season_has_unique_fish) {
    FishingSystem fs;
    // Check that seasonal pools overlap but are not identical
    auto spring = fs.GetAvailableFish(Season::SPRING);
    auto winter = fs.GetAvailableFish(Season::WINTER);
    // Spring and winter should have different sets (some overlap like Carp)
    bool same = spring.size() == winter.size();
    if (same) {
        for (size_t i = 0; i < spring.size(); ++i) {
            if (spring[i]->name != winter[i]->name) { same = false; break; }
        }
    }
    ASSERT_FALSE(same);
}

int main() {
    std::cout << "=== Fishing Tests ===" << std::endl;
    RUN_TEST(test_fish_count);
    RUN_TEST(test_get_fish_valid);
    RUN_TEST(test_get_fish_invalid);
    RUN_TEST(test_spring_fish_available);
    RUN_TEST(test_summer_fish_available);
    RUN_TEST(test_fall_fish_available);
    RUN_TEST(test_winter_fish_available);
    RUN_TEST(test_attempt_catch_success);
    RUN_TEST(test_attempt_catch_miss);
    RUN_TEST(test_attempt_catch_high_skill_high_success);
    RUN_TEST(test_caught_fish_is_valid);
    RUN_TEST(test_fish_difficulty_range);
    RUN_TEST(test_each_season_has_unique_fish);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
