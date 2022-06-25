#include "Geometry/VertexBuffered.hpp"

struct VertexBuffered::Private {
    GLuint m_bufferId = 0; // Named vertex array object.
    GLuint m_colorId = 0; // Named buffer object for color data.
    GLuint m_indexId = 0; // Named element buffer object.
    GLuint m_normalId = 0; // Named buffer object for normal data.
    GLuint m_texelId = 0; // Named buffer object for texel data.
    GLuint m_vertexId = 0; // Named buffer object for vertex data.

    bool m_initialized = false; // Flag indicating if buffer objects were created
    PrimativeType m_primativeType = PrimativeType::Triangles; // Rendering primative used in drawing.
};


VertexBuffered::VertexBuffered()
    : m_pPrivate(std::make_unique<Private>()) {}

VertexBuffered::VertexBuffered(const VertexBuffer& buffer)
    : m_buffer(buffer), m_pPrivate(std::make_unique<Private>()) {}

VertexBuffered::~VertexBuffered() noexcept {

    if (m_pPrivate->m_bufferId)
        glDeleteVertexArrays(1, &m_pPrivate->m_bufferId);

    if (m_pPrivate->m_colorId)
        glDeleteBuffers(1, &m_pPrivate->m_colorId);

    if (m_pPrivate->m_indexId)
        glDeleteBuffers(1, &m_pPrivate->m_indexId);
    
    if (m_pPrivate->m_normalId)
        glDeleteBuffers(1, &m_pPrivate->m_normalId);
    
    if (m_pPrivate->m_texelId)
        glDeleteBuffers(1, &m_pPrivate->m_texelId);
        
    if (m_pPrivate->m_vertexId)
        glDeleteBuffers(1, &m_pPrivate->m_vertexId);
}

VertexBuffered::VertexBuffered(const VertexBuffered& other) {
    *this = other;
}

VertexBuffered& VertexBuffered::operator=(const VertexBuffered& other) {

    if (this != &other) {
        m_pPrivate = std::make_unique<Private>();
        m_buffer = other.m_buffer;
    }

    return *this;
}

VertexBuffered::VertexBuffered(VertexBuffered&& other) noexcept {
    *this = std::move(other);
}

VertexBuffered& VertexBuffered::operator=(VertexBuffered&& other) noexcept {

    if (this != &other) {
        m_pPrivate = std::make_unique<Private>();
        m_buffer = std::move(other.m_buffer);
    }

    return *this;
}

std::optional<std::vector<glm::vec4>> VertexBuffered::colors() const {

    if (m_buffer.colors().empty())
        return std::nullopt;

    return m_buffer.colors();
}

std::optional<std::vector<uint32_t>> VertexBuffered::indices() const {

    if (m_buffer.indices().empty())
        return std::nullopt;

    return m_buffer.indices();
}

std::optional<std::vector<glm::vec3>> VertexBuffered::normals() const {

    if (m_buffer.normals().empty())
        return std::nullopt;

    return m_buffer.normals();
}

std::optional<std::vector<glm::vec2>> VertexBuffered::texels() const {

    if (m_buffer.texels().empty())
        return std::nullopt;

    return m_buffer.texels();
}

std::optional<std::vector<glm::vec3>> VertexBuffered::vertices() const {

    if (m_buffer.vertices().empty())
        return std::nullopt;

    return m_buffer.vertices();
}

GLuint VertexBuffered::id() const {
    return m_pPrivate->m_bufferId;
}

GLuint VertexBuffered::colorBufferId() const {
    return m_pPrivate->m_colorId;
}

GLuint VertexBuffered::indexBufferId() const {
    return m_pPrivate->m_indexId;
}

GLuint VertexBuffered::normalBufferId() const {
    return m_pPrivate->m_normalId;
}

GLuint VertexBuffered::texelBufferId() const {
    return m_pPrivate->m_texelId;
}

GLuint VertexBuffered::vertexBufferId() const {
    return m_pPrivate->m_vertexId;
}

std::optional<GLuint> VertexBuffered::attributeBufferId(const std::string& name) const {

    if (name == "color" && m_pPrivate->m_colorId != 0)
        return m_pPrivate->m_colorId;

    if (name == "normal" && m_pPrivate->m_normalId != 0)
        return m_pPrivate->m_normalId;

    if (name == "texel" && m_pPrivate->m_texelId != 0)
        return m_pPrivate->m_texelId;

    if (name == "position" && m_pPrivate->m_vertexId != 0)
        return m_pPrivate->m_vertexId;

    return std::nullopt;
}

void VertexBuffered::primativeType(PrimativeType type) {
    m_pPrivate->m_primativeType = type;
}

VertexBuffered::PrimativeType VertexBuffered::primativeType() const {
    return m_pPrivate->m_primativeType;
}

void VertexBuffered::initialize() {

    if (!m_pPrivate->m_bufferId)
        glGenVertexArrays(1, &m_pPrivate->m_bufferId);

    if (!m_pPrivate->m_colorId && !m_buffer.colors().empty())
        glGenBuffers(1, &m_pPrivate->m_colorId);

    if (!m_pPrivate->m_indexId && !m_buffer.indices().empty())
        glGenBuffers(1, &m_pPrivate->m_indexId);

    if (!m_pPrivate->m_normalId && !m_buffer.normals().empty())
        glGenBuffers(1, &m_pPrivate->m_normalId);

    if (!m_pPrivate->m_texelId && !m_buffer.texels().empty())
        glGenBuffers(1, &m_pPrivate->m_texelId);

    if (!m_pPrivate->m_vertexId && !m_buffer.vertices().empty())
        glGenBuffers(1, &m_pPrivate->m_vertexId);

    m_pPrivate->m_initialized = true;
}

bool VertexBuffered::initialized() const {
    return m_pPrivate->m_initialized;
}

void VertexBuffered::color(const glm::vec4& color) {

    m_buffer.colors().clear();
    m_buffer.colors().resize(m_buffer.vertices().size(), color);
}
