#include "Inventory.h"

void Inventory::AddItem(const std::string& name, int quantity) {
    // Check if item already exists in inventory
    for (auto& item : m_items) {
        if (item.name == name) {
            item.quantity += quantity;
            return;
        }
    }
    // Add new item if there's room
    if (static_cast<int>(m_items.size()) < MAX_SLOTS) {
        Item newItem;
        newItem.name = name;
        newItem.quantity = quantity;
        newItem.value = 0;
        m_items.push_back(newItem);
    }
}

bool Inventory::RemoveItem(const std::string& name, int quantity) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (it->name == name) {
            if (it->quantity < quantity) return false;
            it->quantity -= quantity;
            if (it->quantity <= 0) {
                m_items.erase(it);
            }
            return true;
        }
    }
    return false;
}

int Inventory::GetItemCount(const std::string& name) const {
    for (const auto& item : m_items) {
        if (item.name == name) {
            return item.quantity;
        }
    }
    return 0;
}
