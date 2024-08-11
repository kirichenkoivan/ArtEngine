#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include "gameObject.h"
#include "scene.h"

class ObjectManager {
public:
    ObjectManager(Scene& scene);
    
    // Создать новый объект и добавить его в сцену
    GameObject* createObject(const std::string& name);

    // Получить все объекты
    const std::vector<GameObject*>& getObjects() const;

private:
    Scene& scene;
    std::vector<GameObject*> objects;
};

#endif // OBJECTMANAGER_H
