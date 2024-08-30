#include "../include/engine/Managers/collisionsManager.h"

CollisionManager::CollisionManager(Scene* scene) : scene(scene) {}

bool CollisionManager::IsCollidesWith(std::string mainObj, std::string secondObj)
{
    GameObject* obj1 = scene->GetGameObject(mainObj);
    GameObject* obj2 = scene->GetGameObject(secondObj);

    if (obj1 != nullptr && obj2 != nullptr){
        return obj1->GetCollider()->Intersects(obj2->GetCollider());
    } else {
        std::cerr << "Game object is empty" << std::endl;
    }
    return false;
}