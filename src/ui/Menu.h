#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

class Renderer;

class Menu {
public:
    Menu() = default;
    ~Menu() = default;

    void AddOption(const std::string& text);
    void Render(Renderer* renderer);
    
    int GetSelectedIndex() const { return m_selectedIndex; }
    void SelectNext();
    void SelectPrevious();

private:
    std::vector<std::string> m_options;
    int m_selectedIndex = 0;
};

#endif // MENU_H
