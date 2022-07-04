#include "IO/TextureLoader.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture TextureLoader::load(const std::filesystem::path& path, Texture::Target target) {
    return load(path, target, false);
}

Texture TextureLoader::load(const std::filesystem::path& path, Texture::Target target, bool flipUVs) {

    Texture texture;
    if (!std::filesystem::is_regular_file(path))
        return texture;

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(flipUVs);
    stbi_uc* pData = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_default);

    if (pData) {

        const Texture::Channels format = [&channels] {
            switch (channels) {
            case 1: return Texture::Channels::R;
            case 2: return Texture::Channels::RG;
            case 3: return Texture::Channels::RGB;
            case 4: return Texture::Channels::RGBA;
            default: return Texture::Channels::RGB;
            }
        }();
        
        texture = Texture{ static_cast<GLuint>(width), static_cast<GLuint>(height), format, format, target };
        texture.initialize();

        glBindTexture(static_cast<GLenum>(target), texture.id());
        glTexImage2D(
            static_cast<GLenum>(target),                    // Target
            0,                                              // Level
            static_cast<GLint>(texture.textureFormat()),    // internal format
            texture.width(),                                // width
            texture.height(),                               // height
            0,                                              // border
            static_cast<GLint>(texture.pixelFormat()),      // format
            GL_UNSIGNED_BYTE,                               // type
            pData                                           // data
        );

        glBindTexture(static_cast<GLenum>(target), 0);
        stbi_image_free(pData);
    }

    return texture;
}
