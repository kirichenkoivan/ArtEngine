#include "../include/engine/Tools/GraphicsLoaders/ShaderLoader.h"

std::shared_ptr<Shader> CreateShaderFromJson(const std::string &filePath)
{
    std::string path = filePath;
    if ( path == "" ){
        Logger::GetInstance().Warning(SHADER_CATEGORY, "Path is empty, default shader will be used");
        path = SHADER_DEFAULT_PATH;
    }

    using json = nlohmann::json;

    std::string jsonString = readFile(path);
    if (jsonString.empty()) {
        Logger::GetInstance().Error(SHADER_CATEGORY, "Failed to load file by this path:" + path);
        assert(false && "Failed to load file by this path:");
    }

    json jsonData;
    try {
        jsonData = nlohmann::json::parse(jsonString);
    } catch (const nlohmann::json::parse_error& e) {
        Logger::GetInstance().Error(SHADER_CATEGORY, "Parse Error");
        assert(false && "Parse Error: ");
    }

    try {
        std::string name = jsonData.at("Name").get<std::string>();
        std::string vertShaderPath = jsonData.at("VertexShaderPath").get<std::string>();
        std::string fragShaderPath = jsonData.at("FragmentShaderPath").get<std::string>();
        std::vector<std::string> uniforms;
        for (const auto& uniform : jsonData.at("Uniforms")){
            uniforms.push_back(uniform);
        }
        size_t shaderHash = ShaderPool::GetInstance().IsShaderAlreadyInPool(vertShaderPath, fragShaderPath);
        
        if (shaderHash != 0){
            return ShaderPool::GetInstance().GetShaderFromPool(shaderHash);
        } else {
            const auto shader = Shader::FromGLSLTextFile(vertShaderPath, fragShaderPath, uniforms, name);
            ShaderPool::GetInstance().AddShaderIntoPool(vertShaderPath, fragShaderPath, shader);
            return shader;
        }
        
    } catch (const json::out_of_range& ex) {
        Logger::GetInstance().Error(SHADER_CATEGORY, "Data Access Error");
        assert(false && "Data Access Error");
    }
}