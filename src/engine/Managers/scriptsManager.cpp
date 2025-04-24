#include "../include/engine/Managers/scriptsManager.h"

ScriptsManager::~ScriptsManager()
{
    wrenFreeVM(vm);
}

void ScriptsManager::InitScripts()
{
    if (vmIsInited)
    {
        return;
    }
    CreateWrenVM();

    std::string scripts = readFile("/FileSystem/Scripts/BaseLibrary/GameEngine.wren");
    std::string mainScript = readFile("/FileSystem/Scripts/Main.wren");

    const char *module = "GameEngine";
    const char *mainModule = "Main";

    const char *script = scripts.c_str();
    const char *chMainScript = mainScript.c_str();

    WrenInterpretResult result = wrenInterpret(vm, module, script);

    if (result != WREN_RESULT_SUCCESS)
    {
        std::cerr << "Failed to load Wren script.\n";
        wrenFreeVM(vm);
        return;
    }

    result = wrenInterpret(vm, mainModule, chMainScript);

    if (result != WREN_RESULT_SUCCESS)
    {
        std::cerr << "Failed to load Wren script.\n";
        wrenFreeVM(vm);
        return;
    }
}

void ScriptsManager::UpdateScripts()
{

    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, "GameEngine", "Engine", 0);
    engineClassHandle = wrenGetSlotHandle(vm, 0);
    if (!engineClassHandle)
    {
        std::cerr << "Failed to get handle for Engine class.\n";
        return;
    }

    gameEngineClass = wrenMakeCallHandle(vm, "Update()");
    if (!gameEngineClass)
    {
        std::cerr << "Failed to create handle for update method.\n";
        return;
    }
    if (wrenCall(vm, gameEngineClass) != WREN_RESULT_SUCCESS)
    {
        std::cerr << "Failed to call update.\n";
    }

    wrenReleaseHandle(vm, engineClassHandle);
    wrenReleaseHandle(vm, gameEngineClass);
}

void ScriptsManager::CreateWrenVM()
{
    wrenInitConfiguration(&config);
    config.writeFn = &WriteFn;
    config.errorFn = &ErrorFn;
    vm = wrenNewVM(&config);
}

void ScriptsManager::WriteFn(WrenVM *vm, const char *text)
{
    std::cout << text << std::endl;
}

void ScriptsManager::ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, const int line, const char *msg)
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

WrenLoadModuleResult ScriptsManager::LoadModuleFn(WrenVM *vm, const char *module)
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