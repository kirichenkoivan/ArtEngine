#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <iostream>
#include <string>

#include "../Entities/scene.h"

class CollisionManager{

    public:

    //Constructor
    CollisionManager(Scene* scene);

    //Methods
    bool IsCollidesWith(std::string mainObj, std::string secondObj);

    private:

    Scene* scene;

};

#endif 