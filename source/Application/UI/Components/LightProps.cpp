#include "UI/Components/LightProps.hpp"
#include "UI/Components/MainFrame.hpp"

#include "Light/DirectionalLight.hpp"

#include <iostream>

#include <imgui.h>

void LightProps::render() {

    if (!m_lightProps.at(0))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Light1")) {
        ImGui::PushID("Light1Properties");
        static_cast<IComponent*>(m_lightProps.at(0))->render();
        ImGui::PopID();
    }

    if (!m_lightProps.at(1))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Light2")) {
        ImGui::PushID("Light2Properties");
        static_cast<IComponent*>(m_lightProps.at(1))->render();
        ImGui::PopID();
    }

    if (!m_lightProps.at(2))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Light3")) {
        ImGui::PushID("Light3Properties");
        static_cast<IComponent*>(m_lightProps.at(2))->render();
        ImGui::PopID();
    }
}

void LightProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel)
        return;

    if (auto pLight = pModel->m_lights.at(0); pLight)
        m_model.m_enabledLights.at(0) = pLight->enabled();

    if (auto pLight = pModel->m_lights.at(1); pLight)
        m_model.m_enabledLights.at(1) = pLight->enabled();

    if (auto pLight = pModel->m_lights.at(2); pLight)
        m_model.m_enabledLights.at(2) = pLight->enabled();
}

const IComponent::DataModel* LightProps::dataModel() const {
    return &m_model;
}

void LightProps::compose(const std::initializer_list<IComponent*>& components) {

    std::size_t componentIndex = 0;
    for (IComponent* pComponent : components) {
        if (componentIndex >= m_lightProps.size())
            break;

        if (auto pLightProp = dynamic_cast<DirectionalLightProps*>(pComponent); pLightProp) {
            m_lightProps.at(componentIndex) = pLightProp;
        }

        ++componentIndex;
    }
}
