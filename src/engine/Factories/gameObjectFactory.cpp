#include "../include/engine/Factories/gameObjectFactory.h"

GameObjectFactory::GameObjectFactory(){
    matFactory = new MaterialFactory(); 
    collisionBoxFactory = new CollisionBoxFactory();
}

GameObject* GameObjectFactory::CreateGameObjectFromXML(std::string fileName)
{
    std::filesystem::path filePath = FindXMLFile(fileName, "/FileSystem");

    if(!filePath.empty()){
        std::vector<std::string> attributes = {"name", "sizeX", "sizeY", "posX", "posY", "rotation", "material", "needCollider"};
        std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXMLFile(filePath);
        std::vector<std::pair<std::string, std::string>> attrs = GetAttributesValues(*doc, attributes);

        std::string name;
        float sizeX = 0.0f;
        float sizeY = 0.0f;
        float posX = 0.0f;
        float posY = 0.0f;
        float rotation = 0.0f;
        std::string matPath;
        bool needCollider = false;

        std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, -1.0f, // Bottom-left
            0.5f, -0.5f, -1.0f, // Bottom-right
            0.5f,  0.5f, -1.0f, // Top-right
            -0.5f,  0.5f, -1.0f  // Top-left
        };

        std::vector<GLfloat> texCoords = {
            0.0f, 0.0f, // Bottom-left
            1.0f, 0.0f, // Bottom-right
            1.0f, 1.0f, // Top-right
            0.0f, 1.0f  // Top-left
        };

        std::vector<GLuint> indices = {
            0, 1, 2, // Первый треугольник
            2, 3, 0  // Второй треугольник
        };

        for (const auto& pair : attrs) {
            if (pair.first == "name") {
                name = pair.second;
            } 
            else if (pair.first == "sizeX") {
                sizeX = XMLToFloat(pair.second);
            } 
            else if (pair.first == "sizeY") {
                sizeY = XMLToFloat(pair.second);
            } 
            else if (pair.first == "posX") {
                posX = XMLToFloat(pair.second);
            }
            else if (pair.first == "posY") {
                posY = XMLToFloat(pair.second);
            }
            else if (pair.first == "rotation") {
                rotation = XMLToFloat(pair.second);
            }
            else if (pair.first == "material") {
                matPath = pair.second;
            }
            else if (pair.first == "needCollider"){
                needCollider = XMLToBool(pair.second);
            }
        }

        Material* mat = matFactory->CreateMaterialFromXML(matPath);
        
        assert(mat != nullptr);

        GameObject* obj = new GameObject(name, mat, vertices, texCoords, indices);

        if (sizeX != 0.0f){
            obj->SetSizeX(sizeX);
        }

        if (sizeY != 0.0f){
            obj->SetSizeY(sizeY);
        }

        if(posX != 0.0f){
            obj->SetPosX(posX);
        }

        if(posY != 0.0f){
            obj->SetPosY(posY);
        }

        if(rotation != 0.0f){
            obj->SetRotation(rotation);
        }

        if (needCollider){
            CollisionBox* box = collisionBoxFactory->CreateCollisionBoxFromXML(obj->GetPosX(), obj->GetPosY(), std::fabs(obj->GetSizeX()), std::fabs(obj->GetSizeY()));
            obj->SetCollider(box);
        }

        obj->SetPosZ(0.0f);

        return obj;

    }
    return nullptr;
}