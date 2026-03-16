#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <vector>

class Inventory;

/**
 * Represents an item available in a shop.
 */
struct ShopItem {
    std::string name;
    int buyPrice;     // Price to buy from shop (0 = not for sale)
    int sellPrice;    // Price shop pays when player sells (0 = cannot sell)
};

/**
 * Shop/commerce system — buy and sell items with NPC merchants.
 * Each shop has its own inventory of items with buy/sell prices.
 */
class ShopSystem {
public:
    ShopSystem();

    // Get the number of shops
    int GetShopCount() const { return static_cast<int>(m_shops.size()); }

    // Get the name of a shop
    const std::string& GetShopName(int shopIndex) const;

    // Get items available in a shop
    const std::vector<ShopItem>& GetShopItems(int shopIndex) const;

    // Get a specific item in a shop by index
    const ShopItem* GetShopItem(int shopIndex, int itemIndex) const;

    // Find an item by name in a shop. Returns nullptr if not found.
    const ShopItem* FindItem(int shopIndex, const std::string& name) const;

    // Buy an item from a shop. Returns true on success.
    // Deducts gold and adds item to player inventory.
    bool BuyItem(int shopIndex, const std::string& itemName,
                 int quantity, int& gold, Inventory* inventory) const;

    // Sell an item to a shop. Returns true on success.
    // Removes item from inventory and adds gold.
    // sellPrice is looked up from the shop's price list.
    bool SellItem(int shopIndex, const std::string& itemName,
                  int quantity, int& gold, Inventory* inventory) const;

    // Get the buy price for an item (0 if not in shop or not for sale)
    int GetBuyPrice(int shopIndex, const std::string& itemName) const;

    // Get the sell price for an item (0 if shop won't buy it)
    int GetSellPrice(int shopIndex, const std::string& itemName) const;

    // Check if the player can afford to buy
    bool CanAfford(int shopIndex, const std::string& itemName,
                   int quantity, int gold) const;

private:
    struct Shop {
        std::string name;
        std::vector<ShopItem> items;
    };

    std::vector<Shop> m_shops;
    void InitShops();

    bool IsValidShop(int shopIndex) const;

    // Empty fallbacks for invalid indices
    static const std::string s_emptyName;
    static const std::vector<ShopItem> s_emptyItems;
};

#endif // SHOP_H
