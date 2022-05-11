#include "OrthographicCamera.hpp"
#include "PerspectiveCamera.hpp"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ)
    : m_leftExtent(left), m_rightExtent(right), m_bottomExtent(bottom), m_topExtent(top) {

    const float width = std::fabs(left - right);
    const float height = std::fabs(bottom - top);
    
    m_nearZ = nearZ;
    m_farZ = farZ;
    m_aspectRatio = width / height;

    const glm::vec3 eyePoint = position();
    const glm::vec3 point = eyePoint + forward() * farZ;
    const float distance = glm::length(point - eyePoint);

    m_fovY = 2.f * glm::atan(height / (2.f * distance));
}

OrthographicCamera::OrthographicCamera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ)
    : Camera(position, aspectRatio, fovY, nearZ, farZ) {

    computeExtents();
}

OrthographicCamera::OrthographicCamera(const PerspectiveCamera& camera)
    : OrthographicCamera(camera.position(), camera.aspectRatio(), camera.fovY(), camera.near(), camera.far()) {}

float OrthographicCamera::leftExtent() const {
    return m_leftExtent;
}

void OrthographicCamera::leftExtent(float value) {
    m_leftExtent = value;
}

float OrthographicCamera::rightExtent() const {
    return m_rightExtent;
}

void OrthographicCamera::rightExtent(float value) {
    m_rightExtent = value;
}

float OrthographicCamera::bottomExtent() const {
    return m_bottomExtent;
}

void OrthographicCamera::bottomExtent(float value) {
    m_bottomExtent = value;
}

float OrthographicCamera::topExtent() const {
    return m_topExtent;
}

void OrthographicCamera::topExtent(float value) {
    m_topExtent = value;
}

float OrthographicCamera::fovY() const {
    return Camera::fovY();
}

void OrthographicCamera::fovY(float value) {

    Camera::fovY(value);
    computeExtents();
}

float OrthographicCamera::aspectRatio() const {
    return Camera::aspectRatio();
}

void OrthographicCamera::aspectRatio(float value) {

    Camera::aspectRatio(value);
    computeExtents();
}

std::vector<Camera::Geometry> OrthographicCamera::debugFrustum() const {

    const float farZ = (position() + forward() * m_farZ).z * 0.1f;
    const float nearZ = (position() + forward() * m_nearZ).z;

    const glm::vec3 farTopLeft{ m_leftExtent, m_topExtent, farZ };
    const glm::vec3 farTopRight{ m_rightExtent, m_topExtent, farZ };
    const glm::vec3 farBottomLeft{ m_leftExtent, m_bottomExtent, farZ };
    const glm::vec3 farBottomRight{ m_rightExtent, m_bottomExtent, farZ };

    const glm::vec3 nearTopLeft{ m_leftExtent, m_topExtent, nearZ };
    const glm::vec3 nearTopRight{ m_rightExtent, m_topExtent, nearZ };
    const glm::vec3 nearBottomLeft{ m_leftExtent, m_bottomExtent, nearZ };
    const glm::vec3 nearBottomRight{ m_rightExtent, m_bottomExtent, nearZ };

    std::vector<Geometry> geom;

    // Frustum lines
    geom.push_back(std::make_pair(nearBottomLeft, farBottomLeft));
    geom.push_back(std::make_pair(nearBottomRight, farBottomRight));
    geom.push_back(std::make_pair(nearTopRight, farTopRight));
    geom.push_back(std::make_pair(nearTopLeft, farTopLeft));

    // Far endcap
    geom.push_back(std::make_pair(farTopLeft, farTopRight));
    geom.push_back(std::make_pair(farTopRight, farBottomRight));
    geom.push_back(std::make_pair(farBottomRight, farBottomLeft));
    geom.push_back(std::make_pair(farBottomLeft, farTopLeft));

    // Near endcap
    geom.push_back(std::make_pair(nearTopLeft, nearTopRight));
    geom.push_back(std::make_pair(nearTopRight, nearBottomRight));
    geom.push_back(std::make_pair(nearBottomRight, nearBottomLeft));
    geom.push_back(std::make_pair(nearBottomLeft, nearTopLeft));

    return geom;
}

glm::mat4 OrthographicCamera::projection() const {
    return glm::ortho(m_leftExtent, m_rightExtent, m_bottomExtent, m_topExtent, m_nearZ, m_farZ);
}

void OrthographicCamera::computeExtents() {

    /*
    * Compute the extents based on the distance the camera is from the target.
    * We're always looking at the target and want to maintain the same view space
    * as what's provided in the perspective projection.
    * 
    *   _______________________________  ---- Far plane
    *   \  |                       |  /
    *    \ |                       | /   ---- Perspective frustum
    *     \|         Target        |/
    * ---  +-----------O-----------+     ---- Extents computed through this method
    *  |   |\                     /| 
    *  |   | \                   / |     ---- Orthographic frustum
    *  |   |  \                 /  | 
    *  |   +---+---------------+---+     ---- Near plane
    *  |        \             /
    *  |d        \           /
    *  |i         \    ^    /
    *  |s          \  / \  /
    *  |t           \  |  /
    *  |             \ | /
    *  |              \|/
    * ---              C                 ---- Camera position
    */

    const float distToTarget = glm::distance(target(), position());
    const float yOffset = distToTarget * glm::tan(m_fovY / 2.f);
    const float xOffset = yOffset * m_aspectRatio;

    m_leftExtent = -xOffset;
    m_rightExtent = xOffset;
    m_bottomExtent = -yOffset;
    m_topExtent = yOffset;
}
