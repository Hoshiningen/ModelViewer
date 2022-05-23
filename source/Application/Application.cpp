#include "Application.hpp"

#include "Camera/OrthographicCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include "Controls/OrbitalControls.hpp"

#include "Geometry/Line.hpp"
#include "Geometry/Plane.hpp"
#include "Geometry/Point.hpp"

#include "Renderer/Renderer.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Application::Private {
    void render();
    void update();

    GLFWwindow* createWindow(const glm::ivec2& dimensions, const std::string& title);

    std::vector<Line> debugFrustum(const OrthographicCamera* pCamera) const;
    std::vector<Line> debugFrustum(const PerspectiveCamera* pCamera) const;

    void drawOrthographicFrustrum(OrthographicCamera* pCamera, Renderer* pRenderer) const;
    void drawPerspectiveFrustrum(PerspectiveCamera* pCamera, Renderer* pRenderer) const;

    Renderer m_renderer;
    std::unique_ptr<Camera> m_pCamera;

    // Debug cameras
    PerspectiveCamera m_persp;
    OrthographicCamera m_ortho;

    GLFWwindow* m_pWindow = nullptr;
    OrbitalControls m_callbacks; // window controls. Sets up an orbital camera.

    // Ensures API shutdown happens after everything has been destroyed.
    static struct Destructor {
        ~Destructor();
    } kDestructor;
};

void Application::Private::render() {

    const glm::vec4 clearColor = kBlack;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const glm::vec3 xAxis{ 1.f, 0.f, 0.f };
    static const glm::vec3 yAxis{ 0.f, 1.f, 0.f };
    static const glm::vec3 zAxis{ 0.f, 0.f, 1.f };

    const glm::vec3 end = { 1.f, 1.f, 0.f };

    m_renderer.draw(Line{ {}, end }, kRed);
    m_renderer.draw(Point{ end }, kBlue);

    m_renderer.draw(Line({}, xAxis), kRed);
    m_renderer.draw(Line({}, yAxis), kGreen);
    m_renderer.draw(Line({}, zAxis), kBlue);

    Plane plane{ {0, 0, 0}, {1, 0, 0}, {1, 1, 0} };
    m_renderer.draw(plane, kRed);

    glfwSwapBuffers(m_pWindow);
}

void Application::Private::update() {

    if (m_pCamera) {
        m_pCamera->update();
    }
}

GLFWwindow* Application::Private::createWindow(const glm::ivec2& dimensions, const std::string& title) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Enable MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* pWindow = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(pWindow);

    return pWindow;
}

std::vector<Line> Application::Private::debugFrustum(const OrthographicCamera* pCamera) const {

    const float farZ = (pCamera->position() + pCamera->forward() * pCamera->far()).z * 0.1f;
    const float nearZ = (pCamera->position() + pCamera->forward() * pCamera->near()).z;

    const glm::vec3 farTopLeft{ pCamera->leftExtent(), pCamera->topExtent(), farZ };
    const glm::vec3 farTopRight{ pCamera->rightExtent(), pCamera->topExtent(), farZ };
    const glm::vec3 farBottomLeft{ pCamera->leftExtent(), pCamera->bottomExtent(), farZ };
    const glm::vec3 farBottomRight{ pCamera->rightExtent(), pCamera->bottomExtent(), farZ };

    const glm::vec3 nearTopLeft{ pCamera->leftExtent(), pCamera->topExtent(), nearZ };
    const glm::vec3 nearTopRight{ pCamera->rightExtent(), pCamera->topExtent(), nearZ };
    const glm::vec3 nearBottomLeft{ pCamera->leftExtent(), pCamera->bottomExtent(), nearZ };
    const glm::vec3 nearBottomRight{ pCamera->rightExtent(), pCamera->bottomExtent(), nearZ };

    std::vector<Line> lines;

    // Frustum lines
    lines.emplace_back(nearBottomLeft, farBottomLeft);
    lines.emplace_back(nearBottomRight, farBottomRight);
    lines.emplace_back(nearTopRight, farTopRight);
    lines.emplace_back(nearTopLeft, farTopLeft);

    // Far endcap
    lines.emplace_back(farTopLeft, farTopRight);
    lines.emplace_back(farTopRight, farBottomRight);
    lines.emplace_back(farBottomRight, farBottomLeft);
    lines.emplace_back(farBottomLeft, farTopLeft);

    // Near endcap
    lines.emplace_back(nearTopLeft, nearTopRight);
    lines.emplace_back(nearTopRight, nearBottomRight);
    lines.emplace_back(nearBottomRight, nearBottomLeft);
    lines.emplace_back(nearBottomLeft, nearTopLeft);

    return lines;
};

