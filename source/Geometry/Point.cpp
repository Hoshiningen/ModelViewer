#include "Geometry/Point.hpp"

Point::Point(const glm::vec3& position) {
    m_buffer.addVertex(position);
}

Point::Point(const Point& other) {
    *this = other;
}

Point& Point::operator=(const Point& other) {
    
    if (this != &other)
        m_buffer = other.m_buffer;

    return *this;
}

Point::Point(Point&& other) noexcept {
    *this = std::move(other);
}

Point& Point::operator=(Point&& other) noexcept {
    
    if (this != &other)
        m_buffer = std::exchange(other.m_buffer, {});

    return *this;
}

glm::vec3 Point::position() const {
    return m_buffer.vertices().front();
}
