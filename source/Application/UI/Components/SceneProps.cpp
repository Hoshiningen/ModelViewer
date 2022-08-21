#include "UI/Components/SceneProps.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void SceneProps::render() {

    if (ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_clearColor)))
        clearColorChanged(m_clearColor);

    if (ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientColor)))
        ambientColorChanged(m_ambientColor);

    if (ImGui::SliderFloat("Ambient Intensity", &m_ambientIntensity, 0.f, 1.f))
        ambientIntensityChanged(m_ambientIntensity);

    ImGui::BeginDisabled();
    ImGui::Checkbox("Wireframe (W)", &m_wireframe);
    ImGui::EndDisabled();
}
