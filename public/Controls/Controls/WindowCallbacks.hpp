#pragma once

#include "Common/SignalMacros.hpp"

#include <glm/vec2.hpp>
#include <sigslot/signal.hpp>

struct GLFWwindow;

class Camera;

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

    DEFINE_CONNECTION(m_signalProjectionChanged, ProjectionChanged)
    DEFINE_CONNECTION(m_signalWireframeChanged, WireframeChanged)
    DEFINE_CONNECTION(m_signalWindowMaximized, WindowMaximized)
    DEFINE_CONNECTION(m_signalWindowSizeChanged, WindowSizeChanged)
    DEFINE_CONNECTION(m_signalWindowPositionChanged, WindowPositionChanged)
    DEFINE_CONNECTION(m_signalSaved, ApplicationSaved)

protected:
    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {}
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {}
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {}
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) {}
    
    virtual void WindowMaximizedImpl(GLFWwindow* pWindow, int maximized) {}
    virtual void WindowSizeImpl(GLFWwindow* pWindow, int width, int height) {}
    virtual void WindowPositionImpl(GLFWwindow* pWindow, int xPos, int yPos) {}

    sigslot::signal<glm::ivec2> m_signalWindowSizeChanged;
    sigslot::signal<glm::ivec2> m_signalWindowPositionChanged;
    sigslot::signal<int> m_signalProjectionChanged;
    sigslot::signal<bool> m_signalWireframeChanged;
    sigslot::signal<bool> m_signalWindowMaximized;
    sigslot::signal<> m_signalSaved;
};
