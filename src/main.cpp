#include "../include/main.h"

Application* app;

int main() {
    app = new ArtEngine();
    app->Run();
    return 0;
}
