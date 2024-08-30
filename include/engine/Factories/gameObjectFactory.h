#ifndef GAME_OBJECT_FACTORY
#define GAME_OBJECT_FACTORY

#include "materialFactory.h"
#include "../Entities/gameObject.h"
#include "../Tools/xmlReader.h"
#include "../Tools/fileLoader.h"
#include "collisionBoxFactory.h"
#include <cassert>
#include <cmath>

class GameObjectFactory{
    public:

    GameObjectFactory();

    GameObject* CreateGameObjectFromXML(std::string fileName);

    private:

    CollisionBoxFactory* collisionBoxFactory;
    MaterialFactory* matFactory;
};

#endif