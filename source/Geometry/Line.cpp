#include "Geometry/Line.hpp"

Line::Line(const glm::vec3& start, const glm::vec3& end) {

    m_buffer.addVertex(start);
    m_buffer.addVertex(end);
}

Line::Line(const Line& other) {
    *this = other;
}

Line& Line::operator=(const Line& other) {

    if (this != &other)
        m_buffer = other.m_buffer;

    return *this;
}

Line::Line(Line&& other) noexcept {
    *this = std::move(other);
}

Line& Line::operator=(Line&& other) noexcept {

    if (this != &other)
        m_buffer = std::exchange(other.m_buffer, {});

    return *this;
}

glm::vec3 Line::start() const {
    return m_buffer.vertices().front();
}

glm::vec3 Line::end() const {
    return m_buffer.vertices().back();
}
