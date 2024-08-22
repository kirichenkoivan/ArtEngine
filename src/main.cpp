#include <emscripten/emscripten.h>
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
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

GameObject* gameObject;
DynamicActor* actor;
Scene* scene;

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
            GLfloat color1[4] = {1.0f, 0.0f, 0.0f, 1.0f};
            gameObject->GetMaterial().SetColor(color1);
        }
    }

    auto gameObjects = scene->GetGameObjects();
    for (auto it1 = gameObjects.begin(); it1 != gameObjects.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != gameObjects.end(); ++it2) {
            GameObject* obj1 = it1->second;
            GameObject* obj2 = it2->second;
            if (obj1->GetCollider().Intersects(obj2->GetCollider())) {
                std::cout << "Collision detected between " << obj1->GetName() << " and " << obj2->GetName() << std::endl;
                // Здесь можно добавить обработку коллизии, например, изменение состояния объектов
            }
        }
    }

    RenderScene(*scene);
}

int main() {
    InputManager::GetInstance().Initialize();
    scene = new Scene();

    const std::string name = "MyMaterial";
    std::string name2 = "MyMaterial2";

    std::string vertexShaderSourceStr = readFile("/shaders/vertex/vertex_shader.glsl");
    std::string fragmentShaderSourceStr = readFile("/shaders/fragment/fragment_shader.glsl");

    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    Material mat = MaterialFactory::CreateMaterialFromXML("mat1.xml");
    Material mat2 = MaterialFactory::CreateMaterialFromXML("mat2.xml");

    GLfloat color1[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat color2[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    mat2.SetColor(color2);

    std::vector<GLfloat> vertices = {
    -0.5f, -0.5f, -1.0f, // Bottom-left
     0.5f, -0.5f, -1.0f, // Bottom-right
     0.5f,  0.5f, -1.0f, // Top-right
    -0.5f,  0.5f, -1.0f  // Top-left
};


    std::vector<GLfloat> texCoords = {
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f  // Top-left
    };

    std::vector<GLuint> indices = {
        0, 1, 2, // Первый треугольник
        2, 3, 0  // Второй треугольник
    };

    CollisionBox coll1(0.5f, 0.0f, 0.5, 0.5);
    GameObject* obj = new GameObject("obj1", mat, vertices, texCoords, indices);
    obj->SetSizeX(-0.5f);
    obj->SetSizeY(-0.5f);
    obj->SetPos(0.5f, 0.0f); 
    obj->SetPosZ(0.11f);// Устанавливаем глубину
    obj->SetRotation(0.0f);
    obj->SetCollider(coll1);  // Set rotation to 0.0
    scene->AddGameObject(obj);

    CollisionBox coll2(0.0f, 0.0f, 0.5f, 0.25f);
    actor = new DynamicActor("actor", mat2, vertices, texCoords, indices);
    actor->SetSizeX(0.5f);
    actor->SetSizeY(0.25f);
    actor->SetPos(0.0f, 0.0f);
    actor->SetPosZ(0.1f); // Устанавливаем другую глубину
    actor->SetRotation(0.0f);
    actor->SetCollider(coll2);
    scene->AddGameObject(actor);

    initRenderer(*scene);
    emscripten_set_main_loop(main_loop, 0, 1);

    delete scene;
    delete obj;

    return 0;
}