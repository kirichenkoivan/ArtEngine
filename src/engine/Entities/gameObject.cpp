#include "../include/engine/Entities/gameObject.h"

GameObject::GameObject(const std::string& name, const Material& material, const std::vector<GLfloat> verts)
    : name(name), material(material), vertices(verts) {
        this->posX = 0.0;
        this->posY = 0.0;
        this->rotation = 0.0;
    }


std::string GameObject::GetName(){
    return name;
}

Material GameObject::GetMaterial(){
    return material;
}

std::vector<GLfloat> GameObject::GetVertices(){
    return vertices;
}

float GameObject::GetSize()
{
    return size;
}

void GameObject::SetSize(float newSize)
{
    size = newSize;
}

float GameObject::GetPosX(){
    return posX;
}

float GameObject::GetPosY(){
    return posY;
}

void GameObject::SetPosX(float x){
    this->posX = x;
}

void GameObject::SetPosY(float y){
    this->posY = y;
}

void GameObject::SetPos(float x, float y){
    this->posX = x;
    this->posY = y;
}

float GameObject::GetRotation(){
    return rotation;
}

void GameObject::SetRotation(float angle){
    this->rotation = angle;
}