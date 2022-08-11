#include "Controls/OrbitalControls.hpp"

#include "Camera/Camera.hpp"

#include "Common/Math.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <iostream>

struct OrbitalControls::Private {
    void updateCamera(const glm::vec3& forward, float deltaX, float deltaY);

    Camera* m_pCamera = nullptr;

    bool m_navigationEnabled = true;
    bool m_leftDrag = false;
    bool m_rightDrag = false;
};

void OrbitalControls::Private::updateCamera(const glm::vec3& forward, float deltaX, float deltaY) {

    static float kPitchRad = 0.f;
    kPitchRad = std::clamp(kPitchRad - deltaY, glm::radians(-89.5f), glm::radians(89.5f));

    static float kYawRad = glm::pi<float>();
    kYawRad -= deltaX;

    if (!m_pCamera)
        return;

    const float dist = glm::distance(m_pCamera->target(), m_pCamera->position());
    const glm::vec3 rotForward = RotateVector(forward, kPitchRad, kYawRad, false);
    m_pCamera->position(glm::normalize(rotForward) * dist);
}


OrbitalControls::OrbitalControls()
    : m_pPrivate(std::make_unique<Private>()) {}

OrbitalControls::~OrbitalControls() noexcept {}

DEFINE_SETTER_COPY(OrbitalControls, camera, m_pPrivate->m_pCamera)
DEFINE_SETTER_COPY(OrbitalControls, navigationEnabled, m_pPrivate->m_navigationEnabled)

void OrbitalControls::FrameBufferSizeImpl(GLFWwindow*, int width, int height) {

    glViewport(0, 0, width, height);

    if (m_pPrivate->m_pCamera)
        m_pPrivate->m_pCamera->aspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void OrbitalControls::CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {

    if (!m_pPrivate->m_navigationEnabled)
        return;

    static auto [lastX, lastY] = std::make_tuple(static_cast<float>(xPos), static_cast<float>(yPos));

    constexpr float kSensitivity = 0.01f;

    const float deltaX = (static_cast<float>(xPos) - lastX) * kSensitivity;
    const float deltaY = (lastY - static_cast<float>(yPos)) * kSensitivity;

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    if (!m_pPrivate->m_pCamera)
        return;

    if (m_pPrivate->m_leftDrag) {

        static const glm::vec3 kForward = m_pPrivate->m_pCamera->forward();
        m_pPrivate->updateCamera(kForward, deltaX, deltaY);
    }

    if (m_pPrivate->m_rightDrag) {
        static float kFoV = m_pPrivate->m_pCamera->fovY();
        kFoV = std::clamp(kFoV - deltaY, glm::radians(0.1f), glm::radians(175.f));

        m_pPrivate->m_pCamera->fovY(kFoV);
    }
}

void OrbitalControls::MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        m_pPrivate->m_leftDrag = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        m_pPrivate->m_leftDrag = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        m_pPrivate->m_rightDrag = true;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        m_pPrivate->m_rightDrag = false;
}

void OrbitalControls::KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        m_signalProjectionChanged(0);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        m_signalProjectionChanged(1);

    static bool m_wireframe = false;
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        m_wireframe = !m_wireframe;
        m_signalWireframeChanged(m_wireframe);
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        if (modifiers & GLFW_MOD_CONTROL)
            m_signalSaved();
    }
}

void OrbitalControls::ScrollImpl(GLFWwindow*, double, double) {}

void OrbitalControls::WindowMaximizedImpl(GLFWwindow* pWindow, int maximized) {
    m_signalWindowMaximized(maximized != 0 ? true : false);
}

void OrbitalControls::WindowSizeImpl(GLFWwindow* pWindow, int width, int height) {
    m_signalWindowSizeChanged(glm::ivec2{ width, height });
}

void OrbitalControls::WindowPositionImpl(GLFWwindow* pWindow, int xPos, int yPos) {
    m_signalWindowPositionChanged(glm::ivec2{ xPos, yPos });
}
