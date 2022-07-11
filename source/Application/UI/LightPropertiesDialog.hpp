#pragma once

#include "UI/Dialog.hpp"

#include "Light/DirectionalLight.hpp"

#include <forward_list>

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>

#include <sigslot/signal.hpp>

class VertexBuffered;

class LightPropertiesDialog : public Dialog {
public:
    LightPropertiesDialog();
    LightPropertiesDialog(const std::string& title);
    LightPropertiesDialog(const std::string& title, ImGuiWindowFlags flags);
    LightPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags);
    LightPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags);

    template<typename... CallArgs>
    sigslot::connection connectLightChanged(CallArgs&&... args) {
        return m_lightChanged.connect(std::forward<CallArgs>(args)...);
    }

    void onModelLoaded(std::forward_list<VertexBuffered>* pModel);

protected:
    virtual void defineUI() override;
    virtual void initializeUI() override;

private:
    sigslot::signal<const DirectionalLight&> m_lightChanged;

    DirectionalLight m_light;
    struct DirectionalLightModel {
        float pitch = -glm::half_pi<float>();
        float yaw = 0.f;

        glm::vec3 direction;
        glm::vec3 color{ 1.f, 1.f, 1.f };
        float intensity = 1.f;
    } m_lightModel;
};