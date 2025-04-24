#include "../include/engine/Tools/shaderLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

const char *ShaderLoader::LoadShaderAsChar(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();

    std::string formattedShaderCode = shaderCode;

    delete[] shaderSource;

    shaderSource = new char[formattedShaderCode.size() + 1];
    std::memcpy(shaderSource, formattedShaderCode.c_str(), formattedShaderCode.size() + 1);

    return shaderSource;
}

ShaderLoader::~ShaderLoader()
{
    delete[] shaderSource;
}