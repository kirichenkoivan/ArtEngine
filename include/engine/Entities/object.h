#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

class Object {

    public:

        //Setters
        virtual void SetSizeX(float sx) = 0;
        virtual void SetSizeY(float xy) = 0;
        virtual void SetPosX(float px) = 0;
        virtual void SetPosY(float py) = 0;
        virtual void SetPosZ(float pz) = 0;
        virtual void SetRotation(float nr) = 0;

        //Getters
        virtual std::string GetName() const = 0;
        virtual float GetSizeX() const = 0;
        virtual float GetSizeY() const = 0;
        virtual float GetPosX() const = 0;
        virtual float GetPosY() const = 0;
        virtual float GetPosZ() const = 0;
        virtual float GetRotation() const = 0;

};

#endif