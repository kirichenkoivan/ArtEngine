#include "../include/engine/Entities/Actors/dynamicActor.h"

DynamicActor::DynamicActor(const std::string& name, Material& material, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& texCoords, const std::vector<GLuint>& indices)
    : GameObject(name, material, vertices, texCoords, indices) {
}

void DynamicActor::Move(float deltaTime){
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_LEFT)) { // Стрелка влево
        SetPos(GetPosX() - moveSpeed * deltaTime, GetPosY());
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_RIGHT)) { // Стрелка вправо
        SetPos(GetPosX() + moveSpeed * deltaTime, GetPosY());
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_UP)) { // Стрелка вверх
        SetPos(GetPosX(), GetPosY() + moveSpeed * deltaTime);
    }
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_DOWN)) { // Стрелка вниз
        SetPos(GetPosX(), GetPosY() - moveSpeed * deltaTime);
    }
}