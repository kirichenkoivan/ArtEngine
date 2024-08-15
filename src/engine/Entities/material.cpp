#include "../include/engine/Entities/material.h"


Material::Material(const std::string& name, const char* vertexShader, const char* fragmentShader) {
    this->name = name;
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
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