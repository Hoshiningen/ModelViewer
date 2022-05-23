#include "Controls/WindowCallbacks.hpp"

#include <GLFW/glfw3.h>

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
