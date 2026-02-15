// Harvest Quest — Inventory system unit tests

#include "systems/Inventory.h"
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

TEST(test_add_item) {
    Inventory inv;
    inv.AddItem("Wood", 5);
    ASSERT_EQ(inv.GetItemCount("Wood"), 5);
}

TEST(test_add_item_stacks) {
    Inventory inv;
    inv.AddItem("Wood", 3);
    inv.AddItem("Wood", 2);
    ASSERT_EQ(inv.GetItemCount("Wood"), 5);
}

TEST(test_remove_item) {
    Inventory inv;
    inv.AddItem("Stone", 10);
    ASSERT_TRUE(inv.RemoveItem("Stone", 4));
    ASSERT_EQ(inv.GetItemCount("Stone"), 6);
}

TEST(test_remove_item_insufficient) {
    Inventory inv;
    inv.AddItem("Stone", 2);
    ASSERT_FALSE(inv.RemoveItem("Stone", 5));
    ASSERT_EQ(inv.GetItemCount("Stone"), 2);
}

TEST(test_remove_item_fully) {
    Inventory inv;
    inv.AddItem("Seed", 3);
    ASSERT_TRUE(inv.RemoveItem("Seed", 3));
    ASSERT_EQ(inv.GetItemCount("Seed"), 0);
}

TEST(test_remove_nonexistent_item) {
    Inventory inv;
    ASSERT_FALSE(inv.RemoveItem("Gold", 1));
}

TEST(test_get_item_count_empty) {
    Inventory inv;
    ASSERT_EQ(inv.GetItemCount("Anything"), 0);
}

TEST(test_clear) {
    Inventory inv;
    inv.AddItem("Wood", 5);
    inv.AddItem("Stone", 3);
    inv.Clear();
    ASSERT_EQ(inv.GetItemCount("Wood"), 0);
    ASSERT_EQ(inv.GetItemCount("Stone"), 0);
    ASSERT_EQ(inv.GetItems().size(), static_cast<size_t>(0));
}

int main() {
    std::cout << "=== Inventory Tests ===" << std::endl;
    RUN_TEST(test_add_item);
    RUN_TEST(test_add_item_stacks);
    RUN_TEST(test_remove_item);
    RUN_TEST(test_remove_item_insufficient);
    RUN_TEST(test_remove_item_fully);
    RUN_TEST(test_remove_nonexistent_item);
    RUN_TEST(test_get_item_count_empty);
    RUN_TEST(test_clear);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
