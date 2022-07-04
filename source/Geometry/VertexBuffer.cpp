#include "Geometry/VertexBuffer.hpp"

struct VertexBuffer::Private {
    //Private() = default;

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

const std::vector<glm::vec2>& VertexBuffer::texels() const {
    return m_pPrivate->m_texels;
}

std::vector<glm::vec2>& VertexBuffer::texels() {
    return m_pPrivate->m_texels;
}

const std::vector<glm::vec4>& VertexBuffer::colors() const {
    return m_pPrivate->m_colors;
}

std::vector<glm::vec4>& VertexBuffer::colors() {
    return m_pPrivate->m_colors;
}
