#include <emscripten/emscripten.h>
#include "engine/renderer.h"

void main_loop() {
    RenderFrame();
}

int main() {
    initRenderer();
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}