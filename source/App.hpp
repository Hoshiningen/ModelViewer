#pragma once

#include "Renderer/Renderer.hpp"
#include "WindowCallbacks.hpp"

#include <filesystem>
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

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

private:
    void render();
    void update();

    Config readConfig(const std::filesystem::path& configPath);
    GLFWwindow* createWindow(const glm::ivec2& dimensions, const std::string& title);

    Renderer m_renderer;
    OrthographicCamera m_orthoCamera;
    PerspectiveCamera m_perspCamera;

    bool m_ortho = false;

    Config m_config;
    GLFWwindow* m_pWindow = nullptr;
    DefaultCallbacks m_callbacks;

    static struct Destructor {
        ~Destructor();
    } kDestructor;
};
