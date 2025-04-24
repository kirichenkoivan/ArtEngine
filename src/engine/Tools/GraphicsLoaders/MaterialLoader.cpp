#include "../include/engine/Tools/GraphicsLoaders/MaterialLoader.h"

std::shared_ptr<Material> CreateMaterialFromJson(const std::string &filePath)
{
    std::string path = filePath;
    if (path == "")
    {
        Logger::GetInstance().Warning(MATERIAL_CATEGORY, "Path is empty, default material will be used");
        path = MATERIAL_DEFAULT_PATH;
    }

    size_t materialHash = MaterialPool::GetInstance().IsMaterialAlreadyInPool(path);
    if (materialHash != 0)
    {
        return MaterialPool::GetInstance().GetMaterialFromPool(materialHash);
    }

    using json = nlohmann::json;
    std::string jsonString = readFile(path);

    if (jsonString.empty())
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Failed to load file by this path:" + path);
        assert(false && "Failed to load file by this path:");
    }

    json jsonData;
    try
    {
        jsonData = nlohmann::json::parse(jsonString);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Parse Error");
        assert(false && "Parse Error: ");
    }

    try
    {
        std::string typeStr = jsonData.at("Type").get<std::string>();

        if (typeStr == "FlatColor")
        {
            return CreateFlatColorMaterial(jsonData, path);
        }
        else if (typeStr == "FlatTexture")
        {
            return CreateFlatTextureMaterial(jsonData, path);
        }
        else if (typeStr == "Phong")
        {
            return CreatePhongMaterial(jsonData, path);
        }
        else
        {
            Logger::GetInstance().Error(MATERIAL_CATEGORY, "Material Type Error");
            assert(false && "Material Type Error");
        }
    }
    catch (const json::out_of_range &ex)
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "Data Access Error");
        assert(false && "Data Access Error");
    }
}

std::shared_ptr<Material> CreateFlatColorMaterial(nlohmann::json &jsonData, std::string &path)
{
    MaterialTypes type;
    std::shared_ptr<Shader> shader;

    type = FLAT_COLOR;
    shader = CreateShaderFromJson(FLAT_COLOR_SHADER_PATH);

    if (!jsonData.contains("Name"))
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "No Name Info In Material!");
        assert(false && "No Name Info In Material!");
    }
    std::string name = jsonData.at("Name").get<std::string>();

    if (!jsonData.contains("Color"))
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "No Color Info In Material!");
        assert(false && "No Color Info In Material!");
    }

    glm::vec4 color = {
        jsonData.at("Color")[0].get<float>() / 255.0f,
        jsonData.at("Color")[1].get<float>() / 255.0f,
        jsonData.at("Color")[2].get<float>() / 255.0f,
        jsonData.at("Color")[3].get<float>() / 255.0f,
    };
    const auto material = Material::CreateMaterial(name, shader, color, type);
    MaterialPool::GetInstance().AddMaterialIntoPool(path, material);
    return material;
}

std::shared_ptr<Material> CreateFlatTextureMaterial(nlohmann::json &jsonData, std::string &path)
{
    MaterialTypes type;
    std::shared_ptr<Shader> shader;

    type = FLAT_TEXTURE;
    shader = CreateShaderFromJson(FLAT_TEXTURE_SHADER_PATH);

    if (!jsonData.contains("Name"))
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "No Name Info In Material!");
        assert(false && "No Name Info In Material!");
    }
    std::string name = jsonData.at("Name").get<std::string>();

    if (!jsonData.contains("Textures"))
    {
        Logger::GetInstance().Error(MATERIAL_CATEGORY, "No Textures Info In Material!");
        assert(false && "No Textures Info In Material!");
    }

    std::vector<std::shared_ptr<Texture>> textures;
    for (const auto &texture : jsonData["Textures"])
    {
        textures.push_back(
            Texture::CreateTexture(texture.at("TexturePath").get<std::string>(), ALBEDO));
    }
    const auto material = Material::CreateMaterial(name, shader, textures, type);

    if (jsonData.contains("EnableTiling"))
    {
        material->EnableTextureTiling(jsonData.at("EnableTiling").get<bool>());
    }

    if (jsonData.contains("Ka"))
    {
        glm::vec3 ambientCoeff;
        ambientCoeff.x = jsonData.at("Ka")[0].get<float>();
        ambientCoeff.y = jsonData.at("Ka")[1].get<float>();
        ambientCoeff.z = jsonData.at("Ka")[2].get<float>();
        material->SetAmbientLightCoeff(ambientCoeff);
    }

    if (jsonData.contains("Kd"))
    {
        glm::vec3 diffuseCoeff;
        diffuseCoeff.x = jsonData.at("Kd")[0].get<float>();
        diffuseCoeff.y = jsonData.at("Kd")[1].get<float>();
        diffuseCoeff.z = jsonData.at("Kd")[2].get<float>();
        material->SetAmbientLightCoeff(diffuseCoeff);
    }

    if (jsonData.contains("Ks"))
    {
        glm::vec3 specularCoeff;
        specularCoeff.x = jsonData.at("Ks")[0].get<float>();
        specularCoeff.y = jsonData.at("Ks")[1].get<float>();
        specularCoeff.z = jsonData.at("Ks")[2].get<float>();
        material->SetAmbientLightCoeff(specularCoeff);
    }

    if (jsonData.contains("Shinines"))
    {
        material->SetShinines(jsonData.at("Shinines").get<float>());
    }

    MaterialPool::GetInstance().AddMaterialIntoPool(path, material);
    return material;
}

std::shared_ptr<Material> CreatePhongMaterial(nlohmann::json &jsonData, std::string &path)
{
    MaterialTypes type;
    std::shared_ptr<Shader> shader;

    type = PHONG;
    shader = CreateShaderFromJson(PHONG_SHADER_PATH);
}