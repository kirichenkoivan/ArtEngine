#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "renderer.h"

void initWebGL() {
    emscripten_run_script(R"(
        const canvas = document.getElementById('canvas');
        const gl = canvas.getContext('webgl');
        if (!gl) {
            console.error("WebGL is not supported");
        } else {
            console.log("WebGL context initialized");
        }
    )");
}

void initRenderer() {
    initWebGL();
}

void RenderFrame() {
    // Логика отрисовки кадра
    emscripten_run_script(R"(
        const gl = Module.canvas.getContext('webgl');
        gl.clearColor(1.0, 0.0, 0.0, 1.0); // Устанавливаем красный цвет
        gl.clear(gl.COLOR_BUFFER_BIT);
    )");
}