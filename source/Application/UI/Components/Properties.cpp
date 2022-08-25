#include "UI/Components/Properties.hpp"

#include <format>

#include <imgui.h>

DEFINE_SETTER_COPY(PropertiesComponent, propertiesComponent, m_pPropertiesComponent)

const char* PropertiesComponent::windowId() const {
    return "Properties";
}

void PropertiesComponent::render() {

    ImGui::Begin(windowId());

    if (ImGui::BeginTabBar("Properties##Tabs")) {

        if (ImGui::BeginTabItem("Properties")) {
            
            if (m_pPropertiesComponent)
                m_pPropertiesComponent->render();
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
