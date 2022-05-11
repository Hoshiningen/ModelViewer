#pragma once

#include "WindowCallbacks.hpp"

struct GLFWwindow;

class Camera;

class OrbitalControls : public WindowCallbacks {
public:
    void camera(Camera* pCamera);

protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) override;

    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) override;
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) override;
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) override;
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) override;

private:
    void updateCamera(const glm::vec3& forward, float deltaX, float deltaY);

    Camera* m_pCamera;

    bool m_leftDrag = false;
    bool m_rightDrag = false;
};
