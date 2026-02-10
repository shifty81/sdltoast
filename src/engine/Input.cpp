#include "Input.h"

Input::Input()
    : m_mouseX(0)
    , m_mouseY(0)
    , m_mouseButtonState(0)
{
    m_currentKeyState.fill(false);
    m_previousKeyState.fill(false);
}

void Input::ProcessEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode < NUM_KEYS) {
            m_currentKeyState[event.key.keysym.scancode] = true;
        }
    }
    else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode < NUM_KEYS) {
            m_currentKeyState[event.key.keysym.scancode] = false;
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        m_mouseButtonState = SDL_GetMouseState(nullptr, nullptr);
    }
}

void Input::Update() {
    m_previousKeyState = m_currentKeyState;
}

bool Input::IsKeyDown(SDL_Scancode key) const {
    if (key >= NUM_KEYS) return false;
    return m_currentKeyState[key];
}

bool Input::IsKeyPressed(SDL_Scancode key) const {
    if (key >= NUM_KEYS) return false;
    return m_currentKeyState[key] && !m_previousKeyState[key];
}

bool Input::IsKeyReleased(SDL_Scancode key) const {
    if (key >= NUM_KEYS) return false;
    return !m_currentKeyState[key] && m_previousKeyState[key];
}

bool Input::IsMouseButtonDown(int button) const {
    return (m_mouseButtonState & SDL_BUTTON(button)) != 0;
}
