#include <emscripten/emscripten.h>
#include "../include/engine/Graphics/renderer.h"
#include "../include/engine/Entities/gameObject.h"
#include "../include/engine/Entities/material.h"
#include "../include/engine/Tools/fileLoader.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

GameObject* gameObject;

void main_loop() {
    static float time = 0.0f;
    time += 0.01f;

    // Move the object from -0.5 to 0.5 along the X axis
    float xPosition = 0.5f * sin(time);
    gameObject->SetPosX(xPosition);
    gameObject->SetRotation(time);
    RenderFrame(*gameObject);
}


int main() {
    const std::string name = "MyMaterial";

    std::string vertexShaderSourceStr = readFile("/shaders/vertex/vertex_shader.glsl");
    std::string fragmentShaderSourceStr = readFile("/shaders/fragment/fragment_shader.glsl");

    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    std::cout << vertexShaderSource << std::endl;
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();
    std::cout << fragmentShaderSource << std::endl;

    // Использование объекта, а не указателя
    Material mat(name, vertexShaderSource, fragmentShaderSource);
    std::vector<GLfloat> vert = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    gameObject = new GameObject(name, mat, vert);
    gameObject->SetPos(0.5, 0.0);
    gameObject->SetSize(0.5);

    initRenderer(*gameObject);
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}
