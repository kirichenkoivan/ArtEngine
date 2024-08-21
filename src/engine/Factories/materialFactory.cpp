#include "../include/engine/Factories/materialFactory.h"

Material MaterialFactory::CreateMaterialFromXML(const std::string& fileName)
{
    std::filesystem::path filePath = FindXMLFile(fileName, "/FileSystem");

    if(!filePath.empty()){
        std::vector<std::string> attributes = {"name", "vertexShader", "fragmentShader", "texture"};
        std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXMLFile(filePath);
        std::vector<std::pair<std::string, std::string>> attrs = GetAttributesValues(*doc, attributes);

        std::string name;
        std::string vertexShaderSource;
        std::string fragmentShaderSource;
        std::string texturePath;

        for (const auto& pair : attrs) {
            if (pair.first == "name") {
                name = pair.second;
            } else if (pair.first == "vertexShader") {
                vertexShaderSource = readFile(pair.second);
                std::cout << "Vertex Shader Source:\n" << vertexShaderSource << std::endl;
            } else if (pair.first == "fragmentShader") {
                fragmentShaderSource = readFile(pair.second);
                std::cout << "Fragment Shader Source:\n" << fragmentShaderSource << std::endl;
            } else if (pair.first == "texture") {
                texturePath = pair.second;
            }
        }

        if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
            std::cerr << "Shader source code is empty!" << std::endl;
            // В случае ошибки вернем пустой материал
            return Material();
        }

        Material mat(name, vertexShaderSource, fragmentShaderSource, texturePath);

        return mat;
    }
    else{
        std::cerr << "Material file path is NULL" << std::endl;
        return Material();
    }
}