#include "UI/Components/Properties/SceneProps.hpp"
#include "UI/Components/MainFrame.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void SceneProps::render() {

    if (ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_model.m_clearColor)))
        clearColorChanged(m_model.m_clearColor);

    if (ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_model.m_ambientColor)))
        ambientColorChanged(m_model.m_ambientColor);

    if (ImGui::SliderFloat("Ambient Intensity", &m_model.m_ambientIntensity, 0.f, 1.f))
        ambientIntensityChanged(m_model.m_ambientIntensity);
}

void SceneProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel || !pModel->m_pAmbientColor || !pModel->m_pClearColor || !pModel->m_pAmbientIntensity)
        return;

    m_model.m_ambientColor = *pModel->m_pAmbientColor;
    m_model.m_clearColor = *pModel->m_pClearColor;
    m_model.m_ambientIntensity = *pModel->m_pAmbientIntensity;
}

const IComponent::DataModel* SceneProps::dataModel() const {
    return &m_model;
}
