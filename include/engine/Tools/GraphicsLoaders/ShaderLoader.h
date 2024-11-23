#ifndef SHADER_LOADER
#define SHADER_LOADER

#include "../../Graphics/Shader.h"
#include "../../Graphics/Pools/ShaderPool.h"
#include "../logger.h"
#include "../fileLoader.h"
#include "libs/json.hpp"
#include <fstream>
#include <iostream>

const std::string SHADER_CATEGORY = "Tools/ShaderLoader";
const std::string SHADER_DEFAULT_PATH = "/FileSystem/Assets/Library/Shaders/DefaultShader.json";

std::shared_ptr<Shader> CreateShaderFromJson(const std::string& filePath = SHADER_DEFAULT_PATH);

#endif