#ifndef DYNAMICACTOR_H
#define DYNAMICACTOR_H

#include "../gameObject.h"
#include "../../Inputs/inputManager.h"
#include "../../Inputs/KeyCodes/keyboard.h"

class DynamicActor : public GameObject {
    public:
        DynamicActor(const std::string& name, Material* material, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& texCoords, const std::vector<GLuint>& indices);

        //Setters
        void SetMoveSpeed(float speed);

        //Getters
        float GetMoveSpeed() const;

        //Methods
        void Move(float deltaTime) override;
        
    private:
        float moveSpeed = 0.1f;
};

#endif