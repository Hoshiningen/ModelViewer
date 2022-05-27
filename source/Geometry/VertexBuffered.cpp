#include "Geometry/VertexBuffered.hpp"

struct VertexBuffered::Private {
    GLuint m_bufferId = 0; // VAO
    GLuint m_normalId = 0; // Normal BO
    GLuint m_vertexId = 0; // Vertex BO
    GLuint m_indexId = 0; // EBO

    bool m_initialized = false;
};


VertexBuffered::VertexBuffered()
    : m_pPrivate(std::make_unique<Private>()) {}

VertexBuffered::VertexBuffered(const std::vector<glm::vec3>& vertices,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<uint32_t>& indices)
    : m_buffer(vertices, normals, indices), m_pPrivate(std::make_unique<Private>()) {}

VertexBuffered::~VertexBuffered() noexcept {

    glDeleteVertexArrays(1, &m_pPrivate->m_bufferId);
    glDeleteBuffers(1, &m_pPrivate->m_indexId);
    glDeleteBuffers(1, &m_pPrivate->m_normalId);
    glDeleteBuffers(1, &m_pPrivate->m_vertexId);
}

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

std::optional<std::vector<uint32_t>> VertexBuffered::indices() const {

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

GLuint VertexBuffered::Id() const {
    return m_pPrivate->m_bufferId;
}

GLuint VertexBuffered::normalBufferId() const {
    return m_pPrivate->m_normalId;
}

GLuint VertexBuffered::vertexBufferId() const {
    return m_pPrivate->m_vertexId;
}

GLuint VertexBuffered::indexBufferId() const {
    return m_pPrivate->m_indexId;
}

void VertexBuffered::initialize() {

    glGenVertexArrays(1, &m_pPrivate->m_bufferId);
    glGenBuffers(1, &m_pPrivate->m_vertexId);
    glGenBuffers(1, &m_pPrivate->m_normalId);
    glGenBuffers(1, &m_pPrivate->m_indexId);

    m_pPrivate->m_initialized = true;
}

bool VertexBuffered::initialized() const {
    return m_pPrivate->m_initialized;
}
