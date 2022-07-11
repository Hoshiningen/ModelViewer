#pragma once

#include "Controls/WindowCallbacks.hpp"

#include <memory>

struct GLFWwindow;

class Camera;

class OrbitalControls : public WindowCallbacks {
public:
    OrbitalControls();

    virtual ~OrbitalControls() noexcept;

    // Disable copy semantics.
    OrbitalControls(const OrbitalControls& other) = delete;
    OrbitalControls& operator=(const OrbitalControls& other) = delete;

    // Disable move semantics.
    OrbitalControls(OrbitalControls&& other) noexcept = delete;
    OrbitalControls& operator=(OrbitalControls&& other) noexcept = delete;

    void camera(Camera* pCamera);
    void navigationEnabled(bool isEnabled);

protected:
    virtual void FrameBufferSizeImpl(GLFWwindow* pWindow, int width, int height) override;

    virtual void CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) override;
    virtual void MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) override;
    virtual void KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) override;
    virtual void ScrollImpl(GLFWwindow* pWindow, double xOffset, double yOffset) override;

private:
    class Private;
    std::unique_ptr<Private> m_pPrivate;
};
