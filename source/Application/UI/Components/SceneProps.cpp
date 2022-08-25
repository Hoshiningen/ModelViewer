#include "UI/Components/SceneProps.hpp"

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

void SceneProps::syncFrom(const std::any& dataModel) {

    if (dataModel.type() != typeid(Model))
        return;

    m_model = std::any_cast<Model>(dataModel);
}
