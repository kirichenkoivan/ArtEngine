#ifndef APPLICATION
#define APPLICATION

#include "ArtCore.h"
#include "ImGui/ImGuiLayer.h"


#include <string>
#include <functional>

class Application {
    
    public:

        Application(const std::string name = "ArtEngine", uint32_t width = 1920, uint32_t height = 1080);
        ~Application() = default;

        void GameLoop();
        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        float GetLastFrameTime();
        void SetLastFrameTime(float time);

        static void MainLoop(void* arg);

        inline GLFWwindow* GetWindow () { return m_Window; }

        inline static Application& Get() { return *s_Instance; }

    private:
        GLFWwindow* m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        const std::string CATEGORY = "Core/Application/";

        static Application* s_Instance;

};

#endif