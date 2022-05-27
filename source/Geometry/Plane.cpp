#include "Geometry/Plane.hpp"

#include <glm/geometric.hpp>

struct Plane::Private {
    Private(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur);

    void buildNonIndexed(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer) const;
    void buildIndexed(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer) const;

    glm::vec3 m_u; // Horizontal direction
    glm::vec3 m_v; // Vertical direction
};

Plane::Private::Private(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur)
    : m_u(glm::normalize(ll - lr)), m_v(glm::normalize(ur - lr)) {}

void Plane::Private::buildNonIndexed(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer) const {

    buffer.addVertex(ur);
    buffer.addVertex(lr);
    buffer.addVertex(ll);
    buffer.addVertex(ll);
    buffer.addVertex(ll + (m_v * glm::distance(lr, ur)));
    buffer.addVertex(ur);

    const glm::vec3 normal = glm::cross(m_v, m_u);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
}

void Plane::Private::buildIndexed(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur, VertexBuffer& buffer) const {

    buffer.addVertex(ur);
    buffer.addVertex(lr);
    buffer.addVertex(ll);
    buffer.addVertex(ll + (m_v * glm::distance(lr, ur)));

    const glm::vec3 normal = glm::cross(m_v, m_u);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);
    buffer.addNormal(normal);

    /*
       ul (3)              ur (0)
        o------------------o
        |                / |
        |    1         /   |
        |            /     |
        |         /        |
        |      /           |
        |   /        2     |
        |/                 |
        o------------------o
       ll (2)              lr (1)
    */

    // Triangle 1
    buffer.addIndex(0);
    buffer.addIndex(2);
    buffer.addIndex(3);

    // Triangle 2
    buffer.addIndex(0);
    buffer.addIndex(1);
    buffer.addIndex(2);
}


Plane::Plane(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur)
    : m_pPrivate(std::make_unique<Private>(ll, lr, ur)) {

    //m_pPrivate->buildNonIndexed(ll, lr, ur, m_buffer);
    m_pPrivate->buildIndexed(ll, lr, ur, m_buffer);
}

Plane::~Plane() noexcept {}

Plane::Plane(const Plane& other) {
    *this = other;
}

Plane& Plane::operator=(const Plane& other) {

    if (this != &other) {

        m_buffer = other.m_buffer;
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
