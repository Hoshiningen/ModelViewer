#include "UI/Components/Viewport.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <imgui.h>

const char* ViewportComponent::windowId() const {
    return "Viewport";
}

void ViewportComponent::render() {

    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNav;

    ImGui::Begin(windowId(), nullptr, windowFlags);

    if (m_framebufferTextureId > 0) {

        const ImTextureID textureId = reinterpret_cast<ImTextureID>(static_cast<GLuint64>(m_framebufferTextureId));
        const ImVec2 size = ImGui::GetWindowSize();
        
        ImGui::Image(textureId, size, { 1, 0 }, { 0, 1 });
    }

    //if (ImGui::BeginTabBar("OpenModels")) {
    //
    //    if (ImGui::BeginTabItem(Utility::Label("Model1.ply", CUBE_ICON).c_str()))
    //        ImGui::EndTabItem();
    //
    //    if (ImGui::BeginTabItem(Utility::Label("Model2.ply", CUBE_ICON).c_str()))
    //        ImGui::EndTabItem();
    //
    //    ImGui::EndTabBar();
    //}

    ImGui::End();
}

DEFINE_SETTER_COPY(ViewportComponent, framebufferTexture, m_framebufferTextureId)
