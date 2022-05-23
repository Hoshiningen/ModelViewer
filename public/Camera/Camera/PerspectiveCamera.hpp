#pragma once

#include "Camera/Camera.hpp"

#include <glm/mat4x4.hpp>

class OrthographicCamera;

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(const OrthographicCamera& camera);

    virtual ~PerspectiveCamera() noexcept = default;

    PerspectiveCamera(const PerspectiveCamera& other);
    PerspectiveCamera& operator=(const PerspectiveCamera& other);

    PerspectiveCamera(PerspectiveCamera&& other) noexcept;
    PerspectiveCamera& operator=(PerspectiveCamera&& other) noexcept;

    using Camera::Camera;

protected:
    virtual glm::mat4 projection() const override;
};
