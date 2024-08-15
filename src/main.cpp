#include <emscripten/emscripten.h>
#include "../include/engine/Graphics/renderer.h"
#include "../include/engine/Entities/gameObject.h"
#include "../include/engine/Entities/material.h"
#include "../include/engine/Tools/fileLoader.h"
#include "../include/engine/Entities/scene.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

GameObject* gameObject;
Scene* scene;

void main_loop() {
    static float time = 0.0f;
    time += 0.01f; // Увеличиваем время для анимации

    for (auto& pair : scene->GetGameObjects()) {
        GameObject* gameObject = pair.second;
        if (gameObject->GetName() == "obj1") {
            // Перемещение объекта от -0.5 до 0.5
            float posX = 0.5f * sin(time);
            gameObject->SetPos(posX, gameObject->GetPosY());
        } else if (gameObject->GetName() == "obj2") {
            // Вращение объекта
            gameObject->SetRotation(time);
        }
    }
    RenderScene(*scene);
}


int main() {
    scene = new Scene();
    const std::string name = "MyMaterial";
    const std::string name2 = "MyMaterial2";

    std::string vertexShaderSourceStr = readFile("/shaders/vertex/vertex_shader.glsl");
    std::string fragmentShaderSourceStr = readFile("/shaders/fragment/fragment_shader.glsl");

    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    Material mat(name, vertexShaderSource, fragmentShaderSource);
    Material mat2(name2, vertexShaderSource, fragmentShaderSource);

    GLfloat color1[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat color2[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    mat.SetColor(color1);
    mat2.SetColor(color2);
    std::vector<GLfloat> vert = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    GameObject* obj = new GameObject("obj1", mat, vert);
    obj->SetSize(0.5f);
    obj->SetPos(0.5f, 0.0f); // Set position to (0.0, 0.0)
    obj->SetRotation(0.0f);  // Set rotation to 0.0
    scene->AddGameObject(obj);

    GameObject* obj1 = new GameObject("obj2", mat2, vert);
    obj1->SetSize(0.5f);
    obj1->SetPos(-0.5f, 0.0f); // Set position to (0.0, 0.0)
    obj1->SetRotation(0.0f);  // Set rotation to 0.0
    scene->AddGameObject(obj1);

    initRenderer(*scene);
    emscripten_set_main_loop(main_loop, 0, 1);

    delete scene;
    delete obj;
    delete obj1;

    return 0;
}
