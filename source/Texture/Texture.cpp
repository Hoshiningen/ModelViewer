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

    bool m_needsMipmap = true;
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

void Texture::minFilter(Filter value) {
    m_pPrivate->m_minFilter = value;
}

unsigned int Texture::width() const {
    return m_pPrivate->m_width;
}

unsigned int Texture::height() const {
    return m_pPrivate->m_height;
}

Texture::Channels Texture::textureFormat() const {
    return m_pPrivate->m_textureFormat;
}

Texture::Channels Texture::pixelFormat() const {
    return m_pPrivate->m_pixelFormat;
}

GLuint Texture::id() const {
    return m_pPrivate->m_id;
}

Texture::Target Texture::target() const {
    return m_pPrivate->m_target;
}

Texture::Filter Texture::minFilter() const {
    return m_pPrivate->m_minFilter;
}

void Texture::magFilter(Filter value) {
    m_pPrivate->m_magFilter = value;
}

Texture::Filter Texture::magFilter() const {
    return m_pPrivate->m_magFilter;
}

void Texture::wrap(Wrap value) {
    m_pPrivate->m_wrapS = value;
    m_pPrivate->m_wrapT = value;
}

void Texture::wrapS(Wrap value) {
    m_pPrivate->m_wrapS = value;
}

Texture::Wrap Texture::wrapS() const {
    return m_pPrivate->m_wrapS;
}

void Texture::wrapT(Wrap value) {
    m_pPrivate->m_wrapT = value;
}

Texture::Wrap Texture::wrapT() const {
    return m_pPrivate->m_wrapT;
}

void Texture::mipmap(bool value) {
    m_pPrivate->m_needsMipmap = value;
}

bool Texture::mipmap() const {
    return m_pPrivate->m_needsMipmap;
}

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

bool Texture::initialized() const {
    return m_pPrivate->m_initialized;
}

void Texture::destroy() const {

    if (m_pPrivate && m_pPrivate->m_initialized)
        glDeleteTextures(1, &m_pPrivate->m_id);
}
