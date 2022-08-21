#include "UI/Components/Properties.hpp"

#include <format>

#include <imgui.h>

DEFINE_SETTER_COPY(PropertiesComponent, itemProperties, m_pItemProperties)
DEFINE_SETTER_CONSTREF(PropertiesComponent, itemName, m_itemName)

const char* PropertiesComponent::windowId() const {
    return "Properties";
}

void PropertiesComponent::render() {

    ImGui::Begin(windowId());

    if (ImGui::BeginTabBar("Properties##Tabs")) {

        std::string tabTitle = "Properties";
        if (m_pItemProperties)
            tabTitle += std::format(" - {}", m_itemName);

        if (ImGui::BeginTabItem(tabTitle.c_str())) {
            
            if (m_pItemProperties)
                m_pItemProperties->render();
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
