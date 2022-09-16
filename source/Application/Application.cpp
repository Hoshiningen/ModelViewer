#include "Application.hpp"

#include "Camera/OrthographicCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include "Common/IRestorable.hpp"
#include "Common/Math.hpp"

#include "Controls/OrbitalControls.hpp"

#include "Geometry/Box.hpp"
#include "Geometry/Line.hpp"
#include "Geometry/Plane.hpp"
#include "Geometry/Point.hpp"

#include "IO/GeometryLoader.hpp"
#include "IO/TextureLoader.hpp"

#include "Light/DirectionalLight.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Material/SolidMaterial.hpp"

#include "Object/Mesh.hpp"

#include "Renderer/Renderer.hpp"

#include "UI/Components/MainFrame.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace {
static constexpr const char* kSettingsFileName = "settings.json";
static constexpr ImGuiWindowFlags kWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

#ifdef GLAD_DEBUG
#define MAP_ERROR_CODE_TO_STRING(Error) {Error, #Error}
void GladOpenGLPostCallback(const char* functionName, void*, int, ...) {

    static const std::unordered_map<int, const char*> kErrorStrings{
        MAP_ERROR_CODE_TO_STRING(GL_INVALID_ENUM),
        MAP_ERROR_CODE_TO_STRING(GL_INVALID_VALUE),
        MAP_ERROR_CODE_TO_STRING(GL_INVALID_OPERATION),
        MAP_ERROR_CODE_TO_STRING(GL_STACK_OVERFLOW),
        MAP_ERROR_CODE_TO_STRING(GL_STACK_UNDERFLOW),
        MAP_ERROR_CODE_TO_STRING(GL_OUT_OF_MEMORY),
        MAP_ERROR_CODE_TO_STRING(GL_INVALID_FRAMEBUFFER_OPERATION),
    };

    const GLenum errorCode = glad_glGetError();
    if (errorCode != GL_NO_ERROR)
        std::cerr << std::format("{}: Error {}\n", functionName, kErrorStrings.at(errorCode));
}
#endif
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

    // Handlers
    void onProjectionChange(Camera::Projection projection);
    void onWireframeModeChange(bool wireframe) const;
    void onInitialized();
    void onSaved();
    void onViewportResized(const glm::uvec2& dimensions);
    void onThemeChanged(int theme);

    sigslot::signal<> initialized;

public:
    Renderer m_renderer;

    GLFWwindow* m_pWindow = nullptr;
    OrbitalControls m_callbacks; // window controls. Sets up an orbital camera.

    // Ui

    MainFrameComponent m_mainFrame;

    struct DataModel {

        // Camera

        std::unique_ptr<Camera> m_pCamera;
        PerspectiveCamera m_persp;
        OrthographicCamera m_ortho;

        // Model

        Mesh mesh;
        LambertianMaterial lambertianMaterial;
        PhongMaterial phongMaterial;
        PhongTexturedMaterial phongTexturedMaterial;

        // Scene

        glm::vec4 clearColor{ 0.305f, 0.520f, 0.828f, 1.f };
        glm::vec3 ambientColor{ 1.f };
        float ambientIntensity = 0.f;
        std::array<DirectionalLight, 3> lights;

        // Window

        glm::vec2 windowSize{ 800, 600 };
        glm::vec2 windowPosition{ 0, 0 };
        bool windowMaximized = false;
        int windowTheme = 0;
    } m_dataModel;
};

