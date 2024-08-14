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
