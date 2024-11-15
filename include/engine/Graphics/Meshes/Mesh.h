#ifndef MESH
#define MESH

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"
#include "../Material.h"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    GLuint texID;
};

class Mesh {

    public:        
        ~Mesh() = default;
        static std::shared_ptr<Mesh> CreateMesh(const std::string& name, std::vector<Vertex> verts, std::vector<uint32_t> indices, std::shared_ptr<Material> material);

        std::string GetName() const { return m_Name; }
        std::vector<Vertex> GetVertices() const { return m_Vertices; }
        std::vector<uint32_t> GetIndices() const { return m_Indices; }
        std::shared_ptr<Material> GetMaterial() const { return m_Material; }

        // Setters
        void SetPosition(float x, float y, float z) { m_Position = {x, y, z}; }
        void SetPositionX(float x) { m_Position.x = x; }
        void SetPositionY(float y) { m_Position.y = y; }
        void SetPositionZ(float z) { m_Position.z = z; }

        void SetScale(float x, float y, float z) { m_Scale = {x, y, z}; }
        void SetScaleX(float x) { m_Scale.x = x; }
        void SetScaleY(float y) { m_Scale.y = y; }
        void SetScaleZ(float z) { m_Scale.z = z; }

        void SetRotation(float x, float y, float z) { m_Rotation = {x, y, z}; }
        void SetRotationX(float x) { m_Rotation.x = x; }
        void SetRotationY(float y) { m_Rotation.y = y; }
        void SetRotationZ(float z) { m_Rotation.z = z; }

        // Getters
        glm::vec3 GetPosition() const { return m_Position; }
        float GetPositionX() const { return m_Position.x; }
        float GetPositionY() const { return m_Position.y; }
        float GetPositionZ() const { return m_Position.z; }

        glm::vec3 GetScale() const { return m_Scale; }
        float GetScaleX() const { return m_Scale.x; }
        float GetScaleY() const { return m_Scale.y; }
        float GetScaleZ() const { return m_Scale.z; }

        glm::vec3 GetRotation() const { return m_Rotation; }
        float GetRotationX() const { return m_Rotation.x; }
        float GetRotationY() const { return m_Rotation.y; }
        float GetRotationZ() const { return m_Rotation.z; }

        glm::mat4 GetMeshMatrix() const;

    private:
        Mesh() = default;

        std::string m_Name;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::shared_ptr<Material> m_Material;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_Scale = { 0.5f, 0.5f, 0.5f };
        glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
};

#endif