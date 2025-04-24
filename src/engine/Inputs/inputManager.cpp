#include "../include/engine/Inputs/inputManager.h"
#include <iostream>

void InputManager::Initialize()
{
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, KeyDownCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, KeyUpCallback);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, MouseDownCallback);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, MouseUpCallback);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, MouseMoveCallback);
}

bool InputManager::IsKeyPressed(int keyCode) const
{
    auto it = keyState.find(keyCode);
    if (it != keyState.end())
    {
        return it->second;
    }
    return false;
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    auto it = mouseState.find(button);
    return it != mouseState.end() && it->second;
}

std::pair<int, int> InputManager::GetMousePosition() const
{
    return std::make_pair(mouseX, mouseY);
}

EM_BOOL InputManager::KeyDownCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    InputManager::GetInstance().keyState[keyEvent->keyCode] = true;
    std::cout << "Key down: " << keyEvent->key << " (code: " << keyEvent->keyCode << ")" << std::endl;
    return EM_TRUE;
}

EM_BOOL InputManager::KeyUpCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    InputManager::GetInstance().keyState[keyEvent->keyCode] = false;
    std::cout << "Key up: " << keyEvent->key << " (code: " << keyEvent->keyCode << ")" << std::endl;
    return EM_TRUE;
}

EM_BOOL InputManager::MouseDownCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    InputManager::GetInstance().mouseState[mouseEvent->button] = true;
    return EM_TRUE;
}

EM_BOOL InputManager::MouseUpCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    InputManager::GetInstance().mouseState[mouseEvent->button] = false;
    return EM_TRUE;
}

EM_BOOL InputManager::MouseMoveCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    InputManager::GetInstance().mouseX = mouseEvent->clientX;
    InputManager::GetInstance().mouseY = mouseEvent->clientY;
    return EM_TRUE;
}