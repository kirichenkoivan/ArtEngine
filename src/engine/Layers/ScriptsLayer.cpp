#include "../include/engine/Layers/ScriptsLayer.h"

ScriptsLayer::ScriptsLayer()
{
    Logger::GetInstance().Info(CATEGORY, "ScriptsLayer Created!");
}

void ScriptsLayer::OnAttach()
{
    wrenInitConfiguration(&config);
    config.writeFn = &WriteFn;
    config.errorFn = &ErrorFn;
    vm = wrenNewVM(&config);
}

void ScriptsLayer::OnDetach()
{
}

void ScriptsLayer::OnUpdate(Timestep ts)
{
}

void ScriptsLayer::OnImGuiRender()
{
}

void ScriptsLayer::WriteFn(WrenVM *vm, const char *text)
{
    std::cout << text << std::endl;
}

void ScriptsLayer::ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, const int line, const char *msg)
{
    switch (errorType)
    {
    case WREN_ERROR_COMPILE:
        printf("[%s line %d] [Error] %s\n", module, line, msg);
        break;
    case WREN_ERROR_STACK_TRACE:
        printf("[%s line %d] in %s\n", module, line, msg);
        break;
    case WREN_ERROR_RUNTIME:
        printf("[Runtime Error] %s\n", msg);
        break;
    }
}

WrenLoadModuleResult ScriptsLayer::LoadModuleFn(WrenVM *vm, const char *module)
{
    WrenLoadModuleResult result{};
    std::string path = findFile(module, "/FileSystem/Scripts").string();
    std::string moduleCode = readFile(path);
    if (moduleCode.empty())
    {
        result.source = nullptr;
        return result;
    }

    char *source = (char *)malloc(moduleCode.size() + 1);
    memcpy(source, moduleCode.c_str(), moduleCode.size());
    source[moduleCode.size()] = '\0';

    result.source = source;
    return result;
}