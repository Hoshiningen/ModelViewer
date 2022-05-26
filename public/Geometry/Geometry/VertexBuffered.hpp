#pragma once

#include "Geometry/VertexBuffer.hpp"

#include <list>
#include <memory>
#include <optional>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>

class VertexBuffered {
public:
    VertexBuffered();
    VertexBuffered(const std::vector<glm::vec3>& vertices,
                   const std::vector<glm::vec3>& normals,
                   const std::list<uint32_t>& indices);

    virtual ~VertexBuffered() noexcept;

    VertexBuffered(const VertexBuffered& other);
    VertexBuffered& operator=(const VertexBuffered& other);

    VertexBuffered(VertexBuffered&& other) noexcept;
    VertexBuffered& operator=(VertexBuffered&& other) noexcept;

    std::optional<std::list<uint32_t>> indices() const;
    std::optional<std::vector<glm::vec3>> vertices() const;
    std::optional<std::vector<glm::vec3>> normals() const;

    GLuint Id() const;
    GLuint normalBufferId() const;
    GLuint vertexBufferId() const;

    void initialize();

protected:
    VertexBuffer m_buffer;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
