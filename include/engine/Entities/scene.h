#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include <string>
#include <type_traits>
#include "gameObject.h"
#include "Actors/dynamicActor.h"
#include "camera.h"

class Scene {
public:
    void AddGameObject(GameObject* gameObject);
    void AddCamera(Camera* cam);
    Camera* GetCamera();
    void RemoveGameObject(const std::string& name);
    GameObject* GetGameObject(const std::string& name);
    std::unordered_map<std::string, GameObject*>& GetGameObjects();

private:
    std::unordered_map<std::string, GameObject*> gameObjects;
    Camera* camera;
};

#endif // SCENE_H