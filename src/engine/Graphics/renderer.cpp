#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include "../include/engine/Graphics/renderer.h"
#include "../include/engine/Entities/material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/libs/stb_image.h"

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

GLuint loadTexture(const std::string& filePath) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}


void initRenderer(Scene& scene) {
    // Initialize WebGL context
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(context);
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto& pair : scene.GetGameObjects()) {
        GameObject* gameObject = pair.second;

        // Load texture if specified
        GLuint texture = 0;
        if (!gameObject->GetMaterial().GetTexturePath().empty()) {
             GLuint texture1 = loadTexture(gameObject->GetMaterial().GetTexturePath());
            std::cout << "texture output " << texture1 << std::endl;
            gameObject->SetTextureID(texture1);
            std::cout << "Texture set for material: " << gameObject->GetMaterial().GetMaterialName() << " (Texture ID: " << gameObject->GetTextureID() << ")" << std::endl;
        } else {
            std::cout << "No texture path specified for material: " << gameObject->GetMaterial().GetMaterialName() << std::endl;
        }

        // Create and compile shaders, then link them into a program
        GLuint shaderProgram = createShaderProgram(gameObject->GetMaterial().GetVertexShader(), gameObject->GetMaterial().GetFragmentShader());
        gameObject->SetShaderProgram(shaderProgram);

        // Get the location of the uniform variables
        GLuint uSizeXLocation = glGetUniformLocation(shaderProgram, "uSizeX");
        GLuint uSizeYLocation = glGetUniformLocation(shaderProgram, "uSizeY");
        GLuint uPositionLocation = glGetUniformLocation(shaderProgram, "uPosition");
        GLuint uRotationLocation = glGetUniformLocation(shaderProgram, "uRotation");
        GLuint uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
        GLuint uTextureLocation = glGetUniformLocation(shaderProgram, "uTexture");
        GLuint uUseTextureLocation = glGetUniformLocation(shaderProgram, "uUseTexture");

        gameObject->SetUniformLocations(uSizeXLocation, uSizeYLocation, uPositionLocation, uRotationLocation, uColorLocation, uTextureLocation, uUseTextureLocation);

        // Create VAO and VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gameObject->GetVertices().size() * sizeof(GLfloat), gameObject->GetVertices().data(), GL_STATIC_DRAW);

        GLuint positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
        glEnableVertexAttribArray(positionAttrib);
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

        // Create VBO for texture coordinates
        GLuint texCoordVBO;
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, gameObject->GetTexCoords().size() * sizeof(GLfloat), gameObject->GetTexCoords().data(), GL_STATIC_DRAW);

        GLuint texCoordAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
        glEnableVertexAttribArray(texCoordAttrib);
        glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

        // Create EBO for indices
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, gameObject->GetIndices().size() * sizeof(GLuint), gameObject->GetIndices().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void updateUniforms(GameObject& gameObject) {
    // Update the uniform variables with the size, position, and rotation of the game object
    glUseProgram(gameObject.GetShaderProgram());
     glUniform1f(gameObject.GetUniformLocations().uSizeXLocation, gameObject.GetSizeX());
    glUniform1f(gameObject.GetUniformLocations().uSizeYLocation, gameObject.GetSizeY());
    glUniform2f(gameObject.GetUniformLocations().uPositionLocation, gameObject.GetPosX(), gameObject.GetPosY());
    glUniform1f(gameObject.GetUniformLocations().uRotationLocation, gameObject.GetRotation());

    // Update the color uniform
    GLfloat color[4];
    gameObject.GetMaterial().GetColor(color);
    glUniform4fv(gameObject.GetUniformLocations().uColorLocation, 1, color);

    // Update the texture uniform
    GLuint texture = gameObject.GetTextureID();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(gameObject.GetUniformLocations().uTextureLocation, 0);

    // Update the useTexture uniform
    bool useTexture = texture != 0;
    glUniform1i(gameObject.GetUniformLocations().uUseTextureLocation, useTexture);
}

void RenderFrame(GameObject& gameObject) {
    // Update uniforms
    updateUniforms(gameObject);

    glUseProgram(gameObject.GetShaderProgram());
    glDrawElements(GL_TRIANGLES, gameObject.GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void RenderScene(Scene& scene) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& pair : scene.GetGameObjects()) {
        GameObject* gameObject = pair.second;
        RenderFrame(*gameObject);
    }
}