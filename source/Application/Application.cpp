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
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Material/SolidMaterial.hpp"

#include "Renderer/Renderer.hpp"

#include "UI/LightPropertiesDialog.hpp"
#include "UI/MaterialPropertiesDialog.hpp"
#include "UI/ModelLoaderDialog.hpp"
#include "UI/ScenePropertiesDialog.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace {
static constexpr ImGuiWindowFlags kWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
} // end unnamed namespace

struct Application::Private {
    Private();

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

    // UI Code
    ModelLoaderDialog m_loaderDialog;
    MaterialPropertiesDialog m_materialPropDialog;
    LightPropertiesDialog m_lightPropDialog;
    ScenePropertiesDialog m_scenePropDialog;

    IMaterial* m_pActiveMaterial = nullptr;
    std::forward_list<VertexBuffered>* m_pActiveModel = nullptr;

    glm::vec4 m_clearColor{ 0.f, 0.f, 0.f, 1.f };

    // Ensures API shutdown happens after everything has been destroyed.
    static struct Destructor {
        ~Destructor();
    } kDestructor;
};

Application::Private::Private()
    : m_loaderDialog("Model Loader", kWindowFlags),
      m_materialPropDialog("Material Properties", kWindowFlags),
      m_lightPropDialog("Light Properties", kWindowFlags),
      m_scenePropDialog("Scene Properties", kWindowFlags) {

    m_loaderDialog.connectModelLoaded([this](std::forward_list<VertexBuffered>* pModel) {
        m_pActiveModel = pModel;

        m_renderer.onModelLoaded(m_pActiveMaterial, m_pActiveModel);
        m_materialPropDialog.onModelLoaded(m_pActiveModel);
        m_lightPropDialog.onModelLoaded(m_pActiveModel);
    });

    m_materialPropDialog.connectMaterialSelected([this](IMaterial* pMaterial) {
        m_pActiveMaterial = pMaterial;
        m_renderer.onModelLoaded(m_pActiveMaterial, m_pActiveModel);
    });

    m_materialPropDialog.connectTextureLoaded(&Renderer::onTextureLoaded, &m_renderer);
    m_lightPropDialog.connectLightChanged(&Renderer::onLightChanged, &m_renderer);
}

void Application::Private::render() {
    
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_loaderDialog.render();
    m_materialPropDialog.render();
    m_lightPropDialog.render();
    m_scenePropDialog.render();

    if (m_pActiveModel && m_pActiveMaterial)
        m_renderer.draw(*m_pActiveModel, *m_pActiveMaterial);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_pWindow);
}

void Application::Private::update() {

    const bool disableNavigation =
        m_loaderDialog.active() ||
        m_materialPropDialog.active() ||
        m_lightPropDialog.active() ||
        m_scenePropDialog.active();

    m_callbacks.navigationEnabled(!disableNavigation);

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

    static SolidMaterial lineMaterial = [] {
        SolidMaterial mat;
        mat.color({1.f, 0.f, 1.f, 1.f});

        return mat;
    }();

    for (const Line& line : debugFrustum(pCamera))
        pRenderer->draw(line, lineMaterial);
}

void Application::Private::drawOrthographicFrustrum(OrthographicCamera* pCamera, Renderer* pRenderer) const {

    if (!pCamera || !pRenderer)
        return;

    static SolidMaterial lineMaterial = [] {
        SolidMaterial mat;
        mat.color({ 0.f, 1.f, 1.f, 1.f });

        return mat;
    }();

   for (const Line& line : debugFrustum(pCamera))
       pRenderer->draw(line, lineMaterial);
}

Application::Private::Destructor::~Destructor() {
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

    // Signals
    m_pPrivate->m_scenePropDialog.connectClearColorChanged(&Application::onClearColorChange, this);

    m_pPrivate->m_callbacks.connectProjectionChanged(&Application::onProjectionChange, this);
    m_pPrivate->m_callbacks.connectProjectionChanged(&ScenePropertiesDialog::onProjectionChange, &m_pPrivate->m_scenePropDialog);
    
    m_pPrivate->m_callbacks.connectWireframeModeChanged(&Application::onWireframeModeChange, this);
    m_pPrivate->m_callbacks.connectWireframeModeChanged(&ScenePropertiesDialog::onWireframeModeChange, &m_pPrivate->m_scenePropDialog);

    // Camera setup
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(m_pPrivate->m_pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void Application::run() {

    while (!glfwWindowShouldClose(m_pPrivate->m_pWindow)) {

        glfwPollEvents();
        m_pPrivate->update();
        m_pPrivate->render();
    }
}

void Application::onProjectionChange(int projection) {

    if (projection == ScenePropertiesDialog::Projection::eOrthographic) {

        const PerspectiveCamera* pCamera = dynamic_cast<const PerspectiveCamera*>(m_pPrivate->m_pCamera.get());
        if (pCamera) {
            m_pPrivate->m_pCamera = std::make_unique<OrthographicCamera>(*pCamera);
            m_pPrivate->m_callbacks.camera(m_pPrivate->m_pCamera.get());
            m_pPrivate->m_renderer.camera(m_pPrivate->m_pCamera.get());
        }
    }

    if (projection == ScenePropertiesDialog::Projection::ePerspective) {

        const OrthographicCamera* pCamera = dynamic_cast<const OrthographicCamera*>(m_pPrivate->m_pCamera.get());
        if (pCamera) {
            m_pPrivate->m_pCamera = std::make_unique<PerspectiveCamera>(*pCamera);
            m_pPrivate->m_callbacks.camera(m_pPrivate->m_pCamera.get());
            m_pPrivate->m_renderer.camera(m_pPrivate->m_pCamera.get());
        }
    }
}

void Application::onWireframeModeChange(bool wireframe) const {

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::onClearColorChange(const glm::vec3& clearColor) {

    m_pPrivate->m_clearColor.r = clearColor.r;
    m_pPrivate->m_clearColor.g = clearColor.g;
    m_pPrivate->m_clearColor.b = clearColor.b;
}
