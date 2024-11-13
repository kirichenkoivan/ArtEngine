#include "engine/Managers/scriptsManager.h"
#include "engine/Core/Application.h"
#include "engine/Layers/RenderLayer.h"

class ArtEngine : public Application {
    public:
    ArtEngine() {
        PushLayer(new RenderLayer());
    }
};