#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class SceneProps : public IComponent {
public:
    struct Model {
        glm::vec3 m_ambientColor;
        glm::vec4 m_clearColor;

        float m_ambientIntensity;
    };

    virtual void render() override;
    virtual void syncFrom(const std::any& dataModel) override;

    sigslot::signal<const glm::vec3&> ambientColorChanged;
    sigslot::signal<const glm::vec4&> clearColorChanged;
    sigslot::signal<float> ambientIntensityChanged;

private:
    Model m_model;
};
