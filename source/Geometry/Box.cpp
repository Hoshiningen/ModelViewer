#include "Geometry/Box.hpp"

struct Box::Private {
    Private() = default;
    Private(float width, float height, float length);

    glm::vec3 m_position{ 0.f, 0.f, 0.f };

    float m_width = 0.f;
    float m_height = 0.f;
    float m_length = 0.f;
};

Box::Private::Private(float width, float height, float length)
    : m_width(width), m_height(height), m_length(length) {}


Box::Box()
    : m_pPrivate(std::make_unique<Private>()) {}

Box::Box(float width, float height, float length)
    : m_pPrivate(std::make_unique<Private>(width, height, length)) {}

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

glm::vec3 Box::position() const {
    return m_pPrivate->m_position;
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
