#ifndef LAYER
#define LAYER

#include <iostream>
#include <string>
#include "Timestep.h"

class Layer {
public:
    Layer(const std::string& name = "Layer") : m_Name(name) {}

    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnEvent(const std::string& event) {}
    virtual void OnImGuiRender() {}

    const std::string& GetName() const { return m_Name; }

private:
    std::string m_Name;
};


#endif