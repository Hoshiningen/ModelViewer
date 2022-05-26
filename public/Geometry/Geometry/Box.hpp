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

    float width() const;
    float height() const;
    float length() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
