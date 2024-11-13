#include "../include/engine/Core/ArtCoreUtils.h"
#include "../include/engine/Core/Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(const std::string name, uint32_t width, uint32_t height)
{
    s_Instance = this;

    bool inited = glfwInit();
    if (!inited) {
        Logger::GetInstance().Error(CATEGORY, "Failed to Initialize GLFW!");
        assert(inited && "Failed to Initialize GLFW!");
    }

    Logger::GetInstance().Info(CATEGORY, "GLFW Succesefully Inited");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); 

    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (m_Window == nullptr) {
        Logger::GetInstance().Error(CATEGORY, "Failed to Create Window!");
        glfwTerminate();
        assert(false && "Failed to Create Window!");
    }

    Logger::GetInstance().Info(CATEGORY, "Window Succesefully Created!");

    if (!gladLoadGLES2Loader((GLADloadproc)eglGetProcAddress)) {
        Logger::GetInstance().Error(CATEGORY, "Failed to initialize OpenGL ES 3.0 with GLAD");
        assert(false && "Failed to initialize OpenGL ES 3.0 with GLAD");
    }

    Logger::GetInstance().Info(CATEGORY, "OpenGL ES 3.0 initialized successfully");

    InputManager::GetInstance().Initialize();
    Logger::GetInstance().Info(CATEGORY, "Input Manager succesefully inited");

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
    
    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

void Application::GameLoop()
{
    float time = (float)glfwGetTime();
    Timestep timestep = time - m_LastFrameTime;
    m_LastFrameTime = time;

    for (Layer* layer : m_LayerStack){
        layer->OnUpdate(timestep);
    }

    m_ImGuiLayer->Begin();

    for (Layer* layer : m_LayerStack){
        layer->OnImGuiRender();
    }

    m_ImGuiLayer->End();
}

void Application::Run()
{
    emscripten_set_main_loop_arg(MainLoop, this, 0, 1);
}

void Application::PushLayer(Layer *layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer *layer)
{
    m_LayerStack.PushOverlay(layer);
}

float Application::GetLastFrameTime()
{
    return m_LastFrameTime;
}

void Application::SetLastFrameTime(float time)
{
    m_LastFrameTime = time;
}

void Application::MainLoop(void *arg)
{
    static_cast<Application*>(arg)->GameLoop();
}
