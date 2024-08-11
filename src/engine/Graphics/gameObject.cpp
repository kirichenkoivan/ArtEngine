#include "../include/engine/Graphics/gameObject.h"

GameObject::GameObject(const std::string& name)
    : name(name), position{0.0f, 0.0f, 0.0f} {}

void GameObject::setShader(const std::string& vertexShader, const std::string& fragmentShader) {
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
}

void GameObject::setGeometry(const std::vector<float>& vertices) {
    this->geometry = vertices;
}

std::string GameObject::getVertexShader() const {
    return vertexShader;
}

std::string GameObject::getFragmentShader() const {
    return fragmentShader;
}

std::vector<float> GameObject::getGeometry() const {
    return geometry;
}
