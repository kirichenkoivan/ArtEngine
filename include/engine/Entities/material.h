#ifndef MATERIAL_H
#define MATERIAL_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>

class Material {
public:
    // Constructor
    Material(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& texturePath = "");
    Material();
    //Setters
    void SetTexture(GLuint texture);
    void SetColor(const GLfloat color[4]);
    void SetTexturePath(const std::string& texturePath);

    //Getters
    std::string GetMaterialName() const;
    const char* GetVertexShader() const;
    const char* GetFragmentShader() const;
    GLuint GetTexture() const;
    std::string GetTexturePath() const;
    void GetColor(GLfloat color[4]) const;

private:
    std::string name;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    GLuint texture;
    std::string texturePath;
    GLfloat color[4];
};

#endif