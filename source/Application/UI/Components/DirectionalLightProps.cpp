#include "UI/Components/DirectionalLightProps.hpp"
#include "UI/Components/SceneTree.hpp"

#include "Common/Constants.hpp"
#include "Common/Math.hpp"

#include "Light/DirectionalLight.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void DirectionalLightProps::render() {

    if (ImGui::Checkbox("Enabled", &m_model.m_enabled))
        enabledChanged(m_model.m_enabled);

    ImGui::BeginDisabled(!m_model.m_enabled); {
        if (ImGui::ColorEdit3("Color", glm::value_ptr(m_model.m_color)))
            colorChanged(m_model.m_color);

        if (ImGui::SliderFloat("Intensity", &m_model.m_intensity, 0.f, 1.f))
            intensityChanged(m_model.m_intensity);

        if (ImGui::SliderAngle("Pitch", &m_model.m_pitch, -90.f, 90.f)) {
            m_model.m_direction = RotateVector(kWorldForward, m_model.m_pitch, m_model.m_yaw, false);
            pitchChanged(m_model.m_pitch);
        }

        if (ImGui::SliderAngle("Yaw", &m_model.m_yaw, 0.f)) {
            m_model.m_direction = RotateVector(kWorldForward, m_model.m_pitch, m_model.m_yaw, false);
            yawChanged(m_model.m_yaw);
        }

        ImGui::BeginDisabled(); {
            ImGui::InputFloat3("Direction", glm::value_ptr(m_model.m_direction));
            ImGui::EndDisabled();
        }

        ImGui::EndDisabled();
    }
}

void DirectionalLightProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    if (auto pModel = dynamic_cast<const DataModel*>(pFrom); pModel)
        m_model = *pModel;
}

const IComponent::DataModel* DirectionalLightProps::dataModel() const {
    return &m_model;
}

void DirectionalLightProps::watch(const IComponent* pComponent) {


}
