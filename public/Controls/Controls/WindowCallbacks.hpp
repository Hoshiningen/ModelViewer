#pragma once

#include <sigslot/signal.hpp>

struct GLFWwindow;

class Camera;

class WindowCallbacks {
public:
    static void FrameBufferSizeCallback(GLFWwindow* pWindow, int width, int height);
    static void CursorPositionCallback(GLFWwindow* pWindow, double xPos, double yPos);
    static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int modifiers);
    static void KeyboardCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers);
    static void ScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);

    virtual ~WindowCallbacks() = default;

    template<typename... CallArgs>
    sigslot::connection connectProjectionChanged(CallArgs&&... args) {
        return m_projectionChanged.connect(std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    sigslot::connection connectWireframeModeChanged(CallArgs&&... args) {
        return m_wireframeChanged.connect(std::forward<CallArgs>(args)...);
    }

protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) {}
    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {}
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {}
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {}
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) {}

    sigslot::signal<int> m_projectionChanged;
    sigslot::signal<bool> m_wireframeChanged;
};
