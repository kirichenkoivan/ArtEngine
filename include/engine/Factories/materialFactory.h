#ifndef MATERIAL_FACTORY_H
#define MATERIAL_FACTORY_H

#include <memory>
#include "../Entities/material.h"
#include "../Tools/xmlReader.h"
#include "../Tools/fileLoader.h"

class MaterialFactory{

    public:
    MaterialFactory();

    static Material* CreateMaterialFromXML(const std::string& fileName);
    
    private:

    
};

#endif