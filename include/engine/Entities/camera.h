#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <cmath>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);

    //Setters
    void SetScreenSize(float width, float height);
    void SetPositionX(float x);
    void SetPositionY(float y);

    //Getters
    float GetPositionX() const { return positionX; }
    float GetPositionY() const { return positionY; }
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

private:
    float screenWidth;
    float screenHeight;
    float positionX;
    float positionY;
};



#endif