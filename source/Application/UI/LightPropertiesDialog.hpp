#pragma once

#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"

#include "Light/DirectionalLight.hpp"

#include <forward_list>

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <sigslot/signal.hpp>

class VertexBuffered;

class LightPropertiesDialog : public Dialog {
public:
    using Dialog::Dialog;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalLightChanged, LightChanged)

    void onModelLoaded(std::forward_list<VertexBuffered>* pModel);

protected:
    virtual void defineUI() override;

private:
    // Signals
    sigslot::signal<const DirectionalLight&> m_signalLightChanged;
    
    float m_pitch = -glm::half_pi<float>();
    float m_yaw = 0.f;

    DirectionalLight m_light;
};