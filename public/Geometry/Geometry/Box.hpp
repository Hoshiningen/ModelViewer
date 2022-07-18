#pragma once

#include "Common/ClassMacros.hpp"

#include "Geometry/VertexBuffered.hpp"

class Box : public VertexBuffered {
public:
    Box();
    Box(float width, float height, float length);

    DECLARE_GETTER_IMMUTABLE_COPY(width, float)
    DECLARE_GETTER_IMMUTABLE_COPY(height, float)
    DECLARE_GETTER_IMMUTABLE_COPY(length, float)

private:
    COMPILATION_FIREWALL_COPY_MOVE(Box)
};
