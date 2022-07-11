#pragma once

#include "Controls/WindowCallbacks.hpp"

#include <memory>

#include <glm/vec3.hpp>

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

    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframe) const;
    void onClearColorChange(const glm::vec3& clearColor);

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
