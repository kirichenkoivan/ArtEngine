cmake_minimum_required(VERSION 3.15)
project(MyGameEngine)

set(CMAKE_CXX_STANDARD 17)

# Установите путь к Emscripten
set(EMSCRIPTEN_ROOT_PATH /Users/ivankiricenko/emsdk/upstream/emscripten)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_output)
# Установите путь к заголовочным файлам Emscripten
include_directories(${EMSCRIPTEN_ROOT_PATH}/cache/sysroot/include)

add_executable(MyGameEngine
    src/main.cpp
    src/engine/renderer.cpp
    src/engine/Graphics/gameObject.cpp
    src/engine/Graphics/scene.cpp
    src/engine/Graphics/objectManager.cpp
    src/engine/DataStructures/matrix4.cpp
)

target_include_directories(MyGameEngine PRIVATE include)

# Установите параметры компиляции для WebAssembly, если используется Emscripten
if (CMAKE_TOOLCHAIN_FILE MATCHES "Emscripten.cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s DISABLE_EXCEPTION_CATCHING=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -O3 -s DISABLE_EXCEPTION_CATCHING=1")
endif()

set_target_properties(MyGameEngine PROPERTIES
    OUTPUT_NAME "my_game_engine"
    SUFFIX ".html"
)