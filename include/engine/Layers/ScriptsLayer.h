#ifndef SCRIPTS_LAYER
#define SCRIPTS_LAYER

#include "./engine/Core/ArtCore.h"
#include "./engine/Core/ArtCoreUtils.h"

#include <thread>
#include <chrono>
#include "../Tools/fileLoader.h"
#include "../WrenVM/include/wren.hpp"

class ScriptsLayer : public Layer {
    public:
        ScriptsLayer();
        virtual ~ScriptsLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;

    private:
        static void WriteFn(WrenVM* vm, const char* text);
        static void ErrorFn(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg);
        static WrenLoadModuleResult LoadModuleFn(WrenVM* vm, const char* module);

    private:
        const std::string CATEGORY = "Layers/ScriptsLayer";

        WrenConfiguration m_Config;
        WrenVM* m_VM;
        WrenHandle* m_GameEngineClass;
        WrenHandle* m_EngineInstanceHandle;
        WrenHandle* m_EngineClassHandle;
};

#endif