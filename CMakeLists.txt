cmake_minimum_required(VERSION 3.15)
project(MyGameEngine)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Путь к Emscripten
set(EMSCRIPTEN_ROOT_PATH /Users/ivankiricenko/emsdk/upstream/emscripten)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_output)

set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_ROOT_PATH}/em++")
set(CMAKE_C_COMPILER "${EMSCRIPTEN_ROOT_PATH}/emcc")


# Установка пути к заголовочным файлам Emscripten
include_directories(${EMSCRIPTEN_ROOT_PATH}/cache/sysroot/include)

# Путь к заголовочным файлам tinyxml2
include_directories(${CMAKE_SOURCE_DIR}/src/engine/Tools)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/Imgui)
include_directories(${IMGUI_DIR})

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# Путь к glm
set(glm_DIR /usr/local/Cellar/glm/1.0.1/lib/cmake/glm)
include_directories(/usr/local/Cellar/glm/1.0.1/include)

# Путь к GLFW
include_directories(/usr/local/Cellar/glfw/3.3.8/include)  # Заголовочные файлы GLFW
link_directories(/usr/local/Cellar/glfw/3.3.8/lib)  # Библиотеки GLFW

# Источники проекта
add_executable(MyGameEngine
    src/main.cpp
    src/engine/Inputs/inputManager.cpp
    src/engine/Tools/shaderLoader.cpp
    src/engine/Tools/tinyxml2.cpp
    src/engine/Tools/xmlReader.cpp
    src/engine/Tools/fileLoader.cpp
    src/engine/Managers/scriptsManager.cpp
    src/engine/Core/LayerStack.cpp
    src/engine/Tools/logger.cpp
    src/engine/Core/Application.cpp
    src/engine/Core/ImGui/ImGuiLayer.cpp
    src/engine/Layers/RenderLayer.cpp
    src/engine/Graphics/Shader.cpp
    src/engine/Graphics/Cameras/OrthographicCamera.cpp
    src/engine/Graphics/Cameras/OrthographicCameraController.cpp
    src/engine/Graphics/Meshes/Mesh.cpp
    src/engine/Graphics/ArtGraphics.cpp
    src/engine/Graphics/Texture.cpp
    src/engine/Core/ArtCore.cpp
    src/engine/Graphics/Material.cpp
    src/engine/Tools/GraphicsLoaders/ShaderLoader.cpp
    src/engine/Tools/GraphicsLoaders/MaterialLoader.cpp
    src/engine/Tools/GraphicsLoaders/MeshLoader.cpp
    src/engine/Graphics/Pools/ShaderPool.cpp
    src/engine/Graphics/Pools/MaterialPool.cpp

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

    #Imgui sources
    ${IMGUI_SOURCES}

    #OpenGL sources
    
)

# Указываем директорию заголовочных файлов
target_include_directories(MyGameEngine
    PRIVATE include
    PRIVATE WrenVM/include  # Путь к заголовочным файлам Wren
    PRIVATE WrenVM/vm       # Путь к vm, содержащий wren_common.h
    PRIVATE WrenVM/optional
)

# Линкуем с GLFW
target_link_libraries(MyGameEngine glfw)

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Core/Glad/glad.c)
target_link_libraries(MyGameEngine glad)


# Устанавливаем параметры компиляции для WebAssembly, если используется Emscripten
if (CMAKE_TOOLCHAIN_FILE MATCHES "Emscripten.cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -s USE_GLFW=3 -s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 --source-map --source-map-base=http://localhost:8888/ --preload-file ../FileSystem@/FileSystem --shell-file=\"${CMAKE_SOURCE_DIR}/templates/template.html\"")
endif()


# Устанавливаем имя и суффикс выходного файла
set_target_properties(MyGameEngine PROPERTIES
    OUTPUT_NAME "my_game_engine"
    SUFFIX ".html"
)

# Определяем макросы для разных типов сборок
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(MyGameEngine PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(MyGameEngine PRIVATE RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Retail")
    target_compile_definitions(MyGameEngine PRIVATE RETAIL)
endif()