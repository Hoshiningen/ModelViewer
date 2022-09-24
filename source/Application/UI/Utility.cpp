#include "UI/Utility.hpp"

#include <format>

#include <imgui.h>

std::string Utility::Label(const char* label, const char* icon) {
    return std::format("{} {}", icon, label);
}

std::string Utility::Label(const std::string& label, const char* icon) {
    return Label(label.c_str(), icon);
}

std::string Utility::Icon(const char* icon) {
    return std::format("{}", icon);
}

void Utility::Center(const char* text) {

    const float windowWidth = ImGui::GetWindowSize().x;
    const float textWidth = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * .5f);
    ImGui::Text(text);
}

void Utility::Center(const std::string& text) {
    Center(text.c_str());
}

float Utility::ComputeRightAlignedCursorPos(const std::initializer_list<const char*>& widgetLabels) {

    static const ImVec2 kFramePadding = ImGui::GetStyle().FramePadding;
    static const ImVec2 kItemSpacing = ImGui::GetStyle().ItemSpacing;

    float totalWidth = 0.f;
    for (const char* label : widgetLabels)
        totalWidth += ImGui::CalcTextSize(label).x + kFramePadding.x * 2;

    totalWidth += (widgetLabels.size() - 1) * kItemSpacing.x;

    return ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - totalWidth;
}

float Utility::ComputeRightAlignedCursorPos(const std::initializer_list<float>& widgetWidths) {

    static const ImVec2 kFramePadding = ImGui::GetStyle().FramePadding;
    static const ImVec2 kItemSpacing = ImGui::GetStyle().ItemSpacing;

    float totalWidth = 0.f;
    for (float width : widgetWidths)
        totalWidth += width;// +kFramePadding.x * 2;

    totalWidth += (widgetWidths.size() - 1) * kItemSpacing.x;

    return ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - totalWidth;
}
