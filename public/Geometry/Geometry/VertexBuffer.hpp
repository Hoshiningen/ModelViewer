#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBuffer {
public:
    VertexBuffer();
    virtual ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer& other);
    VertexBuffer& operator=(const VertexBuffer& other);

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void addColor(const glm::vec4& color);
    void addIndex(uint32_t index);
    void addNormal(const glm::vec3& normal);
    void addTexel(const glm::vec2& texel);
    void addVertex(const glm::vec3& vertex);

    const std::vector<glm::vec4>& colors() const;
    std::vector<glm::vec4>& colors();

    const std::vector<uint32_t>& indices() const;
    std::vector<uint32_t>& indices();

    const std::vector<glm::vec3>& normals() const;
    std::vector<glm::vec3>& normals();

    const std::vector<glm::vec2>& texels() const;
    std::vector<glm::vec2>& texels();

    const std::vector<glm::vec3>& vertices() const;
    std::vector<glm::vec3>& vertices();
    
private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
