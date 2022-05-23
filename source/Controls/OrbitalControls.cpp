#include "Controls/OrbitalControls.hpp"

#include "Camera/Camera.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

struct OrbitalControls::Private {
    void updateCamera(const glm::vec3& forward, float deltaX, float deltaY);

    Camera* m_pCamera = nullptr;

    bool m_leftDrag = false;
    bool m_rightDrag = false;
};

void OrbitalControls::Private::updateCamera(const glm::vec3& forward, float deltaX, float deltaY) {

    static float kPitchRad = 0.f;
    kPitchRad = std::clamp(kPitchRad - deltaY, glm::radians(-89.f), glm::radians(89.f));

    static float kYawRad = glm::pi<float>();
    kYawRad -= deltaX;

    if (!m_pCamera)
        return;

    const glm::quat yawQuat = glm::angleAxis(kYawRad, m_pCamera->worldUp());

    // Compute the new forward vector by rotating the previous by our change in yaw.
    const glm::vec3 newForward = yawQuat * forward;

    // Update the right vector based on the new forward direction.
    const glm::vec3 right = glm::cross(glm::normalize(newForward), m_pCamera->worldUp());
    const glm::quat pitchQuat = glm::angleAxis(kPitchRad, glm::normalize(right));

    const float dist = glm::distance(m_pCamera->target(), m_pCamera->position());
    m_pCamera->position(pitchQuat * newForward * dist);
}


OrbitalControls::OrbitalControls()
    : m_pPrivate(std::make_unique<Private>()) {}

OrbitalControls::~OrbitalControls() noexcept {}

void OrbitalControls::camera(Camera* pCamera) {
    m_pPrivate->m_pCamera = pCamera;
}

void OrbitalControls::FrameBufferSizeImpl(GLFWwindow*, int width, int height) {

    glViewport(0, 0, width, height);

    if (m_pPrivate->m_pCamera)
        m_pPrivate->m_pCamera->aspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void OrbitalControls::CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {

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

    // set perspective
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        m_projectionChanged(ProjectionChange::Perspective);

    // set orthographic
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        m_projectionChanged(ProjectionChange::Orthographic);
}

void OrbitalControls::ScrollImpl(GLFWwindow*, double, double) {}
