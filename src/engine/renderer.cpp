#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "renderer.h"
#include "../include/engine/Graphics/gameObject.h"
#include "../include/engine/Graphics/scene.h"
#include <string>
#include <vector>

void initWebGL() {
    emscripten_run_script(R"(
        const canvas = document.getElementById('canvas');
        const gl = canvas.getContext('webgl');
        if (!gl) {
            console.error("WebGL is not supported");
        } else {
            console.log("WebGL context initialized");
        }
        
        window.shaderProgram = null;
        window.createShader = function(shaderSource, shaderType) {
            const gl = canvas.getContext('webgl');
            const shader = gl.createShader(shaderType === 'vertex' ? gl.VERTEX_SHADER : gl.FRAGMENT_SHADER);
            gl.shaderSource(shader, shaderSource);
            gl.compileShader(shader);
            if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
                console.error(gl.getShaderInfoLog(shader));
            }
            return shader;
        };

        window.createShaderProgram = function(vertexShaderSource, fragmentShaderSource) {
            const gl = canvas.getContext('webgl');
            const vertexShader = window.createShader(vertexShaderSource, 'vertex');
            const fragmentShader = window.createShader(fragmentShaderSource, 'fragment');
            
            const shaderProgram = gl.createProgram();
            gl.attachShader(shaderProgram, vertexShader);
            gl.attachShader(shaderProgram, fragmentShader);
            gl.linkProgram(shaderProgram);
            if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
                console.error(gl.getProgramInfoLog(shaderProgram));
            }
            gl.useProgram(shaderProgram);
            window.shaderProgram = shaderProgram;
        };
    )");
}

void initRenderer() {
    initWebGL();
}

void createShader(const std::string& shaderSource, const std::string& shaderType) {
    emscripten_run_script((
        R"(
        const shaderType = `)" + shaderType + R"(`;
        const gl = canvas.getContext('webgl');
        const shader = window.createShader(`)" + shaderSource + R"(`, shaderType);
        )"
    ).c_str());
}

void createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    emscripten_run_script((
        R"(
        window.createShaderProgram(`)" + vertexShaderSource + R"(`, `)" + fragmentShaderSource + R"(`);
        )"
    ).c_str());
}

void loadGeometry(const std::vector<float>& vertices) {
    std::string verticesString;
    for (const auto& v : vertices) {
        verticesString += std::to_string(v) + ',';
    }
    verticesString.pop_back(); // Удалить последнюю запятую

    emscripten_run_script((
        R"(
        const gl = canvas.getContext('webgl');
        const vertexBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([)" + verticesString + R"( ]), gl.STATIC_DRAW);
        gl.vertexAttribPointer(0, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(0);
        )"
    ).c_str());
}

void RenderFrame(const Scene& scene) {
    // Очищаем экран
    emscripten_run_script(R"(
        const gl = canvas.getContext('webgl');
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    )");

    for (const auto& object : scene.getObjects()) {
        // Загружаем шейдеры и создаем программу шейдера
        std::string vertexShader = object->getVertexShader();
        std::string fragmentShader = object->getFragmentShader();
        createShaderProgram(vertexShader, fragmentShader);

        // Загружаем геометрию
        std::vector<float> geometry = object->getGeometry();
        loadGeometry(geometry);

        // Получаем модельную матрицу
        Matrix4 modelMatrix = object->getModelMatrix();

        // Устанавливаем матрицу модели в шейдер
        emscripten_run_script((
            R"(
            const gl = canvas.getContext('webgl');
            const shaderProgram = window.shaderProgram;
            const modelMatrix = new Float32Array([
                `)" + std::to_string(modelMatrix.m[0]) + R"(`,
                `)" + std::to_string(modelMatrix.m[1]) + R"(`,
                `)" + std::to_string(modelMatrix.m[2]) + R"(`,
                `)" + std::to_string(modelMatrix.m[3]) + R"(`,
                `)" + std::to_string(modelMatrix.m[4]) + R"(`,
                `)" + std::to_string(modelMatrix.m[5]) + R"(`,
                `)" + std::to_string(modelMatrix.m[6]) + R"(`,
                `)" + std::to_string(modelMatrix.m[7]) + R"(`,
                `)" + std::to_string(modelMatrix.m[8]) + R"(`,
                `)" + std::to_string(modelMatrix.m[9]) + R"(`,
                `)" + std::to_string(modelMatrix.m[10]) + R"(`,
                `)" + std::to_string(modelMatrix.m[11]) + R"(`,
                `)" + std::to_string(modelMatrix.m[12]) + R"(`,
                `)" + std::to_string(modelMatrix.m[13]) + R"(`,
                `)" + std::to_string(modelMatrix.m[14]) + R"(`,
                `)" + std::to_string(modelMatrix.m[15]) + R"(` 
            ]);
            gl.uniformMatrix4fv(gl.getUniformLocation(shaderProgram, 'uModelMatrix'), false, modelMatrix);
            gl.drawArrays(gl.TRIANGLES, 0, `)" + std::to_string(geometry.size() / 3) + R"(`);
            )"
        ).c_str());
    }
}
