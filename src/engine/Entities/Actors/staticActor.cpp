#include "../include/engine/Entities/Actors/staticActor.h"

StaticActor::StaticActor(const std::string &name, Material *material, const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &texCoords, const std::vector<GLuint> &indices) 
: GameObject(name, material, vertices, texCoords, indices)  
{
}

RigidBody* StaticActor::GetRigidBody()
{
    if (rigidBody != nullptr){
        return rigidBody;
    } else {
        std::cerr << "In object " << this->GetName() << " rigid body is not exist!!!" << std::endl;
    }
    return nullptr;
}

void StaticActor::AddRigidBody(RigidBody* rb)
{
    this->rigidBody = rb;
}
