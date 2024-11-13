/* 
*****************************************************************
*                        --- Desc ---                           * 
*                                                               *
*   The data structure describing a regular OpenGl shader       *
*            contains vertex and fragment shaders               *
*                                                               *
*****************************************************************
*/

#include "../include/engine/Graphics/Shader.h"

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

Shader *Shader::FromGLSLTextFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    Shader* shader = new Shader();
    shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
    return shader;
}

void Shader::LoadFromGLSLTextFiles(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexSource = readFile(vertexShaderPath);
    std::string fragmentSource = readFile(fragmentShaderPath);

    GLuint program = glCreateProgram();
    int glShaderIDIndex = 0;

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    glAttachShader(program, vertexShader);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLenght = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLenght);

        std::vector<GLchar> infoLog(maxLenght);
        glGetProgramInfoLog(program, maxLenght, &maxLenght, &infoLog[0]);

        glDeleteProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Logger::GetInstance().Error(CATEGORY, infoLog.data());
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_RendererID = program;
}

GLuint Shader::CompileShader(GLenum type, const std::string &source)
{
    GLuint shader = glCreateShader(type);
    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        GLint maxLenght = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLenght);
        std::vector<GLchar> infoLog(maxLenght);
        glGetShaderInfoLog(shader, maxLenght, &maxLenght, &infoLog[0]);

        glDeleteShader(shader);

        Logger::GetInstance().Error(CATEGORY, infoLog.data());
    }
    Logger::GetInstance().Info(CATEGORY, "Shader Compiled Succesefully");
    return shader;
}
