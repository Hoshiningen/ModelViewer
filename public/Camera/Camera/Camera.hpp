#pragma once

#include "Common/ClassMacros.hpp"

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(float aspectRatio, float fovY, float nearZ, float farZ);
    Camera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ);

    static glm::vec3 worldUp();

    DECLARE_GETTER_IMMUTABLE(position, glm::vec3)
    DECLARE_SETTER_CONSTREF(position, glm::vec3)

    DECLARE_GETTER_IMMUTABLE(target, glm::vec3)
    DECLARE_SETTER_CONSTREF(target, glm::vec3)

    DECLARE_GETTER_IMMUTABLE(near, float)
    DECLARE_SETTER_COPY(near, float)

    DECLARE_GETTER_IMMUTABLE(far, float)
    DECLARE_SETTER_COPY(far, float)

    DECLARE_GETTER_IMMUTABLE_COPY(fovX, float)

    virtual float fovY() const;
    virtual void fovY(float value);

    virtual float aspectRatio() const;
    virtual void aspectRatio(float value);

    DECLARE_GETTER_IMMUTABLE(viewProjection, glm::mat4)

    void update();

    DECLARE_GETTER_IMMUTABLE_COPY(forward, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(right, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(up, glm::vec3)

protected:
    virtual glm::mat4 projection() const;

    float m_nearZ = 0.1f; // Distance of the near plane from the camera
    float m_farZ = 100.f; // Distance of the far plane from the camera
    float m_aspectRatio = 1.f;
    float m_fovY = glm::radians(45.0); // Full fovY angle

    glm::mat4 m_viewProjection = glm::identity<glm::mat4>();

private:
    COMPILATION_FIREWALL_COPY_MOVE(Camera)
};
