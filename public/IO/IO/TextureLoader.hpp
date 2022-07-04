#pragma once

#include "Texture/Texture.hpp"

#include <filesystem>

#include <glad/glad.h>

class TextureLoader {
public:
    static Texture load(const std::filesystem::path& path, Texture::Target target);
    static Texture load(const std::filesystem::path& path, Texture::Target target, bool flipUVs);
};
