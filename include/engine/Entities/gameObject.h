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

private:
    std::string name;
    Material material;
    std::vector<GLfloat> vertices;
};

#endif // GAMEOBJECT_H
