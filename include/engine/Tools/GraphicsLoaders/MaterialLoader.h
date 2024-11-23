#ifndef MATERIAL_LOADER
#define MATERIAL_LOADER

#include "../logger.h"
#include "ShaderLoader.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/Pools/MaterialPool.h"

#include "libs/json.hpp"
#include <fstream>
#include <iostream>

const std::string MATERIAL_CATEGORY = "Tools/MaterialLoader";
const std::string MATERIAL_DEFAULT_PATH = "/FileSystem/Assets/Library/Materials/DefaultMat.json";

std::shared_ptr<Material> CreateMaterialFromJson(const std::string& filePath = MATERIAL_DEFAULT_PATH);

#endif