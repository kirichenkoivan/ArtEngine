#ifndef GAME_OBJECT_FACTORY
#define GAME_OBJECT_FACTORY

#include "materialFactory.h"
#include "../Entities/gameObject.h"
#include "../Tools/xmlReader.h"
#include "../Tools/fileLoader.h"

class GameObjectFactory{
    public:

    static GameObject CreateGameObjectFromXML(std::string fileName);

    private:

};

#endif