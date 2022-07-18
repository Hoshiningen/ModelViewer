#pragma once

#include "VertexBuffered.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/vec3.hpp>

class Plane : public VertexBuffered {
public:
    Plane(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur);

    DECLARE_GETTER_IMMUTABLE_COPY(normal, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(width, float)
    DECLARE_GETTER_IMMUTABLE_COPY(height, float)

private:
    COMPILATION_FIREWALL_COPY_MOVE(Plane)
};