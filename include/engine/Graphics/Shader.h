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

class Shader{
    public:
        ~Shader();
        GLuint GetRendererID() { return m_RendererID; }
        static std::shared_ptr<Shader> FromGLSLTextFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:
        Shader() = default;
        void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint CompileShader(GLenum type, const std::string& source);

    private:
        GLuint m_RendererID;
        const std::string CATEGORY = "Graphics/Shader";

};

#endif