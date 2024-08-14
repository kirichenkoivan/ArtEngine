#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include "../include/engine/Graphics/renderer.h"

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
GLuint uSizeLocation, uPositionLocation, uRotationLocation;

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

    // Get the location of the uniform variables
    uSizeLocation = glGetUniformLocation(shaderProgram, "uSize");
    if (uSizeLocation == -1) {
        std::cerr << "Could not find uniform variable 'uSize'" << std::endl;
    }

    uPositionLocation = glGetUniformLocation(shaderProgram, "uPosition");
    if (uPositionLocation == -1) {
        std::cerr << "Could not find uniform variable 'uPosition'" << std::endl;
    }

    uRotationLocation = glGetUniformLocation(shaderProgram, "uRotation");
    if (uRotationLocation == -1) {
        std::cerr << "Could not find uniform variable 'uRotation'" << std::endl;
    }

    // Create VAO and VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // No need to scale vertices here, as it's already done in the constructor
    glBufferData(GL_ARRAY_BUFFER, gameObject.GetVertices().size() * sizeof(GLfloat), gameObject.GetVertices().data(), GL_STATIC_DRAW);

    GLuint positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void updateUniforms(GameObject& gameObject) {
    // Update the uniform variables with the size and position of the game object
    glUseProgram(shaderProgram);
    glUniform1f(uSizeLocation, gameObject.GetSize());
    glUniform2f(uPositionLocation, gameObject.GetPosX(), gameObject.GetPosY());
    glUniform1f(uRotationLocation, gameObject.GetRotation());
}

void RenderFrame(GameObject& gameObject) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update uniforms
    updateUniforms(gameObject);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}