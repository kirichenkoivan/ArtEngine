#ifndef STATIC_ACTOR_H
#define STATIC_ACTOR_H

#include "../gameObject.h"

class StaticActor : public GameObject {
    
    public:
        //Constructor
        StaticActor(const std::string& name, Material* material, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& texCoords, const std::vector<GLuint>& indices);

        RigidBody* GetRigidBody() override;
        void AddRigidBody(RigidBody* rb) override;

    private:

        RigidBody* rigidBody;

};

#endif