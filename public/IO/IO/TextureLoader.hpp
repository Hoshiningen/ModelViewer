#pragma once

#include "Texture/Texture.hpp"

#include <filesystem>

class TextureLoader {
public:
    Texture load(const std::filesystem::path& path) const;
};
