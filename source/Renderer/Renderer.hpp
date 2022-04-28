#pragma once

#include "RendererUtil/Camera.hpp"
#include "RendererUtil/Shader.hpp"

#include <memory>
#include <variant>

class Renderer final {
public:
    void setUp();
    void canvasDimensions(const glm::ivec2& dimensions);

    void camera(Camera* pCamera) {
        m_pCamera = pCamera;
    }

    void drawLine(const glm::vec2& posA, const glm::vec2& posB) const;
    void drawPoint(const glm::vec2& pos) const;

private:
    void loadShaders();
    void createBuffers();

    Camera* m_pCamera = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    Shader m_shader;
};
