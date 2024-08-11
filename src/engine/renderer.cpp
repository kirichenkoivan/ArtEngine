#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "renderer.h"
#include <string>

void initWebGL() {
    emscripten_run_script(R"(
        const canvas = document.getElementById('canvas');
        const gl = canvas.getContext('webgl');
        if (!gl) {
            console.error("WebGL is not supported");
        } else {
            console.log("WebGL context initialized");
        }

        // Глобально объявляем shaderProgram
        window.shaderProgram = null;
    )");
}

void initRenderer() {
    initWebGL();
}

void RenderFrame(const Scene& scene) {
    // Очищаем экран
    emscripten_run_script(R"(
        const gl = Module.canvas.getContext('webgl');
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    )");

    // Проходим по всем объектам на сцене и отрисовываем их
    for (const auto& object : scene.getObjects()) {
        // Загружаем шейдеры
        std::string vertexShader = object->getVertexShader();
        std::string fragmentShader = object->getFragmentShader();
        
        emscripten_run_script((
            R"(
            const vertexShaderSource = `)" + vertexShader + R"(`;
            const fragmentShaderSource = `)" + fragmentShader + R"(`;

            const gl = Module.canvas.getContext('webgl');

            const vertexShader = gl.createShader(gl.VERTEX_SHADER);
            gl.shaderSource(vertexShader, vertexShaderSource);
            gl.compileShader(vertexShader);

            const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
            gl.shaderSource(fragmentShader, fragmentShaderSource);
            gl.compileShader(fragmentShader);

            // Используем глобальную переменную для shaderProgram
            shaderProgram = gl.createProgram();
            gl.attachShader(shaderProgram, vertexShader);
            gl.attachShader(shaderProgram, fragmentShader);
            gl.linkProgram(shaderProgram);
            gl.useProgram(shaderProgram);
            )"
        ).c_str());

        // Загружаем геометрию и отрисовываем
        std::vector<float> geometry = object->getGeometry();
        if (!geometry.empty()) {
            std::string verticesArray = "const vertices = new Float32Array([";
            for (size_t i = 0; i < geometry.size(); ++i) {
                verticesArray += std::to_string(geometry[i]);
                if (i < geometry.size() - 1) {
                    verticesArray += ",";
                }
            }
            verticesArray += "]);";

            emscripten_run_script((
                verticesArray +
                R"(
                const gl = Module.canvas.getContext('webgl');
                
                const buffer = gl.createBuffer();
                gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
                gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

                const position = gl.getAttribLocation(shaderProgram, 'a_position');
                gl.enableVertexAttribArray(position);
                gl.vertexAttribPointer(position, 2, gl.FLOAT, false, 0, 0);

                gl.drawArrays(gl.TRIANGLES, 0, vertices.length / 2);
            )"
            ).c_str());
        }
    }
}
