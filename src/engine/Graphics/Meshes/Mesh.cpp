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
        mesh->SetColorToAllVerts();
    }

    return mesh;
}

void Mesh::SetPosition(float x, float y, float z){
    this->m_Position.x = x;
    this->m_Position.y = y;
    this->m_Position.z = z;
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

void Mesh::SetColorToAllVerts() {
    for (auto vertex : this->m_Vertices){
        //std::cout << this->m_Material->GetColor() << std::endl;
        vertex.color = this->m_Material->GetColor();
    }
}