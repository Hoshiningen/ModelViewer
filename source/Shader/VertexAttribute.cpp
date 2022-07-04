#include "Shader/VertexAttribute.hpp"

#include <compare>

struct VertexAttribute::Private {
    Private(const std::string& name, GLint size, GLenum dataType, GLboolean normalized, std::size_t stride, std::size_t offset);

    std::string m_name;
    GLenum m_dataType = 0;
    GLint m_size = 0;
    GLboolean m_normalized = GL_FALSE;
    std::size_t m_stride = 0;
    std::size_t m_offset = 0;
};

VertexAttribute::Private::Private(const std::string& name, GLint size, GLenum dataType, GLboolean normalized, std::size_t stride, std::size_t offset)
    : m_name(name), m_size(size), m_dataType(dataType), m_normalized(normalized), m_stride(stride), m_offset(offset) {}


VertexAttribute::VertexAttribute(const std::string& name, GLint size, GLenum dataType, GLboolean normalized, std::size_t stride, std::size_t offset)
    : m_pPrivate(std::make_unique<Private>(name, size, dataType, normalized, stride, offset)) {}

VertexAttribute::~VertexAttribute() {}

VertexAttribute::VertexAttribute(const VertexAttribute& other) {
    *this = other;
}

VertexAttribute& VertexAttribute::operator=(const VertexAttribute& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

VertexAttribute::VertexAttribute(VertexAttribute&& other) noexcept {
    *this = std::move(other);
}

VertexAttribute& VertexAttribute::operator=(VertexAttribute&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

GLenum VertexAttribute::dataType() const {
    return m_pPrivate->m_dataType;
}

std::string VertexAttribute::name() const {
    return m_pPrivate->m_name;
}

GLint VertexAttribute::size() const {
    return m_pPrivate->m_size;
}

GLboolean VertexAttribute::normalized() const {
    return m_pPrivate->m_normalized ? GL_TRUE : GL_FALSE;
}

GLsizei VertexAttribute::stride() const {
    return static_cast<GLsizei>(m_pPrivate->m_stride);
}

void* VertexAttribute::offset() const {
    return reinterpret_cast<void*>(m_pPrivate->m_offset);
}
