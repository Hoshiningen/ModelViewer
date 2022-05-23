#include "Camera/OrthographicCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

struct OrthographicCamera::Private {
    Private() = default;
    Private(float left, float right, float bottom, float top);

    void computeExtents(const glm::vec3& target, const glm::vec3& position, float fovY, float aspectRatio);

    float m_leftExtent = -0.5f;
    float m_rightExtent = 0.5f;
    float m_bottomExtent = -0.5f;
    float m_topExtent = 0.5f;
};

OrthographicCamera::Private::Private(float left, float right, float bottom, float top)
    : m_leftExtent(left), m_rightExtent(right), m_bottomExtent(bottom), m_topExtent(top) {}

void OrthographicCamera::Private::computeExtents(const glm::vec3& target, const glm::vec3& position, float fovY, float aspectRatio) {

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

    const float distToTarget = glm::distance(target, position);
    const float yOffset = distToTarget * glm::tan(fovY / 2.f);
    const float xOffset = yOffset * aspectRatio;

    m_leftExtent = -xOffset;
    m_rightExtent = xOffset;
    m_bottomExtent = -yOffset;
    m_topExtent = yOffset;
}

OrthographicCamera::OrthographicCamera()
    : m_pPrivate(std::make_unique<Private>()) {}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ)
    : m_pPrivate(std::make_unique<Private>(left, right, bottom, top)) {

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
    : Camera(position, aspectRatio, fovY, nearZ, farZ), m_pPrivate(std::make_unique<Private>()) {

    m_pPrivate->computeExtents(target(), position, fovY, aspectRatio);
}

OrthographicCamera::OrthographicCamera(const PerspectiveCamera& camera)
    : OrthographicCamera(camera.position(), camera.aspectRatio(), camera.fovY(), camera.near(), camera.far()) {}

OrthographicCamera::OrthographicCamera(const OrthographicCamera& other) {
    *this = other;
}

OrthographicCamera& OrthographicCamera::operator=(const OrthographicCamera& other) {

    if (this != &other) {

        static_cast<Camera&>(*this) = other;

        m_pPrivate->m_bottomExtent = other.m_pPrivate->m_bottomExtent;
        m_pPrivate->m_leftExtent = other.m_pPrivate->m_leftExtent;
        m_pPrivate->m_rightExtent = other.m_pPrivate->m_rightExtent;
        m_pPrivate->m_topExtent = other.m_pPrivate->m_topExtent;
    }

    return *this;
}

OrthographicCamera::OrthographicCamera(OrthographicCamera&& other) noexcept {
    *this = std::move(other);
}

OrthographicCamera& OrthographicCamera::operator=(OrthographicCamera&& other) noexcept {

    if (this != &other) {

        static_cast<Camera&>(*this) = std::move(other);
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);
    }

    return *this;
}

OrthographicCamera::~OrthographicCamera() noexcept {}

float OrthographicCamera::leftExtent() const {
    return m_pPrivate->m_leftExtent;
}

void OrthographicCamera::leftExtent(float value) {
    m_pPrivate->m_leftExtent = value;
}

float OrthographicCamera::rightExtent() const {
    return m_pPrivate->m_rightExtent;
}

void OrthographicCamera::rightExtent(float value) {
    m_pPrivate->m_rightExtent = value;
}

float OrthographicCamera::bottomExtent() const {
    return m_pPrivate->m_bottomExtent;
}

void OrthographicCamera::bottomExtent(float value) {
    m_pPrivate->m_bottomExtent = value;
}

float OrthographicCamera::topExtent() const {
    return m_pPrivate->m_topExtent;
}

void OrthographicCamera::topExtent(float value) {
    m_pPrivate->m_topExtent = value;
}

float OrthographicCamera::fovY() const {
    return Camera::fovY();
}

void OrthographicCamera::fovY(float value) {

    Camera::fovY(value);
    m_pPrivate->computeExtents(target(), position(), value, aspectRatio());
}

float OrthographicCamera::aspectRatio() const {
    return Camera::aspectRatio();
}

void OrthographicCamera::aspectRatio(float value) {

    Camera::aspectRatio(value);
    m_pPrivate->computeExtents(target(), position(), fovY(), value);
}

glm::mat4 OrthographicCamera::projection() const {

    return glm::ortho(
        m_pPrivate->m_leftExtent,
        m_pPrivate->m_rightExtent,
        m_pPrivate->m_bottomExtent,
        m_pPrivate->m_topExtent,
        m_nearZ,
        m_farZ
    );
}
