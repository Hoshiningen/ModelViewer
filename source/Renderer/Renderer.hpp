#pragma once

#include "RendererUtil/Camera.hpp"
#include "RendererUtil/Shader.hpp"

#include <memory>
#include <variant>

constexpr glm::vec4 kRed{1.f, 0.f, 0.f, 1.f };
constexpr glm::vec4 kGreen{0.f, 1.f, 0.f, 1.f };
constexpr glm::vec4 kBlue{0.f, 0.f, 1.f, 1.f };

constexpr glm::vec4 kYellow{1.f, 1.f, 0.f, 1.f };
constexpr glm::vec4 kMagenta{1.f, 0.f, 1.f, 1.f };
constexpr glm::vec4 kCyan{0.f, 1.f, 1.f, 1.f };

class Renderer final {
public:
    void setUp();
    void canvasDimensions(const glm::ivec2& dimensions);

    void camera(Camera* pCamera) {
        m_pCamera = pCamera;
    }

    void drawLine(const glm::vec2& posA, const glm::vec2& posB, const glm::vec4& color) const;
    void drawLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) const;
    void drawPoint(const glm::vec2& pos, const glm::vec4& color) const;
    void drawPoint(const glm::vec3& pos, const glm::vec4& color) const;

private:
    void loadShaders();
    void createBuffers();

    Camera* m_pCamera = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    Shader m_shader;
};
