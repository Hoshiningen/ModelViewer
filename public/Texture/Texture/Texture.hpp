#pragma once

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
    virtual ~Texture() noexcept;

    Texture(const Texture& other);
    Texture& operator=(const Texture& other);

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    unsigned int width() const;
    unsigned int height() const;
    Channels textureFormat() const;
    Channels pixelFormat() const;

    GLuint id() const;
    Target target() const;

    void minFilter(Filter value);
    Filter minFilter() const;

    void magFilter(Filter value);
    Filter magFilter() const;

    void wrap(Wrap value);

    void wrapS(Wrap value);
    Wrap wrapS() const;

    void wrapT(Wrap value);
    Wrap wrapT() const;

    void mipmap(bool value);
    bool mipmap() const;

    void borderColor(const glm::vec4& color);
    std::array<float, 4> borderColor() const;

    void initialize();
    bool initialized() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
