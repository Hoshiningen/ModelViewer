#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

class VertexAttribute {
public:
    VertexAttribute(const std::string& name, GLint size, GLenum dataType, GLboolean normalized, std::size_t stride, std::size_t offset);

    virtual ~VertexAttribute();

    VertexAttribute(const VertexAttribute& other);
    VertexAttribute& operator=(const VertexAttribute& other);

    VertexAttribute(VertexAttribute&& other) noexcept;
    VertexAttribute& operator=(VertexAttribute&& other) noexcept;

    std::string name() const;
    GLint size() const;
    GLenum dataType() const;
    GLboolean normalized() const;
    GLsizei stride() const;
    void* offset() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
