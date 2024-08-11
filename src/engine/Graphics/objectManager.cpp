#include "../include/engine/Graphics/objectManager.h"

ObjectManager::ObjectManager(Scene& scene)
    : scene(scene) {}

GameObject* ObjectManager::createObject(const std::string& name) {
    GameObject* obj = new GameObject(name);
    objects.push_back(obj);
    scene.addObject(obj);
    return obj;
}

const std::vector<GameObject*>& ObjectManager::getObjects() const {
    return objects;
}
