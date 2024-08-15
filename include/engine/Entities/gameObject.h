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
    virtual ~GameObject() = default;

    virtual void Move(float deltaTime);

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
    void SetRotation(float angle);
    float GetRotation();
    
    //Shaders params
    GLuint GetShaderProgram() const;
    void SetShaderProgram(GLuint shaderProgram);
    struct UniformLocations {
        GLuint uSizeLocation;
        GLuint uPositionLocation;
        GLuint uRotationLocation;
        GLuint uColorLocation;
    };
    UniformLocations GetUniformLocations() const;
    void SetVBO(GLuint VBO);
    GLuint GetVBO() const;
    void SetUniformLocations(GLuint uSizeLocation, GLuint uPositionLocation, GLuint uRotationLocation, GLuint uColorLocation);

private:
    std::string name;
    Material material;
    
    //Object params
    std::vector<GLfloat> vertices;
    float size;
    float posX;
    float posY;
    float rotation;

    //Shaders params
    GLuint shaderProgram;
    UniformLocations uniformLocations;
    GLuint VBO;
};

#endif // GAMEOBJECT_H
