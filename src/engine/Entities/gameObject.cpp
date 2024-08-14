#include "../include/engine/Entities/gameObject.h"

GameObject::GameObject(const std::string& name, const Material& material, const std::vector<GLfloat> verts)
    : name(name), material(material), vertices(verts) {}


std::string GameObject::GetName(){
    return name;
}

Material GameObject::GetMaterial(){
    return material;
}

std::vector<GLfloat> GameObject::GetVertices(){
    return vertices;
}