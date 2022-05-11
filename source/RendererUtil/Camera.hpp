#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>
#include <variant>
#include <vector>

class Camera {
public:
    Camera() = default;
    Camera(float aspectRatio, float fovY, float nearZ, float farZ);
    Camera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ);

    glm::vec3 position() const;
    void position(const glm::vec3& value);

    glm::vec3 target() const;
    void target(const glm::vec3& value);

    float near() const;
    void near(float value);

    float far() const;
    void far(float value);

    float fovX() const;

    virtual float fovY() const;
    virtual void fovY(float value);

    virtual float aspectRatio() const;
    virtual void aspectRatio(float value);

    glm::mat4 viewProjection() const;

    void update();

    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    glm::vec3 worldUp() const;

    using Geometry = std::variant<glm::vec3, std::pair<glm::vec3, glm::vec3>>;
    virtual std::vector<Geometry> debugFrustum() const { return {}; };

protected:
    virtual glm::mat4 projection() const;;

    float m_nearZ = 0.1f; // Distance of the near plane from the camera
    float m_farZ = 100.f; // Distance of the far plane from the camera
    float m_aspectRatio = 1.f;
    float m_fovY = glm::radians(45.0); // Full fovY angle

    glm::mat4 m_viewProjection = glm::identity<glm::mat4>();

private:
    glm::mat4 view() const;

    glm::vec3 m_position{ 0.f, 0.f, 0.f };
    glm::vec3 m_target{ 0.f, 0.f, 0.f };
};
