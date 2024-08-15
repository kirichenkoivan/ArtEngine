// scene.cpp
#include "../include/engine/Entities/scene.h"

void Scene::AddGameObject(GameObject* gameObject) {
    gameObjects[gameObject->GetName()] = gameObject;
}

void Scene::RemoveGameObject(const std::string& name) {
    gameObjects.erase(name);
}

GameObject* Scene::GetGameObject(const std::string& name) {
    auto it = gameObjects.find(name);
    if (it != gameObjects.end()) {
        return it->second;
    }
    return nullptr;
}

std::unordered_map<std::string, GameObject*>& Scene::GetGameObjects() {
    return gameObjects;
}