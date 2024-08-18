#include "../include/engine/Entities/gameObject.h"

GameObject::GameObject(const std::string& name, const Material& material, const std::vector<GLfloat> verts, const std::vector<GLfloat> texCoords)
    : name(name), material(material), vertices(verts), texCoords(texCoords) {
        this->posX = 0.0;
        this->posY = 0.0;
        this->rotation = 0.0;
    }

void GameObject::Move(float deltaTime) {
    // abstract
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

std::vector<GLfloat> GameObject::GetTexCoords() {
    return texCoords;
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

GLuint GameObject::GetShaderProgram() const {
    return shaderProgram;
}

void GameObject::SetShaderProgram(GLuint shaderProgram) {
    this->shaderProgram = shaderProgram;
}

GameObject::UniformLocations GameObject::GetUniformLocations() const {
    return uniformLocations;
}

void GameObject::SetUniformLocations(GLuint uSizeLocation, GLuint uPositionLocation, GLuint uRotationLocation, GLuint uColorLocation, GLuint uTextureLocation, GLuint uUseTextureLocation) {
    uniformLocations.uSizeLocation = uSizeLocation;
    uniformLocations.uPositionLocation = uPositionLocation;
    uniformLocations.uRotationLocation = uRotationLocation;
    uniformLocations.uColorLocation = uColorLocation;
    uniformLocations.uTextureLocation = uTextureLocation;
    uniformLocations.uUseTextureLocation = uUseTextureLocation;
}

void GameObject::SetVBO(GLuint VBO) {
    this->VBO = VBO;
}

GLuint GameObject::GetVBO() const {
    return VBO;
}