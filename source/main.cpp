#include "App.hpp"

int main() {

    Application app{ "config.json" };
    if (app.setUp())
        app.run();

    return 0;
}
