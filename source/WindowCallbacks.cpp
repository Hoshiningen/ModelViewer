#include "WindowCallbacks.hpp"

#include "Renderer/Renderer.hpp"
#include "RendererUtil/Camera.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


namespace {
void PrintVector(const std::string& name, const glm::vec3& vector) {
    std::cout << name << ": " << vector.x << "," << vector.y << "," << vector.z << "\n";
}

void PrintAngles(const glm::vec3& vector) {
    std::cout << "Euler Angles: " << glm::degrees(vector.x) << "," << glm::degrees(vector.y) << "," << glm::degrees(vector.z) << "\n";
}

void PrintAngles(const glm::quat& quat) {

    const glm::vec3 angles = glm::eulerAngles(quat);
    std::cout << "Euler Angles: " << glm::degrees(angles.x) << "," << glm::degrees(angles.y) << "," << glm::degrees(angles.z) << "\n";
}

void PrintAngleBetween(const glm::vec3& a, const glm::vec3& b) {
    const float angle = glm::degrees(glm::acos(glm::dot(glm::normalize(a), glm::normalize(b))));
    std::cout << "Angle (degrees): " << angle << "\n";
}
}

void WindowCallbacks::CursorPositionCallback(GLFWwindow* pWindow, double xPos, double yPos) {

    WindowCallbacks* pCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(pWindow));
    if (!pCallbacks)
        return;

    pCallbacks->CursorPositionImpl(pWindow, xPos, yPos);
}

void WindowCallbacks::MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int modifiers) {

    WindowCallbacks* pCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(pWindow));
    if (!pCallbacks)
        return;

    pCallbacks->MouseButtonImpl(pWindow, button, action, modifiers);
}

void WindowCallbacks::KeyboardCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {

    WindowCallbacks* pCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(pWindow));
    if (!pCallbacks)
        return;

    pCallbacks->KeyboardImpl(pWindow, key, scanCode, action, modifiers);
}

void WindowCallbacks::ScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset) {

    WindowCallbacks* pCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(pWindow));
    if (!pCallbacks)
        return;

    pCallbacks->ScrollImpl(pWindow, xOffset, yOffset);
}

void WindowCallbacks::FrameBufferSizeCallback(GLFWwindow* pWindow, int width, int height) {

    WindowCallbacks* pCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(pWindow));
    if (!pCallbacks)
        return;

    pCallbacks->FrameBufferSizeImpl(pWindow, width, height);
}

void OrbitalControls::camera(Camera* pCamera) {
    m_pCamera = pCamera;
}

void OrbitalControls::FrameBufferSizeImpl(GLFWwindow*, int width, int height) {

    glViewport(0, 0, width, height);
    
    if (PerspectiveCamera* pCamera = dynamic_cast<PerspectiveCamera*>(m_pCamera))
        pCamera->aspectRatio(static_cast<float>(width) / static_cast<float>(height));


    if (OrthographicCamera* pCamera = dynamic_cast<OrthographicCamera*>(m_pCamera)) {

        const float w = static_cast<float>(width);
        const float h = static_cast<float>(height);

        pCamera->leftExtent(w / -2);
        pCamera->rightExtent(w / 2);
        pCamera->bottomExtent(h / -2);
        pCamera->topExtent(h / 2);
    }
}

void OrbitalControls::CursorPositionImpl(GLFWwindow* pWindow, double xPos, double yPos) {

    static auto [lastX, lastY] = std::make_tuple(static_cast<float>(xPos), static_cast<float>(yPos));

    constexpr float kSensitivity = 0.01f;

    const float deltaX = (static_cast<float>(xPos) - lastX) * kSensitivity;
    const float deltaY = (lastY - static_cast<float>(yPos)) * kSensitivity;

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    if (m_leftDrag) {

        static const glm::vec3 kForward = m_pCamera->forward();
        updateCamera(kForward, deltaX, deltaY);
    }

    if (m_rightDrag) {

        if (PerspectiveCamera* pCamera = dynamic_cast<PerspectiveCamera*>(m_pCamera)) {

            static float kFoV = pCamera->fov();
            kFoV = std::clamp(kFoV - deltaY, glm::radians(0.1f), glm::radians(130.f));

            pCamera->fov(kFoV);
        }

        if (OrthographicCamera* pCamera = dynamic_cast<OrthographicCamera*>(m_pCamera)) {

            const static float kWidth = std::fabs(pCamera->leftExtent() - pCamera->rightExtent());
            const static float kHeight = std::fabs(pCamera->bottomExtent() - pCamera->topExtent());

            static float kZoom = 1.f;
            kZoom = std::clamp(kZoom - deltaY, 0.2f, 4.f);

            pCamera->leftExtent((kWidth * kZoom) / - 2);
            pCamera->rightExtent((kWidth * kZoom) / 2);
            pCamera->bottomExtent((kHeight * kZoom) / -2);
            pCamera->topExtent((kHeight * kZoom) / 2);
        }
    }
}

void OrbitalControls::MouseButtonImpl(GLFWwindow* pWindow, int button, int action, int modifiers) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        m_leftDrag = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        m_leftDrag = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        m_rightDrag = true;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        m_rightDrag = false;
}

void OrbitalControls::KeyboardImpl(GLFWwindow* pWindow, int key, int scanCode, int action, int modifiers) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    // set perspective
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        false;

    // set orthographic
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        true;
}

void OrbitalControls::ScrollImpl(GLFWwindow*, double, double) {}

void OrbitalControls::updateCamera(const glm::vec3& forward, float deltaX, float deltaY) {

    static float kPitchRad = 0.f;
    kPitchRad = std::clamp(kPitchRad - deltaY, glm::radians(-89.f), glm::radians(89.f));

    static float kYawRad = glm::pi<float>();
    kYawRad -= deltaX;

    const glm::quat yawQuat = glm::angleAxis(kYawRad, m_pCamera->worldUp());

    // Compute the new forward vector by rotating the previous by our change in yaw.
    const glm::vec3 newForward = yawQuat * forward;

    // Update the right vector based on the new forward direction.
    const glm::vec3 right = glm::cross(glm::normalize(newForward), m_pCamera->worldUp());
    const glm::quat pitchQuat = glm::angleAxis(kPitchRad, glm::normalize(right));

    const float dist = glm::distance({}, m_pCamera->position());
    m_pCamera->position(pitchQuat * newForward * dist);
}
