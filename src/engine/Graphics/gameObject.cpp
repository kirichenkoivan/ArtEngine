#include "../include/engine/Graphics/gameObject.h"

GameObject::GameObject(const std::string& name)
    : name(name), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

void GameObject::setShader(const std::string& vertexShader, const std::string& fragmentShader) {
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
}

void GameObject::setGeometry(const std::vector<float>& vertices) {
    this->geometry = vertices;
}

void GameObject::setPosition(const Vector3& position) {
    this->position = position;
}

void GameObject::setRotation(const Vector3& rotation) {
    this->rotation = rotation;
}

void GameObject::setScale(const Vector3& scale) {
    this->scale = scale;
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

Matrix4 GameObject::computeModelMatrix() const {
    // Простейшая реализация для вычисления матрицы модели
    Matrix4 translation = Matrix4::translation(position);
    Matrix4 rotationX = Matrix4::rotationX(rotation.x);
    Matrix4 rotationY = Matrix4::rotationY(rotation.y);
    Matrix4 rotationZ = Matrix4::rotationZ(rotation.z);
    Matrix4 scaling = Matrix4::scaling(scale);

    return translation * (rotationX * (rotationY * (rotationZ * scaling)));
}

Matrix4 GameObject::getModelMatrix() const {
    return computeModelMatrix();
}
