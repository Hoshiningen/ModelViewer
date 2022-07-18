#include "Geometry/VertexBuffer.hpp"

struct VertexBuffer::Private {
    std::vector<glm::vec4> m_colors;
    std::vector<uint32_t> m_indices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texels;
    std::vector<glm::vec3> m_vertices;
};


VertexBuffer::VertexBuffer()
    : m_pPrivate(std::make_unique<Private>()) {}

VertexBuffer::VertexBuffer(const VertexBuffer& other) {
    *this = other;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

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

void VertexBuffer::addTexel(const glm::vec2& texel) {
    m_pPrivate->m_texels.push_back(texel);
}

void VertexBuffer::addColor(const glm::vec4& color) {
    m_pPrivate->m_colors.push_back(color);
}

void VertexBuffer::addIndex(uint32_t index) {
    m_pPrivate->m_indices.push_back(index);
}

DEFINE_GETTER_CONST_CORRECT(VertexBuffer, colors, std::vector<glm::vec4>, m_pPrivate->m_colors)
DEFINE_GETTER_CONST_CORRECT(VertexBuffer, indices, std::vector<uint32_t>, m_pPrivate->m_indices)
DEFINE_GETTER_CONST_CORRECT(VertexBuffer, normals, std::vector<glm::vec3>, m_pPrivate->m_normals)
DEFINE_GETTER_CONST_CORRECT(VertexBuffer, texels, std::vector<glm::vec2>, m_pPrivate->m_texels)
DEFINE_GETTER_CONST_CORRECT(VertexBuffer, vertices, std::vector<glm::vec3>, m_pPrivate->m_vertices)
