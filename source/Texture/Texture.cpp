#include "Texture/Texture.hpp"

#include <stb_image.h>

struct Texture::Private {
    Private() = default;
    Private(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, unsigned char* pData);

    Private(const Private& other);
    Private& operator=(const Private& other);

    unsigned int m_width = 0;
    unsigned int m_height = 0;

    Channels m_textureFormat = Channels::RGB; // Internal texture format
    Channels m_pixelFormat = Channels::RGB; // Pixel data format

    Filter m_minFilter = Filter::Linear;
    Filter m_magFilter = Filter::Linear;

    Wrap m_wrapS = Wrap::Repeat;
    Wrap m_wrapT = Wrap::Repeat;

    stbi_uc* m_pData = nullptr; // Image data

    bool m_needsMipmap = true;

    GLuint m_id = 0; // Texture buffer id
};

Texture::Private::Private(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, unsigned char* pData)
    : m_width(width), m_height(height), m_textureFormat(textureFormat), m_pixelFormat(pixelFormat), m_pData(pData) {}

Texture::Private::Private(const Private& other) {
    *this = other;
}

Texture::Private& Texture::Private::operator=(const Private& other) {
    
    if (this != &other) {

        m_width = other.m_width;
        m_height = other.m_height;
        m_textureFormat = other.m_textureFormat;
        m_pixelFormat = other.m_pixelFormat;
        m_minFilter = other.m_minFilter;
        m_magFilter = other.m_magFilter;
        m_wrapS = other.m_wrapS;
        m_wrapT = other.m_wrapT;
    }

    return *this;
}


Texture::Texture()
    : m_pPrivate(std::make_unique<Private>()) {}

Texture::~Texture() noexcept {}

Texture::Texture(unsigned int width, unsigned int height, Channels textureFormat, Channels pixelFormat, unsigned char* pData)
    : m_pPrivate(std::make_unique<Private>(width, height, textureFormat, pixelFormat, pData)) {
}

Texture::Texture(const Texture& other) {
    *this = other;
}

Texture& Texture::operator=(const Texture& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

Texture::Texture(Texture&& other) noexcept {
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept {
    
    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void Texture::minFilter(Filter value) {
    m_pPrivate->m_minFilter = value;
}

void Texture::width(unsigned int value) {
    m_pPrivate->m_width = value;
}

unsigned int Texture::width() const {
    return m_pPrivate->m_width;
}

void Texture::height(unsigned int value) {
    m_pPrivate->m_height = value;
}

unsigned int Texture::height() const {
    return m_pPrivate->m_height;
}

void Texture::textureFormat(Channels value) {
    m_pPrivate->m_textureFormat = value;
}

Texture::Channels Texture::textureFormat() const {
    return m_pPrivate->m_textureFormat;
}

void Texture::pixelFormat(Channels value) {
    m_pPrivate->m_pixelFormat = value;
}

Texture::Channels Texture::pixelFormat() const {
    return m_pPrivate->m_pixelFormat;
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

void Texture::mapmap(bool value) {
    m_pPrivate->m_needsMipmap = value;
}

bool Texture::mipmap() const {
    return m_pPrivate->m_needsMipmap;
}

void Texture::data(unsigned char* pValue) {
    m_pPrivate->m_pData = pValue;
}

unsigned char* Texture::data() const {
    return m_pPrivate->m_pData;
}

GLuint Texture::id() const {
    return m_pPrivate->m_id;
}
