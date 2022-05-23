#pragma once

#include "Geometry/VertexBuffered.hpp"

#include <glm/vec3.hpp>

class Line : public VertexBuffered {
public:
    Line(const glm::vec3& start, const glm::vec3& end);

    virtual ~Line() noexcept = default;

    Line(const Line& other);
    Line& operator=(const Line& other);

    Line(Line&& other) noexcept;
    Line& operator=(Line&& other) noexcept;

    glm::vec3 start() const;
    glm::vec3 end() const;
};
