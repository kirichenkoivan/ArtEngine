#ifndef MATERIAL
#define MATERIAL

#include "Texture.h"
#include "Shader.h"
#include "../Core/ArtCore.h"
#include "../Core/ArtCoreUtils.h"

enum MaterialTypes
{
    FLAT_COLOR,
    FLAT_TEXTURE,
    PHONG,
};

struct alignas(16) MaterialInfo
{
    glm::vec4 ambientCoeff;
    glm::vec4 diffuseCoeff;
    glm::vec4 specularCoeff;
    float shininess;
    float _padding[3];
};

class Material
{

public:
    ~Material() = default;
    static std::shared_ptr<Material> CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Texture>> textures, MaterialTypes type = FLAT_TEXTURE);
    static std::shared_ptr<Material> CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, glm::vec4 color, MaterialTypes type = FLAT_COLOR);

    std::string GetName() const { return this->m_Name; }
    std::vector<std::shared_ptr<Texture>> GetTextures() const { return this->m_Textures; }
    std::shared_ptr<Shader> GetShader() const { return this->m_Shader; }
    glm::vec4 GetColor() const { return this->m_Color; }
    MaterialTypes GetType() const { return this->m_Type; }

    bool isUsingTexture() const { return this->m_IsUsingTexture; }
    bool IsMaterialStateChanged();

    void EnableTextureTiling(bool enable);
    bool IsTextureTilingEnabled() const { return m_EnableTextureTiling; }

    void SetAmbientLightCoeff(glm::vec3 coef)
    {
        m_Ka = coef;
        m_IsMaterialStateChanged = true;
    }
    void SetDiffuseLightCoeff(glm::vec3 coef)
    {
        m_Kd = coef;
        m_IsMaterialStateChanged = true;
    }
    void SetSpecularLightCoeff(glm::vec3 coef)
    {
        m_Ks = coef;
        m_IsMaterialStateChanged = true;
    }

    void SetShinines(float val) {}

    glm::vec3 GetAmbientLightCoeff() const { return m_Ka; }
    glm::vec3 GetDiffuseLightCoeff() const { return m_Kd; }
    glm::vec3 GetSpecularLightCoeff() const { return m_Ks; }

    MaterialInfo &GetMaterialInfo();

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

    bool m_EnableTextureTiling = false;
    glm::vec3 m_Ka = {1.0, 1.0, 1.0};
    glm::vec3 m_Ks = {1.0, 1.0, 1.0};
    glm::vec3 m_Kd = {1.0, 1.0, 1.0};
    float m_Shinines = 2.0f;

    MaterialInfo m_MaterialInfo;
};

#endif