Application::Private::Private() {

    // Camera setup
    m_dataModel.m_pCamera = [this] {
        std::unique_ptr<PerspectiveCamera> pCamera = std::make_unique<PerspectiveCamera>();
        pCamera->aspectRatio(m_dataModel.windowSize.x / m_dataModel.windowSize.y);
        pCamera->fovY(glm::radians(45.f));
        pCamera->far(150.f);
        pCamera->near(0.01f);
        pCamera->position({ 0.f, 0.f, 5.f });

        return std::move(pCamera);
    }();

    m_callbacks.camera(m_dataModel.m_pCamera.get());

    m_dataModel.m_persp = *static_cast<PerspectiveCamera*>(m_dataModel.m_pCamera.get());
    m_dataModel.m_ortho = m_dataModel.m_persp;

    // Signals
    initialized.connect(&Application::Private::onInitialized, this);

    m_callbacks.windowMaximized.connect([this](bool maximized) { m_dataModel.windowMaximized = maximized; });
    m_callbacks.windowSizeChanged.connect([this](const glm::ivec2& size) { m_dataModel.windowSize = size; });
    m_callbacks.windowPositionChanged.connect([this](const glm::ivec2& position) { m_dataModel.windowPosition = position; });

    m_dataModel.lights.at(0).enabled(true);
    m_dataModel.lights.at(0).pitch(glm::radians(45.f));
    m_dataModel.lights.at(0).yaw(0.f);

    m_dataModel.lights.at(1).enabled(true);
    m_dataModel.lights.at(1).pitch(glm::radians(45.f));
    m_dataModel.lights.at(1).yaw(glm::radians(180.f));

    m_dataModel.lights.at(2).pitch(glm::radians(-90.f));
    m_dataModel.lights.at(2).yaw(0.f);
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

    const GLuint framebufferId = m_renderer.framebufferId();
    const GLuint framebufferTextureId = m_renderer.framebufferTextureId();

    if (framebufferId > 0) {

        glClearColor(
            m_dataModel.clearColor.r,
            m_dataModel.clearColor.g,
            m_dataModel.clearColor.b,
            m_dataModel.clearColor.a
        );

        m_mainFrame.viewport().framebufferTexture(framebufferTextureId);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
        glClear(m_renderer.framebufferBitplane());

        if (m_dataModel.mesh.model() && m_dataModel.mesh.material()) {

            if (!m_dataModel.mesh.initialized()) {
                Renderer::Configure(m_dataModel.mesh);
                Renderer::Allocate(m_dataModel.mesh);
            }

            m_renderer.draw(m_dataModel.mesh);
        }

        m_renderer.purgeFramebuffer();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static_cast<IComponent&>(m_mainFrame).render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_pWindow);
}

void Application::Private::update() {

    m_callbacks.navigationEnabled(m_mainFrame.viewport().active());

    if (m_dataModel.m_pCamera)
        m_dataModel.m_pCamera->update();
}

GLFWwindow* Application::Private::createWindow(const glm::ivec2& dimensions, const std::string& title) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
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

    obj["Window"]["size"] = m_dataModel.windowSize;
    obj["Window"]["position"] = m_dataModel.windowPosition;
    obj["Window"]["maximized"] = m_dataModel.windowMaximized;
    obj["Window"]["theme"] = m_dataModel.windowTheme;
    
    obj["Scene"]["clearColor"] = m_dataModel.clearColor;
    obj["Scene"]["ambientColor"] = m_dataModel.ambientColor;
    obj["Scene"]["ambientIntensity"] = m_dataModel.ambientIntensity;
    
    for (const DirectionalLight& light : m_dataModel.lights) {
        if (const auto json = light.save(); json.is_object())
            obj["Lighting"].push_back(json);
    }
    
    if (const auto json = m_dataModel.mesh.save(); json.is_object())
        obj.update(json);
    
    if (const auto json = m_dataModel.lambertianMaterial.save(); json.is_object())
        obj["Materials"].update(json);
    
    if (const auto json = m_dataModel.phongMaterial.save(); json.is_object())
        obj["Materials"].update(json);
    
    if (const auto json = m_dataModel.phongTexturedMaterial.save(); json.is_object())
        obj["Materials"].update(json);

    return json;
}

