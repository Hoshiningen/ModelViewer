#pragma once

#include "Common/ClassMacros.hpp"

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

    enum class PrimativeType {
        Point = GL_POINTS,
        Lines = GL_LINES,
        LineLoop = GL_LINE_LOOP,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN
    };

    DECLARE_GETTER_IMMUTABLE_COPY(colors, std::optional<std::vector<glm::vec4>>)
    DECLARE_GETTER_IMMUTABLE_COPY(indices, std::optional<std::vector<uint32_t>>)
    DECLARE_GETTER_IMMUTABLE_COPY(normals, std::optional<std::vector<glm::vec3>>)
    DECLARE_GETTER_IMMUTABLE_COPY(texels, std::optional<std::vector<glm::vec2>>)
    DECLARE_GETTER_IMMUTABLE_COPY(vertices, std::optional<std::vector<glm::vec3>>)

    DECLARE_GETTER_IMMUTABLE_COPY(id, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(colorBufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(indexBufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(normalBufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(texelBufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(vertexBufferId, GLuint)

    std::optional<GLuint> attributeBufferId(const std::string& name) const;

    DECLARE_GETTER_IMMUTABLE_COPY(primativeType, PrimativeType)
    DECLARE_SETTER_COPY(primativeType, PrimativeType)

    void initialize();
    DECLARE_GETTER_IMMUTABLE_COPY(initialized, bool)

    DECLARE_SETTER_CONSTREF(color, glm::vec4)

protected:
    VertexBuffer m_buffer;

private:
    COMPILATION_FIREWALL_COPY_MOVE(VertexBuffered)
};
