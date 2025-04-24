#ifndef MESH_LOADER
#define MESH_LOADER

#include "libs/json.hpp"
#include "../../Graphics/Meshes/Mesh.h"
#include "MaterialLoader.h"
#include "../logger.h"
#include <fstream>
#include <iostream>

const std::string MESH_CATEGORY = "Tools/MeshLoader";
const std::string MESH_DEFAULT_PATH = "/FileSystem/Assets/Library/Meshes/DefaultMesh.json";

std::shared_ptr<Mesh> CreateMeshFromJson(const std::string &filePath = MESH_DEFAULT_PATH);

#endif