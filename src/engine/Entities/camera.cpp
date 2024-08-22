#include "../include/engine/Entities/camera.h"

Camera::Camera(float screenWidth, float screenHeight) 
    : screenWidth(screenWidth), screenHeight(screenHeight), positionX(0.0f) {}

void Camera::SetScreenSize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
}

void Camera::SetPositionX(float x) {
    positionX = x;
}

glm::mat4 Camera::GetViewMatrix() const {
    // Создаём матрицу вида, перемещая камеру по оси X
    return glm::translate(glm::mat4(1.0f), glm::vec3(-positionX, 0.0f, 0.0f));
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
}