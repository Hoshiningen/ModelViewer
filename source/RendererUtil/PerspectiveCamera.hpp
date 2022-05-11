#pragma once

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthographicCamera;

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(const OrthographicCamera& camera);

    using Camera::Camera;

    virtual std::vector<Geometry> debugFrustum() const override;

protected:
    virtual glm::mat4 projection() const override;
};
