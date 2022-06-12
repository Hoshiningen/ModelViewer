#pragma once

#include "Geometry/VertexBuffer.hpp"

#include <memory>
#include <optional>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBuffered {
public:
    VertexBuffered();
    VertexBuffered(const VertexBuffer& buffer);
    VertexBuffered(const std::vector<glm::vec3>& vertices,
                   const std::vector<glm::vec3>& normals,
                   const std::vector<glm::vec4>& colors,
                   const std::vector<uint32_t>& indices);

    virtual ~VertexBuffered() noexcept;

    VertexBuffered(const VertexBuffered& other);
    VertexBuffered& operator=(const VertexBuffered& other);

    VertexBuffered(VertexBuffered&& other) noexcept;
    VertexBuffered& operator=(VertexBuffered&& other) noexcept;

    std::optional<std::vector<uint32_t>> indices() const;
    std::optional<std::vector<glm::vec3>> vertices() const;
    std::optional<std::vector<glm::vec3>> normals() const;
    std::optional<std::vector<glm::vec4>> colors() const;

    GLuint Id() const;
    GLuint normalBufferId() const;
    GLuint vertexBufferId() const;
    GLuint colorBufferId() const;
    GLuint indexBufferId() const;

    void initialize();
    bool initialized() const;

    void color(const glm::vec4& color);

protected:
    VertexBuffer m_buffer;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
