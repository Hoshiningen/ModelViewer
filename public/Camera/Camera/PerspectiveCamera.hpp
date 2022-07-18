#pragma once

#include "Camera/Camera.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/mat4x4.hpp>

class OrthographicCamera;

class PerspectiveCamera : public Camera {
public:
    using Camera::Camera;

    PerspectiveCamera() = default;
    PerspectiveCamera(const OrthographicCamera& camera);

    virtual ~PerspectiveCamera() noexcept = default;

    COPY_MOVE_ENABLED(PerspectiveCamera)

protected:
    virtual glm::mat4 projection() const override;
};
