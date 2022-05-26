#include "Geometry/Box.hpp"
#include "Geometry/Plane.hpp"

struct Box::Private {
    Private() = default;
    Private(float width, float height, float length);

    void buildNonIndexed(float width, float height, float length, VertexBuffer& buffer) const;

    glm::vec3 m_position{ 0.f, 0.f, 0.f };

    float m_width = 1.f;
    float m_height = 1.f;
    float m_length = 1.f;
};

Box::Private::Private(float width, float height, float length)
    : m_width(width), m_height(height), m_length(length) {}

void Box::Private::buildNonIndexed(float width, float height, float length, VertexBuffer& buffer) const {

    const float halfWidth = width / 2.f;
    const float halfHeight = height / 2.f;
    const float halfLength = length / 2.f;

    const glm::vec3 ufl{ m_position.x - halfWidth, m_position.y + halfHeight, m_position.z + halfLength };
    const glm::vec3 ufr{ m_position.x + halfWidth, m_position.y + halfHeight, m_position.z + halfLength };
    const glm::vec3 lfr{ m_position.x + halfWidth, m_position.y - halfHeight, m_position.z + halfLength };
    const glm::vec3 lfl{ m_position.x - halfWidth, m_position.y - halfHeight, m_position.z + halfLength };

    const glm::vec3 ubl{ m_position.x - halfWidth, m_position.y + halfHeight, m_position.z - halfLength };
    const glm::vec3 ubr{ m_position.x + halfWidth, m_position.y + halfHeight, m_position.z - halfLength };
    const glm::vec3 lbr{ m_position.x + halfWidth, m_position.y - halfHeight, m_position.z - halfLength };
    const glm::vec3 lbl{ m_position.x - halfWidth, m_position.y - halfHeight, m_position.z - halfLength };

    /*
           ubl          ubr
            o -------- o
            | \        | \
            |  o-------+--o ufr
            |  | ufl   |  |
            |  |       |  |
        lbl o--|-------o  | lbr
             \ |        \ |
          lfl  o----------o lfr
    
    */

    const Plane top{ ufl, ufr, ubr };
    const Plane bottom{ lfl, lbl, lbr };
    const Plane right{ ubr, ufr, lfr };
    const Plane left{ ufl, ubl, lbl };
    const Plane back{ ubl, ubr, lbr };
    const Plane front{ ufr, ufl, lfl };

    const auto Unite = [](const Plane& plane, VertexBuffer& buffer) {

        const auto vertices = plane.vertices();
        const auto normals = plane.normals();

        if (!vertices.has_value() || !normals.has_value())
            return;

        for (const glm::vec3& vertex : *vertices)
            buffer.addVertex(vertex);

        for (const glm::vec3& normal : *normals)
            buffer.addNormal(normal);
    };

    Unite(top, buffer);
    Unite(bottom, buffer);
    Unite(right, buffer);
    Unite(left, buffer);
    Unite(back, buffer);
    Unite(front, buffer);
}


Box::Box()
    : m_pPrivate(std::make_unique<Private>()) {

    m_pPrivate->buildNonIndexed( m_pPrivate->m_width, m_pPrivate->m_height, m_pPrivate->m_length, m_buffer);
}

Box::Box(float width, float height, float length)
    : m_pPrivate(std::make_unique<Private>(width, height, length)) {

    m_pPrivate->buildNonIndexed( m_pPrivate->m_width, m_pPrivate->m_height, m_pPrivate->m_length, m_buffer);
}

Box::~Box() noexcept {}

Box::Box(const Box& other) {
    *this = other;
}

Box& Box::operator=(const Box& other) {
    
    if (this != &other) {

        m_buffer = other.m_buffer;
        m_pPrivate->m_height = other.m_pPrivate->m_height;
        m_pPrivate->m_length = other.m_pPrivate->m_length;
        m_pPrivate->m_position = other.m_pPrivate->m_position;
        m_pPrivate->m_width = other.m_pPrivate->m_width;
    }

    return *this;
}

Box::Box(Box&& other) noexcept {
    *this = std::move(other);
}

Box& Box::operator=(Box&& other) noexcept {

    if (this != &other) {

        m_buffer = std::exchange(other.m_buffer, {});
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);
    }

    return *this;
}

float Box::width() const {
    return m_pPrivate->m_width;
}

float Box::height() const {
    return m_pPrivate->m_height;
}

float Box::length() const {
    return m_pPrivate->m_length;
}
