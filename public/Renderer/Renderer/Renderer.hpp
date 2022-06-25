#pragma once

#include <memory>

#include <glm/vec4.hpp>

constexpr glm::vec4 kBlack{ 0.f, 0.f, 0.f, 1.f };
constexpr glm::vec4 kWhite{ 1.f, 1.f, 1.f, 1.f };

constexpr glm::vec4 kRed{ 1.f, 0.f, 0.f, 1.f };
constexpr glm::vec4 kGreen{ 0.f, 1.f, 0.f, 1.f };
constexpr glm::vec4 kBlue{ 0.f, 0.f, 1.f, 1.f };

constexpr glm::vec4 kYellow{ 1.f, 1.f, 0.f, 1.f };
constexpr glm::vec4 kMagenta{ 1.f, 0.f, 1.f, 1.f };
constexpr glm::vec4 kCyan{ 0.f, 1.f, 1.f, 1.f };

constexpr glm::vec4 kSunlight{ 255.f / 255.f, 232.f / 255.f, 181.f / 255.f, 1.f };

class Camera;
class IMaterial;
class ShaderProgram;
class VertexBuffered;

class Renderer {
public:

    Renderer();
    virtual ~Renderer();

    void setup();
    void camera(Camera* pCamera);

    bool initialize(VertexBuffered& geometry, const IMaterial& material) const;
    void draw(const VertexBuffered& geometry, const IMaterial& material) const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
