#ifndef MESH
#define MESH

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"
#include "../Texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    GLuint texID;
};

class Mesh {

    public:        
        Mesh(const std::string& name, std::vector<Vertex> verts, std::vector<uint32_t> ind, const Texture* texture);
        ~Mesh() = default;

        std::string GetName() const { return m_Name; }
        std::vector<Vertex> GetVertices() const { return m_Vertices; }
        std::vector<uint32_t> GetIndices() const { return m_Indices; }
        Texture* GetTexture() const { return m_Texture; }

    private:
        std::string m_Name;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        Texture* m_Texture;
};

#endif