#include "../include/engine/Graphics/Pools/ShaderPool.h"

size_t ShaderPool::IsShaderAlreadyInPool(const std::string &vertexPath, const std::string &fragPath)
{
    std::string vertexSource = readFile(vertexPath);
    std::string fragSource = readFile(fragPath);

    size_t hashCode = HashAndCombine(vertexSource, fragSource);

    if (m_LoadedShaders.find(hashCode) != m_LoadedShaders.end())
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Shader Found!");
#endif

        return hashCode;
    }
    else
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Shader Not Found!");
#endif

        return 0;
    }

    return 0;
}

void ShaderPool::AddShaderIntoPool(const std::string &vertexPath, const std::string &fragPath, std::shared_ptr<Shader> shader)
{
    if (vertexPath.empty() || fragPath.empty())
    {

#ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Fragment or Vertex Shader Path is Empty");
#endif

        assert(false && "Fragment or Vertex Shader Path is Empty");
    }

    std::string vertexSource = readFile(vertexPath);
    std::string fragSource = readFile(fragPath);

    size_t hashCode = HashAndCombine(vertexSource, fragSource);
    m_LoadedShaders[hashCode] = shader;
}

std::shared_ptr<Shader> ShaderPool::GetShaderFromPool(size_t hashCode)
{
    return m_LoadedShaders[hashCode];
}

size_t ShaderPool::HashAndCombine(const std::string &vertSource, const std::string &fragSource)
{
    return Hash(vertSource) ^ Hash(fragSource);
}

size_t ShaderPool::Hash(const std::string &shaderSource)
{
    std::hash<std::string> hasher;
    return hasher(shaderSource);
}