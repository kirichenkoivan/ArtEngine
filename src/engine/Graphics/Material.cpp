#include "../include/engine/Graphics/Material.h"

std::shared_ptr<Material> Material::CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, std::vector<std::shared_ptr<Texture>> textures, MaterialTypes type)
{
    if (!shader || textures.empty()){
        Logger::GetInstance().Error("Graphics/Material", "Materail Creating Failed! Shader or Material or Name Is NULL");
        assert(false && "Materail Creating Failed! Shader or Material or Name Is NULL");
    }
    std::shared_ptr<Material> material(new Material());
    material->m_Name = name;
    material->m_Shader = shader;
    material->m_Textures = textures;
    material->m_IsUsingTexture = true;
    material->m_Type = type;
    return material;
}

std::shared_ptr<Material> Material::CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, glm::vec4 color, MaterialTypes type)
{
    if (!shader){
        Logger::GetInstance().Error("Graphics/Material", "Materail Creating Failed! Shader or Color Is NULL");
        assert(false && "Materail Creating Failed! Shader or Material or Name Is NULL");
    }
    std::shared_ptr<Material> material(new Material());
    material->m_Name = name;
    material->m_Shader = shader;    
    material->m_Color = color;
    material->m_IsUsingTexture = false;
    material->m_Type = type;
    return material;
}

void Material::EnableTextureTiling(bool enable){
    for (auto& texture : m_Textures){
        texture->SetTiling(enable);
    }
    m_EnableTextureTiling = enable;
    m_IsMaterialStateChanged = true;
}

bool Material::IsMaterialStateChanged()
{
    bool isChanged = m_IsMaterialStateChanged;
    m_IsMaterialStateChanged = false;
    return isChanged;
}
