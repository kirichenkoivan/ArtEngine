#include "../include/engine/Entities/material.h"

Material::Material(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath)
    : name(name), vertexShaderSource(vertexShader), fragmentShaderSource(fragmentShader), texturePath(texturePath), texture(0) {
    GLfloat defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    SetColor(defaultColor);
}

Material::Material(){

}

std::string Material::GetMaterialName() const {
    return name;
}

const char* Material::GetVertexShader() const {
    return vertexShaderSource.c_str();
}

const char* Material::GetFragmentShader() const {
    return fragmentShaderSource.c_str();
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