void Application::Private::restore(const nlohmann::json& settings) {

    if (settings.is_null() || !settings.is_object())
        return;

    if (settings.contains("Window")) {
        const nlohmann::json& json = settings.at("Window");
        
        if (json.contains("size")) {
            json.at("size").get_to(m_dataModel.windowSize);
            glfwSetWindowSize(m_pWindow, m_dataModel.windowSize.x, m_dataModel.windowSize.y);
        }

        if (json.contains("position")) {
            json.at("position").get_to(m_dataModel.windowPosition);
            glfwSetWindowPos(m_pWindow, m_dataModel.windowPosition.x, m_dataModel.windowPosition.y);
        }

        if (json.contains("maximized")) {
            json.at("maximized").get_to(m_dataModel.windowMaximized);

            if (m_dataModel.windowMaximized)
                glfwMaximizeWindow(m_pWindow);
        }

        if (json.contains("theme")) {

            int theme = 0;
            json.at("theme").get_to(theme);
            onThemeChanged(theme);
        }
    }

    if (settings.contains("Scene")) {
        const nlohmann::json& json = settings.at("Scene");

        if (json.contains("clearColor"))
            json.at("clearColor").get_to(m_dataModel.clearColor);

        if (json.contains("ambientColor"))
            json.at("ambientColor").get_to(m_dataModel.ambientColor);

        if (json.contains("ambientIntensity"))
            json.at("ambientIntensity").get_to(m_dataModel.ambientIntensity);
    }

    if (settings.contains("Lighting")) {
        const nlohmann::json& json = settings.at("Lighting");

        if (json.is_array() && json.size() <= m_dataModel.lights.size()) {
            for (std::size_t index = 0; index < json.size(); ++index)
                m_dataModel.lights.at(index).restore(json.at(index).at("DirectionalLight"));
        }
    }

    if (settings.contains(m_dataModel.mesh.id()))
        m_dataModel.mesh.restore(settings.at(m_dataModel.mesh.id().data()));

    if (settings.contains("Materials")) {
        const nlohmann::json& json = settings.at("Materials");

        if (json.contains(m_dataModel.lambertianMaterial.id()))
            m_dataModel.lambertianMaterial.restore(json.at(m_dataModel.lambertianMaterial.id().data()));

        if (json.contains(m_dataModel.phongMaterial.id()))
            m_dataModel.phongMaterial.restore(json.at(m_dataModel.phongMaterial.id().data()));

        if (json.contains(m_dataModel.phongTexturedMaterial.id()))
            m_dataModel.phongTexturedMaterial.restore(json.at(m_dataModel.phongTexturedMaterial.id().data()));
    }
}

void Application::Private::onProjectionChange(Camera::Projection projection) {

    if (projection == Camera::Projection::Orthographic) {

        const PerspectiveCamera* pCamera = dynamic_cast<const PerspectiveCamera*>(m_dataModel.m_pCamera.get());
        if (pCamera) {
            m_dataModel.m_pCamera = std::make_unique<OrthographicCamera>(*pCamera);
            m_callbacks.camera(m_dataModel.m_pCamera.get());
            m_renderer.camera(m_dataModel.m_pCamera.get());
        }
    }

    if (projection == Camera::Projection::Perspective) {

        const OrthographicCamera* pCamera = dynamic_cast<const OrthographicCamera*>(m_dataModel.m_pCamera.get());
        if (pCamera) {
            m_dataModel.m_pCamera = std::make_unique<PerspectiveCamera>(*pCamera);
            m_callbacks.camera(m_dataModel.m_pCamera.get());
            m_renderer.camera(m_dataModel.m_pCamera.get());
        }
    }
}

void Application::Private::onWireframeModeChange(bool wireframe) const {

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::Private::onInitialized() {

    m_mainFrame.viewport().viewportResized.connect(&Application::Private::onViewportResized, this);
    m_mainFrame.exited.connect([this] { glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE); });
    m_mainFrame.themeChanged.connect([this](int theme) { onThemeChanged(theme); });

    // Window controls signals
    m_callbacks.projectionChanged.connect(&Application::Private::onProjectionChange, this);
    m_callbacks.wireframeChanged.connect(&Application::Private::onWireframeModeChange, this);
    m_callbacks.saved.connect(&Application::Private::onSaved, this);

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

    m_dataModel.mesh.material(&m_dataModel.phongMaterial);

    const std::array<DirectionalLight*, 3> lights{
        &m_dataModel.lights.at(0),
        &m_dataModel.lights.at(1),
        &m_dataModel.lights.at(2)
    };

    m_renderer.directionalLights(lights);
    m_renderer.ambientColor(&m_dataModel.ambientColor);
    m_renderer.ambientIntensity(&m_dataModel.ambientIntensity);

    MainFrameComponent::DataModel model;
    model.m_pAmbientColor = &m_dataModel.ambientColor;
    model.m_pClearColor = &m_dataModel.clearColor;
    model.m_pAmbientIntensity = &m_dataModel.ambientIntensity;
    model.m_pMesh = &m_dataModel.mesh;
    model.m_pLambertianMat = &m_dataModel.lambertianMaterial;
    model.m_pPhongMat = &m_dataModel.phongMaterial;
    model.m_pPhongTexturedMat = &m_dataModel.phongTexturedMaterial;
    model.m_lights = lights;
    model.m_pWindowTheme = &m_dataModel.windowTheme;

    static_cast<IComponent&>(m_mainFrame).syncFrom(&model);
}

