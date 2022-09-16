#pragma once

#include "Common/ClassMacros.hpp"
#include "Texture/Texture.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>

class Framebuffer {
public:
    enum class AttachmentPoint : GLenum {
        Color0 = GL_COLOR_ATTACHMENT0,
        Depth = GL_DEPTH_ATTACHMENT,
        Stencil = GL_STENCIL_ATTACHMENT,
        DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
    };
    
    enum class Target : GLenum {
        Draw = GL_DRAW_FRAMEBUFFER,
        Read = GL_READ_FRAMEBUFFER,
        DrawRead = GL_FRAMEBUFFER
    };

    struct TextureAttachment {
        AttachmentPoint attachmentPoint = AttachmentPoint::Color0;
        Texture texture;
    };

    struct RenderbufferAttachment {
        AttachmentPoint attachmentPoint = AttachmentPoint::Depth;
        Texture::Channels internalFormat = Texture::Channels::Depth32;
        GLuint renderbufferId = 0;
    };

    Framebuffer(
        const glm::uvec2& dimensions,
        Target target,
        const TextureAttachment& textureAttachment,
        const RenderbufferAttachment& renderbufferAttachment
    );

    COPY_DISABLED(Framebuffer)

    bool createAttachments();

    bool complete() const;
    GLenum status() const;

    DECLARE_GETTER_IMMUTABLE_COPY(id, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(textureId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(bufferBitplane, GLbitfield)

    void destroy();

private:
    void attachTexture();
    void attachRenderbuffer() const;

    glm::uvec2 m_dimensions{ 0, 0 };
    Target m_target = Target::DrawRead;
    GLuint m_framebufferId = 0;
    TextureAttachment m_textureAttachment;
    RenderbufferAttachment m_renderbufferAttachment;
};
