#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glad/glad.h>
#include <sigslot/signal.hpp>

class ViewportComponent : public IComponent {
public:
    virtual const char* windowId() const override;
    virtual void render() override;

    DECLARE_SETTER_COPY(framebufferTexture, GLuint)

private:
    GLuint m_framebufferTextureId = 0;
};
