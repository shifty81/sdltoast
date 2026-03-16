// Harvest Quest — Shop/Commerce system unit tests

#include "systems/Shop.h"
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

TEST(test_shop_count) {
    ShopSystem shop;
    ASSERT_EQ(shop.GetShopCount(), 3);  // General Store, Blacksmith, Tavern
}

TEST(test_shop_names) {
    ShopSystem shop;
    ASSERT_EQ(shop.GetShopName(0), "General Store");
    ASSERT_EQ(shop.GetShopName(1), "Blacksmith");
    ASSERT_EQ(shop.GetShopName(2), "Tavern");
}

TEST(test_shop_invalid_index) {
    ShopSystem shop;
    ASSERT_TRUE(shop.GetShopName(-1).empty());
    ASSERT_TRUE(shop.GetShopName(99).empty());
    ASSERT_TRUE(shop.GetShopItems(-1).empty());
    ASSERT_TRUE(shop.GetShopItem(-1, 0) == nullptr);
    ASSERT_TRUE(shop.GetShopItem(0, -1) == nullptr);
    ASSERT_TRUE(shop.GetShopItem(0, 999) == nullptr);
}

TEST(test_general_store_has_items) {
    ShopSystem shop;
    const auto& items = shop.GetShopItems(0);
    ASSERT_TRUE(!items.empty());
    // General store should have seeds
    ASSERT_TRUE(shop.FindItem(0, "Parsnip Seeds") != nullptr);
    ASSERT_TRUE(shop.FindItem(0, "Potato Seeds") != nullptr);
    ASSERT_TRUE(shop.FindItem(0, "Tomato Seeds") != nullptr);
}

TEST(test_blacksmith_has_items) {
    ShopSystem shop;
    const auto& items = shop.GetShopItems(1);
    ASSERT_TRUE(!items.empty());
    ASSERT_TRUE(shop.FindItem(1, "Copper Bar") != nullptr);
    ASSERT_TRUE(shop.FindItem(1, "Iron Bar") != nullptr);
}

TEST(test_tavern_has_items) {
    ShopSystem shop;
    const auto& items = shop.GetShopItems(2);
    ASSERT_TRUE(!items.empty());
    ASSERT_TRUE(shop.FindItem(2, "Parsnip Soup") != nullptr);
    ASSERT_TRUE(shop.FindItem(2, "Energy Tonic") != nullptr);
}

TEST(test_buy_item_success) {
    ShopSystem shop;
    Inventory inv;
    int gold = 1000;

    // Buy 2 Parsnip Seeds (20g each)
    bool result = shop.BuyItem(0, "Parsnip Seeds", 2, gold, &inv);
    ASSERT_TRUE(result);
    ASSERT_EQ(gold, 960);  // 1000 - 40
    ASSERT_EQ(inv.GetItemCount("Parsnip Seeds"), 2);
}

TEST(test_buy_item_insufficient_gold) {
    ShopSystem shop;
    Inventory inv;
    int gold = 10;

    // Try to buy Parsnip Seeds (20g) with only 10g
    bool result = shop.BuyItem(0, "Parsnip Seeds", 1, gold, &inv);
    ASSERT_FALSE(result);
    ASSERT_EQ(gold, 10);  // Gold unchanged
    ASSERT_EQ(inv.GetItemCount("Parsnip Seeds"), 0);
}

TEST(test_buy_item_not_for_sale) {
    ShopSystem shop;
    Inventory inv;
    int gold = 1000;

    // Wood has buyPrice = 0 (sell only), can't buy it
    bool result = shop.BuyItem(0, "Wood", 1, gold, &inv);
    ASSERT_FALSE(result);
    ASSERT_EQ(gold, 1000);
}

TEST(test_buy_item_invalid_quantity) {
    ShopSystem shop;
    Inventory inv;
    int gold = 1000;

    ASSERT_FALSE(shop.BuyItem(0, "Parsnip Seeds", 0, gold, &inv));
    ASSERT_FALSE(shop.BuyItem(0, "Parsnip Seeds", -1, gold, &inv));
    ASSERT_EQ(gold, 1000);
}

TEST(test_buy_item_null_inventory) {
    ShopSystem shop;
    int gold = 1000;
    ASSERT_FALSE(shop.BuyItem(0, "Parsnip Seeds", 1, gold, nullptr));
    ASSERT_EQ(gold, 1000);
}

TEST(test_sell_item_success) {
    ShopSystem shop;
    Inventory inv;
    int gold = 0;

    // Add wood to inventory, then sell it
    inv.AddItem("Wood", 10);
    bool result = shop.SellItem(0, "Wood", 5, gold, &inv);
    ASSERT_TRUE(result);
    ASSERT_EQ(gold, 50);  // 5 * 10g each
    ASSERT_EQ(inv.GetItemCount("Wood"), 5);
}

