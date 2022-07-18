#pragma once

#include "Common/ClassMacros.hpp"

#include "Geometry/VertexBuffered.hpp"

#include <glm/vec3.hpp>

class Line : public VertexBuffered {
public:
    Line(const glm::vec3& start, const glm::vec3& end);

    virtual ~Line() noexcept = default;

    COPY_MOVE_ENABLED(Line)

    DECLARE_GETTER_IMMUTABLE_COPY(start, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(end, glm::vec3)
};
