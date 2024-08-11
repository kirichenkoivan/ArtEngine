#include "../include/engine/Graphics/scene.h"

void Scene::addObject(GameObject* object) {
    objects.push_back(object);
}

const std::vector<GameObject*>& Scene::getObjects() const {
    return objects;
}
