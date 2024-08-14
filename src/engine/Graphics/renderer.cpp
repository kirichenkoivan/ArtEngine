#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include "../include/engine/Graphics/renderer.h"

const char* vertexShaderSource = R"(
attribute vec3 aPosition;
void main() {
    gl_Position = vec4(aPosition, 1.0);
}
)";

const char* fragmentShaderSource = R"(
precision mediump float;
void main() {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
}
)";

GLuint createShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

GLuint VAO, VBO, shaderProgram;

void initRenderer(GameObject& gameObject) {
    // Initialize WebGL context
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(context);


    // Create and compile shaders, then link them into a program
    shaderProgram = createShaderProgram(gameObject.GetMaterial().GetVertexShader(), gameObject.GetMaterial().GetFragmentShader());

    // Create VAO and VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gameObject.GetVertices().size() * sizeof(GLfloat), gameObject.GetVertices().data(), GL_STATIC_DRAW);

    GLuint positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
