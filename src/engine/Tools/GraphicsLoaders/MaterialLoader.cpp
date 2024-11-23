#include "../include/engine/Tools/GraphicsLoaders/MaterialLoader.h"

std::shared_ptr<Material> CreateMaterialFromJson(const std::string &filePath)
{
    std::string path = filePath;
    if ( path == "" ){
        Logger::GetInstance().Warning(MATERIAL_CATEGORY, "Path is empty, default material will be used");
        path = MATERIAL_DEFAULT_PATH;
    }

    size_t materialHash = MaterialPool::GetInstance().IsMaterialAlreadyInPool(path);
    if (materialHash != 0){
        return MaterialPool::GetInstance().GetMaterialFromPool(materialHash);
    }

    using json = nlohmann::json;
    std::string jsonString = readFile(path);

    if (jsonString.empty()) {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Failed to load file by this path:" + path);
        assert(false && "Failed to load file by this path:");
    }

    json jsonData;
    try 
    {
        jsonData = nlohmann::json::parse(jsonString);
    } 
    catch (const nlohmann::json::parse_error& e) 
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Parse Error");
        assert(false && "Parse Error: ");
    }

    try 
    {
        std::string name = jsonData.at("Name").get<std::string>();
        std::string shaderPath = jsonData.at("Shader").get<std::string>();

        std::shared_ptr<Shader> shader = CreateShaderFromJson(shaderPath);

        if (jsonData.contains("Textures")) {
            // TODO: Need to add textures support
        }
        else if (jsonData.contains("Color")){
            glm::vec4 color = {
                jsonData.at("Color")[0].get<float>() / 255.0f,
                jsonData.at("Color")[1].get<float>() / 255.0f,
                jsonData.at("Color")[2].get<float>() / 255.0f,
                jsonData.at("Color")[3].get<float>() / 255.0f,
            };

            const auto material = Material::CreateMaterial(name, shader, color);
            MaterialPool::GetInstance().AddMaterialIntoPool(path, material);
            return material;
        }
    } 
    catch (const json::out_of_range& ex) 
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Data Access Error");
        assert(false && "Data Access Error");
    }
}