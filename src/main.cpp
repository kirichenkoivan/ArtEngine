#include <emscripten/emscripten.h>
#include "engine/renderer.h"
#include "../include/engine/Graphics/objectManager.h"

Scene scene;
ObjectManager objectManager(scene);

void main_loop() {
    // Обновление и отрисовка сцены
    RenderFrame(scene);
}

int main() {
    initRenderer();

    // Создание игрового объекта
    GameObject* obj = objectManager.createObject("Object");

    // Настройка шейдеров
    std::string vertexShader = R"(
        attribute vec3 aPosition;
        uniform mat4 uModelMatrix;
        void main() {
            gl_Position = uModelMatrix * vec4(aPosition, 1.0);
        }
    )";

    std::string fragmentShader = R"(
        void main() {
            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    obj->setShader(vertexShader, fragmentShader);

    // Настройка геометрии
    std::vector<float> vertices = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    obj->setGeometry(vertices);

    // Настройка трансформации
    obj->setPosition(Vector3(0.0f, 0.0f, 0.0f));
    obj->setRotation(Vector3(0.0f, 0.0f, 0.0f));
    obj->setScale(Vector3(1.0f, 1.0f, 1.0f));

    // Добавляем объект в сцену
    scene.addObject(obj);
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}
