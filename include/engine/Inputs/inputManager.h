#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <emscripten/html5.h>
#include "KeyCodes/keyboard.h"

class InputManager {
public:
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    void Initialize();
    bool IsKeyPressed(int keyCode) const;

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<int, bool> keyState;

    static EM_BOOL KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
    static EM_BOOL KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
};

#endif // INPUTMANAGER_H