#include "App.hpp"

#include <chrono>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>

namespace {
constexpr const char* kClearColorKey = "clearColor";
constexpr const char* kRedComponentKey = "r";
constexpr const char* kGreenComponentKey = "g";
constexpr const char* kBlueComponentKey = "b";
constexpr const char* kAlphaComponentKey = "a";

constexpr const char* kWindowTitle = "windowTitle";
constexpr const char* kWindowDimensionsKey = "windowDimensions";
constexpr const char* kWidthComponentKey = "width";
constexpr const char* kHeightComponentKey = "height";
} // end unnamed namespace

Application::Application(const std::filesystem::path& configPath)
    : m_config(readConfig(configPath)) {

    const float width = m_config.windowDimensions.x;
    const float height = m_config.windowDimensions.y;

    m_orthoCamera.leftExtent(width / -2);
    m_orthoCamera.rightExtent(width / 2);
    m_orthoCamera.bottomExtent(height / -2);
    m_orthoCamera.topExtent(height / 2);
    m_orthoCamera.far(1000.f);
    m_orthoCamera.near(-1000.f);

    m_perspCamera.aspectRatio(width / height);
    m_perspCamera.fov(glm::radians(45.f));
    m_perspCamera.far(100.f);
    m_perspCamera.near(0.1f);
    
    m_orthoCamera.position({ 0.f, 0.f, 5.f });
    m_perspCamera.position({ 0.f, 0.f, 5.f });

    if (m_ortho)
        m_callbacks.camera(&m_orthoCamera);
    else
        m_callbacks.camera(&m_perspCamera);
}

bool Application::setUp() {

    glfwInit();

    if (!m_pWindow) {
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    m_pWindow = createWindow(m_config.windowDimensions, m_config.windowTitle);
    if (!m_pWindow) {
        std::cerr << "Failed to create a window.\n";
        return false;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to load GLAD functions.\n";
        return false;
    }
    
    m_renderer.setUp();
    m_renderer.canvasDimensions(m_config.windowDimensions);

    if (m_ortho)
        m_renderer.camera(&m_orthoCamera);
    else
        m_renderer.camera(&m_perspCamera);

    glViewport(0, 0, m_config.windowDimensions.x, m_config.windowDimensions.y);

    // Setup window callbacks for modular window controls.
    glfwSetWindowUserPointer(m_pWindow, &m_callbacks);
    glfwSetFramebufferSizeCallback(m_pWindow, WindowCallbacks::FrameBufferSizeCallback);
    glfwSetCursorPosCallback(m_pWindow, WindowCallbacks::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_pWindow, WindowCallbacks::MouseButtonCallback);
    glfwSetKeyCallback(m_pWindow, WindowCallbacks::KeyboardCallback);
    glfwSetScrollCallback(m_pWindow, WindowCallbacks::ScrollCallback);

    return true;
}

GLFWwindow* Application::createWindow(const glm::ivec2& dimensions, const std::string& title) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Enable MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* pWindow = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(pWindow);

    return pWindow;
}

void Application::run() {

    while (!glfwWindowShouldClose(m_pWindow)) {

        glfwPollEvents();
        update();
        render();
    }
}

void Application::render() {

    const glm::vec4 clearColor = m_config.clearColor;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    static const glm::vec3 xAxis{ 1.f, 0.f, 0.f };
    static const glm::vec3 yAxis{ 0.f, 1.f, 0.f };
    static const glm::vec3 zAxis{ 0.f, 0.f, 1.f };

    if (m_ortho) {
        const glm::vec3 end = { 100.f, 100.f, 0.f};

        m_renderer.drawLine({}, end, kRed);
        m_renderer.drawPoint(end, kBlue);

        m_renderer.drawLine({}, xAxis * 200.f, kRed);
        m_renderer.drawLine({}, yAxis * 200.f, kGreen);
        m_renderer.drawLine({}, zAxis * 200.f, kBlue);
    }
    else {
        const glm::vec3 end = { 1.f, 1.f, 0.f };

        m_renderer.drawLine({}, end, kRed);
        m_renderer.drawPoint(end, kBlue);

        m_renderer.drawLine({}, xAxis * 1.f, kRed);
        m_renderer.drawLine({}, yAxis * 1.f, kGreen);
        m_renderer.drawLine({}, zAxis * 1.f, kBlue);
    }

    glfwSwapBuffers(m_pWindow);
}

void Application::update() {

    m_orthoCamera.update();
    m_perspCamera.update();
}

Config Application::readConfig(const std::filesystem::path& configPath) {

    std::ifstream file{ configPath };
    Config config;

    if (!file.is_open()) {
        assert(false);
        return config;
    }
    
    nlohmann::json json;
    file >> json;

    if (json.contains(kClearColorKey) && json.at(kClearColorKey).is_object()) {

        const nlohmann::json color = json.at(kClearColorKey);
        color.at(kRedComponentKey).get_to(config.clearColor.r);
        color.at(kGreenComponentKey).get_to(config.clearColor.g);
        color.at(kBlueComponentKey).get_to(config.clearColor.b);
        color.at(kAlphaComponentKey).get_to(config.clearColor.a);
    }

    if (json.contains(kWindowTitle) && json.at(kWindowTitle).is_string())
        json.at(kWindowTitle).get_to(config.windowTitle);

    if (json.contains(kWindowDimensionsKey) && json.at(kWindowDimensionsKey).is_object()) {

        const nlohmann::json dims = json.at(kWindowDimensionsKey);
        dims.at(kWidthComponentKey).get_to(config.windowDimensions.x);
        dims.at(kHeightComponentKey).get_to(config.windowDimensions.y);
    }

    return config;
}

Application::Destructor::~Destructor() {
    glfwTerminate();
}
