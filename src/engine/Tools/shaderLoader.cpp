#include "../include/engine/Tools/shaderLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

const char* ShaderLoader::LoadShaderAsChar(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();

    // Оборачиваем содержимое шейдера в формат R"()
    std::string formattedShaderCode = shaderCode;

    // Освобождаем предыдущую память, если была выделена
    delete[] shaderSource;

    // Выделяем память для новой строки
    shaderSource = new char[formattedShaderCode.size() + 1];
    std::memcpy(shaderSource, formattedShaderCode.c_str(), formattedShaderCode.size() + 1); // +1 для завершающего нулевого символа

    return shaderSource;
}

ShaderLoader::~ShaderLoader() {
    delete[] shaderSource; // Освобождаем память
}