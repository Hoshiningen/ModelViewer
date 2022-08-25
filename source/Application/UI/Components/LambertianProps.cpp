#include "UI/Components/LambertianProps.hpp"

#include "Common/Constants.hpp"

#include "Material/LambertianMaterial.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void LambertianProps::render() {

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(m_model.m_diffuseColor)))
        diffuseColorChanged(m_model.m_diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &m_model.m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_model.m_diffuseIntensity);
}

void LambertianProps::syncFrom(const std::any& dataModel) {

    if (dataModel.type() != typeid(Model))
        return;

    m_model = std::any_cast<Model>(dataModel);
}
