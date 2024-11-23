#include "../include/engine/Graphics/Material.h"

std::shared_ptr<Material> Material::CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
{
    if (!shader || !texture){
        Logger::GetInstance().Error("Graphics/Material", "Materail Creating Failed! Shader or Material or Name Is NULL");
        assert(false && "Materail Creating Failed! Shader or Material or Name Is NULL");
    }
    std::shared_ptr<Material> material(new Material());
    material->m_Name = name;
    material->m_Shader = shader;
    material->m_Texture = texture;
    material->m_IsUsingTexture = true;
    return material;
}

std::shared_ptr<Material> Material::CreateMaterial(const std::string &name, std::shared_ptr<Shader> shader, glm::vec4 color)
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
    return material;
}
