#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

#include "../Entities/scene.h"
#include "gameObjectFactory.h"
#include "dynamicActorFactory.h"
#include "cameraFactory.h"
#include "../Tools/xmlReader.h"

class SceneFactory{

    public:

    // Constructor
    SceneFactory();

    // Methods

    Scene* CreateSceneFromXML(std::string fileName);

    private:

    GameObjectFactory* gameObjectFactory;
    DynamicActorFactory* dynamicActorFactory;
    CameraFactory* cameraFactory;

};

#endif