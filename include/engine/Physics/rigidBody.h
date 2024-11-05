#ifndef RIGID_BODY_H
#define RIGID_BODY_H

class RigidBody{

    public:

    //Constructors
    RigidBody();
    RigidBody(float posX, float posY, float mass);

    //Getters
    float GetPosX() const;
    float GetPosY() const;
    float GetVelocityX() const;
    float GetVelocityY() const;
    float GetMass() const;

    //Setters
    void SetPosX(float x);
    void SetPosY(float y);
    void SetVelocityX(float x);
    void SetVelocityY(float y);
    void SetAccelerationX(float x);
    void SetAccelerationY(float y);
    void SetMass(float mass);
    void SetRotation(float angle);
    void SetAngularVelocity(float vel);

    //Methods
    void IntegratePosition(float deltaTime);
    void IntegrateRotation(float deltaTime);
    void ApplyForce(float valueX, float valueY);
    void ResetForces();


    private:

        float posX;
        float posY;

        float velocityX;
        float velocityY;

        float accelerationX;
        float accelerationY;

        float mass;
        float rotation;
        float angularVelocity; 
};

#endif