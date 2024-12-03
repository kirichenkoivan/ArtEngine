#include "../include/engine/Graphics/Meshes/Mesh.h"

std::shared_ptr<Mesh> Mesh::CreateMesh(const std::string &name, std::vector<Vertex> verts, std::vector<uint32_t> indices, std::shared_ptr<Material> material)
{
    if (verts.empty() || indices.empty() || !material){
        Logger::GetInstance().Error("Graphics/Mesh", "Mesh Creation Failed - Some Of Arguments Are Null");
        assert(false && "Mesh Creation Failed - Some Of Arguments Are Null");
    }
    std::shared_ptr<Mesh> mesh(new Mesh());
    mesh->m_Name = name;
    mesh->m_Vertices = verts;
    mesh->m_Indices = indices;
    mesh->m_Material = material;
    
    if (!mesh->m_Material->isUsingTexture()){
        mesh->SetColorFromMaterial();
    }
    return mesh;
}

glm::mat4 Mesh::GetMeshMatrix() const
{
    glm::mat4 mesh = glm::mat4(1.0f);
    mesh = glm::translate(mesh, m_Position);
    mesh = glm::rotate(mesh, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
    mesh = glm::rotate(mesh, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
    mesh = glm::rotate(mesh, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
    mesh = glm::scale(mesh, m_Scale);
    return mesh;
}

glm::mat4 Mesh::GetUvMeshMatrix() const
{
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(m_UvPosition, 1.0f));
    matrix = glm::rotate(matrix, glm::radians(m_UvRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, glm::vec3(m_UvScale, 1.0f));
    return matrix;
}

void Mesh::SetColorFromMaterial()
{
    for (auto& vertex : this->m_Vertices){
        vertex.color = this->m_Material->GetColor();
    }
}

bool Mesh::GetIsMeshStateChanged()
{
    bool isChanged = m_IsMeshStateChanged;
    m_IsMeshStateChanged = false;
    return isChanged|| this->m_Material->IsMaterialStateChanged();
}