#ifndef STATIC_ACTOR_FACTORY_H
#define STATIC_ACTOR_FACTORY_H

#include "../Entities/Actors/staticActor.h"
#include "materialFactory.h"
#include "collisionBoxFactory.h"
#include <cmath>

class StaticActorFactory{

    public:

    StaticActorFactory();

    StaticActor* CreateStaticActorFromXML(std::string fileName);

    private:

    MaterialFactory* matFactory;
    CollisionBoxFactory* collisionBoxFactory;

};

#endif