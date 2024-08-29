#include "../include/engine/Factories/collisionBoxFactory.h"


CollisionBoxFactory::CollisionBoxFactory()
{
}

CollisionBox *CollisionBoxFactory::CreateCollisionBoxFromXML(const float& x, const float& y, const float& sX, const float& sY)
{
    CollisionBox* collider = new CollisionBox(x, y, sX, sY);
    return collider;
}
