#include "Geometry/VertexBuffered.hpp"

VertexBuffered::VertexBuffered(const std::vector<glm::vec3>& vertices,
                               const std::vector<glm::vec3>& normals,
                               const std::list<uint32_t>& indices)
    : m_buffer(vertices, normals, indices) {}

VertexBuffered::VertexBuffered(const VertexBuffered& other) {
    *this = other;
}

VertexBuffered& VertexBuffered::operator=(const VertexBuffered& other) {

    if (this != &other)
        m_buffer = other.m_buffer;

    return *this;
}

VertexBuffered::VertexBuffered(VertexBuffered&& other) noexcept {
    *this = std::move(other);
}

VertexBuffered& VertexBuffered::operator=(VertexBuffered&& other) noexcept {

    if (this != &other)
        m_buffer = std::move(other.m_buffer);

    return *this;
}

std::optional<std::list<uint32_t>> VertexBuffered::indices() const {

    if (m_buffer.indices().empty())
        return std::nullopt;

    return m_buffer.indices();
}

std::optional<std::vector<glm::vec3>> VertexBuffered::vertices() const {

    if (m_buffer.vertices().empty())
        return std::nullopt;

    return m_buffer.vertices();
}

std::optional<std::vector<glm::vec3>> VertexBuffered::normals() const {

    if (m_buffer.normals().empty())
        return std::nullopt;

    return m_buffer.normals();
}
