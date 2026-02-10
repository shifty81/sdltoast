#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <array>

/**
 * Input handling system for keyboard and gamepad
 */
class Input {
public:
    Input();
    ~Input() = default;

    void ProcessEvent(const SDL_Event& event);
    void Update();

    // Keyboard
    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyReleased(SDL_Scancode key) const;

    // Mouse
    void GetMousePosition(int& x, int& y) const { x = m_mouseX; y = m_mouseY; }
    bool IsMouseButtonDown(int button) const;

private:
    static constexpr int NUM_KEYS = 512;
    
    std::array<bool, NUM_KEYS> m_currentKeyState;
    std::array<bool, NUM_KEYS> m_previousKeyState;
    
    int m_mouseX, m_mouseY;
    Uint32 m_mouseButtonState;
};

#endif // INPUT_H
