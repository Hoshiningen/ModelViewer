#include "Geometry/Plane.hpp"

#include <glm/geometric.hpp>

struct Plane::Private {
    Private(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer);

    void build(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer);

    glm::vec3 m_center;
    glm::vec3 m_u; // Horizontal direction
    glm::vec3 m_v; // Vertical direction
};

Plane::Private::Private(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer)
    : m_u(glm::normalize(ll - lr)), m_v(glm::normalize(lr - ur)) {

    build(ll, lr, ur, buffer);
}

void Plane::Private::build(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer) {

    buffer.addVertex(ll);
    buffer.addVertex(lr);
    buffer.addVertex(ur);
    buffer.addVertex(ll + (m_v * glm::distance(lr, ur)));

    const glm::vec3 normal = glm::cross(m_v, m_u);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);

    // First triangle.
    buffer.addIndex(0);
    buffer.addIndex(1);
    buffer.addIndex(2);

    // Second triangle.
    buffer.addIndex(0);
    buffer.addIndex(2);
    buffer.addIndex(3);
}


Plane::Plane(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur)
    : m_pPrivate(std::make_unique<Private>(ll, lr, ur, m_buffer)) {}

Plane::~Plane() noexcept {}

Plane::Plane(const Plane& other) {
    *this = other;
}

Plane& Plane::operator=(const Plane& other) {

    if (this != &other) {

        m_buffer = other.m_buffer;
        m_pPrivate->m_center = other.m_pPrivate->m_center;
        m_pPrivate->m_u = other.m_pPrivate->m_u;
        m_pPrivate->m_v = other.m_pPrivate->m_v;
    }

    return *this;
}

Plane::Plane(Plane&& other) noexcept {
    *this = std::move(other);
}

Plane& Plane::operator=(Plane&& other) noexcept {

    if (this != &other) {

        m_buffer = std::exchange(other.m_buffer, {});
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);
    }

    return *this;
}


glm::vec3 Plane::normal() const {
    return glm::cross(m_pPrivate->m_v, m_pPrivate->m_u);
}

float Plane::width() const {
    return glm::length(m_pPrivate->m_u);
}

float Plane::height() const {
    return glm::length(m_pPrivate->m_v);
}
