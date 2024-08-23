#include "../include/engine/Entities/gameObject.h"

GameObject::GameObject(const std::string& name, Material* material, const std::vector<GLfloat> verts, const std::vector<GLfloat> texCoords, const std::vector<GLuint> indices)
    : name(name), material(material), vertices(verts), texCoords(texCoords), indices(indices) {
        this->posX = 0.0;
        this->posY = 0.0;
        this->rotation = 0.0;
        this->sizeX = 1.0;
        this->sizeY = 1.0;
    }


void GameObject::Move(float deltaTime) {
    // abstract
}

std::string GameObject::GetName(){
    return name;
}

Material* GameObject::GetMaterial(){
    return material;
}

std::vector<GLfloat> GameObject::GetVertices(){
    return vertices;
}

std::vector<GLfloat> GameObject::GetTexCoords() {
    return texCoords;
}

float GameObject::GetSizeX() {
    return sizeX;
}

float GameObject::GetSizeY() {
    return sizeY;
}

void GameObject::SetSizeX(float newSizeX) {
    sizeX = newSizeX;
}

void GameObject::SetSizeY(float newSizeY) {
    sizeY = newSizeY;
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

void GameObject::SetUniformLocations(GLuint uSizeXLocation, GLuint uSizeYLocation, GLuint uPositionLocation, GLuint uRotationLocation, GLuint uColorLocation, GLuint uTextureLocation, GLuint uUseTextureLocation) {
    uniformLocations.uSizeXLocation = uSizeXLocation;
    uniformLocations.uSizeYLocation = uSizeYLocation;
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

std::vector<GLuint> GameObject::GetIndices() {
    return indices;
}

CollisionBox GameObject::GetCollider(){
    return collider;
}

void GameObject::SetCollider(CollisionBox newCollider){
    this->collider = newCollider;
}

void GameObject::UpdateColliderPos(){
    this->collider.SetPosX(posX);
    this->collider.SetPosY(posY);
}

void GameObject::SetPosZ(float z) {
    this->posZ = z;
}

float GameObject::GetPosZ() const {
    return posZ;
}
