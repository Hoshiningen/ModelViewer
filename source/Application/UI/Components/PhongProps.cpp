#include "UI/Components/PhongProps.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void PhongProps::render() {

    if (ImGui::ColorEdit4("Ambient Color", glm::value_ptr(m_ambientColor)))
        ambientColorChanged(m_ambientColor);

    if (ImGui::SliderFloat("Ambient Intensity", &m_ambientIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        ambientIntensityChanged(m_ambientIntensity);

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(m_diffuseColor)))
        diffuseColorChanged(m_diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_diffuseIntensity);

    if (ImGui::ColorEdit4("Specular Color", glm::value_ptr(m_specularColor)))
        specularColorChanged(m_specularColor);

    if (ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        specularIntensityChanged(m_specularIntensity);

    if (ImGui::SliderFloat("Shininess", &m_shininess, 1.f, 256.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        shininessChanged(m_shininess);
}
