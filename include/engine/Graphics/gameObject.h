#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

// Структура для хранения позиции объекта
struct Vec3 {
    float x, y, z;
};

class GameObject {
public:
    GameObject(const std::string& name);
    
    // Методы установки данных объекта
    void setShader(const std::string& vertexShader, const std::string& fragmentShader);
    void setGeometry(const std::vector<float>& vertices);

    // Метод для получения данных объекта
    std::string getVertexShader() const;
    std::string getFragmentShader() const;
    std::vector<float> getGeometry() const;

private:
    std::string name;
    std::string vertexShader;
    std::string fragmentShader;
    std::vector<float> geometry; // Геометрия объекта (например, вершины)
    Vec3 position; // Позиция объекта
};

#endif // GAMEOBJECT_H
