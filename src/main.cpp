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
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Scene* scene;
SceneFactory* sceneFactory;
CollisionManager* collisionManager;


void main_loop() {
    static float previousTime = emscripten_get_now();
    float currentTime = emscripten_get_now();
    float deltaTime = (currentTime - previousTime) / 1000.0f; // Преобразование в секунды
    previousTime = currentTime;

    // Обновление состояния объекта
    for (auto& pair : scene->GetGameObjects()) {
        GameObject* gameObject = pair.second;
        gameObject->Move(deltaTime);
        gameObject->UpdateColliderPos();
        if (InputManager::GetInstance().IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            std::cout << "Click!" << std::endl;
        }
    }

    if (collisionManager->IsCollidesWith("obj1", "actor")){
        std::cout << "bruh moment" << std::endl;
    }

float cameraSpeed = 1.0f; // Скорость перемещения камеры
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_D)) {
        scene->GetCamera()->SetPositionX(scene->GetCamera()->GetPositionX() + cameraSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_A)) {
        scene->GetCamera()->SetPositionX(scene->GetCamera()->GetPositionX() - cameraSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_G)) {
        scene->RemoveGameObject("obj1");
    }

    RenderScene(*scene);
}

int main() {

    InputManager::GetInstance().Initialize();

    scene = sceneFactory->CreateSceneFromXML("scene1.xml");
    collisionManager = new CollisionManager(scene);

    initRenderer(*scene);
    emscripten_set_main_loop(main_loop, 0, 1);

    delete scene;

    return 0;
}