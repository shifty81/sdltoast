// Harvest Quest — Energy system unit tests

#include "systems/Energy.h"
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
    Energy e;
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX);
    ASSERT_EQ(e.GetMax(), Energy::DEFAULT_MAX);
    ASSERT_FALSE(e.IsExhausted());
}

TEST(test_consume_success) {
    Energy e;
    ASSERT_TRUE(e.Consume(10));
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX - 10);
}

TEST(test_consume_exact) {
    Energy e;
    ASSERT_TRUE(e.Consume(Energy::DEFAULT_MAX));
    ASSERT_EQ(e.GetCurrent(), 0);
    ASSERT_TRUE(e.IsExhausted());
}

TEST(test_consume_insufficient) {
    Energy e;
    e.Consume(Energy::DEFAULT_MAX - 5);
    ASSERT_FALSE(e.Consume(10));
    ASSERT_EQ(e.GetCurrent(), 5);  // unchanged
}

TEST(test_consume_zero) {
    Energy e;
    ASSERT_TRUE(e.Consume(0));
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX);
}

TEST(test_restore) {
    Energy e;
    e.Consume(30);
    e.Restore(15);
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX - 15);
}

TEST(test_restore_clamped) {
    Energy e;
    e.Consume(10);
    e.Restore(999);
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX);
}

TEST(test_restore_full) {
    Energy e;
    e.Consume(50);
    e.RestoreFull();
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX);
}

TEST(test_increase_max) {
    Energy e;
    e.Consume(20);
    int before = e.GetCurrent();
    e.IncreaseMax(10);
    ASSERT_EQ(e.GetMax(), Energy::DEFAULT_MAX + 10);
    ASSERT_EQ(e.GetCurrent(), before + 10);
}

TEST(test_increase_max_clamp) {
    Energy e;  // full energy
    e.IncreaseMax(10);
    ASSERT_EQ(e.GetMax(), Energy::DEFAULT_MAX + 10);
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX + 10);
}

TEST(test_has_energy) {
    Energy e;
    ASSERT_TRUE(e.HasEnergy(Energy::DEFAULT_MAX));
    ASSERT_FALSE(e.HasEnergy(Energy::DEFAULT_MAX + 1));
    e.Consume(Energy::DEFAULT_MAX);
    ASSERT_FALSE(e.HasEnergy(1));
    ASSERT_TRUE(e.HasEnergy(0));
}

TEST(test_set_current) {
    Energy e;
    e.SetCurrent(42);
    ASSERT_EQ(e.GetCurrent(), 42);
}

TEST(test_set_current_clamped) {
    Energy e;
    e.SetCurrent(999);
    ASSERT_EQ(e.GetCurrent(), Energy::DEFAULT_MAX);
    e.SetCurrent(-10);
    ASSERT_EQ(e.GetCurrent(), 0);
}

TEST(test_set_max) {
    Energy e;
    e.SetMax(50);
    ASSERT_EQ(e.GetMax(), 50);
    ASSERT_EQ(e.GetCurrent(), 50);  // clamped down
}

TEST(test_set_max_minimum) {
    Energy e;
    e.SetMax(0);
    ASSERT_EQ(e.GetMax(), 1);  // minimum is 1
}

TEST(test_action_costs_defined) {
    // Verify all action costs are positive
    ASSERT_TRUE(Energy::COST_TILL > 0);
    ASSERT_TRUE(Energy::COST_WATER > 0);
    ASSERT_TRUE(Energy::COST_PLANT > 0);
    ASSERT_TRUE(Energy::COST_HARVEST > 0);
    ASSERT_TRUE(Energy::COST_CHOP > 0);
    ASSERT_TRUE(Energy::COST_MINE > 0);
    ASSERT_TRUE(Energy::COST_FISH_CAST > 0);
    ASSERT_TRUE(Energy::COST_ATTACK > 0);
}

int main() {
    std::cout << "=== Energy Tests ===" << std::endl;
    RUN_TEST(test_initial_state);
    RUN_TEST(test_consume_success);
    RUN_TEST(test_consume_exact);
    RUN_TEST(test_consume_insufficient);
    RUN_TEST(test_consume_zero);
    RUN_TEST(test_restore);
    RUN_TEST(test_restore_clamped);
    RUN_TEST(test_restore_full);
    RUN_TEST(test_increase_max);
    RUN_TEST(test_increase_max_clamp);
    RUN_TEST(test_has_energy);
    RUN_TEST(test_set_current);
    RUN_TEST(test_set_current_clamped);
    RUN_TEST(test_set_max);
    RUN_TEST(test_set_max_minimum);
    RUN_TEST(test_action_costs_defined);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
