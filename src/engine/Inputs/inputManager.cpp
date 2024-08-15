#include "../include/engine/Inputs/inputManager.h"
#include <iostream>

void InputManager::Initialize() {
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, KeyDownCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, KeyUpCallback);
}

bool InputManager::IsKeyPressed(int keyCode) const {
    auto it = keyState.find(keyCode);
    if (it != keyState.end()) {
        return it->second;
    }
    return false;
}

EM_BOOL InputManager::KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData) {
    InputManager::GetInstance().keyState[keyEvent->keyCode] = true;
    std::cout << "Key down: " << keyEvent->key << " (code: " << keyEvent->keyCode << ")" << std::endl;
    return EM_TRUE;
}

EM_BOOL InputManager::KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData) {
    InputManager::GetInstance().keyState[keyEvent->keyCode] = false;
    std::cout << "Key up: " << keyEvent->key << " (code: " << keyEvent->keyCode << ")" << std::endl;
    return EM_TRUE;
}