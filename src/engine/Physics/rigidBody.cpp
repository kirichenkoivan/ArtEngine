#include "../include/engine/Physics/rigidBody.h"

//Construtors
// *************************************************************************************

RigidBody::RigidBody() 
: posX(0.0f), posY(0.0f), velocityX(0.0f), velocityY(0.0f), accelerationX(0.0f), accelerationY(0.0f), rotation(0.0f), angularVelocity(0.0f), mass(1.0f) 
{
}

RigidBody::RigidBody(float posX, float posY, float mass)
: posX(posX), posY(posY), velocityX(0.0f), velocityY(0.0f), accelerationX(0.0f), accelerationY(0.0f), rotation(0.0f), angularVelocity(0.0f), mass(mass) 
{
}

//Getters
// *************************************************************************************
float RigidBody::GetPosX() const
{
    return posX;
}

float RigidBody::GetPosY() const
{
    return posY;
}

float RigidBody::GetVelocityX() const
{
    return velocityX;
}

float RigidBody::GetVelocityY() const
{
    return velocityY;
}

float RigidBody::GetMass() const
{
    return mass;
}

//Setters
// *************************************************************************************
void RigidBody::SetPosX(float x)
{
    this->posX = x;
}

void RigidBody::SetPosY(float y)
{
    this->posY = y;
}

void RigidBody::SetVelocityX(float x)
{
    this->velocityX = x;
}

void RigidBody::SetVelocityY(float y)
{
    this->velocityY = y;
}

void RigidBody::SetAccelerationX(float x)
{
    this->accelerationX = x;
}

void RigidBody::SetAccelerationY(float y)
{
    this->accelerationY = y;
}

void RigidBody::SetMass(float mass)
{
    this->mass = mass;
}

void RigidBody::SetRotation(float angle)
{
    this->rotation = angle;
}

void RigidBody::SetAngularVelocity(float vel)
{
    this->angularVelocity = vel;
}

//Methods
// *************************************************************************************
void RigidBody::IntegratePosition(float deltaTime)
{
    velocityX = velocityX + accelerationX * deltaTime;
    velocityY = velocityY + accelerationY * deltaTime;

    posX = posX + velocityX * deltaTime;
    posY = posY + velocityY * deltaTime;
}

void RigidBody::IntegrateRotation(float deltaTime)
{
    rotation += angularVelocity * deltaTime;
}

void RigidBody::ApplyForce(float valueX, float valueY)
{
    float resultingAccelerationX = valueX / mass;
    float resultingAccelerationY = valueY / mass;

    accelerationX += resultingAccelerationX;
    accelerationY += resultingAccelerationY;
}

void RigidBody::ResetForces()
{
    accelerationX = 0;
    accelerationY = 0;
}
