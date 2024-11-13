#include "../include/engine/Graphics/Meshes/Mesh.h"

Mesh::Mesh(const std::string &name, std::vector<Vertex> verts, std::vector<uint32_t> ind, const Texture* texture) : m_Name(name), m_Vertices(verts), m_Indices(ind)
{
}