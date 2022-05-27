#pragma once

#include "Controls/WindowCallbacks.hpp"

#include <memory>

struct GLFWwindow;

class Application {
public:
    Application();
    virtual ~Application() noexcept;

    // Disable copy semantics.
    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    // Disable move semantics.
    Application(Application&& other) noexcept = delete;
    Application& operator=(Application&& other) noexcept = delete;

    bool setUp();
    void run();

    void onProjectionChange(WindowCallbacks::ProjectionChange projection);
    void onWireframeModeChange(bool wireframe);

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
