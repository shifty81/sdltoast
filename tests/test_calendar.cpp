// Harvest Quest — Calendar system unit tests

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

TEST(test_initial_state) {
    Calendar cal;
    ASSERT_EQ(cal.GetDay(), 1);
    ASSERT_EQ(cal.GetSeason(), Season::SPRING);
    ASSERT_EQ(cal.GetYear(), 1);
}

TEST(test_advance_day) {
    Calendar cal;
    cal.AdvanceDay();
    ASSERT_EQ(cal.GetDay(), 2);
    ASSERT_EQ(cal.GetSeason(), Season::SPRING);
}

TEST(test_season_transition) {
    Calendar cal;
    // Advance through 28 days of Spring
    for (int i = 0; i < 28; ++i) {
        cal.AdvanceDay();
    }
    ASSERT_EQ(cal.GetDay(), 1);
    ASSERT_EQ(cal.GetSeason(), Season::SUMMER);
}

TEST(test_year_transition) {
    Calendar cal;
    // Advance through 4 seasons × 28 days = 112 days
    for (int i = 0; i < 112; ++i) {
        cal.AdvanceDay();
    }
    ASSERT_EQ(cal.GetDay(), 1);
    ASSERT_EQ(cal.GetSeason(), Season::SPRING);
    ASSERT_EQ(cal.GetYear(), 2);
}

TEST(test_season_name) {
    Calendar cal;
    ASSERT_EQ(cal.GetSeasonName(), "Spring");

    cal.SetSeason(Season::SUMMER);
    ASSERT_EQ(cal.GetSeasonName(), "Summer");

    cal.SetSeason(Season::FALL);
    ASSERT_EQ(cal.GetSeasonName(), "Fall");

    cal.SetSeason(Season::WINTER);
    ASSERT_EQ(cal.GetSeasonName(), "Winter");
}

TEST(test_set_day) {
    Calendar cal;
    cal.SetDay(15);
    ASSERT_EQ(cal.GetDay(), 15);
}

int main() {
    std::cout << "=== Calendar Tests ===" << std::endl;
    RUN_TEST(test_initial_state);
    RUN_TEST(test_advance_day);
    RUN_TEST(test_season_transition);
    RUN_TEST(test_year_transition);
    RUN_TEST(test_season_name);
    RUN_TEST(test_set_day);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
