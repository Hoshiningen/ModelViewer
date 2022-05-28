#pragma once

#include "Geometry/VertexBuffered.hpp"

#include <filesystem>
#include <memory>
#include <forward_list>

class GeometryLoader {
public:
    GeometryLoader();
    virtual ~GeometryLoader();

    GeometryLoader(const GeometryLoader& other);
    GeometryLoader& operator=(const GeometryLoader& other);

    GeometryLoader(GeometryLoader&& other) noexcept;
    GeometryLoader& operator=(GeometryLoader&& other) noexcept;

    std::forward_list<VertexBuffered> loadGeometry(const std::filesystem::path& path) const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};