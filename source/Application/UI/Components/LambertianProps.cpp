#include "UI/Components/LambertianProps.hpp"

#include "Common/Constants.hpp"

#include "Material/LambertianMaterial.hpp"

#include "UI/Components/MainFrame.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void LambertianProps::render() {

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(m_model.m_diffuseColor)))
        diffuseColorChanged(m_model.m_diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &m_model.m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_model.m_diffuseIntensity);
}

void LambertianProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel || !pModel->m_pLambertianMat)
        return;

    m_model.m_diffuseColor = pModel->m_pLambertianMat->diffuseColor();
    m_model.m_diffuseIntensity = pModel->m_pLambertianMat->diffuseIntensity();
}

const IComponent::DataModel* LambertianProps::dataModel() const {
    return &m_model;
}
