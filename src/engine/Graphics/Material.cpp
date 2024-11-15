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
    return material;
}
