#pragma once

#include <Camera/Camera.hpp>

#include <glm/vec2.hpp>
#include <sigslot/signal.hpp>

struct GLFWwindow;

class WindowCallbacks {
public:
    static void CursorPositionCallback(GLFWwindow* pWindow, double xPos, double yPos);
    static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int modifiers);
    static void KeyboardCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers);
    static void ScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);
    
    static void WindowMaximizedCallback(GLFWwindow* pWindow, int maximized);
    static void WindowSizeCallback(GLFWwindow* pWindow, int width, int height);
    static void WindowPositionCallback(GLFWwindow* pWindow, int xPos, int yPos);

    virtual ~WindowCallbacks() = default;

    sigslot::signal<glm::ivec2> windowSizeChanged;
    sigslot::signal<glm::ivec2> windowPositionChanged;
    sigslot::signal<Camera::Projection> projectionChanged;
    sigslot::signal<bool> wireframeChanged;
    sigslot::signal<bool> windowMaximized;
    sigslot::signal<> saved;

protected:
    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {}
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {}
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {}
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) {}
    
    virtual void WindowMaximizedImpl(GLFWwindow* pWindow, int maximized) {}
    virtual void WindowSizeImpl(GLFWwindow* pWindow, int width, int height) {}
    virtual void WindowPositionImpl(GLFWwindow* pWindow, int xPos, int yPos) {}
};
