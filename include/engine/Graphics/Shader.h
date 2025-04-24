/*
*****************************************************************
*                        --- Desc ---                           *
*                                                               *
*   The data structure describing a regular OpenGl shader       *
*            contains vertex and fragment shaders               *
*                                                               *
*****************************************************************
*/

#ifndef SHADER
#define SHADER

#include <string>
#include <vector>

#include "../Core/Glad/glad.h"
#include <EGL/egl.h>
#include <GLFW/glfw3.h>

#include "../Core/ArtCoreUtils.h"

class Shader
{
public:
    ~Shader();
    GLuint GetRendererID() { return m_RendererID; }
    static std::shared_ptr<Shader> FromGLSLTextFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, std::vector<std::string> uniforms, std::string name);
    std::string GetName() const { return m_Name; }

private:
    Shader() = default;
    void LoadFromGLSLTextFiles(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, std::vector<std::string> uniforms);
    GLuint CompileShader(GLenum type, const std::string &source);

private:
    std::string m_Name;
    GLuint m_RendererID;
    std::vector<std::string> m_Uniforms;
    const std::string CATEGORY = "Graphics/Shader";
};

#endif