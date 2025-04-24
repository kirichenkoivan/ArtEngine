#ifndef SHADER_POOL
#define SHADER_POOL

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"
#include "../Shader.h"
#include <unordered_map>
#include <functional>

class ShaderPool
{
public:
    static ShaderPool &GetInstance()
    {
        static ShaderPool instance;
        return instance;
    };

    size_t IsShaderAlreadyInPool(const std::string &vertexPath, const std::string &fragPath);
    void AddShaderIntoPool(const std::string &vertexPath, const std::string &fragPath, std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> GetShaderFromPool(size_t hashCode);

private:
    size_t HashAndCombine(const std::string &vertSource, const std::string &fragSource);
    size_t Hash(const std::string &shaderSource);

private:
    const std::string CATEGORY = "Graphics/Pools/ShaderPool";
    std::unordered_map<size_t, std::shared_ptr<Shader>> m_LoadedShaders;
};

#endif