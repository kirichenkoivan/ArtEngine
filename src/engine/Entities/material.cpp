#include "../include/engine/Entities/material.h"

Material::Material(const std::string& name, const char* vertexShader, const char* fragmentShader, const std::string& texturePath)
    : name(name), vertexShader(vertexShader), fragmentShader(fragmentShader), texturePath(texturePath), texture(0) {
    GLfloat defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    SetColor(defaultColor);
}

std::string Material::GetMaterialName() const {
    return name;
}

const char* Material::GetVertexShader() const {
    return vertexShader;
}

const char* Material::GetFragmentShader() const {
    return fragmentShader;
}

GLuint Material::GetTexture() const {
    return texture;
}

void Material::SetTexture(GLuint texture) {
    this->texture = texture;
}

std::string Material::GetTexturePath() const {
    return texturePath;
}

void Material::SetTexturePath(const std::string& texturePath) {
    this->texturePath = texturePath;
}

void Material::GetColor(GLfloat color[4]) const {
    for (int i = 0; i < 4; ++i) {
        color[i] = this->color[i];
    }
}

void Material::SetColor(const GLfloat color[4]) {
    for (int i = 0; i < 4; ++i) {
        this->color[i] = color[i];
    }
}