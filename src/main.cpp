#include <emscripten/emscripten.h>
#include "../include/engine/Graphics/renderer.h"
#include "../include/engine/Entities/gameObject.h"
#include "../include/engine/Entities/material.h"
#include "../include/engine/Tools/fileLoader.h"
#include "../include/engine/Entities/scene.h"
#include "../include/engine/Inputs/inputManager.h"
#include "../include/engine/Entities/Actors/dynamicActor.h"
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
    }
    RenderScene(*scene);
}

int main() {
    InputManager::GetInstance().Initialize();
    scene = new Scene();
    const std::string name = "MyMaterial";
    const std::string name2 = "MyMaterial2";

    std::string vertexShaderSourceStr = readFile("/shaders/vertex/vertex_shader.glsl");
    std::string fragmentShaderSourceStr = readFile("/shaders/fragment/fragment_shader.glsl");

    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    Material mat(name, vertexShaderSource, fragmentShaderSource, "/textures/amog.png");
    Material mat2(name2, vertexShaderSource, fragmentShaderSource);

    GLfloat color1[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat color2[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    mat2.SetColor(color2);

    std::vector<GLfloat> vert = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 
        0.0f,  0.5f, 0.0f, 
    };

    std::vector<GLfloat> texCoords = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    GameObject* obj = new GameObject("obj1", mat, vert, texCoords);
    obj->SetSize(0.5f);
    obj->SetPos(0.5f, 0.0f); // Set position to (0.0, 0.0)
    obj->SetRotation(0.0f);  // Set rotation to 0.0
    scene->AddGameObject(obj);

    actor = new DynamicActor("actor", mat2, vert, texCoords);
    actor->SetSize(-0.5f);
    actor->SetPos(0.0f, 0.0f);
    actor->SetRotation(0.0f);
    scene->AddGameObject(actor);

    initRenderer(*scene);
    emscripten_set_main_loop(main_loop, 0, 1);

    delete scene;
    delete obj;

    return 0;
}