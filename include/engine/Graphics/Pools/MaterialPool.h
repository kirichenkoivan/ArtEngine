#ifndef MATERIAL_POOL
#define MATERIAL_POOL

#include <unordered_map>
#include <functional>

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"
#include "../Material.h"

class MaterialPool {
    public:
        static MaterialPool& GetInstance() {
                static MaterialPool instance;
                return instance;
        };

        size_t IsMaterialAlreadyInPool(const std::string& materialPath);
        void AddMaterialIntoPool(const std::string& materialPath, std::shared_ptr<Material> material);
        std::shared_ptr<Material> GetMaterialFromPool(size_t hashCode);

    private:
        size_t Hash(const std::string& materialSource);

    private:
        const std::string CATEGORY = "Graphics/Pools/MaterialPool";
        std::unordered_map<size_t, std::shared_ptr<Material>> m_LoadedMaterials;
};

#endif