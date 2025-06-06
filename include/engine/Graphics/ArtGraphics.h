#ifndef ART_GRAPHICS
#define ART_GRAPHICS

#include "Shader.h"
#include "Texture.h"
#include "Cameras/OrthographicCamera.h"
#include "Cameras/OrthographicCameraController.h"
#include "Meshes/Mesh.h"
#include "Material.h"
#include "Light/LightSource.h"
#include "Light/PointLight.h"
#include "Light/DirectLight.h"
#include "Light/SpotLight.h"
#include "../../libs/stb_image.h"
#include "../Tools/GraphicsLoaders/MeshLoader.h"

GLuint LoadTexture(const std::string &path);

#endif