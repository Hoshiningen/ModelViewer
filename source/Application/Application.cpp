#include "Application.hpp"

#include "Camera/OrthographicCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include "Common/IRestorable.hpp"
#include "Common/Math.hpp"
#include "Common/SignalMacros.hpp"

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

#include "Object/Mesh.hpp"

#include "Renderer/Renderer.hpp"

#include "UI/LightPropertiesDialog.hpp"
#include "UI/MaterialPropertiesDialog.hpp"
#include "UI/ModelLoaderDialog.hpp"
#include "UI/ScenePropertiesDialog.hpp"

#include <fstream>
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
static constexpr const char* kSettingsFileName = "settings.json";
static constexpr ImGuiWindowFlags kWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
} // end unnamed namespace

struct Application::Private : private IRestorable {
    Private();
    ~Private();
    
    // Ensures API shutdown happens after everything has been destroyed.
    static struct APICleanUp {
        ~APICleanUp();
    } kAPICleanUp;

    void render();
    void update();

    GLFWwindow* createWindow(const glm::ivec2& dimensions, const std::string& title);

    // Inherited via IRestorable
    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    void fitSceneToModel(std::forward_list<VertexBuffered>* pModel);

    // Signals
    DEFINE_CONNECTION(m_signalInitialized, ApplicationInitialized)

    // Handlers
    void onClearColorChange(const glm::vec3& clearColor);
    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframe) const;
    void onInitialized();
    void onSaved();

public:
    sigslot::signal<> m_signalInitialized;

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

    Mesh m_mesh;

    glm::vec3* m_pClearColor = nullptr;

    glm::vec2 m_windowSize{ 800, 600 };
    glm::vec2 m_windowPosition{ 800, 600 };

    bool m_windowMaximized = false;
};

Application::Private::Private()
    : m_loaderDialog("Model Loader", kWindowFlags),
      m_materialPropDialog("Material Properties", kWindowFlags),
      m_lightPropDialog("Directional Light Properties", kWindowFlags),
      m_scenePropDialog("Scene Properties", kWindowFlags) {

    // Camera setup
    m_pCamera = [this] {
        std::unique_ptr<PerspectiveCamera> pCamera = std::make_unique<PerspectiveCamera>();
        pCamera->aspectRatio(m_windowSize.x / m_windowSize.y);
        pCamera->fovY(glm::radians(45.f));
        pCamera->far(150.f);
        pCamera->near(0.01f);
        pCamera->position({ 0.f, 0.f, 5.f });

        return std::move(pCamera);
    }();

    m_callbacks.camera(m_pCamera.get());

    m_persp = *static_cast<PerspectiveCamera*>(m_pCamera.get());
    m_ortho = m_persp;

    // Signals
    connectApplicationInitialized(&Application::Private::onInitialized, this);
    
    m_callbacks.connectWindowMaximized([this](bool maximized) {
        m_windowMaximized = maximized;
    });

    m_callbacks.connectWindowSizeChanged([this](const glm::ivec2& size) {
        m_windowSize = size;
    });

    m_callbacks.connectWindowPositionChanged([this](const glm::ivec2& position) {
        m_windowPosition = position;
    });
}

Application::Private::~Private() {
    onSaved();
}

