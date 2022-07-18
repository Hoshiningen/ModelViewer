#pragma once

#include "Common/ClassMacros.hpp"
#include "Controls/WindowCallbacks.hpp"

#include <glm/vec3.hpp>

struct GLFWwindow;

class Application {
public:
    Application();

    COPY_MOVE_DISABLED(Application)

    bool setUp();
    void run();

private:
    COMPILATION_FIREWALL(Application)
};
