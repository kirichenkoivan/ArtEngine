cmake_minimum_required(VERSION 3.15)
project(MyGameEngine)

set(CMAKE_CXX_STANDARD 17)

# Установите путь к Emscripten
set(EMSCRIPTEN_ROOT_PATH /Users/ivankiricenko/emsdk/upstream/emscripten)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_output)

# Установите путь к заголовочным файлам Emscripten
include_directories(${EMSCRIPTEN_ROOT_PATH}/cache/sysroot/include)

# Укажите путь к tinyxml2
include_directories(${CMAKE_SOURCE_DIR}/src/engine/Tools)

add_executable(MyGameEngine
    src/main.cpp
    src/engine/Graphics/renderer.cpp
    src/engine/Entities/gameObject.cpp
    src/engine/Entities/scene.cpp
    src/engine/Inputs/inputManager.cpp
    src/engine/Entities/Actors/dynamicActor.cpp
    src/engine/Entities/material.cpp
    src/engine/Tools/shaderLoader.cpp
    src/engine/Tools/tinyxml2.cpp # Добавляем tinyxml2.cpp в проект
    src/engine/Factories/materialFactory.cpp
    src/engine/Factories/gameObjectFactory.cpp
    src/engine/Tools/xmlReader.cpp
    src/engine/Tools/fileLoader.cpp
    src/engine/Entities/Colliders/collisionBox.cpp
)

target_include_directories(MyGameEngine PRIVATE include)

# Установите параметры компиляции для WebAssembly, если используется Emscripten
if (CMAKE_TOOLCHAIN_FILE MATCHES "Emscripten.cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -O3 -s --preload-file ../src/shaders@/shaders --preload-file ../src/textures@/textures --preload-file ../FileSystem@/FileSystem")
endif()

set_target_properties(MyGameEngine PROPERTIES
    OUTPUT_NAME "my_game_engine"
    SUFFIX ".html"
)
