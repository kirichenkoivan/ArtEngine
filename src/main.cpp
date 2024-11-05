#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "../include/engine/Graphics/renderer.h"
#include "../include/engine/Entities/gameObject.h"
#include "../include/engine/Entities/material.h"
#include "../include/engine/Tools/fileLoader.h"
#include "../include/engine/Entities/scene.h"
#include "../include/engine/Inputs/inputManager.h"
#include "../include/engine/Entities/Actors/dynamicActor.h"
#include "../include/engine/Tools/xmlReader.h"
#include "../include/engine/Factories/materialFactory.h"
#include "../include/engine/Entities/Colliders/collisionBox.h"
#include "../include/engine/Entities/camera.h"
#include "../include/engine/Factories/gameObjectFactory.h"
#include "../include/engine/Factories/sceneFactory.h"
#include "../include/engine/Managers/collisionsManager.h"
#include "../include/engine/Managers/scriptsManager.h"
#include "../WrenVM/include/wren.hpp"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>

Scene* scene;
SceneFactory* sceneFactory;
CollisionManager* collisionManager;
WrenConfiguration config;
WrenVM* vm;
WrenHandle* gameEngineClass;
WrenHandle* engineInstanceHandle;

void main_loop() {
    /*
    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, engineInstanceHandle); // Передаем экземпляр `Engine`

    if (wrenCall(vm, gameEngineClass) != WREN_RESULT_SUCCESS) {
        std::cerr << "Failed to call update.\n";
    }
    */

   ScriptsManager::GetInstance().UpdateScripts();

    // Пример использования deltaTime
    static float previousTime = emscripten_get_now();
    float currentTime = emscripten_get_now();
    float deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    // Обновление объектов в сцене
    for (auto& pair : scene->GetGameObjects()) {
        GameObject* gameObject = pair.second;
        gameObject->Move(deltaTime);
        gameObject->UpdateColliderPos();
        
        if (InputManager::GetInstance().IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            std::cout << "Click!" << std::endl;
        }
    }

    if (collisionManager->IsCollidesWith("obj1", "actor")) {
        std::cout << "Collision detected!\n";
    }

    // Управление камерой
    float cameraSpeed = 1.0f;
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_D)) {
        scene->GetCamera()->SetPositionX(scene->GetCamera()->GetPositionX() + cameraSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_A)) {
        scene->GetCamera()->SetPositionX(scene->GetCamera()->GetPositionX() - cameraSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_W)) {
        scene->GetCamera()->SetPositionY(scene->GetCamera()->GetPositionY() + cameraSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_S)) {
        scene->GetCamera()->SetPositionY(scene->GetCamera()->GetPositionY() - cameraSpeed * deltaTime);
    }

    RenderScene(*scene);
}
/*
void writeFn(WrenVM* vm, const char* text) {
    std::cout << text << std::endl;
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
    switch (errorType)
    {
        case WREN_ERROR_COMPILE:
            printf("[%s line %d] [Error] %s\n", module, line, msg);
            break;
        case WREN_ERROR_STACK_TRACE:
            printf("[%s line %d] in %s\n", module, line, msg);
            break;
        case WREN_ERROR_RUNTIME:
            printf("[Runtime Error] %s\n", msg);
            break;
    }
}

void InitWren() {
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
    vm = wrenNewVM(&config);

    // Загружаем Wren-скрипт
    std::string scripts = readFile("/FileSystem/testScript.wren");
    const char* module = "main";
    const char* script = scripts.c_str();

    WrenInterpretResult result = wrenInterpret(vm, module, script);

    if (result != WREN_RESULT_SUCCESS) {
        std::cerr << "Failed to load Wren script.\n";
        wrenFreeVM(vm);
        return;
    }

    // Получаем класс Engine из модуля main
    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, module, "Engine", 0);
    WrenHandle* engineClassHandle = wrenMakeCallHandle(vm, "new()");
    if (!engineClassHandle) {
        std::cerr << "Failed to create handle for new method.\n";
        wrenFreeVM(vm);
        return;
    }

    // Вызов конструктора для создания экземпляра Engine
    if (wrenCall(vm, engineClassHandle) != WREN_RESULT_SUCCESS) {
        std::cerr << "Failed to create Engine instance.\n";
        //wrenFreeHandle(vm, engineClassHandle);
        wrenFreeVM(vm);
        return;
    }

    // Получение экземпляра из возвращенного значения
    engineInstanceHandle = wrenGetSlotHandle(vm, 0);
    wrenReleaseHandle(vm, engineClassHandle); // Освобождаем handle класса, он больше не нужен

    // Создаем handle для вызова update() на экземпляре
    gameEngineClass = wrenMakeCallHandle(vm, "update()");
    if (!gameEngineClass) {
        std::cerr << "Failed to create handle for update method.\n";
        //wrenFreeHandle(vm, engineInstanceHandle);
        wrenFreeVM(vm);
        return;
    }
}
*/

int main() {
    InputManager::GetInstance().Initialize();
    ScriptsManager::GetInstance().InitScripts();
    scene = sceneFactory->CreateSceneFromXML("scene1.xml");
    collisionManager = new CollisionManager(scene);

    initRenderer(*scene);
    emscripten_set_main_loop(main_loop, 0, 1);

    delete scene;

    //wrenFreeVM(vm);
    return 0;
}
