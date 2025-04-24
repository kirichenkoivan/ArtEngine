#include "../include/engine/Graphics/Pools/MaterialPool.h"

size_t MaterialPool::IsMaterialAlreadyInPool(const std::string &materialPath)
{
    std::string materialSource = readFile(materialPath);

    size_t hashCode = Hash(materialSource);

    if (m_LoadedMaterials.find(hashCode) != m_LoadedMaterials.end())
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Material Found!");
#endif

        return hashCode;
    }
    else
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Material Not Found!");
#endif

        return 0;
    }
    return 0;
}

void MaterialPool::AddMaterialIntoPool(const std::string &materialPath, std::shared_ptr<Material> material)
{
    if (materialPath.empty() || !material)
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Material Path or Material is Empty!");
#endif

        assert(false && "Material Path or Material is Empty!");
    }

    std::string materialSource = readFile(materialPath);

    size_t hashCode = Hash(materialSource);
    m_LoadedMaterials[hashCode] = material;
}

std::shared_ptr<Material> MaterialPool::GetMaterialFromPool(size_t hashCode)
{
    return m_LoadedMaterials[hashCode];
}

size_t MaterialPool::Hash(const std::string &materialSource)
{
    std::hash<std::string> hasher;
    return hasher(materialSource);
}
