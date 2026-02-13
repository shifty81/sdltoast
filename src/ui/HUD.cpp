#include "HUD.h"
#include "../engine/Renderer.h"
#include <string>
#include <vector>

void HUD::SetDayInfo(int day, const std::string& season, int year) {
    m_day = day;
    m_season = season;
    m_year = year;
}

void HUD::AddInventoryLine(const std::string& line) {
    m_inventoryLines.push_back(line);
}

void HUD::ClearInventoryLines() {
    m_inventoryLines.clear();
}

void HUD::Render(Renderer* renderer) {
    // Draw health hearts
    for (int i = 0; i < m_maxHealth; ++i) {
        unsigned char r = (i < m_currentHealth) ? 255 : 100;
        renderer->FillRect(10 + i * 25, 10, 20, 20, r, 50, 50);
    }
    
    // Draw gold count
    std::string goldText = "Gold: " + std::to_string(m_gold);
    renderer->DrawGameText(goldText.c_str(), 10, 38, 16, 255, 215, 0);

    // Draw day/season info (top-right)
    std::string dayText = m_season + " " + std::to_string(m_day) + ", Year " + std::to_string(m_year);
    renderer->DrawGameText(dayText.c_str(), 600, 10, 16, 255, 255, 255);

    // Draw action text (bottom-center)
    if (!m_actionText.empty()) {
        renderer->DrawGameText(m_actionText.c_str(), 250, 570, 16, 255, 255, 200);
    }

    // Draw inventory overlay if open
    if (m_showInventory) {
        // Semi-transparent background
        renderer->FillRect(150, 80, 500, 440, 20, 20, 40, 200);
        renderer->DrawRect(150, 80, 500, 440, 200, 200, 200);
        renderer->DrawGameText("INVENTORY", 330, 90, 20, 255, 255, 255);

        int yOffset = 120;
        for (const auto& line : m_inventoryLines) {
            renderer->DrawGameText(line.c_str(), 170, yOffset, 16, 220, 220, 220);
            yOffset += 22;
        }
        if (m_inventoryLines.empty()) {
            renderer->DrawGameText("(empty)", 170, yOffset, 16, 150, 150, 150);
        }
    }
}
