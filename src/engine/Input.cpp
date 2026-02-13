#include "Input.h"

Input::Input()
{
}

void Input::Update() {
    // Raylib handles input polling internally each frame
}

bool Input::IsKeyDown(int key) const {
    return ::IsKeyDown(key);
}

bool Input::IsKeyPressed(int key) const {
    return ::IsKeyPressed(key);
}

bool Input::IsKeyReleased(int key) const {
    return ::IsKeyReleased(key);
}

void Input::GetMousePosition(int& x, int& y) const {
    Vector2 pos = ::GetMousePosition();
    x = static_cast<int>(pos.x);
    y = static_cast<int>(pos.y);
}

bool Input::IsMouseButtonDown(int button) const {
    return ::IsMouseButtonDown(button);
}
