#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

class CollisionBox{

    public:

    //Constructors

    CollisionBox(const float& x, const float& y, const float& sX, const float& sY);
    CollisionBox();

    //Setters

    void SetPosX(float x);
    void SetPosY(float y);
    void SetSizeX(float x);
    void SetSizeY(float y);

    //Getters

    float GetPosX();
    float GetPosY();
    float GetSizeX();
    float GetSizeY();

    //Methods

    bool Intersects(const CollisionBox* other) const;

    private:
    
    float posX;
    float posY;
    float sizeX;
    float sizeY;

};

#endif