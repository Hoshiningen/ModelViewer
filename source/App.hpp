#pragma once

#include "OrbitalControls.hpp"

#include "Renderer/Renderer.hpp"
#include "RendererUtil/PerspectiveCamera.hpp"
#include "RendererUtil/OrthographicCamera.hpp"

#include <filesystem>
#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

class Camera;

struct Config {
    glm::vec4 clearColor = { 0.f, 0.f , 0.f, 1.f };
    glm::ivec2 windowDimensions = { 800, 600 };
    std::string windowTitle = "OpenGL Application";
};

class Application {
public:
    explicit Application(const std::filesystem::path& configPath);

    bool setUp();
    void run();

    void onProjectionChange(WindowCallbacks::ProjectionChange projection);

private:
    void render();
    void update();

    Config readConfig(const std::filesystem::path& configPath);
    GLFWwindow* createWindow(const glm::ivec2& dimensions, const std::string& title);

    Renderer m_renderer;
    std::unique_ptr<Camera> m_pCamera;

    // Debug cameras
    PerspectiveCamera m_persp;
    OrthographicCamera m_ortho;

    Config m_config;
    GLFWwindow* m_pWindow = nullptr;
    OrbitalControls m_callbacks; // window controls. Sets up an orbital camera.

    // Ensures API shutdown happens after everything has been destroyed.
    static struct Destructor {
        ~Destructor();
    } kDestructor;
};
