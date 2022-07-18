#pragma once

#include "Common/ClassMacros.hpp"

#include "Controls/WindowCallbacks.hpp"

struct GLFWwindow;

class Camera;

class OrbitalControls : public WindowCallbacks {
public:
    OrbitalControls();

    COPY_MOVE_DISABLED(OrbitalControls)

    DECLARE_SETTER_COPY(camera, Camera*)
    DECLARE_SETTER_COPY(navigationEnabled, bool)

protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) override;

    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) override;
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) override;
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) override;
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) override;
    
    virtual void WindowMaximizedImpl(GLFWwindow* pWindow, int maximized) override;
    virtual void WindowSizeImpl(GLFWwindow* pWindow, int width, int height) override;
    virtual void WindowPositionImpl(GLFWwindow* pWindow, int xPos, int yPos) override;

private:
    COMPILATION_FIREWALL(OrbitalControls)
};
