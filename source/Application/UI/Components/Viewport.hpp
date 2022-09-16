#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <sigslot/signal.hpp>

class ViewportComponent : public IComponent {
public:
    DECLARE_SETTER_COPY(framebufferTexture, GLuint)

    DECLARE_GETTER_IMMUTABLE_COPY(active, bool)

    sigslot::signal<const glm::uvec2&> viewportResized;

private:
    virtual const char* windowId() const override;
    virtual void render() override;

    bool m_windowActive = false;
    GLuint m_framebufferTextureId = 0;

    glm::uvec2 m_windowSize{ 0.f, 0.f };
};
