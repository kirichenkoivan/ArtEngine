#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include <string>
#include "gameObject.h"

class Scene {
public:
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(const std::string& name);
    GameObject* GetGameObject(const std::string& name);
    std::unordered_map<std::string, GameObject*>& GetGameObjects();

private:
    std::unordered_map<std::string, GameObject*> gameObjects;
};

#endif // SCENE_H