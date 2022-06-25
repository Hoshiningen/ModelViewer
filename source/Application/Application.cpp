#include "Application.hpp"

#include "Camera/OrthographicCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include "Controls/OrbitalControls.hpp"

#include "Geometry/Box.hpp"
#include "Geometry/Line.hpp"
#include "Geometry/Plane.hpp"
#include "Geometry/Point.hpp"

#include "IO/GeometryLoader.hpp"
#include "IO/TextureLoader.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/MeshMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/SolidMaterial.hpp"
#include "Material/SolidPointLineMaterial.hpp"

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
    void loadData();
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

    GeometryLoader m_geometryLoader;

    std::forward_list<VertexBuffered> m_swordMesh;
    std::forward_list<VertexBuffered> m_squirrelMesh;
    std::forward_list<VertexBuffered> m_spiderMesh;
    MeshMaterial m_furMaterial;

    SolidPointLineMaterial m_xAxisMaterial;
    SolidPointLineMaterial m_yAxisMaterial;
    SolidPointLineMaterial m_zAxisMaterial;
    
    PhongMaterial m_goldMaterial;

    // Ensures API shutdown happens after everything has been destroyed.
    static struct Destructor {
        ~Destructor();
    } kDestructor;
};

void Application::Private::loadData() {

    //const std::filesystem::path swordMesh = "D:\\Meshes\\Sword_StaticMesh\\sword.obj";
    //m_swordMesh = m_geometryLoader.load(swordMesh);

    const std::filesystem::path squirrelMesh = "D:\\Meshes\\Squirrel_SkeleMesh\\ShadeTail.obj";
    m_squirrelMesh = m_geometryLoader.load(squirrelMesh);

    const std::filesystem::path spiderMesh = "D:\\Meshes\\BlackWidow_SkeleMesh\\blackwidow.obj";
    m_spiderMesh = m_geometryLoader.load(spiderMesh);

    m_furMaterial.diffuseMap([] {
        Texture texture = TextureLoader::load("D:\\Meshes\\Squirrel_SkeleMesh\\Material Base Color.png", Texture::Target::Texture2D);
        texture.mipmap(true);
        texture.minFilter(Texture::Filter::LinearMipmapLinear);
        texture.magFilter(Texture::Filter::Linear);

        return texture;
    }());

    //for (auto& buffer : m_spiderMesh)
    //    buffer.color(kWhite);

    m_xAxisMaterial.color(kRed);
    m_yAxisMaterial.color(kGreen);
    m_zAxisMaterial.color(kBlue);

    m_goldMaterial.ambientColor({ 0.24725f, 0.1995f, 0.0745f });
    m_goldMaterial.diffuseColor({ 0.75164f, 0.60648f, 0.22648f });
    m_goldMaterial.ambientColor({ 0.628281f, 0.555802f, 0.366065f });
    m_goldMaterial.shininess(0.4f * 128.f);
}

void Application::Private::render() {
    
    const glm::vec4 clearColor = kBlack;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const glm::vec3 xAxis{ 1.f, 0.f, 0.f };
    static const glm::vec3 yAxis{ 0.f, 1.f, 0.f };
    static const glm::vec3 zAxis{ 0.f, 0.f, 1.f };

    //m_renderer.draw(Line({}, xAxis), m_xAxisMaterial);
    //m_renderer.draw(Line({}, yAxis), m_yAxisMaterial);
    //m_renderer.draw(Line({}, zAxis), m_zAxisMaterial);

    for (VertexBuffered& geometry : m_spiderMesh) {

        if (!geometry.initialized()) {
            if (!m_renderer.initialize(geometry, m_furMaterial))
                return;
        }
    
        m_renderer.draw(geometry, m_furMaterial);
    }

    glfwSwapBuffers(m_pWindow);
}

void Application::Private::update() {

    if (m_pCamera)
        m_pCamera->update();
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

    static SolidPointLineMaterial lineMaterial = [] {
        SolidPointLineMaterial mat;
        mat.color(kMagenta);

        return mat;
    }();

    for (const Line& line : debugFrustum(pCamera))
        pRenderer->draw(line, lineMaterial);
}

void Application::Private::drawOrthographicFrustrum(OrthographicCamera* pCamera, Renderer* pRenderer) const {

    if (!pCamera || !pRenderer)
        return;

    static SolidPointLineMaterial lineMaterial = [] {
        SolidPointLineMaterial mat;
        mat.color(kCyan);

        return mat;
    }();

   for (const Line& line : debugFrustum(pCamera))
       pRenderer->draw(line, lineMaterial);
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
        pCamera->position({ 0.f, 0.f, 5.f });

        return std::move(pCamera);
    }();

    // Event handlers.
    m_pPrivate->m_callbacks.connectProjectionChanged(&Application::onProjectionChange, this);
    m_pPrivate->m_callbacks.connectWireframeModeChanged(&Application::onWireframeModeChange, this);

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
    
    m_pPrivate->loadData();
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

void Application::onWireframeModeChange(bool wireframe) {

    m_pPrivate->m_goldMaterial.wireframe(wireframe);
    m_pPrivate->m_furMaterial.wireframe(wireframe);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
