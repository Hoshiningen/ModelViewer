#include "UI/Components/Properties/PhongProps.hpp"

#include "Material/PhongMaterial.hpp"

#include "UI/Components/MainFrame.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void PhongProps::render() {

    if (ImGui::ColorEdit4("Ambient Color", glm::value_ptr(m_model.m_ambientColor)))
        ambientColorChanged(m_model.m_ambientColor);

    if (ImGui::SliderFloat("Ambient Intensity", &m_model.m_ambientIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        ambientIntensityChanged(m_model.m_ambientIntensity);

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(m_model.m_diffuseColor)))
        diffuseColorChanged(m_model.m_diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &m_model.m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_model.m_diffuseIntensity);

    if (ImGui::ColorEdit4("Specular Color", glm::value_ptr(m_model.m_specularColor)))
        specularColorChanged(m_model.m_specularColor);

    if (ImGui::SliderFloat("Specular Intensity", &m_model.m_specularIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        specularIntensityChanged(m_model.m_specularIntensity);

    if (ImGui::SliderFloat("Shininess", &m_model.m_shininess, 1.f, 256.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        shininessChanged(m_model.m_shininess);
}

void PhongProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel || !pModel->m_pPhongMat)
        return;

    m_model.m_ambientColor = pModel->m_pPhongMat->ambientColor();
    m_model.m_diffuseColor = pModel->m_pPhongMat->diffuseColor();
    m_model.m_specularColor = pModel->m_pPhongMat->specularColor();
    m_model.m_ambientIntensity = pModel->m_pPhongMat->ambientIntensity();
    m_model.m_diffuseIntensity = pModel->m_pPhongMat->diffuseIntensity();
    m_model.m_specularIntensity = pModel->m_pPhongMat->specularIntensity();
    m_model.m_shininess = pModel->m_pPhongMat->shininess();
}

const IComponent::DataModel* PhongProps::dataModel() const {
    return &m_model;
}
