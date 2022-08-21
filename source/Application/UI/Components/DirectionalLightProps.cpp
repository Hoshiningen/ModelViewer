#include "UI/Components/DirectionalLightProps.hpp"

#include "Common/Constants.hpp"
#include "Common/Math.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void DirectionalLightProps::render() {

    if (ImGui::ColorEdit3("Color", glm::value_ptr(m_color)))
        colorChanged(m_color);

    if (ImGui::SliderFloat("Intensity", &m_intensity, 0.f, 1.f))
        intensityChanged(m_intensity);

    if (ImGui::SliderAngle("Pitch", &m_pitch, -90.f, 90.f)) {
        m_direction = RotateVector(kWorldForward, m_pitch, m_yaw, false);
        directionChanged(m_direction);
    }

    if (ImGui::SliderAngle("Yaw", &m_yaw, 0.f)) {
        m_direction = RotateVector(kWorldForward, m_pitch, m_yaw, false);
        directionChanged(m_direction);
    }

    ImGui::BeginDisabled(); {
        ImGui::InputFloat3("Direction", glm::value_ptr(m_direction));
        ImGui::EndDisabled();
    }
}
