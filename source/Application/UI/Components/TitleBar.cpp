#include "Ui/Components/TitleBar.hpp"
#include "UI/Components/ModelLoader.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <imgui.h>

void TitleBarComponent::render() {

    bool openModelLoader = false;
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Model..."))
                openModelLoader = true;
            
            ImGui::EndMenu();
        }

        Utility::Center("Model Viewer");

        const float posX = Utility::ComputeRightAlignedCursorPos({ MINIMIZE_ICON, MAXIMIZE_ICON, XMARK_ICON });
        ImGui::SetCursorPosX(posX);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
        ImGui::Button(Utility::Icon(MINIMIZE_ICON).c_str());
        ImGui::Button(Utility::Icon(MAXIMIZE_ICON).c_str());
        ImGui::Button(Utility::Icon(XMARK_ICON).c_str());
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        ImGui::EndMainMenuBar();
    }

    if (openModelLoader && !ImGui::IsPopupOpen(ModelLoaderComponent::kWindowId)) {
        ImGui::OpenPopup(ModelLoaderComponent::kWindowId);
    }
}
