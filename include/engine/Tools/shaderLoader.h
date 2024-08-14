#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

class ShaderLoader {
public:
    // Читает GLSL файл и возвращает его содержимое в формате const char*
    const char* LoadShaderAsChar(const std::string& filePath);

    // Деструктор освобождает память, выделенную под содержимое шейдера
    ~ShaderLoader();

private:
    char* shaderSource = nullptr; // Указатель на содержимое шейдера
};

#endif // SHADER_LOADER_H
