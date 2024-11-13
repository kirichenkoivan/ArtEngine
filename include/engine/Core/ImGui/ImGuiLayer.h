#ifndef IMGUI_LAYER
#define IMGUI_LAYER

#include "../Layer.h"
#include "../ArtCore.h"

#include "../Imgui/imgui.h"
#include "../Imgui/backends/imgui_impl_glfw.h"
#include "../Imgui/backends/imgui_impl_opengl3.h"

class ImGuiLayer : public Layer{
    
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
};

#endif