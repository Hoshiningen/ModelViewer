#include "Texture/Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

struct Texture::Private {
    Private() = default;
    Private(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, Target target);

    unsigned int m_width = 0;
    unsigned int m_height = 0;

    Channels m_textureFormat = Channels::RGB; // Internal texture format
    Channels m_pixelFormat = Channels::RGB; // Pixel data format

    Filter m_minFilter = Filter::Linear;
    Filter m_magFilter = Filter::Linear;

    Wrap m_wrapS = Wrap::Repeat;
    Wrap m_wrapT = Wrap::Repeat;

    bool m_mipmap = false;
    bool m_initialized = false;

    Target m_target = Target::Texture2D;

    std::array<float, 4> m_borderColor{ 0.f, 0.f, 0.f, 0.f };

    GLuint m_id = 0; // Texture buffer id
};

Texture::Private::Private(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, Target target)
    : m_width(width), m_height(height), m_textureFormat(textureFormat), m_pixelFormat(pixelFormat), m_target(target) {}


Texture::Texture()
    : m_pPrivate(std::make_unique<Private>()) {}

Texture::~Texture() noexcept {}

Texture::Texture(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, Target target)
    : m_pPrivate(std::make_unique<Private>(width, height, textureFormat, pixelFormat, target)) {}

Texture::Texture(const Texture& other) {
    *this = other;
}

Texture& Texture::operator=(const Texture& other) {

    if (this != &other) {
        destroy();
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);
    }

    return *this;
}

Texture::Texture(Texture&& other) noexcept {
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept {
    
    if (this != &other) {
        destroy();
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);
    }

    return *this;
}

DEFINE_GETTER_IMMUTABLE(Texture, width, unsigned int, m_pPrivate->m_width)
DEFINE_GETTER_IMMUTABLE(Texture, height, unsigned int, m_pPrivate->m_height)
DEFINE_GETTER_IMMUTABLE(Texture, textureFormat, Texture::Channels, m_pPrivate->m_textureFormat)
DEFINE_GETTER_IMMUTABLE(Texture, pixelFormat, Texture::Channels, m_pPrivate->m_pixelFormat)

DEFINE_GETTER_IMMUTABLE_COPY(Texture, id, GLuint, m_pPrivate->m_id)
DEFINE_GETTER_IMMUTABLE_COPY(Texture, target, Texture::Target, m_pPrivate->m_target)

DEFINE_GETTER_IMMUTABLE_COPY(Texture, minFilter, Texture::Filter, m_pPrivate->m_minFilter)
DEFINE_SETTER_COPY(Texture, minFilter, m_pPrivate->m_minFilter)

DEFINE_GETTER_IMMUTABLE_COPY(Texture, magFilter, Texture::Filter, m_pPrivate->m_magFilter)
DEFINE_SETTER_COPY(Texture, magFilter, m_pPrivate->m_magFilter)

void Texture::wrap(Wrap value) {
    m_pPrivate->m_wrapS = value;
    m_pPrivate->m_wrapT = value;
}

DEFINE_GETTER_IMMUTABLE_COPY(Texture, wrapS, Texture::Wrap, m_pPrivate->m_wrapS)
DEFINE_SETTER_COPY(Texture, wrapS, m_pPrivate->m_wrapS)

DEFINE_GETTER_IMMUTABLE_COPY(Texture, wrapT, Texture::Wrap, m_pPrivate->m_wrapT)
DEFINE_SETTER_COPY(Texture, wrapT, m_pPrivate->m_wrapT)

DEFINE_GETTER_IMMUTABLE_COPY(Texture, mipmap, bool, m_pPrivate->m_mipmap)
DEFINE_SETTER_COPY(Texture, wrapT, m_pPrivate->m_mipmap)

void Texture::borderColor(const glm::vec4& color) {
    m_pPrivate->m_borderColor = { color.r, color.g, color.b, color.a };
}

std::array<float, 4> Texture::borderColor() const {
    return m_pPrivate->m_borderColor;
}

void Texture::initialize() {
    glGenTextures(1, &m_pPrivate->m_id);
    m_pPrivate->m_initialized = true;
}

DEFINE_GETTER_IMMUTABLE_COPY(Texture, initialized, bool, m_pPrivate->m_initialized)

void Texture::destroy() const {

    if (m_pPrivate && m_pPrivate->m_initialized)
        glDeleteTextures(1, &m_pPrivate->m_id);
}
