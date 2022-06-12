#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer(const std::vector<glm::vec3>& vertices,
                 const std::vector<glm::vec3>& normals,
                 const std::vector<glm::vec4>& colors,
                 const std::vector<uint32_t>& indices);

    virtual ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer& other);
    VertexBuffer& operator=(const VertexBuffer& other);

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void addVertex(const glm::vec3& vertex);
    void addNormal(const glm::vec3& normal);
    void addColor(const glm::vec4& color);
    void addIndex(uint32_t index);

    const std::vector<uint32_t>& indices() const;
    std::vector<uint32_t>& indices();

    const std::vector<glm::vec3>& vertices() const;
    std::vector<glm::vec3>& vertices();
    
    const std::vector<glm::vec3>& normals() const;
    std::vector<glm::vec3>& normals();

    const std::vector<glm::vec4>& colors() const;
    std::vector<glm::vec4>& colors();

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
