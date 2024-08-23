#include "../include/engine/Factories/materialFactory.h"

MaterialFactory::MaterialFactory() {}

Material* MaterialFactory::CreateMaterialFromXML(const std::string& fileName)
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
            } else if (pair.first == "fragmentShader") {
                fragmentShaderSource = readFile(pair.second);
            } else if (pair.first == "texture") {
                texturePath = pair.second;
                std::cout << "texture path: "<< texturePath << std::endl;
            }
        }

        if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
            std::cerr << "Shader source code is empty!" << std::endl;
            Material* mat = new Material();
            return mat;
        }

        if(texturePath == "" || texturePath.empty()){
            std::cout << "meow" << std::endl;
            Material* mat = new Material(name, vertexShaderSource, fragmentShaderSource);
            return mat;
        }
        else{
            Material* mat = new Material(name, vertexShaderSource, fragmentShaderSource, texturePath);
            return mat; 
        }

    }
    else{
        std::cerr << "Material file path is NULL" << std::endl;
        Material* mat = new Material();
        return mat;
    }
}