#pragma once

#include "Geometry/VertexBuffered.hpp"

#include <glm/vec3.hpp>

class Point : public VertexBuffered {
public:
    explicit Point(const glm::vec3& position);

    virtual ~Point() noexcept = default;

    Point(const Point& other);
    Point& operator=(const Point& other);

    Point(Point&& other) noexcept;
    Point& operator=(Point&& other) noexcept;

    glm::vec3 position() const;
};
