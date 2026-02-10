#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>

struct Item {
    std::string name;
    int quantity;
    int value;
};

class Inventory {
public:
    void AddItem(const std::string& name, int quantity);
    bool RemoveItem(const std::string& name, int quantity);
    int GetItemCount(const std::string& name) const;

private:
    std::vector<Item> m_items;
    static constexpr int MAX_SLOTS = 36;
};

#endif // INVENTORY_H
