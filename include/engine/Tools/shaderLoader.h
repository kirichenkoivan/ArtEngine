#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

class ShaderLoader
{
public:
    const char *LoadShaderAsChar(const std::string &filePath);

    ~ShaderLoader();

private:
    char *shaderSource = nullptr;
};

#endif
