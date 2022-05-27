#include "Geometry/VertexBuffer.hpp"

struct VertexBuffer::Private {
    Private() = default;
    Private(const std::vector<glm::vec3>& vertices,
            const std::vector<glm::vec3>& normals,
            const std::vector<uint32_t>& indices);

    std::vector<uint32_t> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
};

VertexBuffer::Private::Private(const std::vector<glm::vec3>& vertices,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<uint32_t>& indices)
    : m_vertices(vertices), m_normals(normals), m_indices(indices) {}


VertexBuffer::VertexBuffer()
    : m_pPrivate(std::make_unique<Private>()) {}

VertexBuffer::VertexBuffer(const std::vector<glm::vec3>& vertices,
                           const std::vector<glm::vec3>& normals,
                           const std::vector<uint32_t>& indices)
    : m_pPrivate(std::make_unique<Private>(vertices, normals, indices)) {}

VertexBuffer::VertexBuffer(const VertexBuffer& other) {
    *this = other;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {

    if (this != &other) {

        m_pPrivate->m_indices = other.m_pPrivate->m_indices;
        m_pPrivate->m_normals = other.m_pPrivate->m_normals;
        m_pPrivate->m_vertices = other.m_pPrivate->m_vertices;
    }

    return *this;
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
    *this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

VertexBuffer::~VertexBuffer() noexcept {}

void VertexBuffer::addVertex(const glm::vec3& vertex) {
    m_pPrivate->m_vertices.push_back(vertex);
}

void VertexBuffer::addNormal(const glm::vec3& normal) {
    m_pPrivate->m_normals.push_back(normal);
}

void VertexBuffer::addIndex(uint32_t index) {
    m_pPrivate->m_indices.push_back(index);
}

const std::vector<uint32_t>& VertexBuffer::indices() const {
    return m_pPrivate->m_indices;
}

std::vector<uint32_t>& VertexBuffer::indices() {
    return m_pPrivate->m_indices;
}

const std::vector<glm::vec3>& VertexBuffer::vertices() const {
    return m_pPrivate->m_vertices;
}

std::vector<glm::vec3>& VertexBuffer::vertices() {
    return m_pPrivate->m_vertices;
}

const std::vector<glm::vec3>& VertexBuffer::normals() const {
    return m_pPrivate->m_normals;
}

std::vector<glm::vec3>& VertexBuffer::normals() {
    return m_pPrivate->m_normals;
}
