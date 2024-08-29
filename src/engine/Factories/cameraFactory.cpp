#include "../include/engine/Factories/cameraFactory.h"

CameraFactory::CameraFactory(){}

Camera* CameraFactory::CreateCameraFromXML(std::string fileName){

    std::filesystem::path filePath = FindXMLFile(fileName, "/FileSystem");

    if(!filePath.empty()){
        std::vector<std::string> attributes = {"screenWidth", "screenHeight", "posX"};
        std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXMLFile(filePath);
        std::vector<std::pair<std::string, std::string>> attrs = GetAttributesValues(*doc, attributes);

        float screenWidth = NULL;
        float screenHeight = NULL;
        float posX = NULL;

        for (const auto& pair : attrs) {
            if (pair.first == "screenWidth") {
                if(pair.second != ""){
                    screenWidth = XMLToFloat(pair.second);
                } else {
                    std::cerr << "Attrubute is empty" << std::endl;
                }
            } else if (pair.first == "screenHeight") {
                if(pair.second != ""){
                    screenHeight = XMLToFloat(pair.second);
                } else {
                    std::cerr << "Attrubute is empty" << std::endl;
                }
            } else if (pair.first == "posX") {
                if(pair.second != ""){
                    posX = XMLToFloat(pair.second);
                } else {
                    std::cerr << "Attrubute is empty" << std::endl;
                }
            } 
        }

        if (screenWidth != NULL && screenHeight != NULL && posX != NULL){
            Camera* cam = new Camera(screenWidth, screenHeight);
            cam->SetPositionX(posX);
            return cam;
        }
    } else {
        std::cerr << "File path is empty" << std::endl;
    }
    return nullptr;
}