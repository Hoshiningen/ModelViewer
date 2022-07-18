#include "Camera/Camera.hpp"

struct Camera::Private {
    Private() = default;
    Private(const glm::vec3& target, const glm::vec3& position);

    glm::mat4 view(const glm::vec3& up) const;

    glm::vec3 m_position{ 0.f, 0.f, 0.f };
    glm::vec3 m_target{ 0.f, 0.f, 0.f };
};

Camera::Private::Private(const glm::vec3& position, const glm::vec3& target)
    : m_position(position), m_target(target) {}

glm::mat4 Camera::Private::view(const glm::vec3& up) const {
    return glm::lookAt(m_position, m_target, up);
}


Camera::Camera()
    : m_pPrivate(std::make_unique<Private>()) {}

Camera::Camera(float aspectRatio, float fovY, float nearZ, float farZ)
    : Camera({}, aspectRatio, fovY, nearZ, farZ) {}

Camera::Camera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ)
    : m_pPrivate(std::make_unique<Private>(position, glm::vec3{})), m_aspectRatio(aspectRatio), m_fovY(fovY), m_nearZ(nearZ), m_farZ(farZ) {}

Camera::Camera(const Camera& other) {
    *this = other;
}

Camera& Camera::operator=(const Camera& other) {
    
    if (this != &other) {

        m_aspectRatio = other.m_aspectRatio;
        m_farZ = other.m_farZ;
        m_fovY = other.m_fovY;
        m_nearZ = other.m_nearZ;
        m_viewProjection = other.m_viewProjection;

        m_pPrivate->m_position = other.m_pPrivate->m_position;
        m_pPrivate->m_target = other.m_pPrivate->m_target;
    }

    return *this;
}

Camera::Camera(Camera&& other) noexcept {
    *this = std::move(other);
}

Camera& Camera::operator=(Camera&& other) noexcept {

    if (this != &other) {

        m_aspectRatio = std::exchange(other.m_aspectRatio, 0.f);
        m_farZ = std::exchange(other.m_farZ, 0.f);
        m_fovY = std::exchange(other.m_fovY, 0.f);
        m_nearZ = std::exchange(other.m_nearZ, 0.f);
        m_viewProjection = std::exchange(other.m_viewProjection, glm::identity<glm::mat4>());

        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);
    }

    return *this;
}

Camera::~Camera() noexcept {}

glm::vec3 Camera::worldUp() {
    static const glm::vec3 kUp{ 0.f, 1.f, 0.f };
    return kUp;
}

DEFINE_GETTER_IMMUTABLE(Camera, position, glm::vec3, m_pPrivate->m_position)
DEFINE_SETTER_CONSTREF(Camera, position, m_pPrivate->m_position)

DEFINE_GETTER_IMMUTABLE(Camera, target, glm::vec3, m_pPrivate->m_target)
DEFINE_SETTER_CONSTREF(Camera, target, m_pPrivate->m_target)

DEFINE_GETTER_IMMUTABLE(Camera, near, float, m_nearZ)
DEFINE_SETTER_COPY(Camera, near, m_nearZ)

DEFINE_GETTER_IMMUTABLE(Camera, far, float, m_farZ)
DEFINE_SETTER_COPY(Camera, far, m_farZ)

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

DEFINE_GETTER_IMMUTABLE(Camera, viewProjection, glm::mat4, m_viewProjection)

void Camera::update() {
    m_viewProjection = projection() * m_pPrivate->view(up());
}

glm::vec3 Camera::forward() const {
    return glm::normalize(m_pPrivate->m_target - m_pPrivate->m_position);
}

glm::vec3 Camera::right() const {
    return glm::normalize(glm::cross(forward(), worldUp()));
}

glm::vec3 Camera::up() const {
    return glm::normalize(glm::cross(right(), forward()));
}

glm::mat4 Camera::projection() const {
    return glm::identity<glm::mat4>();
}
