#pragma once

#include "Common/ClassMacros.hpp"

#include "Geometry/VertexBuffered.hpp"

#include <glm/vec3.hpp>

class Point : public VertexBuffered {
public:
    explicit Point(const glm::vec3& position);

    virtual ~Point() noexcept = default;

    COPY_MOVE_ENABLED(Point)

    DECLARE_GETTER_IMMUTABLE(position, glm::vec3)
};
