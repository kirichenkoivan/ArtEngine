#ifndef CAMERA_FACTORY_H
#define CAMERA_FACTORY_H

#include "../Entities/camera.h"
#include "../Tools/xmlReader.h"
#include <string>

class CameraFactory{

    public:

        //Constructor
        CameraFactory();

        //Methods
        Camera* CreateCameraFromXML(std::string fileName);


    private:

};

#endif