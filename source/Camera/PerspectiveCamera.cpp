#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthographicCamera.hpp"

PerspectiveCamera::PerspectiveCamera(const OrthographicCamera& camera)
    : Camera(camera.position(), camera.aspectRatio(), camera.fovY(), camera.near(), camera.far()) {}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& other) {
    *this = other;
}

PerspectiveCamera& PerspectiveCamera::operator=(const PerspectiveCamera& other) {

    if (this != &other)
        static_cast<Camera&>(*this) = other;
    
    return *this;
}

PerspectiveCamera::PerspectiveCamera(PerspectiveCamera&& other) noexcept {
    *this = std::move(other);
}

PerspectiveCamera& PerspectiveCamera::operator=(PerspectiveCamera&& other) noexcept {

    if (this != &other)
        static_cast<Camera&>(*this) = std::move(other);

    return *this;
}

glm::mat4 PerspectiveCamera::projection() const {
    return glm::perspective(m_fovY, m_aspectRatio, m_nearZ, m_farZ);
}


