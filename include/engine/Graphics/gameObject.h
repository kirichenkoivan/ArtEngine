#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include "../DataStructures/vector3.h" // Для 3D-векторов
#include "../DataStructures/matrix4.h" // Для 4x4 матриц

class GameObject {
public:
    GameObject(const std::string& name);

    void setShader(const std::string& vertexShader, const std::string& fragmentShader);
    void setGeometry(const std::vector<float>& vertices);
    void setPosition(const Vector3& position);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);
    std::string getVertexShader() const;
    std::string getFragmentShader() const;
    std::vector<float> getGeometry() const;
    Matrix4 getModelMatrix() const;

private:
    std::string name;
    std::string vertexShader;
    std::string fragmentShader;
    std::vector<float> geometry;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Matrix4 computeModelMatrix() const;
};

#endif // GAMEOBJECT_H