std::vector<Line> Application::Private::debugFrustum(const PerspectiveCamera* pCamera) const {

    const glm::vec3 eyePoint = pCamera->position();

    const auto ComputeOffsets2D = [&eyePoint, pCamera](float distFromCamera, float scalar = 1.f) {

        const glm::vec3 point = (eyePoint + pCamera->forward() * distFromCamera) * scalar;
        const float len = glm::length(point - eyePoint);

        const float yOffset = len * glm::tan(pCamera->fovY() / 2);
        const float xOffset = yOffset * pCamera->aspectRatio();

        return std::make_tuple(xOffset, yOffset, point.z);
    };

    const auto MakeBox = [&ComputeOffsets2D](float distFromCamera, float scalar = 1.f) {

        struct Box {
            glm::vec3 bottomLeft;
            glm::vec3 bottomRight;
            glm::vec3 topRight;
            glm::vec3 topLeft;
        } box;

        const auto [xOffset, yOffset, z] = ComputeOffsets2D(distFromCamera, scalar);

        box.bottomLeft = { -xOffset, -yOffset, z };
        box.bottomRight = { xOffset, -yOffset, z };
        box.topRight = { xOffset, yOffset, z };
        box.topLeft = { -xOffset, yOffset, z };

        return box;
    };

    const auto farBox = MakeBox(pCamera->far(), 0.1f);
    const auto originBox = MakeBox(glm::length(pCamera->target() - eyePoint));
    const auto nearBox = MakeBox(pCamera->near());

    std::vector<Line> lines;

    // Frustum lines
    lines.emplace_back(eyePoint, farBox.bottomLeft);
    lines.emplace_back(eyePoint, farBox.bottomRight);
    lines.emplace_back(eyePoint, farBox.topRight);
    lines.emplace_back(eyePoint, farBox.topLeft);

    // Far endcap
    lines.emplace_back(farBox.topLeft, farBox.topRight);
    lines.emplace_back(farBox.topRight, farBox.bottomRight);
    lines.emplace_back(farBox.bottomRight, farBox.bottomLeft);
    lines.emplace_back(farBox.bottomLeft, farBox.topLeft);

    // Near endcap
    lines.emplace_back(nearBox.topLeft, nearBox.topRight);
    lines.emplace_back(nearBox.topRight, nearBox.bottomRight);
    lines.emplace_back(nearBox.bottomRight, nearBox.bottomLeft);
    lines.emplace_back(nearBox.bottomLeft, nearBox.topLeft);

    return lines;
};

void Application::Private::drawPerspectiveFrustrum(PerspectiveCamera* pCamera, Renderer* pRenderer) const {

    if (!pCamera || !pRenderer)
        return;

    for (const Line& line : debugFrustum(pCamera))
        pRenderer->draw(line, kMagenta);
}

void Application::Private::drawOrthographicFrustrum(OrthographicCamera* pCamera, Renderer* pRenderer) const {

    if (!pCamera || !pRenderer)
        return;

    for (const Line& line : debugFrustum(pCamera))
        pRenderer->draw(line, kCyan);
}

