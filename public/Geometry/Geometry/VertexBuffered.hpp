#pragma once

#include "Geometry/VertexBuffer.hpp"

#include <forward_list>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

class VertexBuffered {
public:
    VertexBuffered() = default;
    VertexBuffered(const std::vector<glm::vec3>& vertices,
                   const std::vector<glm::vec3>& normals,
                   const std::forward_list<uint32_t>& indices);

    virtual ~VertexBuffered() noexcept = default;

    VertexBuffered(const VertexBuffered& other);
    VertexBuffered& operator=(const VertexBuffered& other);

    VertexBuffered(VertexBuffered&& other) noexcept;
    VertexBuffered& operator=(VertexBuffered&& other) noexcept;

    std::optional<std::forward_list<uint32_t>> indices() const;
    std::optional<std::vector<glm::vec3>> vertices() const;
    std::optional<std::vector<glm::vec3>> normals() const;

protected:
    VertexBuffer m_buffer;
};
