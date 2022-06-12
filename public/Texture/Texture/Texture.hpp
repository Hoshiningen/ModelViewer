#pragma once

#include <memory>

#include <glad/glad.h>

class Texture {
public:
    enum class Wrap {
        Repeat, ClampToEdge, ClampToBorder, MirroredRepeat
    };

    enum class Filter {
        Nearest, Linear
    };

    enum class Channels {
        R, RG, RGB, RGBA
    };

    Texture();
    Texture(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, unsigned char* pData);
    virtual ~Texture() noexcept;

    Texture(const Texture& other);
    Texture& operator=(const Texture& other);

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void width(unsigned int value);
    unsigned int width() const;

    void height(unsigned int value);
    unsigned int height() const;

    void textureFormat(Channels value);
    Channels textureFormat() const;

    void pixelFormat(Channels value);
    Channels pixelFormat() const;

    void data(unsigned char* pValue);
    unsigned char* data() const;

    void minFilter(Filter value);
    Filter minFilter() const;

    void magFilter(Filter value);
    Filter magFilter() const;

    void wrap(Wrap value);

    void wrapS(Wrap value);
    Wrap wrapS() const;

    void wrapT(Wrap value);
    Wrap wrapT() const;

    void mapmap(bool value);
    bool mipmap() const;

    GLuint id() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
