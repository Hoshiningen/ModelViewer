#include "IO/TextureLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture TextureLoader::load(const std::filesystem::path& path) const {

    Texture texture;
    if (!std::filesystem::is_regular_file(path))
        return texture;

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* pData = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_default);

    if (pData != nullptr) {

        texture.data(pData);
        texture.width(width);
        texture.height(height);

        const Texture::Channels format = [&channels] {
            switch (channels) {
            case 1: return Texture::Channels::R;
            case 2: return Texture::Channels::RG;
            case 3: return Texture::Channels::RGB;
            case 4: return Texture::Channels::RGBA;
            default: return Texture::Channels::RGB;
            }
        }();

        texture.pixelFormat(format);
        texture.textureFormat(format);
    }

    return texture;
}
