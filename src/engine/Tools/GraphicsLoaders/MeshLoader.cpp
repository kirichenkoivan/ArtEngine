#include "../include/engine/Tools/GraphicsLoaders/MeshLoader.h"

std::shared_ptr<Mesh> CreateMeshFromJson(const std::string &filePath)
{
    std::string path = filePath;
    if (path == "")
    {
        Logger::GetInstance().Warning(MESH_CATEGORY, "Path is empty, default mesh will be used");
        path = MESH_DEFAULT_PATH;
    }

    using json = nlohmann::json;
    std::cout << path << std::endl;
    std::string jsonString = readFile(path);

    if (jsonString.empty())
    {
        Logger::GetInstance().Error(MESH_CATEGORY, "Failed to load file by this path:" + path);
        assert(false && "Failed to load file by this path:");
    }

    json jsonData;
    try
    {
        jsonData = nlohmann::json::parse(jsonString);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        Logger::GetInstance().Error(MESH_CATEGORY, "Parse Error");
        assert(false && "Parse Error: ");
    }

    try
    {
        std::string name = jsonData.at("Name").get<std::string>();

        std::shared_ptr<Material> material;
        if (jsonData.contains("Material"))
        {
            material = CreateMaterialFromJson(jsonData.at("Material").get<std::string>());
        }
        else
        {
            material = CreateMaterialFromJson();
        }

        if (!jsonData.contains("Vertices") || !jsonData.contains("Indices"))
        {
            Logger::GetInstance().Error(MESH_CATEGORY, "Vertices or Indices is empty!");
            assert(false && "Vertices or Indices is empty!");
        }

        std::vector<Vertex> verts;
        for (const auto &vertex : jsonData["Vertices"])
        {
            Vertex vert;
            vert.position = {
                vertex.at("Position")[0].get<float>(),
                vertex.at("Position")[1].get<float>(),
                vertex.at("Position")[2].get<float>()};
            vert.color = {
                vertex.at("Color")[0].get<float>() / 255.0f,
                vertex.at("Color")[1].get<float>() / 255.0f,
                vertex.at("Color")[2].get<float>() / 255.0f,
                1.0f};
            vert.texCoords = {
                vertex.at("TexCoords")[0].get<float>(),
                vertex.at("TexCoords")[1].get<float>(),
            };

            vert.normal = {
                vertex.at("Normal")[0].get<float>(),
                vertex.at("Normal")[1].get<float>(),
                vertex.at("Normal")[2].get<float>()};

            vert.texID = vertex.at("TexID").get<GLuint>();

            verts.push_back(vert);
        }

        std::vector<uint32_t> indices;
        for (const auto &ind : jsonData["Indices"])
        {
            indices.push_back(ind.get<uint32_t>());
        }
        if (verts.empty())
        {
            Logger::GetInstance().Error(MESH_CATEGORY, "Vertices is empty!");
        }
        if (indices.empty())
        {
            Logger::GetInstance().Error(MESH_CATEGORY, "Indices is empty!");
        }
        if (!material)
        {
            Logger::GetInstance().Error(MESH_CATEGORY, "Material is empty!");
        }
        return Mesh::CreateMesh(name, verts, indices, material);
    }
    catch (const json::out_of_range &ex)
    {
        Logger::GetInstance().Error(MESH_CATEGORY, "Data Access Error");
        assert(false && "Data Access Error");
    }
}