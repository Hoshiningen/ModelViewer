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

class Camera;
class VertexBuffered;

class Renderer {
public:
    Renderer();
    virtual ~Renderer();

    void setup();
    void camera(Camera* pCamera);

    void draw(const VertexBuffered& geometry, const glm::vec4& color) const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