TEST(test_sell_item_insufficient_inventory) {
    ShopSystem shop;
    Inventory inv;
    int gold = 0;

    inv.AddItem("Wood", 3);
    bool result = shop.SellItem(0, "Wood", 5, gold, &inv);
    ASSERT_FALSE(result);
    ASSERT_EQ(gold, 0);
    ASSERT_EQ(inv.GetItemCount("Wood"), 3);
}

TEST(test_sell_item_not_buyable_by_shop) {
    ShopSystem shop;
    Inventory inv;
    int gold = 0;

    // Parsnip Seeds has sellPrice = 0, shop won't buy them back
    inv.AddItem("Parsnip Seeds", 5);
    bool result = shop.SellItem(0, "Parsnip Seeds", 1, gold, &inv);
    ASSERT_FALSE(result);
    ASSERT_EQ(gold, 0);
}

TEST(test_sell_item_null_inventory) {
    ShopSystem shop;
    int gold = 0;
    ASSERT_FALSE(shop.SellItem(0, "Wood", 1, gold, nullptr));
}

TEST(test_get_buy_price) {
    ShopSystem shop;
    ASSERT_EQ(shop.GetBuyPrice(0, "Parsnip Seeds"), 20);
    ASSERT_EQ(shop.GetBuyPrice(0, "Potato Seeds"), 30);
    ASSERT_EQ(shop.GetBuyPrice(0, "Wood"), 0);  // Not for sale
    ASSERT_EQ(shop.GetBuyPrice(0, "Nonexistent"), 0);
}

TEST(test_get_sell_price) {
    ShopSystem shop;
    ASSERT_EQ(shop.GetSellPrice(0, "Wood"), 10);
    ASSERT_EQ(shop.GetSellPrice(0, "Stone"), 10);
    ASSERT_EQ(shop.GetSellPrice(0, "Parsnip"), 15);
    ASSERT_EQ(shop.GetSellPrice(0, "Parsnip Seeds"), 0);  // Can't sell seeds back
}

TEST(test_can_afford) {
    ShopSystem shop;
    ASSERT_TRUE(shop.CanAfford(0, "Parsnip Seeds", 1, 100));
    ASSERT_TRUE(shop.CanAfford(0, "Parsnip Seeds", 5, 100));  // 5*20=100
    ASSERT_FALSE(shop.CanAfford(0, "Parsnip Seeds", 6, 100)); // 6*20=120 > 100
    ASSERT_FALSE(shop.CanAfford(0, "Parsnip Seeds", 0, 100)); // Invalid quantity
    ASSERT_FALSE(shop.CanAfford(0, "Wood", 1, 100));           // Not for sale
}

TEST(test_buy_sell_roundtrip) {
    ShopSystem shop;
    Inventory inv;
    int gold = 200;

    // Buy from tavern: Parsnip Soup at 60g
    ASSERT_TRUE(shop.BuyItem(2, "Parsnip Soup", 2, gold, &inv));
    ASSERT_EQ(gold, 80);  // 200 - 120
    ASSERT_EQ(inv.GetItemCount("Parsnip Soup"), 2);

    // Sell back to tavern: Parsnip Soup at 30g
    ASSERT_TRUE(shop.SellItem(2, "Parsnip Soup", 1, gold, &inv));
    ASSERT_EQ(gold, 110);  // 80 + 30
    ASSERT_EQ(inv.GetItemCount("Parsnip Soup"), 1);
}

TEST(test_find_item_nonexistent) {
    ShopSystem shop;
    ASSERT_TRUE(shop.FindItem(0, "Magic Wand") == nullptr);
    ASSERT_TRUE(shop.FindItem(1, "Parsnip Seeds") == nullptr);  // Not in blacksmith
}

int main() {
    std::cout << "=== Shop Tests ===" << std::endl;
    RUN_TEST(test_shop_count);
    RUN_TEST(test_shop_names);
    RUN_TEST(test_shop_invalid_index);
    RUN_TEST(test_general_store_has_items);
    RUN_TEST(test_blacksmith_has_items);
    RUN_TEST(test_tavern_has_items);
    RUN_TEST(test_buy_item_success);
    RUN_TEST(test_buy_item_insufficient_gold);
    RUN_TEST(test_buy_item_not_for_sale);
    RUN_TEST(test_buy_item_invalid_quantity);
    RUN_TEST(test_buy_item_null_inventory);
    RUN_TEST(test_sell_item_success);
    RUN_TEST(test_sell_item_insufficient_inventory);
    RUN_TEST(test_sell_item_not_buyable_by_shop);
    RUN_TEST(test_sell_item_null_inventory);
    RUN_TEST(test_get_buy_price);
    RUN_TEST(test_get_sell_price);
    RUN_TEST(test_can_afford);
    RUN_TEST(test_buy_sell_roundtrip);
    RUN_TEST(test_find_item_nonexistent);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
