#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "gameObject.h"

class Scene {
public:
    void addObject(GameObject* object);
    const std::vector<GameObject*>& getObjects() const;

private:
    std::vector<GameObject*> objects;
};

#endif // SCENE_H
