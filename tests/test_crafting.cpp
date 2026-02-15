// Harvest Quest — Crafting system unit tests

#include "systems/Crafting.h"
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

TEST(test_recipe_count) {
    Crafting crafting;
    ASSERT_EQ(crafting.GetRecipeCount(), 8);
}

TEST(test_recipe_names) {
    Crafting crafting;
    ASSERT_EQ(crafting.GetRecipe(0).resultName, "Fence");
    ASSERT_EQ(crafting.GetRecipe(1).resultName, "Chest");
    ASSERT_EQ(crafting.GetRecipe(2).resultName, "Sprinkler");
    ASSERT_EQ(crafting.GetRecipe(3).resultName, "Parsnip Soup");
    ASSERT_EQ(crafting.GetRecipe(4).resultName, "Baked Potato");
    ASSERT_EQ(crafting.GetRecipe(5).resultName, "Tomato Sauce");
    ASSERT_EQ(crafting.GetRecipe(6).resultName, "Stone Wall");
    ASSERT_EQ(crafting.GetRecipe(7).resultName, "Bridge");
}

TEST(test_can_craft_with_sufficient_materials) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 5);
    ASSERT_TRUE(crafting.CanCraft(0, &inv)); // Fence: Wood x5
}

TEST(test_cannot_craft_insufficient_materials) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 3);
    ASSERT_FALSE(crafting.CanCraft(0, &inv)); // Fence needs Wood x5
}

TEST(test_cannot_craft_empty_inventory) {
    Crafting crafting;
    Inventory inv;
    ASSERT_FALSE(crafting.CanCraft(0, &inv));
}

TEST(test_cannot_craft_null_inventory) {
    Crafting crafting;
    ASSERT_FALSE(crafting.CanCraft(0, nullptr));
}

TEST(test_cannot_craft_invalid_index) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 100);
    ASSERT_FALSE(crafting.CanCraft(-1, &inv));
    ASSERT_FALSE(crafting.CanCraft(99, &inv));
}

TEST(test_craft_success) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 5);
    ASSERT_TRUE(crafting.Craft(0, &inv)); // Fence
    ASSERT_EQ(inv.GetItemCount("Wood"), 0);
    ASSERT_EQ(inv.GetItemCount("Fence"), 1);
}

TEST(test_craft_multi_ingredient) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 3);
    inv.AddItem("Stone", 3);
    ASSERT_TRUE(crafting.Craft(2, &inv)); // Sprinkler
    ASSERT_EQ(inv.GetItemCount("Wood"), 0);
    ASSERT_EQ(inv.GetItemCount("Stone"), 0);
    ASSERT_EQ(inv.GetItemCount("Sprinkler"), 1);
}

TEST(test_craft_failure_no_consume) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 2);
    ASSERT_FALSE(crafting.Craft(0, &inv)); // Fence needs 5
    ASSERT_EQ(inv.GetItemCount("Wood"), 2); // Materials not consumed
}

TEST(test_craft_leaves_excess_materials) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 10);
    ASSERT_TRUE(crafting.Craft(0, &inv)); // Fence: Wood x5
    ASSERT_EQ(inv.GetItemCount("Wood"), 5); // 10 - 5 = 5 remaining
    ASSERT_EQ(inv.GetItemCount("Fence"), 1);
}

TEST(test_craft_bridge_expensive) {
    Crafting crafting;
    Inventory inv;
    inv.AddItem("Wood", 20);
    inv.AddItem("Stone", 10);
    ASSERT_TRUE(crafting.Craft(7, &inv)); // Bridge
    ASSERT_EQ(inv.GetItemCount("Wood"), 0);
    ASSERT_EQ(inv.GetItemCount("Stone"), 0);
    ASSERT_EQ(inv.GetItemCount("Bridge"), 1);
}

int main() {
    std::cout << "=== Crafting Tests ===" << std::endl;
    RUN_TEST(test_recipe_count);
    RUN_TEST(test_recipe_names);
    RUN_TEST(test_can_craft_with_sufficient_materials);
    RUN_TEST(test_cannot_craft_insufficient_materials);
    RUN_TEST(test_cannot_craft_empty_inventory);
    RUN_TEST(test_cannot_craft_null_inventory);
    RUN_TEST(test_cannot_craft_invalid_index);
    RUN_TEST(test_craft_success);
    RUN_TEST(test_craft_multi_ingredient);
    RUN_TEST(test_craft_failure_no_consume);
    RUN_TEST(test_craft_leaves_excess_materials);
    RUN_TEST(test_craft_bridge_expensive);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
