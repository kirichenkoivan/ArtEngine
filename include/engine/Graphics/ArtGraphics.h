#ifndef ART_GRAPHICS
#define ART_GRAPHICS

#include "Shader.h"
#include "Texture.h"
#include "Cameras/OrthographicCamera.h"
#include "Cameras/OrthographicCameraController.h"
#include "Meshes/Mesh.h"
#include "../../libs/stb_image.h"

Texture* CreateTexture(const std::string& path, TextureType type);
Mesh* CreateMesh(const std::string& name, std::vector<Vertex> verts, std::vector<uint32_t> ind, const Texture* texture);
GLuint LoadTexture(const std::string& path);

#endif