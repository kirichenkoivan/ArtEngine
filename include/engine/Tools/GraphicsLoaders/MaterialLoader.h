#ifndef MATERIAL_LOADER
#define MATERIAL_LOADER

#include "../logger.h"
#include "ShaderLoader.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/Pools/MaterialPool.h"

#include "libs/json.hpp"
#include <fstream>
#include <iostream>

const std::string MATERIAL_CATEGORY = "Tools/MaterialLoader";
const std::string MATERIAL_DEFAULT_PATH = "/FileSystem/Assets/Library/Materials/DefaultMat.json";

const std::string FLAT_TEXTURE_SHADER_PATH = "/FileSystem/Assets/Library/Shaders/FlatTextureShader.json";
const std::string FLAT_COLOR_SHADER_PATH = "/FileSystem/Assets/Library/Shaders/FlatColorShader.json";
const std::string PHONG_SHADER_PATH = "/FileSystem/Assets/Library/Shaders/PhongShader.json";

std::shared_ptr<Material> CreateMaterialFromJson(const std::string& filePath = MATERIAL_DEFAULT_PATH);

std::shared_ptr<Material> CreateFlatColorMaterial(nlohmann::json& jsonData, std::string& path);
std::shared_ptr<Material> CreateFlatTextureMaterial(nlohmann::json& jsonData, std::string& path);
std::shared_ptr<Material> CreatePhongMaterial(nlohmann::json& jsonData, std::string& path);

#endif