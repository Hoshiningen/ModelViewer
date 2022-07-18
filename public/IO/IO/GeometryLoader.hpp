#pragma once

#include "Common/ClassMacros.hpp"

#include "Geometry/VertexBuffered.hpp"

#include <filesystem>
#include <forward_list>

class GeometryLoader {
public:
    GeometryLoader();

    std::forward_list<VertexBuffered> load(const std::filesystem::path& path) const;

private:
    COMPILATION_FIREWALL_COPY_MOVE(GeometryLoader)
};