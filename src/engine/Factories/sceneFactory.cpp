#include "../include/engine/Factories/sceneFactory.h"

SceneFactory::SceneFactory(){
    gameObjectFactory = new GameObjectFactory();
    dynamicActorFactory = new DynamicActorFactory();
    cameraFactory = new CameraFactory();
}

Scene* SceneFactory::CreateSceneFromXML(std::string fileName){

    std::filesystem::path filePath = FindXMLFile(fileName, "/FileSystem");
    std::vector<std::pair<std::string, std::string>> attrs;
    Scene* scene = new Scene();
    GameObject* gameObject;

    if(!filePath.empty()){
        std::vector<std::string> attributes = {"type", "object", "posX", "posY"};
        std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXMLFile(filePath);

        tinyxml2::XMLElement* root = doc->FirstChildElement("GameEntity");
        if (!root) {
            std::cout << "No root element found" << std::endl;
            return scene;
        }

        std::string type;
        std::string object;
        float posX = 0.0f;
        float posY = 0.0f;

        for (tinyxml2::XMLElement* obj = root->FirstChildElement("GameObject"); obj != nullptr; obj = obj->NextSiblingElement("GameObject")) {
            attrs = GetAttributesValues(obj, attributes);

            for (const auto& pair : attrs) {
                if (pair.first == "object") {
                    object = pair.second;
                } 
                else if (pair.first == "type"){
                    type = pair.second;
                }
                else if (pair.first == "posX") {
                    if (pair.second != ""){
                        posX = XMLToFloat(pair.second);
                    }
                } 
                else if (pair.first == "posY") {
                    if (pair.second != ""){
                        posY = XMLToFloat(pair.second);
                    }
                } 
            }

            if (object != ""){
                if(type == "GameObject"){
                    GameObject* obj = gameObjectFactory->CreateGameObjectFromXML(object);
                    scene->AddGameObject(obj);
                    if (posX != 0.0f){
                        scene->GetGameObject(obj->GetName())->SetPosX(posX);
                    }

                    if (posY != 0.0f){
                        scene->GetGameObject(obj->GetName())->SetPosY(posY);
                    }
                }
                else if (type == "DynamicActor"){
                    DynamicActor* obj = dynamicActorFactory->CreateDynamicActorFromXML(object);
                    scene->AddGameObject(obj);
                    if (posX != 0.0f){
                        scene->GetGameObject(obj->GetName())->SetPosX(posX);
                    }

                    if (posY != 0.0f){
                        scene->GetGameObject(obj->GetName())->SetPosY(posY);
                    }
                }
                else if (type == "Camera"){
                    Camera* cam = cameraFactory->CreateCameraFromXML(object);
                    scene->AddCamera(cam);
                }
            }
            else{
                std::cerr << "Object field is empty, GameObject was skipped" << std::endl;
            }
        }
    }
    else{
        std::cerr << "Document Path is incorrect, created empty scene" << std::endl;
    }
    
    return scene;
}