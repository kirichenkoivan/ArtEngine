#ifndef MATERIAL
#define MATERIAL

#include "Texture.h"
#include "Shader.h"
#include "../Core/ArtCore.h"
#include "../Core/ArtCoreUtils.h"

class Material {

    public:
        ~Material() = default;
        static std::shared_ptr<Material> CreateMaterial(const std::string& name, std::shared_ptr<Shader> shader , std::shared_ptr<Texture> texture);
        static std::shared_ptr<Material> CreateMaterial(const std::string& name, std::shared_ptr<Shader> shader , glm::vec4 color);

        std::string GetName() const { return m_Name; }
        std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }
        glm::vec4 GetColor() const { return m_Color; }
        bool isUsingTexture() const { return m_IsUsingTexture; }

    private:
        const std::string CATEGORY = "Graphics/Material/";

        Material() = default;
        std::string m_Name;
        std::shared_ptr<Shader> m_Shader;
        bool m_IsUsingTexture;
        std::shared_ptr<Texture> m_Texture;
        glm::vec4 m_Color;
};

#endif