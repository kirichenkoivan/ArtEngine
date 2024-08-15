#ifndef MATERIAL_H
#define MATERIAL_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>

class Material {
public:
    // Constructor
    Material(const std::string& name, const char* vertexShader, const char* fragmentShader);

    std::string GetMaterialName() const;
    const char* GetVertexShader() const;
    const char* GetFragmentShader() const;

    void GetColor(GLfloat color[4]) const;
    void SetColor(const GLfloat color[4]);

private:
    std::string name;
    const char* vertexShader;
    const char* fragmentShader;
    GLfloat color[4];
};


#endif