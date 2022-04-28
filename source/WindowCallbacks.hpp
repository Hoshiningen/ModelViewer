#pragma once

struct GLFWwindow;

class WindowCallbacks {
public:
    static void FrameBufferSizeCallback(GLFWwindow* pWindow, int width, int height);

    static void CursorPositionCallback(GLFWwindow* pWindow, double xPos, double yPos);
    static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int modifiers);
    static void KeyboardCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers);
    static void ScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);

protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) {}
    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {}
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {}
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {}
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) {}
};

class DefaultCallbacks : public WindowCallbacks {
protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) override;

    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) override;
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) override;
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) override;
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) override;
};