Application::Private::Destructor::~Destructor() {
    glfwTerminate();
}

Application::Application()
    : m_pPrivate(std::make_unique<Private>()) {

    const float width = 800.f;
    const float height = 600.f;

    m_pPrivate->m_pCamera = [&width, &height] {
        std::unique_ptr<PerspectiveCamera> pCamera = std::make_unique<PerspectiveCamera>();
        pCamera->aspectRatio(width / height);
        pCamera->fovY(glm::radians(45.f));
        pCamera->far(150.f);
        pCamera->near(0.01f);
        pCamera->position({ 0.f, 0.f, 35.f });

        return std::move(pCamera);
    }();

    m_pPrivate->m_callbacks.connectProjectionChanged(&Application::onProjectionChange, this);
    m_pPrivate->m_callbacks.camera(m_pPrivate->m_pCamera.get());

    m_pPrivate->m_persp = *static_cast<PerspectiveCamera*>(m_pPrivate->m_pCamera.get());
    m_pPrivate->m_ortho = m_pPrivate->m_persp;
}

Application::~Application() noexcept {}

bool Application::setUp() {

    glfwInit();

    if (!m_pPrivate->m_pWindow) {
        glfwDestroyWindow(m_pPrivate->m_pWindow);
        m_pPrivate->m_pWindow = nullptr;
    }

    m_pPrivate->m_pWindow = m_pPrivate->createWindow({ 800, 600 }, "Model Viewer");
    if (!m_pPrivate->m_pWindow) {
        std::cerr << "Failed to create a window.\n";
        return false;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to load GLAD functions.\n";
        return false;
    }
    
    m_pPrivate->m_renderer.setup();
    m_pPrivate->m_renderer.camera(m_pPrivate->m_pCamera.get());

    glViewport(0, 0, 800, 600);

    // Setup window callbacks for modular window controls.
    glfwSetWindowUserPointer(m_pPrivate->m_pWindow, &m_pPrivate->m_callbacks);
    glfwSetFramebufferSizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::FrameBufferSizeCallback);
    glfwSetCursorPosCallback(m_pPrivate->m_pWindow, WindowCallbacks::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_pPrivate->m_pWindow, WindowCallbacks::MouseButtonCallback);
    glfwSetKeyCallback(m_pPrivate->m_pWindow, WindowCallbacks::KeyboardCallback);
    glfwSetScrollCallback(m_pPrivate->m_pWindow, WindowCallbacks::ScrollCallback);

    return true;
}

void Application::run() {

    while (!glfwWindowShouldClose(m_pPrivate->m_pWindow)) {

        glfwPollEvents();
        m_pPrivate->update();
        m_pPrivate->render();
    }
}

void Application::onProjectionChange(WindowCallbacks::ProjectionChange projection) {

    if (projection == WindowCallbacks::ProjectionChange::Orthographic) {

        const PerspectiveCamera* pCamera = dynamic_cast<const PerspectiveCamera*>(m_pPrivate->m_pCamera.get());
        if (pCamera) {
            m_pPrivate->m_pCamera = std::make_unique<OrthographicCamera>(*pCamera);
            m_pPrivate->m_callbacks.camera(m_pPrivate->m_pCamera.get());
            m_pPrivate->m_renderer.camera(m_pPrivate->m_pCamera.get());
        }
    }

    if (projection == WindowCallbacks::ProjectionChange::Perspective) {

        const OrthographicCamera* pCamera = dynamic_cast<const OrthographicCamera*>(m_pPrivate->m_pCamera.get());
        if (pCamera) {
            m_pPrivate->m_pCamera = std::make_unique<PerspectiveCamera>(*pCamera);
            m_pPrivate->m_callbacks.camera(m_pPrivate->m_pCamera.get());
            m_pPrivate->m_renderer.camera(m_pPrivate->m_pCamera.get());
        }
    }
}
