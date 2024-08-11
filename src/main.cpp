#include <emscripten/emscripten.h>
#include "engine/renderer.h"
#include "../include/engine/Graphics/scene.h"
#include "../include/engine/Graphics/gameObject.h"

Scene scene;

void main_loop() {
    RenderFrame(scene);
}

int main() {
    initRenderer();

    // Создаём игровой объект
    GameObject* obj = new GameObject("Triangle");
    obj->setShader(
        R"(
            attribute vec4 a_position;
            void main() {
                gl_Position = a_position;
            }
        )",
        R"(
            void main() {
                gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Красный цвет
            }
        )"
    );

    obj->setGeometry({
        0.0f,  0.5f,  // Вершина 1
       -0.5f, -0.5f,  // Вершина 2
        0.5f, -0.5f   // Вершина 3
    });

    // Добавляем объект на сцену
    scene.addObject(obj);

    // Запускаем основной цикл
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
