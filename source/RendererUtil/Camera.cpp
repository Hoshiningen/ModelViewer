#include "Camera.hpp"

Camera::Camera(float aspectRatio, float fovY, float nearZ, float farZ)
    : Camera({}, aspectRatio, fovY, nearZ, farZ) {}

Camera::Camera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ)
    : m_position(position), m_aspectRatio(aspectRatio), m_fovY(fovY), m_nearZ(nearZ), m_farZ(farZ) {}

glm::vec3 Camera::position() const {
    return m_position;
}

void Camera::position(const glm::vec3& value) {
    m_position = value;
}

glm::vec3 Camera::target() const {
    return m_target;
}

void Camera::target(const glm::vec3& value) {
    m_target = value;
}

float Camera::near() const {
    return m_nearZ;
}

void Camera::near(float value) {
    m_nearZ = value;
}

float Camera::far() const {
    return m_farZ;
}

void Camera::far(float value) {
    m_farZ = value;
}

float Camera::fovX() const {
    return glm::atan(glm::tan(m_fovY / 2) * m_aspectRatio) * 2;
}

float Camera::fovY() const {
    return m_fovY;
}

void Camera::fovY(float value) {
    m_fovY = value;
}

float Camera::aspectRatio() const {
    return m_aspectRatio;
}

void Camera::aspectRatio(float value) {
    m_aspectRatio = value;
}

glm::mat4 Camera::viewProjection() const {
    return m_viewProjection;
}

void Camera::update() {
    m_viewProjection = projection() * view();
}

glm::vec3 Camera::forward() const {
    return glm::normalize(target() - m_position);
}

glm::vec3 Camera::right() const {
    return glm::normalize(glm::cross(forward(), worldUp()));
}

glm::vec3 Camera::up() const {
    return glm::normalize(glm::cross(right(), forward()));
}

glm::vec3 Camera::worldUp() const {
    static const glm::vec3 kUp{ 0.f, 1.f, 0.f };
    return kUp;
}

glm::mat4 Camera::projection() const {
    return glm::identity<glm::mat4>();
}

glm::mat4 Camera::view() const {
    return glm::lookAt(m_position, target(), up());
}
