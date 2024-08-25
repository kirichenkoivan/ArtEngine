#ifndef DYNAMIC_ACTOR_FACTORY_H
#define DYNAMIC_ACTOR_FACTORY_H

#include "../Entities/Actors/dynamicActor.h"
#include "materialFactory.h"

class DynamicActorFactory{

    public:

    DynamicActorFactory();

    DynamicActor* CreateDynamicActorFromXML(std::string fileName);

    private:

    MaterialFactory* matFactory;

};

#endif