void Application::Private::onSaved() {

    std::ofstream file{ kSettingsFileName };
    file << save();
}

void Application::Private::onViewportResized(const glm::uvec2& dimensions) {

    m_renderer.createFramebuffer(dimensions);
    glViewport(0, 0, dimensions.x, dimensions.y);

    if (m_dataModel.m_pCamera)
        m_dataModel.m_pCamera->aspectRatio(static_cast<float>(dimensions.x) / static_cast<float>(dimensions.y));
}

void Application::Private::onThemeChanged(int theme) {

    m_dataModel.windowTheme = theme;

    switch (m_dataModel.windowTheme) {
    case 0: ImGui::StyleColorsClassic(); break;
    case 1: ImGui::StyleColorsLight(); break;
    case 2: ImGui::StyleColorsDark(); break;
    default: break;
    }
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

    m_pPrivate->m_pWindow = m_pPrivate->createWindow({ m_pPrivate->m_dataModel.windowSize.x, m_pPrivate->m_dataModel.windowSize.y}, "Model Viewer");
    if (!m_pPrivate->m_pWindow) {
        std::cerr << "Failed to create a window.\n";
        return false;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to load GLAD functions.\n";
        return false;
    }

#ifdef GLAD_DEBUG
    // Setup error handling for OpenGL calls
    glad_set_post_callback(GladOpenGLPostCallback);
#endif

    m_pPrivate->m_renderer.setup();
    m_pPrivate->m_renderer.camera(m_pPrivate->m_dataModel.m_pCamera.get());

    // Setup window callbacks for modular window controls.
    glfwSetWindowUserPointer(m_pPrivate->m_pWindow, &m_pPrivate->m_callbacks);
    
    glfwSetCursorPosCallback(m_pPrivate->m_pWindow, WindowCallbacks::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_pPrivate->m_pWindow, WindowCallbacks::MouseButtonCallback);
    glfwSetKeyCallback(m_pPrivate->m_pWindow, WindowCallbacks::KeyboardCallback);
    glfwSetScrollCallback(m_pPrivate->m_pWindow, WindowCallbacks::ScrollCallback);
    
    glfwSetWindowMaximizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowMaximizedCallback);
    glfwSetWindowSizeCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowSizeCallback);
    glfwSetWindowPosCallback(m_pPrivate->m_pWindow, WindowCallbacks::WindowPositionCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 15.f);
    
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;
    fontConfig.GlyphMinAdvanceX = 15.f;

    static std::array<ImWchar, 2> iconRange{ 0xe005, 0xf8ff };
    ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 15.f, &fontConfig, iconRange.data());

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameBorderSize = 1.f;
    style.FrameRounding = 2.f;
    style.ItemSpacing.x = 7.f;
    style.ItemSpacing.y = 7.f;
    style.IndentSpacing = 21.f;
    style.TabRounding = 3.f;
    style.ScrollbarRounding = 2.f;

    ImGui_ImplGlfw_InitForOpenGL(m_pPrivate->m_pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_pPrivate->initialized();

    return true;
}

void Application::run() {

    while (!glfwWindowShouldClose(m_pPrivate->m_pWindow)) {

        glfwPollEvents();
        m_pPrivate->update();
        m_pPrivate->render();
    }
}
