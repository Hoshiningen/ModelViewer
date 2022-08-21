#include "FrameBuffer.hpp"

#include "Renderer/Renderer.hpp"

Framebuffer::Framebuffer(
    const glm::uvec2& dimensions,
    Target target,
    const TextureAttachment& textureAttachment,
    const RenderbufferAttachment& renderbufferAttachment
)
    : m_dimensions(dimensions), m_target(target) {

    glGenFramebuffers(1, &m_framebufferId);
}

bool Framebuffer::createAttachments() {

    attachTexture();
    attachRenderbuffer();

    return complete();
}

bool Framebuffer::complete() const {
    return status() == GL_FRAMEBUFFER_COMPLETE;
}

GLenum Framebuffer::status() const {
    return glCheckFramebufferStatus(static_cast<GLenum>(m_target));
}

DEFINE_GETTER_IMMUTABLE_COPY(Framebuffer, id, GLuint, m_framebufferId)
DEFINE_GETTER_IMMUTABLE_COPY(Framebuffer, textureId, GLuint, m_textureAttachment.texture.id())

GLbitfield Framebuffer::bufferBitplane() const {

    GLbitfield buffers = GL_COLOR_BUFFER_BIT;

    if (m_renderbufferAttachment.attachmentPoint == AttachmentPoint::Depth)
        buffers |= GL_DEPTH_BUFFER_BIT;

    if (m_renderbufferAttachment.attachmentPoint == AttachmentPoint::Stencil)
        buffers |= GL_STENCIL_BUFFER_BIT;

    if (m_renderbufferAttachment.attachmentPoint == AttachmentPoint::DepthStencil)
        buffers |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

    return buffers;
}

void Framebuffer::destroy() {

    glDeleteFramebuffers(1, &m_framebufferId);
    glDeleteRenderbuffers(1, &m_renderbufferAttachment.renderbufferId);
    m_textureAttachment.texture.destroy();
}

void Framebuffer::attachTexture() {

    Renderer::Allocate(m_textureAttachment.texture, nullptr);
    Renderer::Create(m_textureAttachment.texture);

    glBindFramebuffer(static_cast<GLenum>(m_target), m_framebufferId);

    glFramebufferTexture2D(
        static_cast<GLenum>(m_target),
        static_cast<GLenum>(m_textureAttachment.attachmentPoint),
        static_cast<GLenum>(m_textureAttachment.texture.target()),
        m_textureAttachment.texture.id(),
        0
    );

    glBindFramebuffer(static_cast<GLenum>(m_target), 0);
}

void Framebuffer::attachRenderbuffer() const {

    glBindFramebuffer(static_cast<GLenum>(m_target), m_framebufferId);

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferAttachment.renderbufferId);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        static_cast<GLenum>(m_renderbufferAttachment.attachmentPoint),
        m_dimensions.x,
        m_dimensions.y
    );
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(
        static_cast<GLenum>(m_target),
        static_cast<GLenum>(m_renderbufferAttachment.attachmentPoint),
        GL_RENDERBUFFER,
        m_renderbufferAttachment.renderbufferId
    );

    glBindFramebuffer(static_cast<GLenum>(m_target), 0);
}
