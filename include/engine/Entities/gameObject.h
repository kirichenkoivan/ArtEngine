#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
#include "material.h"
#include <vector>

class GameObject {
public:
    // Constructor
    GameObject(const std::string& name, const Material& material, const std::vector<GLfloat> verts);

    std::string GetName();
    Material GetMaterial();
    std::vector<GLfloat> GetVertices();
    float GetSize();
    void SetSize(float newSize);
    void SetPosX(float x);
    void SetPosY(float y);
    void SetPos(float x, float y);
    float GetPosX();
    float GetPosY();

private:
    std::string name;
    Material material;
    
    //Object params
    std::vector<GLfloat> vertices;
    float size;
    float posX;
    float posY;
};

#endif // GAMEOBJECT_H
