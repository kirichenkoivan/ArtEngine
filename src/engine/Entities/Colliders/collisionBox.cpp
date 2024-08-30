#include "../include/engine/Entities/Colliders/collisionBox.h"

CollisionBox::CollisionBox(const float& x, const float& y, const float& sX, const float& sY)
    : posX(x), posY(y), sizeX(sX), sizeY(sY) {}

CollisionBox::CollisionBox(){

}

//Setters

void CollisionBox::SetPosX(float x){
    this->posX = x;
}

void CollisionBox::SetPosY(float y){
    this->posY = y;
}

void CollisionBox::SetSizeX(float x){
    this->sizeX = x;
}

void CollisionBox::SetSizeY(float y){
    this->sizeY = y;
}

//Getters

float CollisionBox::GetPosX(){
    return posX;
}

float CollisionBox::GetPosY(){
    return posY;
}

float CollisionBox::GetSizeX(){
    return sizeX;
}

float CollisionBox::GetSizeY(){
    return sizeY;
}

//Methods

bool CollisionBox::Intersects(const CollisionBox* other) const {
    return posX < other->posX + other->sizeX &&
           posX + sizeX > other->posX &&
           posY < other->posY + other->sizeY &&
           posY + sizeY > other->posY;
}