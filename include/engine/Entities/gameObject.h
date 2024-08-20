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
    GameObject(const std::string& name, const Material& material, const std::vector<GLfloat> verts, const std::vector<GLfloat> texCoords, const std::vector<GLuint> indices);
    
    //Abstract methods
    virtual ~GameObject() = default;
    virtual void Move(float deltaTime);

    //Setters
    void SetSizeX(float newSizeX);
    void SetSizeY(float newSizeY);
    void SetPosX(float x);
    void SetPosY(float y);
    void SetPos(float x, float y);
    void SetRotation(float angle);

    //Getters
    std::string GetName();
    Material GetMaterial();
    std::vector<GLfloat> GetVertices();
    std::vector<GLfloat> GetTexCoords();
    std::vector<GLuint> GetIndices();
    float GetSizeX();
    float GetSizeY();
    float GetPosX();
    float GetPosY();
    float GetRotation();
    
    // Shaders params
    GLuint GetShaderProgram() const;
    void SetShaderProgram(GLuint shaderProgram);
    struct UniformLocations {
        GLuint uSizeXLocation;
        GLuint uSizeYLocation;
        GLuint uPositionLocation;
        GLuint uRotationLocation;
        GLuint uColorLocation;
        GLuint uTextureLocation;
        GLuint uUseTextureLocation;
    };
    UniformLocations GetUniformLocations() const;
    void SetVBO(GLuint VBO);
    GLuint GetVBO() const;
    void SetUniformLocations(GLuint uSizeXLocation, GLuint uSizeYLocation, GLuint uPositionLocation, GLuint uRotationLocation, GLuint uColorLocation, GLuint uTextureLocation, GLuint uUseTextureLocation);
    void SetTextureID(GLuint id) {this->textureId = id;};
    GLuint GetTextureID() const {return textureId;}

private:
    //Base Params
    std::string name;
    Material material;
    
    // Object params
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
    GLuint textureId;
    float sizeX;
    float sizeY;
    float posX;
    float posY;
    float rotation;

    // Shaders params
    GLuint shaderProgram;
    UniformLocations uniformLocations;
    GLuint VBO;
};

#endif // GAMEOBJECT_H