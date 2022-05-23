#pragma once

#include "Geometry/VertexBuffered.hpp"

#include <memory>

#include <glm/vec3.hpp>

class Box : public VertexBuffered {
public:
    Box();
    Box(float width, float height, float length);
    
    virtual ~Box() noexcept;

    Box(const Box& other);
    Box& operator=(const Box& other);

    Box(Box&& other) noexcept;
    Box& operator=(Box&& other) noexcept;

    void position(const glm::vec3& value);
    glm::vec3 position() const;

    void width(float value);
    float width() const;

    void height(float value);
    float height() const;
    
    void length(float value);
    float length() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
