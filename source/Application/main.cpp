#include "Application.hpp"

int main() {

    Application app;
    if (app.setUp())
        app.run();

    return 0;
}
