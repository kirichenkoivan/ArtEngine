cmake_minimum_required(VERSION 3.15)
project(MyGameEngine)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Путь к Emscripten
set(EMSCRIPTEN_ROOT_PATH /Users/ivankiricenko/emsdk/upstream/emscripten)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_output)

# Установка пути к заголовочным файлам Emscripten
include_directories(${EMSCRIPTEN_ROOT_PATH}/cache/sysroot/include)

# Путь к заголовочным файлам tinyxml2
include_directories(${CMAKE_SOURCE_DIR}/src/engine/Tools)

# Путь к glm
set(glm_DIR /usr/local/Cellar/glm/1.0.1/lib/cmake/glm)
include_directories(/usr/local/Cellar/glm/1.0.1/include)

# Источники проекта
add_executable(MyGameEngine
    src/main.cpp
    src/engine/Graphics/renderer.cpp
    src/engine/Entities/gameObject.cpp
    src/engine/Entities/scene.cpp
    src/engine/Inputs/inputManager.cpp
    src/engine/Entities/Actors/dynamicActor.cpp
    src/engine/Entities/material.cpp
    src/engine/Tools/shaderLoader.cpp
    src/engine/Tools/tinyxml2.cpp
    src/engine/Factories/materialFactory.cpp
    src/engine/Factories/gameObjectFactory.cpp
    src/engine/Factories/dynamicActorFactory.cpp
    src/engine/Factories/cameraFactory.cpp
    src/engine/Factories/collisionBoxFactory.cpp
    src/engine/Factories/sceneFactory.cpp
    src/engine/Tools/xmlReader.cpp
    src/engine/Tools/fileLoader.cpp
    src/engine/Entities/Colliders/collisionBox.cpp
    src/engine/Entities/camera.cpp
    src/engine/Managers/collisionsManager.cpp
    src/engine/Managers/scriptsManager.cpp

    # Wren VM sources
    WrenVM/optional/wren_opt_meta.c
    WrenVM/optional/wren_opt_random.c 
    WrenVM/vm/wren_compiler.c
    WrenVM/vm/wren_core.c
    WrenVM/vm/wren_debug.c
    WrenVM/vm/wren_primitive.c
    WrenVM/vm/wren_utils.c
    WrenVM/vm/wren_value.c
    WrenVM/vm/wren_vm.c
)

# Указываем директорию заголовочных файлов
target_include_directories(MyGameEngine
    PRIVATE include
    PRIVATE WrenVM/include  # Путь к заголовочным файлам Wren
    PRIVATE WrenVM/vm       # Путь к vm, содержащий wren_common.h
    PRIVATE WrenVM/optional
)

# Установите параметры компиляции для WebAssembly, если используется Emscripten
if (CMAKE_TOOLCHAIN_FILE MATCHES "Emscripten.cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -O3 -s --preload-file ../src/shaders@/shaders --preload-file ../src/textures@/textures --preload-file ../FileSystem@/FileSystem")
endif()

# Устанавливаем имя и суффикс выходного файла
set_target_properties(MyGameEngine PROPERTIES
    OUTPUT_NAME "my_game_engine"
    SUFFIX ".html"
)
