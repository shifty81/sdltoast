#include "Shop.h"
#include "Inventory.h"

const std::string ShopSystem::s_emptyName = "";
const std::vector<ShopItem> ShopSystem::s_emptyItems = {};

ShopSystem::ShopSystem() {
    InitShops();
}

void ShopSystem::InitShops() {
    // General Store — seeds, basic supplies
    Shop generalStore;
    generalStore.name = "General Store";
    generalStore.items = {
        {"Parsnip Seeds",  20,  0},
        {"Potato Seeds",   30,  0},
        {"Tomato Seeds",   40,  0},
        {"Wood",            0, 10},
        {"Stone",           0, 10},
        {"Fence",          50,  0},
        {"Chest",         100,  0},
        {"Parsnip",         0, 15},
        {"Potato",          0, 25},
        {"Tomato",          0, 35},
    };
    m_shops.push_back(std::move(generalStore));

    // Blacksmith — tools, ores, combat gear
    Shop blacksmith;
    blacksmith.name = "Blacksmith";
    blacksmith.items = {
        {"Copper Ore",      0, 20},
        {"Iron Ore",        0, 40},
        {"Gold Ore",        0, 80},
        {"Copper Bar",    100,  50},
        {"Iron Bar",      200, 100},
        {"Gold Bar",      400, 200},
        {"Sword Upgrade", 500,   0},
    };
    m_shops.push_back(std::move(blacksmith));

    // Tavern — food, drinks, special items
    Shop tavern;
    tavern.name = "Tavern";
    tavern.items = {
        {"Parsnip Soup",   60, 30},
        {"Baked Potato",   80, 40},
        {"Tomato Sauce",   70, 35},
        {"Energy Tonic",  150,  0},
        {"Fish Stew",     120, 60},
    };
    m_shops.push_back(std::move(tavern));
}

bool ShopSystem::IsValidShop(int shopIndex) const {
    return shopIndex >= 0 && shopIndex < static_cast<int>(m_shops.size());
}

const std::string& ShopSystem::GetShopName(int shopIndex) const {
    if (!IsValidShop(shopIndex)) return s_emptyName;
    return m_shops[shopIndex].name;
}

const std::vector<ShopItem>& ShopSystem::GetShopItems(int shopIndex) const {
    if (!IsValidShop(shopIndex)) return s_emptyItems;
    return m_shops[shopIndex].items;
}

const ShopItem* ShopSystem::GetShopItem(int shopIndex, int itemIndex) const {
    if (!IsValidShop(shopIndex)) return nullptr;
    const auto& items = m_shops[shopIndex].items;
    if (itemIndex < 0 || itemIndex >= static_cast<int>(items.size())) return nullptr;
    return &items[itemIndex];
}

const ShopItem* ShopSystem::FindItem(int shopIndex, const std::string& name) const {
    if (!IsValidShop(shopIndex)) return nullptr;
    for (const auto& item : m_shops[shopIndex].items) {
        if (item.name == name) return &item;
    }
    return nullptr;
}

bool ShopSystem::BuyItem(int shopIndex, const std::string& itemName,
                         int quantity, int& gold, Inventory* inventory) const {
    if (!inventory || quantity <= 0) return false;

    const ShopItem* item = FindItem(shopIndex, itemName);
    if (!item || item->buyPrice <= 0) return false;  // Not for sale

    int totalCost = item->buyPrice * quantity;
    if (gold < totalCost) return false;  // Can't afford

    gold -= totalCost;
    inventory->AddItem(itemName, quantity);
    return true;
}

bool ShopSystem::SellItem(int shopIndex, const std::string& itemName,
                          int quantity, int& gold, Inventory* inventory) const {
    if (!inventory || quantity <= 0) return false;

    const ShopItem* item = FindItem(shopIndex, itemName);
    if (!item || item->sellPrice <= 0) return false;  // Shop won't buy

    // Check player has enough items
    if (inventory->GetItemCount(itemName) < quantity) return false;

    if (!inventory->RemoveItem(itemName, quantity)) return false;

    gold += item->sellPrice * quantity;
    return true;
}

int ShopSystem::GetBuyPrice(int shopIndex, const std::string& itemName) const {
    const ShopItem* item = FindItem(shopIndex, itemName);
    if (!item) return 0;
    return item->buyPrice;
}

int ShopSystem::GetSellPrice(int shopIndex, const std::string& itemName) const {
    const ShopItem* item = FindItem(shopIndex, itemName);
    if (!item) return 0;
    return item->sellPrice;
}

bool ShopSystem::CanAfford(int shopIndex, const std::string& itemName,
                           int quantity, int gold) const {
    if (quantity <= 0) return false;
    const ShopItem* item = FindItem(shopIndex, itemName);
    if (!item || item->buyPrice <= 0) return false;
    return gold >= item->buyPrice * quantity;
}