Application::Private::APICleanUp::~APICleanUp() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Private::render() {

    if (m_pClearColor)
        glClearColor(m_pClearColor->r, m_pClearColor->g, m_pClearColor->b, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_loaderDialog.render();
    m_materialPropDialog.render();
    m_lightPropDialog.render();
    m_scenePropDialog.render();

    m_renderer.draw(m_mesh);

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

std::string_view Application::Private::id() const {
    return "Application";
}

nlohmann::json Application::Private::save() const {

    nlohmann::json json;
    nlohmann::json& obj = json[id().data()];

    obj["windowSize"] = m_windowSize;
    obj["windowPosition"] = m_windowPosition;
    obj["windowMaximized"] = m_windowMaximized;

    if (const auto json = m_lightPropDialog.save(); json.is_object())
        obj.update(json);

    if (const auto json = m_loaderDialog.save(); json.is_object())
        obj.update(json);

    if (const auto json = m_materialPropDialog.save(); json.is_object())
        obj.update(json);

    if (const auto json = m_scenePropDialog.save(); json.is_object())
        obj.update(json);

    if (const auto json = m_mesh.save(); json.is_object())
        obj.update(json);

    return json;
}

void Application::Private::restore(const nlohmann::json& settings) {

    if (settings.is_null() || !settings.is_object())
        return;

    if (settings.contains("windowSize"))
        settings["windowSize"].get_to(m_windowSize);

    if (settings.contains("windowPosition"))
        settings["windowPosition"].get_to(m_windowPosition);

    if (settings.contains("windowMaximized"))
        settings["windowMaximized"].get_to(m_windowMaximized);

    if (settings.contains(m_lightPropDialog.id()))
        m_lightPropDialog.restore(settings.at(m_lightPropDialog.id().data()));

    if (settings.contains(m_loaderDialog.id()))
        m_loaderDialog.restore(settings.at(m_loaderDialog.id().data()));

    if (settings.contains(m_materialPropDialog.id()))
        m_materialPropDialog.restore(settings.at(m_materialPropDialog.id().data()));

    if (settings.contains(m_scenePropDialog.id()))
        m_scenePropDialog.restore(settings.at(m_scenePropDialog.id().data()));

    if (settings.contains(m_mesh.id()))
        m_mesh.restore(settings.at(m_mesh.id().data()));

    if (m_windowMaximized)
        glfwMaximizeWindow(m_pWindow);

    glfwSetWindowSize(m_pWindow, m_windowSize.x, m_windowSize.y);
    glfwSetWindowPos(m_pWindow, m_windowPosition.x, m_windowPosition.y);
    glViewport(0, 0, m_windowSize.x, m_windowSize.y);
}

void Application::Private::onProjectionChange(int projection) {

    if (projection == ScenePropertiesDialog::Projection::eOrthographic) {

        const PerspectiveCamera* pCamera = dynamic_cast<const PerspectiveCamera*>(m_pCamera.get());
        if (pCamera) {
            m_pCamera = std::make_unique<OrthographicCamera>(*pCamera);
            m_callbacks.camera(m_pCamera.get());
            m_renderer.camera(m_pCamera.get());
        }
    }

    if (projection == ScenePropertiesDialog::Projection::ePerspective) {

        const OrthographicCamera* pCamera = dynamic_cast<const OrthographicCamera*>(m_pCamera.get());
        if (pCamera) {
            m_pCamera = std::make_unique<PerspectiveCamera>(*pCamera);
            m_callbacks.camera(m_pCamera.get());
            m_renderer.camera(m_pCamera.get());
        }
    }
}

void Application::Private::onWireframeModeChange(bool wireframe) const {

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::Private::fitSceneToModel(std::forward_list<VertexBuffered>* pModel) {

    if (!pModel)
        return;

    m_mesh.position() = -ComputeCenter(pModel);
    m_mesh.scale() = ComputeScale(pModel, kMaxModelSize);
}

void Application::Private::onInitialized() {

    // Setup data models for the scene
    m_materialPropDialog.mesh(&m_mesh);
    m_loaderDialog.mesh(&m_mesh);
    m_scenePropDialog.mesh(&m_mesh);

    m_pClearColor = m_scenePropDialog.clearColor();
    m_renderer.ambientColor(m_scenePropDialog.ambientColor(), m_scenePropDialog.ambientIntensity());

    for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex)
        m_renderer.directionalLight(m_lightPropDialog.directionalLight(lightIndex), lightIndex);

    // Model loader signals
    m_loaderDialog.connectModelLoaded([this]() {
        fitSceneToModel(m_mesh.model());
        m_renderer.initializeMesh(m_mesh);
    });

    // Material property signals
    m_materialPropDialog.connectMaterialSelectionChanged([this]() {
        m_renderer.initializeMesh(m_mesh);
    });

    m_materialPropDialog.connectTextureLoaded(&Renderer::onTextureLoaded, &m_renderer);

    // Window controls signals
    m_callbacks.connectProjectionChanged(&Application::Private::onProjectionChange, this);
    m_callbacks.connectProjectionChanged(&ScenePropertiesDialog::onProjectionChange, &m_scenePropDialog);
    
    m_callbacks.connectWireframeChanged(&Application::Private::onWireframeModeChange, this);
    m_callbacks.connectWireframeChanged(&ScenePropertiesDialog::onWireframeModeChange, &m_scenePropDialog);
    
    m_callbacks.connectApplicationSaved(&Application::Private::onSaved, this);

    // Restore application settings
    if (std::filesystem::is_regular_file(kSettingsFileName) && std::filesystem::file_size(kSettingsFileName) > 0) {

        std::ifstream file{ kSettingsFileName };
        nlohmann::json settings;
        file >> settings;

        try {
            if (settings.is_object() && settings.contains(id()))
                restore(settings.at(id().data()));
        }
        catch (nlohmann::detail::type_error& error) {
            std::cerr << error.what();
        }
        catch (nlohmann::detail::out_of_range& error) {
            std::cerr << error.what();
        }
        catch (nlohmann::detail::other_error& error) {
            std::cerr << "An unknown nlohmann error occurred.\n";
        }
    }
}

void Application::Private::onSaved() {

    std::ofstream file{ kSettingsFileName };
    file << save();
}


Application::Application()
    : m_pPrivate(std::make_unique<Private>()) {}

Application::~Application() noexcept {}

bool Application::setUp() {

    glfwInit();

    if (!m_pPrivate->m_pWindow) {
        glfwDestroyWindow(m_pPrivate->m_pWindow);
        m_pPrivate->m_pWindow = nullptr;
    }

    m_pPrivate->m_pWindow = m_pPrivate->createWindow({ m_pPrivate->m_windowSize.x, m_pPrivate->m_windowSize.y}, "Model Viewer");
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

    glViewport(0, 0, m_pPrivate->m_windowSize.x, m_pPrivate->m_windowSize.y);

    // Setup window callbacks for modular window controls.
    glfwSetWindowUserPointer(m_pPrivate->m_pWindow, &m_pPrivate->m_callbacks);
    glfwSetFramebufferSizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::FrameBufferSizeCallback);
    
    glfwSetCursorPosCallback(m_pPrivate->m_pWindow, WindowCallbacks::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_pPrivate->m_pWindow, WindowCallbacks::MouseButtonCallback);
    glfwSetKeyCallback(m_pPrivate->m_pWindow, WindowCallbacks::KeyboardCallback);
    glfwSetScrollCallback(m_pPrivate->m_pWindow, WindowCallbacks::ScrollCallback);
    
    glfwSetWindowMaximizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowMaximizedCallback);
    glfwSetWindowSizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowSizeCallback);
    glfwSetWindowPosCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowPositionCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(m_pPrivate->m_pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_pPrivate->m_signalInitialized();

    return true;
}

void Application::run() {

    while (!glfwWindowShouldClose(m_pPrivate->m_pWindow)) {

        glfwPollEvents();
        m_pPrivate->update();
        m_pPrivate->render();
    }
}
