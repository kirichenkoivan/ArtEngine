# ArtEngine
![Локальное изображение](ArtEngineBanner.png)
## Description
`ArtEngine` is a game engine designed to work with WebAssembly and WebGL. The engine supports working with objects, scenes, materials, collisions, and includes the ability to load shaders and textures.

## Requirements

### System Requirements
- **Operating System**: Windows, macOS, Linux
- **Compiler**: C++17 support (e.g., `clang++`, `g++`)
- **CMake**: Minimum version 3.15
- **Emscripten**: Required for compiling the project to WebAssembly

### Dependencies
- **Emscripten**: Ensure that Emscripten is installed and the path to its root directory is specified in the `EMSCRIPTEN_ROOT_PATH` variable in `CMakeLists.txt`.
- **TinyXML2**: Included directly through the source files located in the project.

## Building the Project

### 1. Installing Emscripten
Download and install Emscripten by following the official [guide](https://emscripten.org/docs/getting_started/downloads.html). After installation, configure the environment variables for Emscripten.

### 2. Configuring the Project
- Clone the project repository:
    ```bash
    git clone https://github.com/kirichenkoivan/ArtEngine.git
    cd ArtEngine
    ```

- Ensure that the path to Emscripten is correctly set in the `CMakeLists.txt` file:
    ```cmake
    set(EMSCRIPTEN_ROOT_PATH /path/to/emsdk/upstream/emscripten)
    ```

### 3. Building the Project
To build the project, follow these steps:

1. Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

2. Run `cmake` to generate the build files:
    ```bash
    emcmake cmake .. 
    ```

3. Build the project:
    ```bash
    emmake make  
    ```

### 4. Running the Project
After a successful build, the file `my_game_engine.html` will appear in the `build_output` directory. To run the project, you need to move the `buil_output` folder to the directory of your local server (for example OpenServer or MAMP).

## Notes
- The project will be built as a WebAssembly application, generating an HTML file to run in a browser.
- Ensure that all necessary resources (shaders, textures, etc.) are located in the appropriate project directories.




# ArtEngine

## Описание
`ArtEngine` — это игровой движок, предназначенный для работы с WebAssembly и WebGL. Движок поддерживает работу с объектами, сценами, материалами, коллизиями и включает возможность загрузки шейдеров и текстур.

## Требования

### Системные требования
- **Операционная система**: Windows, macOS, Linux
- **Компилятор**: Поддержка C++17 (например, `clang++`, `g++`)
- **CMake**: Минимальная версия 3.15
- **Emscripten**: Для компиляции проекта в WebAssembly

### Зависимости
- **Emscripten**: Убедитесь, что Emscripten установлен и путь до его корневого каталога прописан в переменной `EMSCRIPTEN_ROOT_PATH` в `CMakeLists.txt`.
- **TinyXML2**: Подключается напрямую через исходные файлы, расположенные в проекте.

## Сборка проекта

### 1. Установка Emscripten
Скачайте и установите Emscripten, следуя официальной [инструкции](https://emscripten.org/docs/getting_started/downloads.html). После установки, настройте переменные окружения для Emscripten.

### 2. Настройка проекта
- Склонируйте репозиторий с проектом:
    ```bash
    git clone https://github.com/kirichenkoivan/ArtEngine.git
    cd ArtEngine
    ```

- Убедитесь, что путь к Emscripten корректно указан в файле `CMakeLists.txt`:
    ```cmake
    set(EMSCRIPTEN_ROOT_PATH /path/to/emsdk/upstream/emscripten)
    ```

### 3. Сборка проекта
Для сборки проекта выполните следующие шаги:

1. Создайте директорию для сборки:
    ```bash
    mkdir build
    cd build
    ```

2. Вызовите `cmake` для генерации файлов сборки:
    ```bash
    emcmake cmake .. 
    ```

3. Соберите проект:
    ```bash
    emmake make  
    ```

### 4. Запуск проекта
После успешной сборки, в директории `build_output` появится файл `my_game_engine.html`. Для запуска проекта необходимо перенести папку `build_output` в директорию вашего локального сервера (например OpenServer или MAMP).

## Примечания
- Проект будет собран как WebAssembly приложение, сгенерировав HTML-файл для запуска в браузере.
- Убедитесь, что все необходимые ресурсы (шейдеры, текстуры и т.д.) находятся в соответствующих директориях проекта.
