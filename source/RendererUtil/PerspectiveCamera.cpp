#include "PerspectiveCamera.hpp"
#include "OrthographicCamera.hpp"

PerspectiveCamera::PerspectiveCamera(const OrthographicCamera& camera)
    : Camera(camera.position(), camera.aspectRatio(), camera.fovY(), camera.near(), camera.far()) {}

std::vector<Camera::Geometry> PerspectiveCamera::debugFrustum() const {

    const glm::vec3 eyePoint = position();

    const auto ComputeOffsets2D = [this, &eyePoint](float distFromCamera, float scalar = 1.f) {
        
        const glm::vec3 point = (eyePoint + forward() * distFromCamera) * scalar;
        const float len = glm::length(point - eyePoint);

        const float yOffset = len * glm::tan(m_fovY / 2);
        const float xOffset = yOffset * m_aspectRatio;

        return std::make_tuple(xOffset, yOffset, point.z);
    };
    
    const auto MakeBox = [&ComputeOffsets2D](float distFromCamera, float scalar = 1.f) {

        struct Box {
            glm::vec3 bottomLeft;
            glm::vec3 bottomRight;
            glm::vec3 topRight;
            glm::vec3 topLeft;
        } box;

        const auto [xOffset, yOffset, z] = ComputeOffsets2D(distFromCamera, scalar);

        box.bottomLeft = { -xOffset, -yOffset, z };
        box.bottomRight = { xOffset, -yOffset, z };
        box.topRight = { xOffset, yOffset, z };
        box.topLeft = { -xOffset, yOffset, z };

        return box;
    };

    const auto farBox = MakeBox(m_farZ, 0.1f);
    const auto originBox = MakeBox(glm::length(target() - eyePoint));
    const auto nearBox = MakeBox(m_nearZ);

    std::vector<Geometry> geom;
    geom.push_back(eyePoint);

    // Frustum lines
    geom.push_back(std::make_pair(eyePoint, farBox.bottomLeft));
    geom.push_back(std::make_pair(eyePoint, farBox.bottomRight));
    geom.push_back(std::make_pair(eyePoint, farBox.topRight));
    geom.push_back(std::make_pair(eyePoint, farBox.topLeft));

    // Far endcap
    geom.push_back(std::make_pair(farBox.topLeft, farBox.topRight));
    geom.push_back(std::make_pair(farBox.topRight, farBox.bottomRight));
    geom.push_back(std::make_pair(farBox.bottomRight, farBox.bottomLeft));
    geom.push_back(std::make_pair(farBox.bottomLeft, farBox.topLeft));

    // Near endcap
    geom.push_back(std::make_pair(nearBox.topLeft, nearBox.topRight));
    geom.push_back(std::make_pair(nearBox.topRight, nearBox.bottomRight));
    geom.push_back(std::make_pair(nearBox.bottomRight, nearBox.bottomLeft));
    geom.push_back(std::make_pair(nearBox.bottomLeft, nearBox.topLeft));

    return geom;
}

glm::mat4 PerspectiveCamera::projection() const {
    return glm::perspective(m_fovY, m_aspectRatio, m_nearZ, m_farZ);
}
