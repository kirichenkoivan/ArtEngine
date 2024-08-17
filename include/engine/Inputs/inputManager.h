#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <emscripten/html5.h>
#include "KeyCodes/keyboard.h"
#include "KeyCodes/mouse.h"

class InputManager {
public:
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    void Initialize();
    bool IsKeyPressed(int keyCode) const;
    bool IsMouseButtonPressed(int button) const;
    std::pair<int, int> GetMousePosition() const;

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<int, bool> keyState;
    std::unordered_map<int, bool> mouseState;
    int mouseX = 0;
    int mouseY = 0;

    static EM_BOOL KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
    static EM_BOOL KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
    static EM_BOOL MouseDownCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
    static EM_BOOL MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
    static EM_BOOL MouseMoveCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
};

#endif // INPUTMANAGER_H