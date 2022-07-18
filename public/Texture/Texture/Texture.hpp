#pragma once

#include "Common/ClassMacros.hpp"

#include <array>
#include <memory>

#include <glad/glad.h>
#include <glm/vec4.hpp>

class Texture {
public:
    enum class Wrap : GLenum {
        Repeat = GL_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT
    };

    enum class Filter {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class Channels : GLint {
        R = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    enum class Target : GLenum {
        Texture2D = GL_TEXTURE_2D
    };

    Texture();
    Texture(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, Target target);

    DECLARE_GETTER_IMMUTABLE(width, unsigned int)
    DECLARE_GETTER_IMMUTABLE(height, unsigned int)
    DECLARE_GETTER_IMMUTABLE(textureFormat, Channels)
    DECLARE_GETTER_IMMUTABLE(pixelFormat, Channels)

    DECLARE_GETTER_IMMUTABLE(id, GLuint)
    DECLARE_GETTER_IMMUTABLE(target, Target)

    DECLARE_GETTER_IMMUTABLE(minFilter, Filter)
    DECLARE_SETTER_COPY(minFilter, Filter)

    DECLARE_GETTER_IMMUTABLE(magFilter, Filter)
    DECLARE_SETTER_COPY(magFilter, Filter)

    DECLARE_SETTER_COPY(wrap, Wrap)

    DECLARE_GETTER_IMMUTABLE(wrapS, Wrap)
    DECLARE_SETTER_COPY(wrapS, Wrap)

    DECLARE_GETTER_IMMUTABLE(wrapT, Wrap)
    DECLARE_SETTER_COPY(wrapT, Wrap)

    DECLARE_GETTER_IMMUTABLE(mipmap, bool)
    DECLARE_SETTER_COPY(wrapT, bool)

    std::array<float, 4> borderColor() const;
    DECLARE_SETTER_CONSTREF(borderColor, glm::vec4)

    void initialize();
    DECLARE_GETTER_IMMUTABLE(initialized, bool)

    void destroy() const;

private:
    COMPILATION_FIREWALL_COPY_MOVE(Texture)
};
