#pragma once

#include "Geometry/VertexBuffer.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBuffered {
public:
    VertexBuffered();
    VertexBuffered(const VertexBuffer& buffer);

    virtual ~VertexBuffered() noexcept;

    VertexBuffered(const VertexBuffered& other);
    VertexBuffered& operator=(const VertexBuffered& other);

    VertexBuffered(VertexBuffered&& other) noexcept;
    VertexBuffered& operator=(VertexBuffered&& other) noexcept;

    enum class PrimativeType {
        Point = GL_POINTS,
        Lines = GL_LINES,
        LineLoop = GL_LINE_LOOP,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN
    };

    std::optional<std::vector<glm::vec4>> colors() const;
    std::optional<std::vector<uint32_t>> indices() const;
    std::optional<std::vector<glm::vec3>> normals() const;
    std::optional<std::vector<glm::vec2>> texels() const;
    std::optional<std::vector<glm::vec3>> vertices() const;

    GLuint id() const;
    GLuint colorBufferId() const;
    GLuint indexBufferId() const;
    GLuint normalBufferId() const;
    GLuint texelBufferId() const;
    GLuint vertexBufferId() const;

    std::optional<GLuint> attributeBufferId(const std::string& name) const;

    void primativeType(PrimativeType type);
    PrimativeType primativeType() const;

    void initialize();
    bool initialized() const;

    void color(const glm::vec4& color);

protected:
    VertexBuffer m_buffer;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
