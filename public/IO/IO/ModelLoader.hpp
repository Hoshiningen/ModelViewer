#pragma once

#include "Common/ClassMacros.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Texture/Texture.hpp"

#include <filesystem>
#include <forward_list>
#include <string>
#include <unordered_map>
#include <vector>

class ModelLoader {
public:
    static std::vector<std::string> SupportedExtensions();

    ModelLoader();

    struct ModelProperties {
        std::forward_list<VertexBuffered> meshes;
        std::unordered_multimap<Texture::Type, std::filesystem::path> texturePaths;
    };

    ModelProperties load(const std::filesystem::path& path) const;

private:
    COMPILATION_FIREWALL_COPY_MOVE(ModelLoader)
};