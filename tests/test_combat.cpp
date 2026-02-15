// Harvest Quest — Combat system unit tests
// Tests pure-logic static methods (no Raylib dependency)

#include "systems/Combat.h"
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

// --- CalculateDamage tests ---

TEST(test_damage_basic) {
    ASSERT_EQ(Combat::CalculateDamage(10, 3), 7);
}

TEST(test_damage_zero_defense) {
    ASSERT_EQ(Combat::CalculateDamage(5, 0), 5);
}

TEST(test_damage_equal_stats) {
    ASSERT_EQ(Combat::CalculateDamage(5, 5), 0);
}

TEST(test_damage_defense_exceeds_attack) {
    ASSERT_EQ(Combat::CalculateDamage(3, 10), 0);
}

TEST(test_damage_zero_attack) {
    ASSERT_EQ(Combat::CalculateDamage(0, 5), 0);
}

TEST(test_damage_both_zero) {
    ASSERT_EQ(Combat::CalculateDamage(0, 0), 0);
}

// --- CheckCollision tests ---

TEST(test_collision_overlap) {
    ASSERT_TRUE(Combat::CheckCollision(
        0, 0, 10, 10,   // rect 1
        5, 5, 10, 10));  // rect 2 overlaps
}

TEST(test_collision_no_overlap) {
    ASSERT_FALSE(Combat::CheckCollision(
        0, 0, 10, 10,     // rect 1
        20, 20, 10, 10));  // rect 2 far away
}

TEST(test_collision_touching_edge) {
    // Touching at exact edge — not overlapping in AABB
    ASSERT_FALSE(Combat::CheckCollision(
        0, 0, 10, 10,
        10, 0, 10, 10));
}

TEST(test_collision_contained) {
    // One rect fully inside another
    ASSERT_TRUE(Combat::CheckCollision(
        0, 0, 100, 100,
        10, 10, 5, 5));
}

TEST(test_collision_partial_x) {
    // Overlaps in X but not Y
    ASSERT_FALSE(Combat::CheckCollision(
        0, 0, 10, 10,
        5, 20, 10, 10));
}

TEST(test_collision_partial_y) {
    // Overlaps in Y but not X
    ASSERT_FALSE(Combat::CheckCollision(
        0, 0, 10, 10,
        20, 5, 10, 10));
}

TEST(test_collision_zero_size) {
    // Zero-size rect doesn't collide
    ASSERT_FALSE(Combat::CheckCollision(
        5, 5, 0, 0,
        5, 5, 10, 10));
}

// --- Constants tests ---

TEST(test_attack_range_positive) {
    ASSERT_TRUE(Combat::ATTACK_RANGE > 0);
}

TEST(test_damage_cooldown_positive) {
    ASSERT_TRUE(Combat::DAMAGE_COOLDOWN > 0);
}

int main() {
    std::cout << "=== Combat Tests ===" << std::endl;
    RUN_TEST(test_damage_basic);
    RUN_TEST(test_damage_zero_defense);
    RUN_TEST(test_damage_equal_stats);
    RUN_TEST(test_damage_defense_exceeds_attack);
    RUN_TEST(test_damage_zero_attack);
    RUN_TEST(test_damage_both_zero);
    RUN_TEST(test_collision_overlap);
    RUN_TEST(test_collision_no_overlap);
    RUN_TEST(test_collision_touching_edge);
    RUN_TEST(test_collision_contained);
    RUN_TEST(test_collision_partial_x);
    RUN_TEST(test_collision_partial_y);
    RUN_TEST(test_collision_zero_size);
    RUN_TEST(test_attack_range_positive);
    RUN_TEST(test_damage_cooldown_positive);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
