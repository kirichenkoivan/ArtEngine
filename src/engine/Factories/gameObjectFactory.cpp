#include "../include/engine/Factories/gameObjectFactory.h"


GameObject GameObjectFactory::CreateGameObjectFromXML(std::string fileName)
{
    std::filesystem::path filePath = FindXMLFile(fileName, "/FileSystem");

    if(!filePath.empty()){
        std::vector<std::string> attributes = {"name", "material", "posX", "posY", "sizeX", "sizeY", "rotation"};
        std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXMLFile(filePath);
        std::vector<std::pair<std::string, std::string>> attrs = GetAttributesValues(*doc, attributes);
    
    }
}