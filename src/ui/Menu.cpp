#include "Menu.h"
#include "../engine/Renderer.h"

void Menu::AddOption(const std::string& text) {
    m_options.push_back(text);
}

void Menu::Render(Renderer* renderer) {
    // TODO: Render menu options with selection
}

void Menu::SelectNext() {
    if (!m_options.empty()) {
        m_selectedIndex = (m_selectedIndex + 1) % m_options.size();
    }
}

void Menu::SelectPrevious() {
    if (!m_options.empty()) {
        m_selectedIndex = (m_selectedIndex - 1 + m_options.size()) % m_options.size();
    }
}
