#ifndef MATERIAL
#define MATERIAL

#include "Texture.h"
#include "Shader.h"
#include "../Core/ArtCore.h"
#include "../Core/ArtCoreUtils.h"

enum MaterialTypes {
    FLAT_COLOR,
    FLAT_TEXTURE,
    PHONG,
};

class Material {

    public: 
        //Constuctors
        ~Material() = default;
        static std::shared_ptr<Material> CreateMaterial(const std::string& name, std::shared_ptr<Shader> shader , std::vector<std::shared_ptr<Texture>> textures, MaterialTypes type = FLAT_TEXTURE);
        static std::shared_ptr<Material> CreateMaterial(const std::string& name, std::shared_ptr<Shader> shader , glm::vec4 color, MaterialTypes type = FLAT_COLOR);

        //Getter
        std::string GetName() const { return this->m_Name; }
        std::vector<std::shared_ptr<Texture>> GetTextures() const { return this->m_Textures; }
        std::shared_ptr<Shader> GetShader() const { return this->m_Shader; }
        glm::vec4 GetColor() const { return this->m_Color; }
        MaterialTypes GetType() const { return this->m_Type; }

        //Flags
        bool isUsingTexture() const { return this->m_IsUsingTexture; }
        bool IsMaterialStateChanged();

        //Textures Params
        void EnableTextureTiling(bool enable);
        bool IsTextureTilingEnabled() const { return m_EnableTextureTiling; }

    private:
        const std::string CATEGORY = "Graphics/Material/";

        Material() = default;
        std::string m_Name;
        std::shared_ptr<Shader> m_Shader;
        std::vector<std::shared_ptr<Texture>> m_Textures;
        glm::vec4 m_Color;
        MaterialTypes m_Type;
        bool m_IsUsingTexture;
        bool m_IsMaterialStateChanged = true;

        // Material Params
        bool m_EnableTextureTiling = false;
};

#endif