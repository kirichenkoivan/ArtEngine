#ifndef COLLISION_BOX_FACTORY
#define COLLISION_BOX_FACTORY

#include "../Entities/Colliders/collisionBox.h"
#include <string>

class CollisionBoxFactory{

    public:

    CollisionBoxFactory();

    CollisionBox* CreateCollisionBoxFromXML(const float& x, const float& y, const float& sX, const float& sY);

    private:

};

#endif