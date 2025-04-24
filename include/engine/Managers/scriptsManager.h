#ifndef SCRIPTS_MANAGER
#define SCRIPTS_MANAGER

#include <iostream>
#include <thread>
#include <chrono>
#include "../Tools/fileLoader.h"
#include "../WrenVM/include/wren.hpp"

class ScriptsManager
{
public:
    static ScriptsManager &GetInstance()
    {
        static ScriptsManager instance;
        return instance;
    };

    ScriptsManager(const ScriptsManager &) = delete;
    ScriptsManager &operator=(const ScriptsManager &) = delete;
    ~ScriptsManager();

    void InitScripts();
    void UpdateScripts();

private:
    ScriptsManager() { vmIsInited = false; };

    WrenConfiguration config;
    WrenVM *vm;
    WrenHandle *gameEngineClass;
    WrenHandle *engineInstanceHandle;
    WrenHandle *engineClassHandle;

    bool vmIsInited;

    void CreateWrenVM();

    static void WriteFn(WrenVM *vm, const char *text);
    static void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, const int line, const char *msg);
    static WrenLoadModuleResult LoadModuleFn(WrenVM *vm, const char *module);
};

#endif