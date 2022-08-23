#include "UI/Components/Viewport.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <iostream>

#include <imgui.h>

const char* ViewportComponent::windowId() const {
    return "Viewport";
}

void ViewportComponent::render() {

    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNav;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
    ImGui::Begin(windowId(), nullptr, windowFlags);

    m_windowActive = ImGui::IsItemActive() || ImGui::IsWindowHovered();

    const ImVec2 windowSize = ImGui::GetWindowSize();

    if (glm::uvec2 currentSize{ windowSize.x, windowSize.y }; currentSize != m_windowSize) {
        m_windowSize = currentSize;
        viewportResized(currentSize);
    }

    if (m_framebufferTextureId > 0) {

        const ImTextureID textureId = reinterpret_cast<ImTextureID>(static_cast<GLuint64>(m_framebufferTextureId));
        ImGui::Image(textureId, windowSize, { 0, 1 }, { 1, 0 });
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

DEFINE_SETTER_COPY(ViewportComponent, framebufferTexture, m_framebufferTextureId)
DEFINE_GETTER_IMMUTABLE_COPY(ViewportComponent, active, bool, m_windowActive)
