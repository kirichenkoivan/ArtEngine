#ifndef MESH
#define MESH

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"
#include "../Material.h"

#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    GLuint texID;
    int objID;
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
        void SetPosition(float x, float y, float z) { m_Position = {x, y, z}; m_IsMeshStateChanged = true; }
        void SetPositionX(float x) { m_Position.x = x; m_IsMeshStateChanged = true; }
        void SetPositionY(float y) { m_Position.y = y; m_IsMeshStateChanged = true; }
        void SetPositionZ(float z) { m_Position.z = z; m_IsMeshStateChanged = true; }

        void SetScale(float x, float y, float z) { m_Scale = {x, y, z}; m_IsMeshStateChanged = true; }
        void SetScaleX(float x) { m_Scale.x = x; m_IsMeshStateChanged = true; }
        void SetScaleY(float y) { m_Scale.y = y; m_IsMeshStateChanged = true; }
        void SetScaleZ(float z) { m_Scale.z = z; m_IsMeshStateChanged = true; }

        void SetRotation(float x, float y, float z) { m_Rotation = {x, y, z}; m_IsMeshStateChanged = true; }
        void SetRotationX(float x) { m_Rotation.x = x; m_IsMeshStateChanged = true; }
        void SetRotationY(float y) { m_Rotation.y = y; m_IsMeshStateChanged = true; }
        void SetRotationZ(float z) { m_Rotation.z = z; m_IsMeshStateChanged = true; }

        void SetUvPosition(float x, float y) { m_UvPosition = {x, y}; m_IsMeshStateChanged = true; }
        void SetUvPositionX(float x) { m_UvPosition.x = x; m_IsMeshStateChanged = true; }
        void SetUvPositionY(float y) { m_UvPosition.y = y; m_IsMeshStateChanged = true; }

        void SetUvScale(float x, float y) { m_UvScale = {x, y}; m_IsMeshStateChanged = true; }
        void SetUvScaleX(float x) { m_UvScale.x = x; m_IsMeshStateChanged = true; }
        void SetUvScaleY(float y) { m_UvScale.y = y; m_IsMeshStateChanged = true; }
        
        void SetUvRotation(float value) { m_UvRotation = value; m_IsMeshStateChanged = true; }

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

        glm::vec2 GetUvScale() const { return m_UvScale; }
        float GetUvScaleX() const { return m_UvScale.x; }
        float GetUvScaleY() const { return m_UvScale.y; }

        glm::vec2 GetUvPosition() const { return m_UvPosition; }
        float GetUvPositionX() const { return m_UvPosition.x; }
        float GetUvPositionY() const { return m_UvPosition.y; }

        float GetUvRotation() const { return m_UvRotation; }

        glm::mat4 GetMeshMatrix() const;
        glm::mat4 GetUvMeshMatrix() const;

        void SetColorFromMaterial();

        bool GetIsMeshStateChanged();

    private:
        Mesh() = default;

        std::string m_Name;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::shared_ptr<Material> m_Material;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_Scale = { 0.5f, 0.5f, 0.5f };
        glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

        glm::vec2 m_UvPosition = { 0.0f, 0.0f };
        glm::vec2 m_UvScale = { 1.0f, 1.0f };
        float m_UvRotation = 0.0f;

        bool m_IsMeshStateChanged = true;
};

#endif