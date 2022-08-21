#include "UI/Components/LambertianProps.hpp"

#include "Common/Constants.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void LambertianProps::render() {

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(m_diffuseColor)))
        diffuseColorChanged(m_diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_diffuseIntensity);
}
